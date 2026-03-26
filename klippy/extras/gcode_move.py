# G-Code G1 movement commands (and associated coordinate manipulation)
#
# Copyright (C) 2016-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class GCodeMove:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        printer.register_event_handler("klippy:ready", self._handle_ready)
        printer.register_event_handler("klippy:shutdown", self._handle_shutdown)
        printer.register_event_handler("toolhead:set_position",
                                       self.reset_last_position)
        printer.register_event_handler("toolhead:manual_move",
                                       self.reset_last_position)
        printer.register_event_handler("toolhead:update_extra_axes",
                                       self._update_extra_axes)
        printer.register_event_handler("gcode:command_error",
                                       self.reset_last_position)
        printer.register_event_handler("extruder:activate_extruder",
                                       self._handle_activate_extruder)
        printer.register_event_handler("homing:home_rails_end",
                                       self._handle_home_rails_end)
        self.is_printer_ready = False
        # Register g-code commands
        gcode = printer.lookup_object('gcode')
        handlers = [
            'G1', 'G20', 'G21',
            'M82', 'M83', 'G90', 'G91', 'G92', 'M220', 'M221',
            'SET_GCODE_OFFSET', 'SAVE_GCODE_STATE', 'RESTORE_GCODE_STATE',
        ]
        for cmd in handlers:
            func = getattr(self, 'cmd_' + cmd)
            desc = getattr(self, 'cmd_' + cmd + '_help', None)
            gcode.register_command(cmd, func, False, desc)
        gcode.register_command('G0', self.cmd_G1)
        gcode.register_command('M114', self.cmd_M114, True)
        gcode.register_command('GET_POSITION', self.cmd_GET_POSITION, True,
                               desc=self.cmd_GET_POSITION_help)
        self.Coord = gcode.Coord
        # G-Code coordinate manipulation
        self.absolute_coord = self.absolute_extrude = True
        self.base_position = [0.0, 0.0, 0.0, 0.0]
        self.last_position = [0.0, 0.0, 0.0, 0.0]
        self.homing_position = [0.0, 0.0, 0.0, 0.0]
        self.axis_map = {'X':0, 'Y': 1, 'Z': 2, 'E': 3}
        self.speed = 25.
        self.speed_factor = 1. / 60.
        self.extrude_factor = 1.
        # G-Code state
        self.saved_states = {}
        self.move_transform = self.move_with_transform = None
        self.position_with_transform = (lambda: [0., 0., 0., 0.])
    def _handle_ready(self):
        self.is_printer_ready = True
        if self.move_transform is None:
            toolhead = self.printer.lookup_object('toolhead')
            self.move_with_transform = toolhead.move
            self.position_with_transform = toolhead.get_position
        self.reset_last_position()
    def _handle_shutdown(self):
        if not self.is_printer_ready:
            return
        self.is_printer_ready = False
        logging.info("gcode state: absolute_coord=%s absolute_extrude=%s"
                     " base_position=%s last_position=%s homing_position=%s"
                     " speed_factor=%s extrude_factor=%s speed=%s",
                     self.absolute_coord, self.absolute_extrude,
                     self.base_position, self.last_position,
                     self.homing_position, self.speed_factor,
                     self.extrude_factor, self.speed)
    def _handle_activate_extruder(self):
        self.reset_last_position()
        self.extrude_factor = 1.
        self.base_position[3] = self.last_position[3]
    def _handle_home_rails_end(self, homing_state, rails):
        self.reset_last_position()
        for axis in homing_state.get_axes():
            self.base_position[axis] = self.homing_position[axis]
    def set_move_transform(self, transform, force=False):
        if self.move_transform is not None and not force:
            raise self.printer.config_error(
                "G-Code move transform already specified")
        old_transform = self.move_transform
        if old_transform is None:
            old_transform = self.printer.lookup_object('toolhead', None)
        self.move_transform = transform
        self.move_with_transform = transform.move
        self.position_with_transform = transform.get_position
        return old_transform
    def _get_gcode_position(self):
        p = [lp - bp for lp, bp in zip(self.last_position, self.base_position)]
        p[3] /= self.extrude_factor
        return p
    def _get_gcode_speed(self):
        return self.speed / self.speed_factor
    def _get_gcode_speed_override(self):
        return self.speed_factor * 60.
    def get_status(self, eventtime=None):
        move_position = self._get_gcode_position()
        return {
            'speed_factor': self._get_gcode_speed_override(),
            'speed': self._get_gcode_speed(),
            'extrude_factor': self.extrude_factor,
            'absolute_coordinates': self.absolute_coord,
            'absolute_extrude': self.absolute_extrude,
            'homing_origin': self.Coord(*self.homing_position[:4]),
            'position': self.Coord(*self.last_position[:4]),
            'gcode_position': self.Coord(*move_position[:4]),
        }
    def reset_last_position(self):
        if self.is_printer_ready:
            self.last_position = self.position_with_transform()
    def _update_extra_axes(self):
        toolhead = self.printer.lookup_object('toolhead')
        axis_map = {'X':0, 'Y': 1, 'Z': 2, 'E': 3}
        extra_axes = toolhead.get_extra_axes()
        for index, ea in enumerate(extra_axes):
            if ea is None:
                continue
            gcode_id = ea.get_axis_gcode_id()
            if gcode_id is None or gcode_id in axis_map or gcode_id in "FN":
                continue
            axis_map[gcode_id] = index
        self.axis_map = axis_map
        self.base_position[4:] = [0.] * (len(extra_axes) - 4)
        self.reset_last_position()
    # G-Code movement commands
    def cmd_G1(self, gcmd):
        # Move
        params = gcmd.get_command_parameters()

        # 保存起始位置（用于计算距离和拆分）
        start_position = list(self.last_position)

        try:
            for axis, pos in self.axis_map.items():
                if axis in params:
                    v = float(params[axis])
                    absolute_coord = self.absolute_coord
                    if axis == 'E':
                        v *= self.extrude_factor
                        if not self.absolute_extrude:
                            absolute_coord = False
                    if not absolute_coord:
                        # value relative to position of last move
                        self.last_position[pos] += v
                    else:
                        # value relative to base coordinate position
                        self.last_position[pos] = v + self.base_position[pos]
            if 'F' in params:
                gcode_speed = float(params['F'])
                if gcode_speed <= 0.:
                    raise gcmd.error("Invalid speed in '%s'"
                                     % (gcmd.get_commandline(),))
                self.speed = gcode_speed * self.speed_factor
        except ValueError as e:
            raise gcmd.error("Unable to parse move '%s'"
                             % (gcmd.get_commandline(),))

        # 保存目标位置
        end_position = list(self.last_position)

        # 检查是否有姿态参数（ABC）
        has_orientation = any(axis in params for axis in ['A', 'B', 'C'])

        # 获取 executor（用于拆分判断和姿态入队）
        executor = self.printer.lookup_object('nrc_robot_arm_executor', None)

        # 两段式拆分逻辑（仅当启用且有姿态变化时）
        if (has_orientation and executor and
            hasattr(executor, 'enable_move_split') and executor.enable_move_split):

            import math
            from .quaternion_utils import (
                euler_to_quaternion, get_rotation_angle
            )

            # 计算 XYZ 移动距离
            dx = end_position[0] - start_position[0]
            dy = end_position[1] - start_position[1]
            dz = end_position[2] - start_position[2]
            total_distance = math.sqrt(dx*dx + dy*dy + dz*dz)

            # 解析目标姿态
            a_deg = gcmd.get_float('A', None)
            b_deg = gcmd.get_float('B', None)
            c_deg = gcmd.get_float('C', None)

            # 计算目标姿态（机械臂坐标系）
            base_rx = math.radians(180.0)
            base_ry = math.radians(0.0)
            base_rz = math.radians(0.0)

            offset_a = math.radians(a_deg) if a_deg is not None else 0.0
            offset_b = math.radians(b_deg) if b_deg is not None else 0.0
            offset_c = math.radians(c_deg) if c_deg is not None else 0.0

            # 适配 NRC 机械臂 XYZ 欧拉角顺序：
            # 仿真代码使用 ZYX 顺序，机械臂使用 XYZ 顺序
            # 需要对 B（ry）和 C（rz）取反才能得到正确的姿态
            target_rx = base_rx + offset_a
            target_ry = base_ry - offset_b  # 注意: 取反以适配 XYZ 顺序
            target_rz = base_rz - offset_c  # 注意: 取反以适配 XYZ 顺序
            target_orientation = (target_rx, target_ry, target_rz)

            # 获取当前姿态并计算姿态变化量
            current_orientation = executor.get_current_orientation()
            q_current = euler_to_quaternion(*current_orientation)
            q_target = euler_to_quaternion(*target_orientation)
            delta_theta_rad = get_rotation_angle(q_current, q_target)
            delta_theta_deg = math.degrees(delta_theta_rad)

            # 判断是否需要拆分（有足够的距离 + 有姿态变化）
            if total_distance > 0.001 and delta_theta_deg > 0.1:
                # 执行两段式拆分
                self._execute_split_move(
                    start_position, end_position,
                    current_orientation, target_orientation,
                    delta_theta_deg, total_distance,
                    executor, gcmd
                )
                return  # 拆分完成，直接返回

        # 原有逻辑：不拆分，直接执行
        self.move_with_transform(self.last_position, self.speed)

        # 姿态入队（如果有 ABC 参数）
        if has_orientation and executor and hasattr(executor, 'enqueue_orientation'):
            import math

            a_deg = gcmd.get_float('A', None)
            b_deg = gcmd.get_float('B', None)
            c_deg = gcmd.get_float('C', None)

            base_rx = math.radians(180.0)
            base_ry = math.radians(0.0)
            base_rz = math.radians(0.0)

            offset_a = math.radians(a_deg) if a_deg is not None else 0.0
            offset_b = math.radians(b_deg) if b_deg is not None else 0.0
            offset_c = math.radians(c_deg) if c_deg is not None else 0.0

            # 适配 NRC 机械臂 XYZ 欧拉角顺序：
            # 仿真代码使用 ZYX 顺序，机械臂使用 XYZ 顺序
            # 需要对 B（ry）和 C（rz）取反才能得到正确的姿态
            rx = base_rx + offset_a
            ry = base_ry - offset_b  # 注意: 取反以适配 XYZ 顺序
            rz = base_rz - offset_c  # 注意: 取反以适配 XYZ 顺序

            toolhead = self.printer.lookup_object('toolhead')
            print_time = toolhead.get_last_move_time()
            executor.enqueue_orientation(print_time, rx, ry, rz)

            # 更新 planned_orientation（保持与拆分路径一致）
            if hasattr(executor, 'set_planned_orientation'):
                executor.set_planned_orientation(rx, ry, rz)

    def _execute_split_move(self, start_pos, end_pos,
                           current_orient, target_orient,
                           delta_theta_deg, total_distance,
                           executor, gcmd):
        """
        执行姿态协调移动方案（Orientation-Coordinated Move）

        方案（v4 - 微小移动 + 姿态调整时间）：
        - Move 1: 以当前姿态完成 100% 的直线移动 + 100% 挤出
        - Move 2: 微小移动（0.001mm），用于姿态调整，无挤出
          - 时间由姿态变化量和角速度决定
          - 机械臂在此期间完成姿态过渡
          - 挤出机等待（不挤出）

        核心原理：
        1. 位置移动和挤出完全同步（消除挤出超前问题）
        2. 微小移动产生 TrapQ 条目，确保航点生成覆盖姿态变化时间
        3. 机械臂有足够时间（T_orient）完成姿态调整
        4. 0.001mm 的位置误差可忽略（远小于打印精度）

        参数:
            start_pos: 起始位置 [X, Y, Z, E, ...]
            end_pos: 目标位置 [X, Y, Z, E, ...]
            current_orient: 当前姿态 (rx, ry, rz) 弧度
            target_orient: 目标姿态 (rx, ry, rz) 弧度
            delta_theta_deg: 姿态变化量（度）
            total_distance: XYZ 总移动距离（mm）
            executor: nrc_robot_arm_executor 实例
            gcmd: G-code 命令对象
        """
        import math

        # 配置参数
        MICRO_MOVE_DISTANCE = 0.001  # 微小移动距离（mm），肉眼不可见
        angular_velocity = executor.fixed_angular_velocity  # 默认 10°/s

        # 计算姿态变化所需时间
        T_orient = delta_theta_deg / angular_velocity if angular_velocity > 0 else 0.1
        T_orient = max(T_orient, 0.05)  # 最小 50ms，避免除零

        # 计算微小移动的速度（确保移动时间 = T_orient）
        micro_move_speed = MICRO_MOVE_DISTANCE / T_orient  # mm/s

        # 调试日志
        logging.info(
            f"[ORIENT-COORD] 姿态协调移动: "
            f"当前=({math.degrees(current_orient[0]):.1f}°, {math.degrees(current_orient[1]):.1f}°, {math.degrees(current_orient[2]):.1f}°), "
            f"目标=({math.degrees(target_orient[0]):.1f}°, {math.degrees(target_orient[1]):.1f}°, {math.degrees(target_orient[2]):.1f}°), "
            f"Δθ={delta_theta_deg:.1f}°, T_orient={T_orient*1000:.0f}ms"
        )

        toolhead = self.printer.lookup_object('toolhead')

        # === 关键：锁定航点发送，确保 Move 1 和 Move 2 一起被发送 ===
        executor.begin_orientation_move()

        try:
            # 锁定 planned_orientation 为当前姿态
            executor.set_planned_orientation(*current_orient)

            # 获取当前时间点
            current_print_time = toolhead.get_last_move_time()

            # 入队姿态：移动段保持当前姿态
            executor.enqueue_orientation(current_print_time, *current_orient)

            # === Move 1: 完整移动（100% 距离 + 100% 挤出），使用当前姿态 ===
            logging.info(
                f"[ORIENT-COORD] Move 1 (主移动): 距离={total_distance:.2f}mm, "
                f"E={end_pos[3] - start_pos[3]:.3f}mm, 速度={self.speed*60:.0f}mm/min, 姿态=当前"
            )
            self.last_position = list(end_pos)
            self.move_with_transform(self.last_position, self.speed,
                                     target_orientation=current_orient)

            # 获取 Move 1 结束时间
            move1_end_time = toolhead.get_last_move_time()
            logging.info(f"[ORIENT-DEBUG] Move 1 结束时间: {move1_end_time:.6f}s")

            # 入队姿态：Move 1 结束时仍为当前姿态
            executor.enqueue_orientation(move1_end_time, *current_orient)

            # === Move 2: 微小移动（0.001mm），用于姿态调整 ===
            # 计算微小移动的方向（沿原移动方向继续）
            dx = end_pos[0] - start_pos[0]
            dy = end_pos[1] - start_pos[1]
            dz = end_pos[2] - start_pos[2]

            if total_distance > 0.0001:
                # 沿移动方向微小移动
                ratio = MICRO_MOVE_DISTANCE / total_distance
                micro_dx = dx * ratio
                micro_dy = dy * ratio
                micro_dz = dz * ratio
            else:
                # 如果原移动距离为0，沿Z轴微小移动
                micro_dx = 0.0
                micro_dy = 0.0
                micro_dz = MICRO_MOVE_DISTANCE

            # 计算 Move 2 终点位置
            micro_end_pos = list(end_pos)
            micro_end_pos[0] += micro_dx
            micro_end_pos[1] += micro_dy
            micro_end_pos[2] += micro_dz

            # 关键修复：姿态调整期间添加回抽，防止下一段挤出提前执行
            # 用户报告的问题：4秒姿态调整期间，下一段G-code的挤出提前执行
            # 原因：Move 2的4秒时间被挤出器理解为"正常运动时间"
            # 解决：添加负挤出（回抽），确保姿态调整期间无多余挤料
            retraction_amount = executor.orientation_retraction  # 默认0.5mm
            micro_end_pos[3] = end_pos[3] - retraction_amount  # 回抽，而非占位挤出

            logging.info(
                f"[ORIENT-COORD] Move 2 (姿态调整 + 回抽): 距离={MICRO_MOVE_DISTANCE:.3f}mm, "
                f"速度={micro_move_speed*60:.3f}mm/min, T={T_orient*1000:.0f}ms, "
                f"姿态: {math.degrees(current_orient[0]):.1f}° → {math.degrees(target_orient[0]):.1f}°, "
                f"回抽={retraction_amount:.2f}mm (防止下一段挤出提前)"
            )

            # 记录 Move 2 开始时间
            move2_start_time = toolhead.get_last_move_time()
            logging.info(f"[ORIENT-DEBUG] Move 2 开始时间: {move2_start_time:.6f}s")

            self.last_position = micro_end_pos
            self.move_with_transform(self.last_position, micro_move_speed,
                                     target_orientation=target_orient)

            # 获取 Move 2 结束时间
            move2_end_time = toolhead.get_last_move_time()
            actual_move2_duration = move2_end_time - move2_start_time
            logging.info(
                f"[ORIENT-DEBUG] Move 2 结束时间: {move2_end_time:.6f}s, "
                f"实际持续时间: {actual_move2_duration*1000:.1f}ms (计划: {T_orient*1000:.0f}ms)"
            )

            # 入队姿态：Move 2 结束时为目标姿态
            executor.enqueue_orientation(move2_end_time, *target_orient)

            # 更新 planned_orientation 为目标姿态
            executor.set_planned_orientation(*target_orient)

            # 关键修复：插入延迟，确保挤出机和机械臂同步开始下一段
            #
            # 问题根因：机械臂的时间戳有 initial_timestamp_offset_ms 的补偿
            #          挤出机的时间戳无补偿，导致挤出机提前开始下一段
            #
            # 解决方案：在 Move 2 结束后插入 dwell，延迟下一段的开始时间
            #
            # 关键优化：使用实测延迟而非配置值，确保精确同步
            #   - 实测延迟：first_waypoint_execution_delay（如 687ms）
            #   - 配置值：initial_timestamp_offset_ms（如 220ms）
            #   - 使用实测值可消除误差
            #
            # 时间线分析（航点及时发送，50ms内）：
            #   t1: Move 1 结束，Move 2 开始
            #   t1 + 50ms: 航点发送到机械臂
            #   t1 + 50ms + base_dwell: 机械臂开始执行姿态调整
            #   t1 + T_orient: Move 2 结束（挤出机视角）
            #   t1 + 50ms + base_dwell + T_orient: 机械臂姿态调整完成
            #
            # 同步要求：
            #   挤出机的下一段开始时间 >= 机械臂姿态调整完成时间
            #   (t1 + T_orient) + dwell >= t1 + 50ms + base_dwell + T_orient
            #   dwell >= 50ms + base_dwell
            #
            # 因此：dwell = base_dwell（已包含50ms发送延迟的余量）

            # 基础dwell时间：优先使用实测延迟
            if executor.first_waypoint_execution_delay is not None:
                base_dwell = executor.first_waypoint_execution_delay
                delay_source = "实测延迟"
            else:
                base_dwell = executor.initial_timestamp_offset_ms / 1000.0
                delay_source = "配置值（未测量实测延迟）"

            # dwell时间：只需要base_dwell
            # 因为Move 2本身已经持续了T_orient，航点在Move 2开始时就发送（50ms内）
            # 机械臂在Move 2期间执行姿态调整，只需要补偿启动延迟
            dwell_time = base_dwell

            # 日志记录
            actual_orient_time = move2_end_time - move1_end_time
            logging.info(
                f"[ORIENT-COORD] 完成: 主移动 {total_distance:.2f}mm @ {self.speed*60:.0f}mm/min, "
                f"姿态调整 {delta_theta_deg:.1f}° 用时 {actual_orient_time*1000:.0f}ms"
            )

            # === 关键修复：调整调用顺序 ===
            # 1. 先刷新 lookahead 队列到 TrapQ
            toolhead.flush_step_generation()
            logging.debug("[ORIENT-COORD] 强制刷新 lookahead 队列")

            # 2. 立即提取 TrapQ 并发送航点（在轮询之前）
            # 这确保 Move 1 和 Move 2 一起发送，时间戳不会过期
            executor.end_orientation_move()

            # 3. 最后执行 dwell，等待机械臂完成姿态调整
            # 记录 dwell 开始时间
            dwell_start_time = toolhead.get_last_move_time()
            logging.info(f"[ORIENT-DEBUG] dwell 开始时间: {dwell_start_time:.6f}s")

            toolhead.dwell(dwell_time)

            # 记录 dwell 结束时间
            dwell_end_time = toolhead.get_last_move_time()
            actual_dwell_duration = dwell_end_time - dwell_start_time
            logging.info(
                f"[ORIENT-DEBUG] dwell 结束时间: {dwell_end_time:.6f}s, "
                f"实际持续时间: {actual_dwell_duration*1000:.1f}ms (计划: {dwell_time*1000:.1f}ms)"
            )
            logging.info(
                f"[ORIENT-COORD] 插入延迟 {dwell_time*1000:.1f}ms（{delay_source}），"
                f"补偿机械臂启动延迟"
            )

        finally:
            # 确保 orientation_move_pending 标志被重置（防止异常情况）
            # 注意：正常情况下 end_orientation_move() 已经在上面调用了
            # 这里只是为了异常情况的保险
            if executor.orientation_move_pending:
                logging.warning("[ORIENT-COORD] 异常情况：orientation_move_pending 仍为 True，强制重置")
                with executor.orientation_move_lock:
                    executor.orientation_move_pending = False

    # G-Code coordinate manipulation
    def cmd_G20(self, gcmd):
        # Set units to inches
        raise gcmd.error('Machine does not support G20 (inches) command')
    def cmd_G21(self, gcmd):
        # Set units to millimeters
        pass
    def cmd_M82(self, gcmd):
        # Use absolute distances for extrusion
        self.absolute_extrude = True
    def cmd_M83(self, gcmd):
        # Use relative distances for extrusion
        self.absolute_extrude = False
    def cmd_G90(self, gcmd):
        # Use absolute coordinates
        self.absolute_coord = True
    def cmd_G91(self, gcmd):
        # Use relative coordinates
        self.absolute_coord = False
    def cmd_G92(self, gcmd):
        # Set position
        offsets = [ gcmd.get_float(a, None) for a in 'XYZE' ]
        for i, offset in enumerate(offsets):
            if offset is not None:
                if i == 3:
                    offset *= self.extrude_factor
                self.base_position[i] = self.last_position[i] - offset
        if offsets == [None, None, None, None]:
            self.base_position[:4] = self.last_position[:4]
    def cmd_M114(self, gcmd):
        # Get Current Position
        p = self._get_gcode_position()
        gcmd.respond_raw("X:%.3f Y:%.3f Z:%.3f E:%.3f" % tuple(p))
    def cmd_M220(self, gcmd):
        # Set speed factor override percentage
        value = gcmd.get_float('S', 100., above=0.) / (60. * 100.)
        self.speed = self._get_gcode_speed() * value
        self.speed_factor = value
    def cmd_M221(self, gcmd):
        # Set extrude factor override percentage
        new_extrude_factor = gcmd.get_float('S', 100., above=0.) / 100.
        last_e_pos = self.last_position[3]
        e_value = (last_e_pos - self.base_position[3]) / self.extrude_factor
        self.base_position[3] = last_e_pos - e_value * new_extrude_factor
        self.extrude_factor = new_extrude_factor
    cmd_SET_GCODE_OFFSET_help = "Set a virtual offset to g-code positions"
    def cmd_SET_GCODE_OFFSET(self, gcmd):
        move_delta = [0., 0., 0., 0.]
        for pos, axis in enumerate('XYZE'):
            offset = gcmd.get_float(axis, None)
            if offset is None:
                offset = gcmd.get_float(axis + '_ADJUST', None)
                if offset is None:
                    continue
                offset += self.homing_position[pos]
            delta = offset - self.homing_position[pos]
            move_delta[pos] = delta
            self.base_position[pos] += delta
            self.homing_position[pos] = offset
        # Move the toolhead the given offset if requested
        if gcmd.get_int('MOVE', 0):
            speed = gcmd.get_float('MOVE_SPEED', self.speed, above=0.)
            for pos, delta in enumerate(move_delta):
                self.last_position[pos] += delta
            self.move_with_transform(self.last_position, speed)
    cmd_SAVE_GCODE_STATE_help = "Save G-Code coordinate state"
    def cmd_SAVE_GCODE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        self.saved_states[state_name] = {
            'absolute_coord': self.absolute_coord,
            'absolute_extrude': self.absolute_extrude,
            'base_position': list(self.base_position),
            'last_position': list(self.last_position),
            'homing_position': list(self.homing_position),
            'speed': self.speed, 'speed_factor': self.speed_factor,
            'extrude_factor': self.extrude_factor,
        }
    cmd_RESTORE_GCODE_STATE_help = "Restore a previously saved G-Code state"
    def cmd_RESTORE_GCODE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        state = self.saved_states.get(state_name)
        if state is None:
            raise gcmd.error("Unknown g-code state: %s" % (state_name,))
        # Restore state
        self.absolute_coord = state['absolute_coord']
        self.absolute_extrude = state['absolute_extrude']
        self.base_position[:4] = state['base_position'][:4]
        self.homing_position = list(state['homing_position'])
        self.speed = state['speed']
        self.speed_factor = state['speed_factor']
        self.extrude_factor = state['extrude_factor']
        # Restore the relative E position
        e_diff = self.last_position[3] - state['last_position'][3]
        self.base_position[3] += e_diff
        # Move the toolhead back if requested
        if gcmd.get_int('MOVE', 0):
            speed = gcmd.get_float('MOVE_SPEED', self.speed, above=0.)
            self.last_position[:3] = state['last_position'][:3]
            self.move_with_transform(self.last_position, speed)
    cmd_GET_POSITION_help = (
        "Return information on the current location of the toolhead")
    def cmd_GET_POSITION(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead', None)
        if toolhead is None:
            raise gcmd.error("Printer not ready")
        kin = toolhead.get_kinematics()
        steppers = kin.get_steppers()
        mcu_pos = " ".join(["%s:%d" % (s.get_name(), s.get_mcu_position())
                            for s in steppers])
        cinfo = [(s.get_name(), s.get_commanded_position()) for s in steppers]
        stepper_pos = " ".join(["%s:%.6f" % (a, v) for a, v in cinfo])
        kinfo = zip("XYZ", kin.calc_position(dict(cinfo)))
        kin_pos = " ".join(["%s:%.6f" % (a, v) for a, v in kinfo])
        toolhead_pos = " ".join(["%s:%.6f" % (a, v) for a, v in zip(
            "XYZE", toolhead.get_position()[:4])])
        gcode_pos = " ".join(["%s:%.6f"  % (a, v)
                              for a, v in zip("XYZE", self.last_position)])
        base_pos = " ".join(["%s:%.6f"  % (a, v)
                             for a, v in zip("XYZE", self.base_position)])
        homing_pos = " ".join(["%s:%.6f"  % (a, v)
                               for a, v in zip("XYZ", self.homing_position)])
        gcmd.respond_info("mcu: %s\n"
                          "stepper: %s\n"
                          "kinematic: %s\n"
                          "toolhead: %s\n"
                          "gcode: %s\n"
                          "gcode base: %s\n"
                          "gcode homing: %s"
                          % (mcu_pos, stepper_pos, kin_pos, toolhead_pos,
                             gcode_pos, base_pos, homing_pos))

def load_config(config):
    return GCodeMove(config)
