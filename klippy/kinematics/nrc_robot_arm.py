# NRC Robot Arm Kinematics
#
# Copyright (C) 2025  Klipper Robot Arm Integration Project
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import re


class NRCRobotArmKinematics:
    """
    机械臂运动学模块

    核心特点：
    - 不创建 XYZ 步进电机（get_steppers() 返回空列表）
    - 提供边界检查和速度限制
    - 管理归零状态
    - 轨迹执行由 NRCRobotArmExecutor 负责
    """

    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        self.toolhead = toolhead

        # 读取机械臂连接配置（从 [printer] 节读取）
        self.robot_ip = config.get('robot_ip')
        self.robot_port = config.get('robot_port', '6001')  # 字符串格式

        # 验证 IP 地址格式
        ip_pattern = r'^(\d{1,3}\.){3}\d{1,3}$'
        if not re.match(ip_pattern, self.robot_ip):
            raise config.error(f"Invalid robot_ip format: {self.robot_ip}")

        # 验证端口格式（字符串，但内容应为数字）
        try:
            port_int = int(self.robot_port)
            if not (1 <= port_int <= 65535):
                raise ValueError()
        except ValueError:
            raise config.error(
                f"Invalid robot_port: {self.robot_port} (must be 1-65535)"
            )

        # 读取速度和加速度限制
        self.max_speed_mmps = config.getfloat('max_speed_mmps', 50.0, minval=0.1)
        self.max_accel_mmps2 = config.getfloat(
            'max_accel_mmps2', 3000.0, minval=1.0
        )

        # 读取角速度限制配置（姿态协调，新增）
        self.max_angular_velocity = config.getfloat(
            'max_angular_velocity', 30.0, minval=1.0
        )  # 最大角速度（°/s），用于姿态-直线运动时间协调
        self.orientation_time_margin = config.getfloat(
            'orientation_time_margin', 1.2, minval=1.0
        )  # 姿态时间安全系数（默认20%余量）
        self.enable_auto_speed_reduction = config.getboolean(
            'enable_auto_speed_reduction', False
        )  # 自动降速开关（默认禁用，建议先警告模式调试）

        # 读取归零速度配置
        self.home_speed_mmps = config.getfloat('home_speed_mmps', 15.0, minval=1.0)

        # 读取归零点配置
        self.home_position = [
            config.getfloat('home_position_x', 0.0),
            config.getfloat('home_position_y', 0.0),
            config.getfloat('home_position_z', 50.0)
        ]

        # 读取工作空间边界
        self.axes_min = toolhead.Coord(
            config.getfloat('position_min_x', -500),
            config.getfloat('position_min_y', -500),
            config.getfloat('position_min_z', 0),
            0
        )
        self.axes_max = toolhead.Coord(
            config.getfloat('position_max_x', 500),
            config.getfloat('position_max_y', 500),
            config.getfloat('position_max_z', 500),
            0
        )

        # 状态变量
        self.homed_axes = ""
        self.current_pos = [0., 0., 0., 0.]
        self.executor = None  # 延迟初始化（在后续阶段实现）

        # 注册事件处理器
        self.printer.register_event_handler("klippy:connect", self._handle_connect)

        logging.info(
            f"NRC Robot Arm Kinematics initialized: "
            f"IP={self.robot_ip}, Port={self.robot_port}"
        )

    def _handle_connect(self):
        """Klipper 连接后初始化"""
        # 阶段 1：暂不查找 executor（executor 在后续阶段实现）
        logging.info("NRC Robot Arm kinematics connected")

    def get_steppers(self):
        """
        返回步进电机列表

        关键设计：返回空列表，因为 XYZ 不创建步进电机
        轨迹由 NRCRobotArmExecutor 基于 TrapQ 驱动机械臂
        """
        return []

    def calc_position(self, stepper_positions):
        """
        从步进电机位置计算笛卡尔坐标（正运动学）

        关键设计：由于没有 XYZ 步进电机，返回内部维护的当前位置
        """
        return list(self.current_pos)

    def set_position(self, newpos, homing_axes):
        """
        设置当前位置（用于归零后）

        参数：
            newpos: 新位置 [x, y, z, e]
            homing_axes: 归零的轴（如 'xyz'）
        """
        self.current_pos = list(newpos)

        # 更新归零状态
        if homing_axes:
            for axis_name in homing_axes:
                if axis_name in 'xyz' and axis_name not in self.homed_axes:
                    self.homed_axes += axis_name

        logging.info(
            f"Position set to {newpos}, homed_axes: {self.homed_axes}"
        )

    def check_move(self, move):
        """
        检查移动是否合法，可修改速度/加速度限制

        关键功能：
        1. 检查是否已归零
        2. 检查边界
        3. 姿态协调验证（新增）：确保姿态变化时间 ≤ 直线运动时间
        4. 限制速度和加速度
        """
        # 检查是否已归零
        if not self.homed_axes:
            raise move.move_error("Must home axes first")

        # 检查边界
        end_pos = move.end_pos
        if (end_pos[0] < self.axes_min[0] or end_pos[0] > self.axes_max[0] or
            end_pos[1] < self.axes_min[1] or end_pos[1] > self.axes_max[1] or
            end_pos[2] < self.axes_min[2] or end_pos[2] > self.axes_max[2]):
            raise move.move_error(
                f"Move out of range: X={end_pos[0]:.2f} Y={end_pos[1]:.2f} "
                f"Z={end_pos[2]:.2f}"
            )

        # 姿态协调验证（使用前向传递的目标姿态）
        executor = self.printer.lookup_object('nrc_robot_arm_executor', None)
        if executor:
            # 计算运动时间参数
            move_distance = move.move_d  # 直线距离（mm）
            move_velocity = move.max_cruise_v2 ** 0.5  # 巡航速度（mm/s）

            if move_distance > 0 and move_velocity > 0:
                T_move = move_distance / move_velocity  # 直线运动时间（秒）

                # 获取前向传递的目标姿态
                target_orientation = getattr(move, 'target_orientation', None)
                delta_theta = 0.0

                if target_orientation is not None and hasattr(executor, 'get_current_orientation'):
                    import math
                    from extras.quaternion_utils import euler_to_quaternion, get_rotation_angle

                    # 获取当前姿态
                    current_orientation = executor.get_current_orientation()

                    # 调试日志：输出姿态信息
                    logging.info(
                        f"[COORD-DEBUG] Orientation: current={tuple(math.degrees(x) for x in current_orientation)}, "
                        f"target={tuple(math.degrees(x) for x in target_orientation)}"
                    )

                    # 使用四元数精确计算姿态变化
                    q_current = euler_to_quaternion(*current_orientation)
                    q_target = euler_to_quaternion(*target_orientation)
                    delta_theta_rad = get_rotation_angle(q_current, q_target)
                    delta_theta = math.degrees(delta_theta_rad)

                # 调试日志：始终输出姿态变化量
                toolhead = self.printer.lookup_object('toolhead')
                current_print_time = toolhead.get_last_move_time()
                logging.info(
                    f"[COORD-DEBUG] Move: t={current_print_time:.3f}s → {current_print_time + T_move:.3f}s ({T_move*1000:.0f}ms), "
                    f"distance={move_distance:.1f}mm, velocity={move_velocity:.1f}mm/s, Δθ={delta_theta:.3f}°"
                )

                # 如果存在姿态变化，进行协调验证
                if delta_theta > 0.1:  # 忽略极小的角度变化（<0.1°）
                    # 计算姿态调整所需最小时间
                    T_orientation_min = (delta_theta / self.max_angular_velocity) * self.orientation_time_margin

                    # 计算时间余量倍数
                    time_margin_ratio = T_move / T_orientation_min if T_orientation_min > 0 else float('inf')

                    # 协调性检查
                    if T_move < T_orientation_min:
                        # 时间不足，需要降速
                        v_safe = move_distance / T_orientation_min

                        if self.enable_auto_speed_reduction:
                            # 自动降速模式
                            logging.warning(
                                f"[COORD] 🔽 自动降速: {move_velocity:.1f} → {v_safe:.1f} mm/s "
                                f"(姿态变化 Δθ={delta_theta:.1f}°, 需要 {T_orientation_min*1000:.0f}ms, "
                                f"直线仅 {T_move*1000:.0f}ms, 余量 {time_margin_ratio:.2f}x)"
                            )
                            move.limit_speed(v_safe, self.max_accel_mmps2)
                        else:
                            # 仅警告模式
                            logging.warning(
                                f"[COORD] ⚠️ 警告: 运动时间不足! "
                                f"直线时间 {T_move*1000:.0f}ms < 姿态时间 {T_orientation_min*1000:.0f}ms "
                                f"(Δθ={delta_theta:.1f}°, 速度 {move_velocity:.1f} mm/s, 余量 {time_margin_ratio:.2f}x). "
                                f"建议降速至 {v_safe:.1f} mm/s 或启用 enable_auto_speed_reduction"
                            )
                    else:
                        # 时间充足，仅记录诊断信息
                        if time_margin_ratio < 2.0:  # 余量<2倍时提示
                            logging.info(
                                f"[COORD] ✅ 时间余量充足: {time_margin_ratio:.2f}x "
                                f"(姿态 {delta_theta:.1f}° 需要 {T_orientation_min*1000:.0f}ms, "
                                f"直线提供 {T_move*1000:.0f}ms)"
                            )

        # 根据机械臂能力限制速度和加速度
        move.limit_speed(self.max_speed_mmps, self.max_accel_mmps2)

    def home(self, homing_state):
        """
        执行归零操作

        完整实现：实际移动机械臂到归零点，确保位置状态一致
        """
        # 标记已归零的轴
        for axis in homing_state.get_axes():
            if axis < 3:
                axis_name = 'xyz'[axis]
                if axis_name not in self.homed_axes:
                    self.homed_axes += axis_name

        logging.info(
            f"开始归零操作: 目标位置 {self.home_position}, "
            f"归零速度 {self.home_speed_mmps} mm/s"
        )

        # 获取 executor 引用
        try:
            executor = self.printer.lookup_object('nrc_robot_arm_executor')
        except Exception as e:
            raise self.printer.command_error(
                f"无法获取 nrc_robot_arm_executor: {e}。"
                f"请确保配置文件中包含 [nrc_robot_arm_executor] 部分。"
            )

        # 调用 executor 的阻塞式归零移动方法
        try:
            executor.move_to_home_position_blocking(
                self.home_position,
                self.home_speed_mmps
            )
        except Exception as e:
            # 归零失败，清除归零状态
            self.homed_axes = ""
            raise self.printer.command_error(
                f"归零移动失败: {e}"
            )

        # 移动完成后，更新当前位置为归零点
        self.current_pos = self.home_position + [0.]
        # 同步更新 Klipper 工具头位置（确保前端显示为归零点坐标而非 0,0,0）
        try:
            self.toolhead.set_position(self.home_position + [0.])
        except Exception as e:
            logging.warning(f"⚠️ 无法在 ToolHead 中设置归零位置: {e}")


        logging.info(
            f"✅ 归零完成: 位置 {self.home_position}, "
            f"已归零轴: {self.homed_axes}"
        )

        # 归零完成后自动下电
        try:
            executor.power_off_robot()
        except Exception as e:
            logging.warning(f"⚠️ 归零后下电失败: {e}")
            # 下电失败不影响归零状态，只记录警告

    def clear_homing_state(self, clear_axes):
        """
        清除归零状态

        参数：
            clear_axes: 要清除的轴（如 'xyz'）
        """
        for axis_name in clear_axes:
            if axis_name in self.homed_axes:
                self.homed_axes = self.homed_axes.replace(axis_name, '')

        logging.info(f"Homing state cleared for axes: {clear_axes}")

    def get_status(self, eventtime):
        """
        返回状态信息（用于查询和监控）

        返回：
            dict: 包含 homed_axes, axis_minimum, axis_maximum
        """
        return {
            'homed_axes': self.homed_axes,
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max
        }


def load_kinematics(toolhead, config):
    """
    Klipper 加载运动学模块的入口函数

    参数：
        toolhead: ToolHead 对象
        config: 配置对象

    返回：
        NRCRobotArmKinematics: 运动学实例
    """
    return NRCRobotArmKinematics(toolhead, config)

