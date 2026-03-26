#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
NRC Robot Arm Trajectory Executor

此模块负责从 Klipper 的 TrapQ（梯形运动队列）中提取运动数据，
并将其转换为 NRC 机械臂可执行的轨迹航点序列。

主要功能：
1. 周期性轮询 TrapQ，提取已完成的运动段（moves）
2. 将梯形速度曲线采样为固定时间间隔的航点
3. 计算相对时间戳（相对于打印开始时间）
4. 发送航点序列到 NRC 机械臂控制器

技术特性：
- 轮询频率：10ms（100 Hz）
- 采样间隔：50ms（20 Hz）
- 安全边际：150ms（确保 move 已 finalize）
- 网络延迟补偿：50ms

日志级别说明:
- logging.info(): 关键操作和汇总信息
- logging.debug(): 详细调试信息(需要设置log_level=DEBUG才能看到)
- logging.warning(): 警告信息
- logging.error(): 错误信息

作者：AI Agent
日期：2025-11-06
版本：1.1.0 (Phase 3) - 修复时间戳延迟问题 + 清理冗余日志
"""

import logging
import chelper
import threading
import math
import time

# ========== 常量定义 ==========

# 轮询和采样参数
POLL_INTERVAL = 0.01        # 轮询间隔（秒）：10ms = 100 Hz
SAMPLING_INTERVAL = 0.05    # 采样间隔（秒）：50ms = 20 Hz（降低采样密度，增加航点间时间间隔）
SAFETY_MARGIN = 0.010       # 安全边际（秒）：10ms（极小值，依赖trapq_finalize_moves）
MIN_MOVE_DURATION = 0.0100   # 最小 move 持续时间（秒）：过滤掉太短的 move（加速/减速段）
MOVE_GAP_THRESHOLD = 0.050  # Move 间隔阈值（秒）：超过此值认为是新命令

# 流式发送触发参数（Phase 3 优化）
SEND_TIME_WINDOW = 0.5      # 时间窗口触发阈值（秒）：500ms（流式发送的主要阈值）
SEND_IDLE_THRESHOLD = 5     # 空闲触发阈值（次数）：5次 = 50ms（降低以更快响应）
SEND_MOVE_COUNT_LIMIT = 50  # 数量触发阈值（个数）：50个moves（允许更大批次）
FORCED_SEND_THRESHOLD = 0.05 # 强制发送阈值（秒）：50ms（确保航点及时发送，避免时间戳过期）
                             # 关键修复：从0.5秒降低到50ms，解决长时间Move导致时间戳过期的问题

# 时间戳计算参数
NETWORK_DELAY_MS = 50.0     # 网络延迟补偿（毫秒）
BUFFER_TIME = 1.0           # 提前发送缓冲时间（秒）
# 注意：INITIAL_TIMESTAMP_OFFSET_MS 已移至配置参数，可通过配置文件调整

# 批量提取参数
MAX_BATCH_SIZE = 128        # 单次提取的最大 move 数量

# 性能监控参数
WARN_CALLBACK_TIME_MS = 5.0 # 回调耗时警告阈值（毫秒）


# ========== 姿态队列数据结构 ==========

class OrientationQueueEntry:
    """
    姿态队列条目

    存储单个时刻的末端姿态（欧拉角表示）

    属性：
        print_time (float): Klipper的print_time时间戳（秒）
        rx (float): 绕X轴旋转角度（弧度）
        ry (float): 绕Y轴旋转角度（弧度）
        rz (float): 绕Z轴旋转角度（弧度）
    """
    def __init__(self, print_time, rx, ry, rz):
        self.print_time = print_time
        self.rx = rx
        self.ry = ry
        self.rz = rz

    def __repr__(self):
        import math
        return (f"OrientationQueueEntry(t={self.print_time:.6f}s, "
                f"rx={math.degrees(self.rx):.2f}°, "
                f"ry={math.degrees(self.ry):.2f}°, "
                f"rz={math.degrees(self.rz):.2f}°)")


# ========== 主类定义 ==========

class NRCRobotArmExecutor:
    """
    NRC 机械臂轨迹执行器

    负责从 TrapQ 提取运动数据，转换为航点序列，并发送到机械臂。

    时间管理设计：
    - 作业启动与时间管理完全解耦，作业可以在任何时刻启动
    - 时间基准在打印开始时建立（第一个打印运动命令到达时）
    - 第一个航点时间戳为 0ms，后续航点相对于第一个航点计时
    - 归零操作不影响时间基准，保持 print_time 的连续性
    - 测量第一个航点的实际执行延迟，用于验证和诊断
    """
    
    def __init__(self, config):
        """
        初始化执行器
        
        参数：
            config: Klipper 配置对象
        """
        self.printer = config.get_printer()
        self.name = config.get_name()
        
        # 核心对象引用（在 _handle_connect 中初始化）
        self.toolhead = None
        self.trapq = None
        self.kinematics = None
        self.reactor = None
        
        # FFI 接口引用（在 _handle_connect 中初始化）
        self.ffi_main = None
        self.ffi_lib = None
        self.trapq_finalize_moves = None
        self.trapq_extract_old = None

        # NRC SDK 接口（在 _handle_ready 中初始化）
        self.nrc = None                 # NRC SDK 模块引用
        self.socket_fd_6001 = -1        # 控制端口（状态查询、作业启动）
        self.socket_fd_7000 = -1        # 运动端口（servo_move 专用）
        self.nrc_lock = threading.Lock()  # 互斥锁保护所有 NRC SDK 调用

        # 状态缓存变量（由状态查询定时器更新）
        self.cached_position_joint = [0.0] * 6  # 关节坐标（6个关节角度，单位：度或弧度）
        self.cached_position_cartesian = [0.0] * 6  # 直角坐标/基坐标系 [x, y, z, rx, ry, rz]
        self.cached_position_user = [0.0] * 6  # 用户坐标系 [x, y, z, rx, ry, rz]
        self.cached_servo_state = 0           # 0=停止, 1=就绪, 2=报警, 3=运行
        self.cached_running_state = 0         # 运行状态
        self.cached_mode = 0                  # 0=示教, 1=远程, 2=运行
        self.cached_line_speed = 0.0          # 实时线速度（mm/s）
        self.last_position_query_time = 0.0   # 上次位置查询时间
        self.last_state_query_time = 0.0      # 上次状态查询时间
        self.last_mode_query_time = 0.0       # 上次模式查询时间

        # 固定姿态配置（从配置文件读取）
        self.fixed_orientation_rx = config.getfloat('fixed_orientation_rx', 3.14159)
        self.fixed_orientation_ry = config.getfloat('fixed_orientation_ry', 0.0)
        self.fixed_orientation_rz = config.getfloat('fixed_orientation_rz', 0.0)

        # G-code姿态语义配置（新增，用于ABC参数映射）
        self.base_orientation_a = config.getfloat('base_orientation_a', 180.0)  # G-code A0对应的机械臂角度（度）
        self.base_orientation_b = config.getfloat('base_orientation_b', 0.0)
        self.base_orientation_c = config.getfloat('base_orientation_c', 0.0)
        self.gcode_orientation_mode = config.get('gcode_orientation_mode', 'simple')  # simple/quaternion

        # 四元数插值配置（可选优化）
        self.use_quaternion_interpolation = config.getboolean('use_quaternion_interpolation', False)

        # 速度限制配置（从配置文件读取）
        self.max_velocity = config.getfloat('max_velocity', None)  # mm/s，None表示不限制

        # 用户坐标系转换配置（从配置文件读取）
        self.user_coord_number = config.getint('user_coord_number', 1)  # 用户坐标系编号（1-20）
        self.T_base_user = None  # 用户坐标系到基坐标系的变换矩阵（4x4，列表形式）
        self.coord_transform_logged = False  # 是否已记录转换信息

        # 自动化作业启动配置（从配置文件读取）
        self.enable_auto_job_start = config.getboolean('enable_auto_job_start', False)  # 默认禁用
        self.auto_job_name = config.get('auto_job_name', 'DAYIN')
        self.homing_job_name = config.get('homing_job_name', 'GUILIN')  # 归零专用作业名称

        # 时间戳补偿配置（从配置文件读取）
        self.initial_timestamp_offset_ms = config.getfloat('initial_timestamp_offset_ms', 220.0)  # 初始时间戳偏移（毫秒）
        # 用于补偿机械臂启动延迟，建议值：300-450ms（根据实测延迟调整）
        # 如果打印开始时挤出过多，增加此值；如果挤出不足，减少此值

        # ===== 两段式 Move 拆分配置（姿态协调优化）=====
        self.fixed_angular_velocity = config.getfloat('fixed_angular_velocity', 10.0)  # 固定角速度（°/s）
        self.rear_distance_ratio = config.getfloat('rear_distance_ratio', 0.15)  # 后段距离比例（15%，确保足够的采样点）
        self.enable_move_split = config.getboolean('enable_move_split', True)  # 启用两段式拆分
        self.orientation_retraction = config.getfloat('orientation_retraction', 0.5)  # 姿态调整期间的回抽量（mm）

        # 延迟自动调整配置（从配置文件读取）
        self.enable_auto_delay_adjustment = config.getboolean('enable_auto_delay_adjustment', False)  # 默认禁用
        self.delay_adjustment_threshold_ms = config.getfloat('delay_adjustment_threshold_ms', 50.0)  # 延迟调整阈值（毫秒）

        # ===== 预校准配置（自动测量延迟并应用）=====
        self.enable_pre_calibration = config.getboolean('enable_pre_calibration', False)  # 默认禁用
        self.pre_calibration_distance_mm = config.getfloat('pre_calibration_distance_mm', 5.0)  # Z轴移动距离（mm）
        self.calibrated_timestamp_offset_ms = None  # 校准后的时间戳偏移（运行时设置）

        # 姿态管理（动态姿态打印支持）
        self.orientation_queue = []  # List[OrientationQueueEntry]
        self.orientation_lock = threading.Lock()  # 保护队列的互斥锁
        self.last_orientation = None  # 最后的姿态（用于向后兼容和插值边界）
        self.planned_orientation = None  # 当前规划的姿态（修复：避免入队未来姿态影响当前判断）
        logging.info("Dynamic orientation printing enabled (orientation queue initialized)")

        # 姿态协调锁定（防止 Move 拆分期间提前发送航点）
        self.orientation_move_pending = False  # 标志：正在进行姿态协调移动
        self.orientation_move_lock = threading.Lock()  # 保护标志的互斥锁

        # 状态变量
        self.is_paused = False          # 是否暂停
        self.is_first_batch = True      # 是否首批发送（用于clearBuffer判断）
        self.last_sent_time = 0.0       # 最后发送的时间戳
        self.start_print_time = None    # 打印起始时间（第一个move的起始时间）
        self.first_waypoint_time = None # [已废弃] 保留用于状态重置，但不再用于时间戳计算

        # 打印开始时刻的 print_time（用于时间戳计算）
        self.print_start_print_time = None    # 打印开始时刻的 print_time（秒），用于时间戳计算

        # 第一个航点执行延迟测量（用于诊断）
        self.first_waypoint_send_time = None        # 第一个航点发送时间（相对时间，秒）
        self.first_waypoint_execution_delay = None  # 第一个航点执行延迟（秒）
        self.initial_position = None                # 打印开始前的初始位置
        self.delay_measurement_thread = None        # 延迟测量线程

        # 监控统计变量
        self.poll_callback_times = []       # 轮询回调耗时列表（毫秒）
        self.trapq_extract_delays = []      # TrapQ 提取延迟列表（毫秒）
        self.sync_errors = []               # 同步误差列表（毫秒）
        self.max_poll_callback_time_ms = 0.0  # 最大轮询回调耗时（毫秒）
        self.max_trapq_extract_delay_ms = 0.0  # 最大 TrapQ 提取延迟（毫秒）
        self.warning_count = 0              # 告警次数

        # 作业状态变量
        self.job_started = False        # 作业是否已启动
        self.job_name = None            # 当前作业名称

        # 统计变量
        self.total_waypoints_sent = 0   # 已发送的航点总数
        self.total_moves_extracted = 0  # 已提取的 move 总数
        self.poll_count = 0             # 轮询次数
        self.batch_count = 0            # 批次计数器（从1开始）

        # 批次间姿态连续性保证（新增）
        self.last_sent_waypoint = None  # 上一批最后发送的航点 (ts_ms, x, y, z, rx, ry, rz)
        # 使用配置的 fixed_angular_velocity 作为批次间最大角速度限制
        # 确保批次间过渡时间与姿态调整时间（T_orient）一致，保证打印稳定性
        self.max_angular_velocity_dps = self.fixed_angular_velocity  # 最大角速度（度/秒）

        # Move 累积缓冲区（优化：检测ToolHead空闲状态）
        self.accumulated_moves = []     # 累积的 moves
        self.last_move_end_time = None  # 上一个 move 的结束时间
        self.last_accumulate_time = None  # 上次累积的时间（用于超时检测）

        # 两段式拆分保护（防止拆分过程中提前发送）
        self.split_move_in_progress = False  # 是否正在执行两段式拆分

        # ToolHead 空闲检测（用于混合触发策略）
        self.last_print_time = None     # 上次的 print_time
        self.idle_count = 0             # 连续空闲计数（print_time 不变的次数）
        # idle_threshold 已移除，改用全局常量 SEND_IDLE_THRESHOLD

        # 跳过第一批（加速段）标志（方案C：不跳过，使用累积时间戳）
        self.skip_first_batch = False   # 不跳过第一批
        self.first_batch_skipped = False  # 是否已跳过第一批

        # 空闲等待状态（非阻塞）
        self._send_accumulated_after_idle = False
        self._idle_wait_until = None


        # 定时器引用
        self.poll_timer = None
        self.status_query_timer = None  # 状态查询定时器
        
        # 注册事件处理器
        self.printer.register_event_handler("klippy:connect", 
                                           self._handle_connect)
        self.printer.register_event_handler("klippy:ready", 
                                           self._handle_ready)
        
        logging.info("NRC Robot Arm Executor initialized")
    
    def _handle_connect(self):
        """
        Klipper 连接后初始化

        查找必要的对象引用并初始化 FFI 接口。
        """
        # 查找 toolhead 对象
        self.toolhead = self.printer.lookup_object('toolhead')

        # 获取 trapq 引用
        self.trapq = self.toolhead.get_trapq()

        # 获取 kinematics 对象（通过 toolhead）
        self.kinematics = self.toolhead.get_kinematics()

        # 获取 reactor
        self.reactor = self.printer.get_reactor()

        # 初始化 FFI 接口
        self.ffi_main, self.ffi_lib = chelper.get_ffi()
        self.trapq_finalize_moves = self.ffi_lib.trapq_finalize_moves
        self.trapq_extract_old = self.ffi_lib.trapq_extract_old

        logging.info(
            f"NRC Executor connected: toolhead and trapq ready, "
            f"kinematics={self.kinematics.__class__.__name__}"
        )

        # 注册 G-code 命令
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('NRC_CONNECT', self.cmd_NRC_CONNECT,
                              desc=self.cmd_NRC_CONNECT_help)
        gcode.register_command('NRC_DISCONNECT', self.cmd_NRC_DISCONNECT,
                              desc=self.cmd_NRC_DISCONNECT_help)
        gcode.register_command('NRC_SHUTDOWN', self.cmd_NRC_SHUTDOWN,
                              desc=self.cmd_NRC_SHUTDOWN_help)
        gcode.register_command('NRC_POWER_ON', self.cmd_NRC_POWER_ON,
                              desc=self.cmd_NRC_POWER_ON_help)
        gcode.register_command('NRC_POWER_OFF', self.cmd_NRC_POWER_OFF,
                              desc=self.cmd_NRC_POWER_OFF_help)
        gcode.register_command('NRC_STATUS', self.cmd_NRC_STATUS,
                              desc=self.cmd_NRC_STATUS_help)
        gcode.register_command('NRC_SET_MODE', self.cmd_NRC_SET_MODE,
                              desc=self.cmd_NRC_SET_MODE_help)
        gcode.register_command('NRC_RESET_ERROR', self.cmd_NRC_RESET_ERROR,
                              desc=self.cmd_NRC_RESET_ERROR_help)
        gcode.register_command('NRC_START_JOB', self.cmd_NRC_START_JOB,
                              desc=self.cmd_NRC_START_JOB_help)
        gcode.register_command('NRC_PAUSE', self.cmd_NRC_PAUSE,
                              desc=self.cmd_NRC_PAUSE_help)
        gcode.register_command('NRC_RESUME', self.cmd_NRC_RESUME,
                              desc=self.cmd_NRC_RESUME_help)
        logging.info("NRC G-code commands registered (11 commands)")

        # 注册事件监听器
        self.printer.register_event_handler("homing:home_rails_end",
                                           self._handle_homing_end)

    def _import_nrc_sdk(self):
        """
        导入 NRC SDK 模块

        动态添加 NRC SDK 路径并导入 nrc_interface 模块。
        如果导入失败，抛出配置错误。
        """
        import sys, os

        # 添加 NRC SDK 路径
        nrc_sdk_path = os.path.join(
            os.path.dirname(__file__),
            '../../lib/nrc_sdk'
        )

        if os.path.exists(nrc_sdk_path):
            sys.path.insert(0, os.path.abspath(nrc_sdk_path))
            logging.info(f"NRC SDK path added: {os.path.abspath(nrc_sdk_path)}")
        else:
            raise self.printer.config_error(
                f"NRC SDK path not found: {os.path.abspath(nrc_sdk_path)}\n"
                f"Please ensure lib/nrc_sdk/ directory exists"
            )

        # 导入 NRC 接口
        try:
            import nrc_interface as nrc
            self.nrc = nrc
            logging.info("✅ NRC SDK loaded successfully")

            # 尝试获取版本信息
            try:
                version = self.nrc.get_library_version()
                logging.info(f"NRC SDK version: {version}")
            except Exception:
                logging.info("NRC SDK version: unknown")

        except ImportError as e:
            raise self.printer.config_error(
                f"Failed to load NRC SDK: {e}\n"
                f"Please ensure lib/nrc_sdk/ contains nrc_interface.py and _nrc_host.so"
            )

    def _connect_control_port(self):
        """
        连接控制端口（6001）

        用于状态查询、作业启动、模式切换等控制操作。
        """
        robot_ip = self.kinematics.robot_ip
        robot_port = str(self.kinematics.robot_port)

        logging.info(f"Connecting to control port {robot_ip}:{robot_port}...")

        with self.nrc_lock:
            self.socket_fd_6001 = self.nrc.connect_robot(robot_ip, robot_port)

        logging.info(f"connect_robot returned: {self.socket_fd_6001} (type: {type(self.socket_fd_6001)})")

        if self.socket_fd_6001 <= 0:
            raise self.printer.command_error(
                f"Failed to connect to control port {robot_ip}:{robot_port}, socket_fd={self.socket_fd_6001}"
            )

        logging.info(f"✅ Control port connected: fd={self.socket_fd_6001}")

    def _connect_servo_port(self):
        """
        连接运动端口（7000）

        专用于 servo_move() 运动控制。
        """
        robot_ip = self.kinematics.robot_ip

        logging.info(f"Connecting to servo port {robot_ip}:7000...")

        with self.nrc_lock:
            self.socket_fd_7000 = self.nrc.connect_robot(robot_ip, "7000")

        logging.info(f"connect_robot returned: {self.socket_fd_7000} (type: {type(self.socket_fd_7000)})")

        if self.socket_fd_7000 <= 0:
            raise self.printer.command_error(
                f"Failed to connect to servo port {robot_ip}:7000, socket_fd={self.socket_fd_7000}"
            )

        logging.info(f"✅ Servo port connected: fd={self.socket_fd_7000}")

    def _disconnect_robot(self):
        """
        断开机械臂连接

        断开控制端口和运动端口的连接。
        """
        # 输出监控统计信息
        if len(self.poll_callback_times) > 0:
            avg_callback_time = sum(self.poll_callback_times) / len(self.poll_callback_times)
            logging.info("=" * 60)
            logging.info("[MONITOR] 📊 性能统计信息")
            logging.info(f"[MONITOR]   • 轮询回调次数: {len(self.poll_callback_times)}")
            logging.info(f"[MONITOR]   • 平均回调耗时: {avg_callback_time:.2f}ms")
            logging.info(f"[MONITOR]   • 最大回调耗时: {self.max_poll_callback_time_ms:.2f}ms")
            logging.info(f"[MONITOR]   • 告警次数: {self.warning_count}")
            logging.info("=" * 60)

        if self.socket_fd_6001 > 0:
            with self.nrc_lock:
                self.nrc.disconnect_robot(self.socket_fd_6001)
            logging.info("Control port disconnected")
            self.socket_fd_6001 = -1

        if self.socket_fd_7000 > 0:
            with self.nrc_lock:
                self.nrc.disconnect_robot(self.socket_fd_7000)
            logging.info("Servo port disconnected")
            self.socket_fd_7000 = -1

    def _auto_start_job_on_first_move(self):
        """
        首次运动时自动启动作业（完整6步流程）

        注意：此方法不再被自动调用（2025-11-11 修改）
        - 旧逻辑：在 _trapq_poll_callback() 中自动调用（已删除）
        - 新逻辑：在 _send_waypoints_to_robot() 中自动启动作业

        保留此方法以供：
        - 手动调用（如需要）
        - 未来扩展使用

        调用 _auto_start_job() 执行完整的启动流程。
        """
        logging.info("First move detected, auto-starting job...")

        try:
            # 调用完整的5步启动流程
            self._auto_start_job()

            # 设置作业状态标志
            self.job_started = True
            self.job_name = self.auto_job_name

            logging.info("=" * 60)
            logging.info("Job auto-started successfully, proceeding to send waypoints")

        except Exception as e:
            logging.error(f"Failed to auto-start job: {e}")
            raise

    def _auto_start_job(self):
        """
        自动化启动作业（5步流程）

        步骤：
        1. 清除错误
        2. 设置伺服就绪
        3. 打开作业文件
        4. 切换到运行模式
        5. 启动作业

        注意：
        - 不调用 job_stop()，避免状态混乱
        - 不预先清空缓冲区，依赖首次发送时的 clearBuffer=True
        - 完全对齐参考实现 dev/gcode_executor_servo_clean.py
        """
        job_name = self.auto_job_name

        logging.info("=" * 60)
        logging.info("Starting automated job startup sequence")
        logging.info(f"Job name: {job_name}")
        logging.info("=" * 60)

        # 步骤1: 清除错误
        logging.info("Step 1/5: Clearing errors...")
        with self.nrc_lock:
            ret = self.nrc.clear_error(self.socket_fd_6001)
        if ret == 0:
            logging.info("✅ Errors cleared")
        elif ret == -4:
            logging.info("✅ No errors to clear")
        else:
            logging.warning(f"⚠️ clear_error returned {ret}, continuing...")

        # 步骤2: 检查并设置伺服就绪
        logging.info("Step 2/5: Checking servo state...")
        with self.nrc_lock:
            ret_list = self.nrc.get_servo_state(self.socket_fd_6001, 0)

        if isinstance(ret_list, list) and len(ret_list) > 1:
            current_state = ret_list[1]
            state_names = {0: "Stop", 1: "Ready", 2: "Alarm", 3: "Running"}
            logging.info(f"Current servo state: {state_names.get(current_state, 'Unknown')} ({current_state})")

            if current_state == 1:
                logging.info("✅ Servo already ready")
            else:
                logging.info("Setting servo ready...")
                with self.nrc_lock:
                    ret = self.nrc.set_servo_state(self.socket_fd_6001, 1)
                if ret == 0:
                    logging.info("✅ Servo ready")
                elif ret == -4:
                    logging.info("✅ Servo ready (operation not allowed, already in correct state)")
                else:
                    raise self.printer.command_error(
                        f"Failed to set servo ready: ret={ret}"
                    )
        else:
            logging.warning(f"Failed to get servo state: {ret_list}, trying to set anyway...")
            with self.nrc_lock:
                ret = self.nrc.set_servo_state(self.socket_fd_6001, 1)
            if ret == 0 or ret == -4:
                logging.info("✅ Servo ready")
            else:
                raise self.printer.command_error(
                    f"Failed to set servo ready: ret={ret}"
                )

        # 步骤3: 打开作业文件
        logging.info(f"Step 3/5: Opening job '{job_name}'...")
        with self.nrc_lock:
            ret = self.nrc.job_open(self.socket_fd_6001, job_name)
        if ret == 0:
            logging.info(f"✅ Job '{job_name}' opened")
        elif ret == -4 or ret == -3:
            # -4/-3 可能表示作业已经打开，这是正常情况
            logging.info(f"✅ Job '{job_name}' already open")
        else:
            raise self.printer.command_error(
                f"Failed to open job '{job_name}': ret={ret}"
            )

        # 步骤4: 切换到运行模式
        logging.info("Step 4/5: Switching to run mode...")
        with self.nrc_lock:
            ret = self.nrc.set_current_mode(self.socket_fd_6001, 2)
        if ret == 0:
            logging.info("✅ Run mode activated")
        elif ret == -4:
            logging.info("✅ Already in run mode")
        else:
            logging.warning(f"⚠️ set_current_mode returned {ret}, continuing...")

        # 步骤5: 启动作业
        logging.info("Step 5/5: Starting job...")
        with self.nrc_lock:
            ret = self.nrc.job_run(self.socket_fd_6001, job_name)
        if ret == 0:
            logging.info(f"✅ Job '{job_name}' started")
        elif ret == -4 or ret == -3:
            # -4/-3 可能表示作业已在运行
            logging.info(f"✅ Job '{job_name}' already running")
        else:
            raise self.printer.command_error(
                f"Failed to start job: ret={ret}"
            )
        logging.info("✅ Job started successfully")
        logging.info("=" * 60)

    def _clear_servo_buffer(self):
        """
        清空伺服端口（7000）的缓冲区

        ⚠️ 警告：此方法不应在作业启动流程中使用！
        - 发送 (0,0,0) 点位可能导致意外移动
        - 应依赖首次发送数据时的 clearBuffer=True

        保留此方法仅供：
        - 调试目的
        - 特殊场景下的手动调用

        参考：dev/gcode_executor_servo_clean.py 不使用此方法
        """
        logging.info("Clearing servo buffer (sending 3 clear commands)...")

        for i in range(3):
            param = self.nrc.ServoMovePara()
            param.clearBuffer = True  # 清空缓冲区
            param.targetMode = 1
            param.sendMode = 1
            param.runMode = 1
            param.coord = 1
            param.size = 1
            param.sum = 1
            param.count = 1
            param.extMove = 0

            # 发送当前位置作为清空命令
            param.pos = self.nrc.VectorVectorDouble()
            point = self.nrc.VectorDouble()
            # 使用原点坐标
            for _ in range(7):
                point.append(0.0)
            param.pos.append(point)

            # 填充速度和加速度
            param.axisvel = self.nrc.VectorVectorDouble()
            vel = self.nrc.VectorDouble()
            for _ in range(7):
                vel.append(0.0)
            param.axisvel.append(vel)

            param.axisacc = self.nrc.VectorVectorDouble()
            acc = self.nrc.VectorDouble()
            for _ in range(7):
                acc.append(0.0)
            param.axisacc.append(acc)

            # 时间戳为0
            param.timeStamp = self.nrc.VectorDouble()
            param.timeStamp.append(0.0)

            # 发送清空命令
            with self.nrc_lock:
                ret = self.nrc.servo_move(self.socket_fd_7000, param)

            if ret != 0:
                logging.warning(f"Clear command {i+1}/3 failed: ret={ret}")
            else:
                logging.debug(f"Clear command {i+1}/3 sent")

            # 短暂延迟
            import time
            time.sleep(0.05)

    def _send_waypoints_to_robot(self, timestamped_waypoints, original_waypoints=None):
        """
        发送航点到机械臂（使用端口7000）

        注意：
        - 作业必须在调用此方法前已启动
        - 作业启动逻辑已移至 _trapq_poll_callback() 中

        参数：
            timestamped_waypoints: [(relative_time_ms, x, y, z, rx, ry, rz), ...]  # 7维航点
            original_waypoints: [(abs_time, x, y, z, rx, ry, rz), ...] 原始航点（用于同步日志计算）
        """
        if not timestamped_waypoints:
            logging.warning("No waypoints to send (empty list)")
            return

        if len(timestamped_waypoints) == 0:
            logging.warning("No waypoints to send (zero length)")
            return

        logging.info(f"Preparing to send {len(timestamped_waypoints)} waypoints...")

        # === 批次间姿态连续性检查和过渡航点插入 ===
        # 如果上一批发送了航点，检查新批次第一个航点与上一批最后一个航点的姿态差
        # 如果姿态变化过大，插入过渡航点以确保角速度不超限
        if self.last_sent_waypoint is not None and timestamped_waypoints:
            import math

            last_wp = self.last_sent_waypoint  # (ts_ms, x, y, z, rx, ry, rz)
            first_wp = timestamped_waypoints[0]

            # 解析姿态（弧度）
            last_rx, last_ry, last_rz = last_wp[4], last_wp[5], last_wp[6]
            first_rx, first_ry, first_rz = first_wp[4], first_wp[5], first_wp[6]

            # 计算姿态变化（使用最短路径）
            def angle_diff(a1, a2):
                """计算两个角度之间的最短路径差（弧度）"""
                diff = a2 - a1
                while diff > math.pi:
                    diff -= 2 * math.pi
                while diff < -math.pi:
                    diff += 2 * math.pi
                return diff

            drx = angle_diff(last_rx, first_rx)
            dry = angle_diff(last_ry, first_ry)
            drz = angle_diff(last_rz, first_rz)
            delta_theta = math.sqrt(drx*drx + dry*dry + drz*drz)  # 总角度变化（弧度）
            delta_theta_deg = math.degrees(delta_theta)

            # 计算时间差（毫秒）
            time_diff_ms = first_wp[0] - last_wp[0]

            if time_diff_ms > 0 and delta_theta_deg > 0.1:
                # 计算角速度（度/秒）
                angular_velocity_dps = delta_theta_deg / (time_diff_ms / 1000.0)

                if angular_velocity_dps > self.max_angular_velocity_dps:
                    # 角速度超限，需要扩展时间窗口并插入过渡航点
                    logging.warning(
                        f"[CONTINUITY] ⚠️ 批次间角速度超限: {angular_velocity_dps:.1f}°/s > {self.max_angular_velocity_dps:.1f}°/s "
                        f"(Δθ={delta_theta_deg:.1f}°, Δt={time_diff_ms:.1f}ms)"
                    )

                    # 计算需要的过渡时间（确保角速度不超限）
                    required_time_ms = (delta_theta_deg / self.max_angular_velocity_dps) * 1000.0

                    # 计算需要的时间延展量
                    time_extension_ms = required_time_ms - time_diff_ms
                    if time_extension_ms < 0:
                        time_extension_ms = 0

                    # === 关键修复：扩展时间窗口 ===
                    # 将当前批次所有航点的时间戳向后偏移
                    if time_extension_ms > 0:
                        extended_waypoints = []
                        for wp in timestamped_waypoints:
                            new_ts = wp[0] + time_extension_ms
                            extended_waypoints.append((new_ts, wp[1], wp[2], wp[3], wp[4], wp[5], wp[6]))
                        timestamped_waypoints = extended_waypoints

                        logging.info(
                            f"[CONTINUITY] 📐 扩展时间窗口: {time_diff_ms:.1f}ms → {time_diff_ms + time_extension_ms:.1f}ms "
                            f"(偏移 {time_extension_ms:.1f}ms)"
                        )

                    # 更新 first_wp（因为时间戳已经偏移）
                    first_wp = timestamped_waypoints[0]

                    # 计算需要插入的过渡航点数量
                    # 新的时间窗口 = time_diff_ms + time_extension_ms = required_time_ms
                    new_time_window_ms = time_diff_ms + time_extension_ms
                    sampling_interval_ms = 50.0  # 采样间隔（毫秒），降低以获得更平滑的过渡
                    num_transition_points = max(1, int(new_time_window_ms / sampling_interval_ms))

                    # 生成过渡航点（在扩展后的时间窗口内均匀分布）
                    # 关键修复：位置锁定为上一批次最后位置，只让姿态变化
                    # 这样 NRC 控制器在执行姿态过渡时不会产生位置偏移
                    transition_waypoints = []
                    locked_x = last_wp[1]  # 锁定位置
                    locked_y = last_wp[2]
                    locked_z = last_wp[3]

                    for i in range(1, num_transition_points + 1):
                        ratio = i / (num_transition_points + 1)

                        # 时间戳：从上一批最后时间戳开始，均匀分布到当前批次第一个时间戳（已偏移）
                        t_ms = last_wp[0] + (first_wp[0] - last_wp[0]) * ratio

                        # 位置：锁定为上一批次最后位置（关键修复！）
                        x = locked_x
                        y = locked_y
                        z = locked_z

                        # 姿态：使用最短路径插值
                        rx = last_rx + drx * ratio
                        ry = last_ry + dry * ratio
                        rz = last_rz + drz * ratio

                        transition_waypoints.append((t_ms, x, y, z, rx, ry, rz))

                    # 插入过渡航点到当前批次开头
                    timestamped_waypoints = transition_waypoints + list(timestamped_waypoints)

                    logging.info(
                        f"[CONTINUITY] ✅ 插入 {num_transition_points} 个过渡航点以确保姿态连续性 "
                        f"(新批次大小: {len(timestamped_waypoints)}, 过渡时间: {new_time_window_ms:.1f}ms)"
                    )

        # 如果是第一批航点，启动延迟测量
        if self.first_waypoint_send_time is None and self.print_start_print_time is not None:
            import time
            import threading

            # 记录发送时间
            self.first_waypoint_send_time = time.perf_counter()

            # 查询并记录当前位置
            try:
                current_pos = self.nrc.VectorDouble()
                ret = self.nrc.get_current_position(self.socket_fd_6001, 0, current_pos)
                if ret == 0:
                    self.initial_position = [current_pos[i] for i in range(6)]
                    logging.info(
                        f"[DELAY] 记录初始位置: "
                        f"X={current_pos[0]:.2f}, Y={current_pos[1]:.2f}, Z={current_pos[2]:.2f}"
                    )

                    # 启动延迟测量线程
                    self.delay_measurement_thread = threading.Thread(
                        target=self._measure_first_waypoint_delay,
                        daemon=True
                    )
                    self.delay_measurement_thread.start()
                    logging.info("[DELAY] 延迟测量线程已启动")
                else:
                    logging.warning(f"[DELAY] 无法查询初始位置，返回码: {ret}")
            except Exception as e:
                logging.error(f"[DELAY] 启动延迟测量失败: {e}", exc_info=True)

        # 验证航点数据有效性
        for i, waypoint in enumerate(timestamped_waypoints):
            # 向后兼容：支持4维和7维航点
            if len(waypoint) == 7:
                t_ms, x, y, z, rx, ry, rz = waypoint
            elif len(waypoint) == 4:
                t_ms, x, y, z = waypoint
                rx, ry, rz = (self.fixed_orientation_rx,
                             self.fixed_orientation_ry,
                             self.fixed_orientation_rz)
            else:
                raise ValueError(f"Invalid waypoint structure at {i}: {waypoint}")

            if not all(isinstance(v, (int, float)) for v in [t_ms, x, y, z, rx, ry, rz]):
                logging.error(f"Invalid waypoint {i}: ({t_ms}, {x}, {y}, {z}, {rx}, {ry}, {rz})")
                raise ValueError(f"Waypoint {i} contains non-numeric values")
            if any(v != v for v in [x, y, z, rx, ry, rz]):  # NaN check
                logging.error(f"NaN detected in waypoint {i}: ({x}, {y}, {z}, {rx}, {ry}, {rz})")
                raise ValueError(f"Waypoint {i} contains NaN values")
            if any(abs(v) == float('inf') for v in [x, y, z, rx, ry, rz]):  # Inf check
                logging.error(f"Inf detected in waypoint {i}: ({x}, {y}, {z}, {rx}, {ry}, {rz})")
                raise ValueError(f"Waypoint {i} contains Inf values")

        # 增加批次计数器
        self.batch_count += 1

        # === 时间戳过期检测 ===
        # 检测航点时间戳是否过期（相对于当前print_time）
        if self.print_start_print_time is not None and timestamped_waypoints:
            current_print_time = self.toolhead.get_last_move_time()
            current_relative_time_ms = (current_print_time - self.print_start_print_time) * 1000.0

            first_waypoint_ts = timestamped_waypoints[0][0]
            last_waypoint_ts = timestamped_waypoints[-1][0]

            # 检测第一个航点是否过期
            if first_waypoint_ts < current_relative_time_ms:
                time_lag_ms = current_relative_time_ms - first_waypoint_ts
                logging.warning(
                    f"[TIMESTAMP] ⚠️ 航点时间戳过期检测:\n"
                    f"  当前时间: {current_relative_time_ms:.1f}ms (相对于打印开始)\n"
                    f"  第一个航点时间戳: {first_waypoint_ts:.1f}ms\n"
                    f"  最后航点时间戳: {last_waypoint_ts:.1f}ms\n"
                    f"  时间滞后: {time_lag_ms:.1f}ms\n"
                    f"  ⚠️ 航点发送延迟过大，可能导致机械臂执行异常！"
                )
            else:
                # 时间戳正常，记录调试信息
                time_ahead_ms = first_waypoint_ts - current_relative_time_ms
                logging.debug(
                    f"[TIMESTAMP] ✅ 航点时间戳正常:\n"
                    f"  当前时间: {current_relative_time_ms:.1f}ms\n"
                    f"  第一个航点时间戳: {first_waypoint_ts:.1f}ms\n"
                    f"  时间提前: {time_ahead_ms:.1f}ms"
                )

        # 创建参数对象
        param = self.nrc.ServoMovePara()
        # clearBuffer策略（配合全局连续时间戳）：
        # - 首次发送时清空缓冲区（is_first_batch=True）
        # - 后续所有命令都追加到缓冲区（is_first_batch=False）
        # - 只在归零、连接、手动重置时重置为True
        param.clearBuffer = self.is_first_batch
        param.targetMode = 1    # 1=连续轨迹
        param.sendMode = 1      # 1=一次传输部分点位（流式发送）
        param.runMode = 1       # 1=边接收边运动（流式发送必须）
        param.coord = 1         # 1=笛卡尔坐标
        param.size = len(timestamped_waypoints)  # 本批次点数
        param.sum = 999         # 总批次数（未知，设为大值）
        param.count = self.batch_count  # 当前批次号（累计，不重置）
        param.extMove = 0       # 扩展移动标志

        # 填充位置和姿态数据（每个航点7个值：x,y,z,rx,ry,rz,ext_axis）
        param.pos = self.nrc.VectorVectorDouble()  # 二维向量
        for waypoint in timestamped_waypoints:
            # 向后兼容：支持4维和7维航点
            if len(waypoint) == 7:  # 包含姿态
                t_ms, x, y, z, rx, ry, rz = waypoint
            elif len(waypoint) == 4:  # 仅位置（向后兼容）
                t_ms, x, y, z = waypoint
                rx, ry, rz = (self.fixed_orientation_rx,
                             self.fixed_orientation_ry,
                             self.fixed_orientation_rz)
            else:
                raise ValueError(f"Invalid waypoint structure: {waypoint}")

            point = self.nrc.VectorDouble()
            point.append(x)
            point.append(y)
            point.append(z)
            point.append(rx)  # 使用插值后的姿态（或固定姿态）
            point.append(ry)
            point.append(rz)
            point.append(0.0)  # 第7个元素：外部轴（默认0.0）
            param.pos.append(point)

        # 填充速度数据（必需字段，避免段错误）
        param.axisvel = self.nrc.VectorVectorDouble()
        for _ in timestamped_waypoints:
            vel = self.nrc.VectorDouble()
            for _ in range(7):
                vel.append(0.0)  # 速度由时间戳控制
            param.axisvel.append(vel)

        # 填充加速度数据（必需字段，避免段错误）
        param.axisacc = self.nrc.VectorVectorDouble()
        for _ in timestamped_waypoints:
            acc = self.nrc.VectorDouble()
            for _ in range(7):
                acc.append(0.0)  # 加速度由时间戳控制
            param.axisacc.append(acc)

        # 填充时间戳（不需要补偿，因为我们的时间戳是相对于start_print_time的）
        param.timeStamp = self.nrc.VectorDouble()
        for waypoint in timestamped_waypoints:
            t_ms = waypoint[0]  # 第一个元素总是时间戳
            param.timeStamp.append(t_ms)

        # 记录关键参数（简化版）
        import math
        logging.info(
            f"servo_move() Parameters (Batch {self.batch_count}): "
            f"clearBuffer={param.clearBuffer}, "
            f"size={param.size}, "
            f"count={param.count}/{param.sum}"
        )
        logging.info(
            f"  First waypoint: pos=({param.pos[0][0]:.2f}, {param.pos[0][1]:.2f}, {param.pos[0][2]:.2f}), "
            f"orient=({math.degrees(param.pos[0][3]):.1f}°, {math.degrees(param.pos[0][4]):.1f}°, {math.degrees(param.pos[0][5]):.1f}°), "
            f"ts={param.timeStamp[0]:.1f}ms"
        )
        if len(timestamped_waypoints) > 1:
            logging.info(
                f"  Last waypoint: pos=({param.pos[-1][0]:.2f}, {param.pos[-1][1]:.2f}, {param.pos[-1][2]:.2f}), "
                f"orient=({math.degrees(param.pos[-1][3]):.1f}°, {math.degrees(param.pos[-1][4]):.1f}°, {math.degrees(param.pos[-1][5]):.1f}°), "
                f"ts={param.timeStamp[-1]:.1f}ms"
            )

        # 发送（使用端口7000 + 互斥锁）
        with self.nrc_lock:
            ret = self.nrc.servo_move(self.socket_fd_7000, param)

        if ret != 0:
            raise self.printer.command_error(
                f"servo_move failed: ret={ret}, waypoints={len(timestamped_waypoints)}"
            )

        # 更新状态
        if self.is_first_batch:
            self.is_first_batch = False
            logging.info(
                "✅ First batch sent (clearBuffer=True), "
                "subsequent commands will append (clearBuffer=False) until reset/homing/reconnect"
            )

        self.total_waypoints_sent += len(timestamped_waypoints)
        logging.info(
            f"✅ Sent {len(timestamped_waypoints)} waypoints "
            f"(total: {self.total_waypoints_sent})"
        )

        # 更新最后发送的航点（用于批次间姿态连续性检查）
        if timestamped_waypoints:
            self.last_sent_waypoint = timestamped_waypoints[-1]

        # 同步调试日志
        self._log_sync_status(timestamped_waypoints, original_waypoints)

    def _log_sync_status(self, timestamped_waypoints, original_waypoints=None):
        """
        记录同步状态调试日志（增强版 - 2025-11-14）

        核心概念说明：
        1. **时间基准偏移**：机械臂和挤出机使用不同的时间基准
           - 挤出机：从print_start_print_time开始计时（0ms）
           - 机械臂：从print_start_print_time + initial_timestamp_offset_ms开始计时（300ms）
           - 时间基准偏移 = 机械臂时间戳 - 挤出机print_time = -initial_timestamp_offset_ms
           - 这个偏移是**设计预期**，用于补偿机械臂启动延迟

        2. **真实同步误差**：实际执行时刻与理想时刻的偏差
           - 理想情况：机械臂在挤出机开始后initial_timestamp_offset_ms时开始执行
           - 实际情况：机械臂在挤出机开始后first_waypoint_execution_delay时开始执行
           - 真实同步误差 = first_waypoint_execution_delay - initial_timestamp_offset_ms
           - 这个误差应该接近0ms（±50ms可接受）

        参数：
            timestamped_waypoints: 刚发送的航点列表 [(ts_ms, x, y, z, rx, ry, rz), ...]
            original_waypoints: 原始航点列表 [(abs_time, x, y, z, rx, ry, rz), ...]，用于准确计算挤出机位置
        """
        if not timestamped_waypoints or self.print_start_print_time is None:
            return

        try:
            # ========== 计算时间值 ==========

            # 1. 挤出机的print_time（相对于打印开始）
            if original_waypoints and len(original_waypoints) > 0:
                last_sent_abs_time = original_waypoints[-1][0]
                extruder_time_ms = (last_sent_abs_time - self.print_start_print_time) * 1000.0
                time_source = "已发送航点"
            else:
                # 降级方案：使用ToolHead的时间（可能包含未发送的moves）
                toolhead = self.printer.lookup_object('toolhead')
                current_print_time = toolhead.get_last_move_time()
                extruder_time_ms = (current_print_time - self.print_start_print_time) * 1000.0
                time_source = "ToolHead总时间（⚠️可能包含未发送moves）"

            # 2. 机械臂的时间戳（相对于机械臂内部时钟t=0）
            arm_timestamp_ms = timestamped_waypoints[-1][0]

            # 3. 时间基准偏移（设计值）
            time_base_offset_ms = extruder_time_ms - arm_timestamp_ms
            design_offset_ms = -self.initial_timestamp_offset_ms

            # 4. 真实同步误差（如果已测量启动延迟）
            if self.first_waypoint_execution_delay is not None:
                real_sync_error_ms = (self.first_waypoint_execution_delay * 1000) - self.initial_timestamp_offset_ms
                has_delay_measurement = True
            else:
                real_sync_error_ms = None
                has_delay_measurement = False

            # ========== 输出日志 ==========

            log_lines = [
                "[SYNC] ═══════════════════════════════════════════════════════════",
                "[SYNC] 同步状态诊断（增强版）",
                "[SYNC] ───────────────────────────────────────────────────────────",
                "[SYNC] 📊 时间基准信息：",
                f"[SYNC]   • 挤出机时间: {extruder_time_ms:.1f}ms (相对于打印开始, 来源: {time_source})",
                f"[SYNC]   • 机械臂时间戳: {arm_timestamp_ms:.1f}ms (相对于机械臂内部时钟)",
                f"[SYNC]   • 批次大小: {len(timestamped_waypoints)} 航点",
                "[SYNC] ───────────────────────────────────────────────────────────",
                "[SYNC] 🎯 时间基准偏移分析：",
                f"[SYNC]   • 实测偏移: {time_base_offset_ms:+.1f}ms (挤出机时间 - 机械臂时间戳)",
                f"[SYNC]   • 设计偏移: {design_offset_ms:+.1f}ms (配置: initial_timestamp_offset_ms={self.initial_timestamp_offset_ms:.1f}ms)",
                f"[SYNC]   • 偏移误差: {abs(time_base_offset_ms - design_offset_ms):.1f}ms",
            ]

            # 添加偏移状态评估
            offset_diff = abs(time_base_offset_ms - design_offset_ms)
            if offset_diff < 1.0:
                log_lines.append("[SYNC]   • 状态: ✅ 时间基准偏移正常（误差<1ms）")
            elif offset_diff < 10.0:
                log_lines.append(f"[SYNC]   • 状态: ⚠️  时间基准偏移轻微漂移（误差{offset_diff:.1f}ms）")
            else:
                log_lines.append(f"[SYNC]   • 状态: ❌ 时间基准偏移异常（误差{offset_diff:.1f}ms）")

            log_lines.append("[SYNC] ───────────────────────────────────────────────────────────")

            # 添加真实同步误差信息
            if has_delay_measurement:
                log_lines.extend([
                    "[SYNC] 🔍 真实同步误差分析：",
                    f"[SYNC]   • 实测启动延迟: {self.first_waypoint_execution_delay*1000:.1f}ms (首次航点执行延迟)",
                    f"[SYNC]   • 配置补偿偏移: {self.initial_timestamp_offset_ms:.1f}ms (initial_timestamp_offset_ms)",
                    f"[SYNC]   • 真实同步误差: {real_sync_error_ms:+.1f}ms (实测延迟 - 配置偏移)",
                ])

                # 评估真实同步误差
                if abs(real_sync_error_ms) < 20.0:
                    log_lines.append("[SYNC]   • 状态: ✅ 同步质量优秀（误差<20ms）")
                elif abs(real_sync_error_ms) < 50.0:
                    log_lines.append(f"[SYNC]   • 状态: ✅ 同步质量良好（误差{abs(real_sync_error_ms):.1f}ms）")
                elif abs(real_sync_error_ms) < 100.0:
                    log_lines.append(f"[SYNC]   • 状态: ⚠️  同步质量一般（误差{abs(real_sync_error_ms):.1f}ms）")
                else:
                    log_lines.append(f"[SYNC]   • 状态: ❌ 同步质量较差（误差{abs(real_sync_error_ms):.1f}ms）")
                    log_lines.append(f"[SYNC]   • 建议: 调整 initial_timestamp_offset_ms 为 {self.first_waypoint_execution_delay*1000:.0f}ms")
            else:
                log_lines.extend([
                    "[SYNC] 🔍 真实同步误差分析：",
                    "[SYNC]   • 实测启动延迟: 未测量（等待首次航点执行）",
                    "[SYNC]   • 真实同步误差: 无法计算（需要启动延迟数据）",
                ])

            log_lines.append("[SYNC] ═══════════════════════════════════════════════════════════")

            # 输出所有日志行（合并为一条，避免性能问题）
            logging.info("\n".join(log_lines))

            # ========== 警告检查 ==========

            # 检查时间基准偏移异常
            if offset_diff > 50.0:
                logging.warning(
                    f"[SYNC] ⚠️  警告：时间基准偏移异常！\n"
                    f"[SYNC]     实测偏移: {time_base_offset_ms:+.1f}ms\n"
                    f"[SYNC]     设计偏移: {design_offset_ms:+.1f}ms\n"
                    f"[SYNC]     偏移误差: {offset_diff:.1f}ms\n"
                    f"[SYNC]     可能原因：时间戳计算错误或print_start_print_time未正确初始化"
                )

            # 检查真实同步误差过大
            if has_delay_measurement and abs(real_sync_error_ms) > 100.0:
                logging.warning(
                    f"[SYNC] ⚠️  警告：真实同步误差过大！\n"
                    f"[SYNC]     实测启动延迟: {self.first_waypoint_execution_delay:.1f}ms\n"
                    f"[SYNC]     配置补偿偏移: {self.initial_timestamp_offset_ms:.1f}ms\n"
                    f"[SYNC]     真实同步误差: {real_sync_error_ms:+.1f}ms\n"
                    f"[SYNC]     建议：将 initial_timestamp_offset_ms 调整为 {self.first_waypoint_execution_delay:.0f}ms"
                )

        except Exception as e:
            logging.error(f"[SYNC] 同步状态日志失败: {e}", exc_info=True)

    def _stop_job_internal(self):
        """
        停止当前作业（内部方法）

        返回：
            True: 成功停止
            False: 停止失败
        """
        try:
            logging.info("正在停止当前作业...")
            with self.nrc_lock:
                ret = self.nrc.job_stop(self.socket_fd_6001)

            if ret == 0 or ret == -4:  # 0=成功, -4=已停止
                logging.info("✅ 作业已停止")
                return True
            else:
                logging.warning(f"⚠️  停止作业返回: {ret}")
                return False
        except Exception as e:
            logging.error(f"❌ 停止作业异常: {e}")
            return False

    def _auto_start_job_with_name(self, job_name):
        """
        自动化启动指定名称的作业

        执行步骤：
        1. 清除错误
        2. 设置伺服就绪
        3. 打开作业文件
        4. 切换到运行模式
        5. 启动作业运行

        参数：
            job_name: 作业名称

        返回：
            True: 成功启动
            False: 启动失败
        """
        import time

        try:
            logging.info(f"🤖 开始自动化启动作业: {job_name}")

            # 步骤1: 清除错误
            logging.info("步骤1/5: 清除系统错误...")
            with self.nrc_lock:
                ret = self.nrc.clear_error(self.socket_fd_6001)
            if ret == 0 or ret == -4:
                logging.info("✅ 系统错误已清除")
            time.sleep(0.3)

            # 步骤2: 设置伺服就绪（如果未运行）
            logging.info("步骤2/5: 检查伺服状态...")
            with self.nrc_lock:
                ret_list = self.nrc.get_servo_state(self.socket_fd_6001, 0)

            current_state = 0
            if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
                current_state = ret_list[1]
                state_names = {0: "停止", 1: "就绪", 2: "报警", 3: "运行"}
                logging.info(f"当前伺服状态: {state_names.get(current_state, '未知')} ({current_state})")

            # 如果不是运行状态，设置伺服就绪
            if current_state != 3:
                logging.info("设置伺服就绪...")
                with self.nrc_lock:
                    ret = self.nrc.set_servo_state(self.socket_fd_6001, 1)  # 1=就绪
                if ret == 0 or ret == -4:
                    logging.info("✅ 伺服已就绪")
                else:
                    logging.warning(f"⚠️ 设置伺服就绪返回: {ret}，继续执行")
            else:
                logging.info("✅ 伺服已运行，跳过设置就绪步骤")
            time.sleep(0.5)

            # 步骤3: 打开作业文件
            logging.info(f"步骤3/5: 打开作业文件 '{job_name}'...")
            with self.nrc_lock:
                ret = self.nrc.job_open(self.socket_fd_6001, job_name)
            if ret == 0 or ret == -4 or ret == -3:
                logging.info(f"✅ 作业 '{job_name}' 已打开")
            else:
                logging.error(f"❌ 打开作业失败，错误码: {ret}")
                return False
            time.sleep(0.5)

            # 步骤4: 切换到运行模式
            logging.info("步骤4/5: 切换到运行模式...")
            with self.nrc_lock:
                ret = self.nrc.set_current_mode(self.socket_fd_6001, 2)  # 2=运行模式
            if ret == 0 or ret == -4:
                logging.info("✅ 已切换到运行模式")
            else:
                logging.error(f"❌ 切换运行模式失败，错误码: {ret}")
                return False
            time.sleep(0.5)

            # 步骤5: 启动作业运行
            logging.info(f"步骤5/5: 启动作业 '{job_name}' 运行...")
            with self.nrc_lock:
                ret = self.nrc.job_run(self.socket_fd_6001, job_name)
            if ret == 0 or ret == -4 or ret == -3:
                logging.info(f"✅ 作业 '{job_name}' 已启动运行")
            else:
                logging.error(f"❌ 启动作业运行失败，错误码: {ret}")
                return False
            time.sleep(0.5)

            logging.info(f"🎉 自动化启动作业 '{job_name}' 完成！")
            return True

        except Exception as e:
            logging.error(f"❌ 自动化启动作业异常: {e}")
            import traceback
            traceback.print_exc()
            return False

    def move_to_home_position_blocking(self, home_pos, home_speed):
        """
        阻塞式移动机械臂到归零点（使用专用归零作业）

        使用预先创建的专用归零作业 GUILIN 执行归零操作。
        此方法用于归零操作，确保机械臂实际位置与 Klipper 内部状态一致。

        执行流程：
        1. 停止当前作业（如果有）
        2. 启动归零作业
        3. 等待作业完成（轮询运行状态）
        4. 验证位置（用户坐标系，容差2mm）
        5. 停止归零作业
        6. 重置作业状态（但不重置时间基准）

        时间管理：
        - 归零操作不影响时间基准（print_start_print_time）
        - 保持 print_time 的连续性，符合传统打印机设计
        - 归零只是状态重置，不参与打印时间管理

        参数：
            home_pos: [x, y, z] 归零点坐标（用户坐标系，单位：mm）
            home_speed: 归零速度（mm/s）- 注意：实际速度由归零作业内部定义

        异常：
            如果未连接、移动失败或超时，抛出 printer.command_error
        """
        import time

        # 检查连接状态
        if self.socket_fd_6001 <= 0:
            raise self.printer.command_error(
                "Cannot home: robot arm not connected. Please run NRC_CONNECT first."
            )

        logging.info("=" * 60)
        logging.info("开始归零移动（使用专用归零作业）")
        logging.info(f"  目标位置: X={home_pos[0]:.2f}, Y={home_pos[1]:.2f}, Z={home_pos[2]:.2f}")
        logging.info(f"  归零作业: {self.homing_job_name}")
        logging.info("=" * 60)

        # 步骤1: 停止当前作业（如果有）
        if self.job_started:
            logging.info("检测到作业正在运行，先停止...")
            if not self._stop_job_internal():
                logging.warning("停止作业失败，但继续执行归零")

        # 步骤2: 启动归零作业（会自动上电）
        logging.info(f"正在启动归零作业 '{self.homing_job_name}'（作业启动会自动上电）...")
        if not self._auto_start_job_with_name(self.homing_job_name):
            raise self.printer.command_error(
                f"Failed to start homing job '{self.homing_job_name}'. "
                f"Please ensure the job exists in the robot controller."
            )

        # 步骤3: 等待作业完成（轮询运行状态）
        logging.info("等待归零作业完成...")
        timeout = 30.0  # 30秒超时
        poll_interval = 0.5  # 500ms 轮询间隔
        start_time = time.time()

        while True:
            elapsed = time.time() - start_time
            if elapsed > timeout:
                raise self.printer.command_error(
                    f"Homing timeout after {timeout:.1f}s"
                )

            # 查询机器人运行状态
            running_status = 0
            with self.nrc_lock:
                ret_list = self.nrc.get_robot_running_state(self.socket_fd_6001, running_status)

            if isinstance(ret_list, list) and len(ret_list) > 1:
                actual_state = ret_list[1]
                # 0=空闲, 1=运行中, 2=暂停
                if actual_state == 0:
                    logging.info(f"✅ 归零作业已完成（耗时 {elapsed:.1f}s）")
                    break

            time.sleep(poll_interval)

        # 步骤4: 验证位置（用户坐标系，容差2mm）
        logging.info("验证归零位置...")

        # 等待一小段时间，确保机械臂完全停止
        time.sleep(0.2)

        pos_vec = self.nrc.VectorDouble()
        with self.nrc_lock:
            ret = self.nrc.get_current_position(
                self.socket_fd_6001, 3, pos_vec  # coord=3用户坐标系
            )

        # 详细记录返回值
        logging.info(f"get_current_position() 返回码: {ret}")
        logging.info(f"pos_vec 大小: {pos_vec.size()}")

        if ret == 0 and pos_vec.size() >= 3:
            # 提取XYZ坐标（前3个元素）
            current_xyz = [pos_vec[0], pos_vec[1], pos_vec[2]]
            logging.info(f"当前位置: X={current_xyz[0]:.2f}, Y={current_xyz[1]:.2f}, Z={current_xyz[2]:.2f}")

            # 计算位置误差
            import math
            error = math.sqrt(
                (current_xyz[0] - home_pos[0])**2 +
                (current_xyz[1] - home_pos[1])**2 +
                (current_xyz[2] - home_pos[2])**2
            )

            tolerance = 2.0  # 2mm 容差
            if error > tolerance:
                logging.warning(
                    f"⚠️  归零位置误差较大: {error:.2f}mm (容差: {tolerance:.2f}mm)"
                )
            else:
                logging.info(f"✅ 归零位置验证通过，误差: {error:.2f}mm")
        else:
            logging.warning(
                f"⚠️  无法获取当前位置，跳过位置验证 "
                f"(ret={ret}, pos_vec.size()={pos_vec.size()})"
            )

        # 步骤5: 停止归零作业
        logging.info("停止归零作业...")
        self._stop_job_internal()

        # 步骤6: 重置作业状态和轨迹提取器状态
        # 注意：标记 move 已被轨迹提取器过滤，所以可以安全地设置 job_started=False
        self.job_started = False
        self.job_name = None

        # 重置轨迹提取器状态（关键：避免跳过后续命令）
        self.last_sent_time = 0.0
        self.accumulated_moves = []
        self.last_move_end_time = None
        self.idle_count = 0
        self.last_print_time = None
        self.batch_count = 0
        self.is_first_batch = True
        self.last_sent_waypoint = None  # 重置批次间连续性跟踪
        self.coord_transform_logged = False  # 重置坐标转换日志标志

        # 重置姿态状态（关键修复：归零后姿态应该回到初始固定姿态）
        with self.orientation_lock:
            self.orientation_queue = []  # 清空姿态队列
            self.last_orientation = None  # 重置最后姿态
            self.planned_orientation = None  # 重置规划姿态（让 get_current_orientation 返回固定姿态）
        logging.info("✅ 姿态状态已重置为固定姿态 (%.1f°, %.1f°, %.1f°)" % (
            math.degrees(self.fixed_orientation_rx),
            math.degrees(self.fixed_orientation_ry),
            math.degrees(self.fixed_orientation_rz)
        ))

        # 注意：归零操作不重置时间基准，保持 print_time 的连续性
        # 这符合传统打印机的设计，归零不影响打印时间管理
        logging.info("✅ 归零完成，时间基准保持不变（符合传统打印机设计）")

        logging.info("✅ 作业状态和轨迹提取器状态已重置")

        logging.info("=" * 60)
        logging.info("归零移动完成")
        logging.info("=" * 60)

    def _ensure_robot_powered_on(self):
        """
        确保机械臂已上电

        检查机械臂伺服状态，如果未上电则自动执行上电流程：
        1. 清除错误
        2. 设置伺服就绪
        3. 上电

        异常：
            如果上电失败，抛出 printer.command_error
        """
        import time

        # 检查当前伺服状态
        logging.info("检查机械臂伺服状态...")
        with self.nrc_lock:
            ret_list = self.nrc.get_servo_state(self.socket_fd_6001, 0)

        if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
            current_state = ret_list[1]
            state_names = {0: "停止", 1: "就绪", 2: "报警", 3: "运行"}
            logging.info(f"当前伺服状态: {state_names.get(current_state, '未知')} ({current_state})")

            # 如果已经在运行状态，无需上电
            if current_state == 3:
                logging.info("✅ 机械臂已上电，无需重复上电")
                return

            # 如果处于报警状态，需要先清除错误
            if current_state == 2:
                logging.info("⚠️ 机械臂处于报警状态，清除错误...")
                with self.nrc_lock:
                    ret = self.nrc.clear_error(self.socket_fd_6001)
                if ret != 0 and ret != -4:
                    raise self.printer.command_error(
                        f"清除错误失败: ret={ret}"
                    )
                time.sleep(0.5)
        else:
            logging.warning(f"无法获取伺服状态: {ret_list}，尝试执行上电流程...")

        # 执行上电流程
        logging.info("开始上电流程...")

        # 步骤1: 清除错误
        logging.info("  步骤1/3: 清除错误...")
        with self.nrc_lock:
            ret = self.nrc.clear_error(self.socket_fd_6001)
        if ret == 0:
            logging.info("  ✅ 错误已清除")
        elif ret == -4:
            logging.info("  ✅ 无错误需要清除")
        else:
            logging.warning(f"  ⚠️ clear_error 返回 {ret}，继续...")
        time.sleep(0.3)

        # 步骤2: 设置伺服就绪
        logging.info("  步骤2/3: 设置伺服就绪...")
        with self.nrc_lock:
            ret = self.nrc.set_servo_state(self.socket_fd_6001, 1)
        if ret == 0:
            logging.info("  ✅ 伺服已就绪")
        elif ret == -4:
            logging.info("  ✅ 伺服已就绪（已处于正确状态）")
        else:
            raise self.printer.command_error(
                f"设置伺服就绪失败: ret={ret}"
            )
        time.sleep(0.5)

        # 步骤3: 上电
        logging.info("  步骤3/3: 机械臂上电...")
        with self.nrc_lock:
            ret = self.nrc.set_servo_poweron(self.socket_fd_6001)
        if ret != 0:
            raise self.printer.command_error(
                f"机械臂上电失败: ret={ret}"
            )
        logging.info("  ✅ 机械臂上电成功")
        time.sleep(2.0)  # 等待上电完成

        # 验证上电状态
        with self.nrc_lock:
            ret_list = self.nrc.get_servo_state(self.socket_fd_6001, 0)
        if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
            final_state = ret_list[1]
            if final_state == 3:
                logging.info("✅ 上电流程完成，机械臂处于运行状态")
            else:
                logging.warning(f"⚠️ 上电后状态异常: {final_state}（期望3=运行）")

        logging.info("=" * 60)

    def power_off_robot(self):
        """
        机械臂下电

        安全地将机械臂下电，用于归零完成后或其他需要下电的场景。

        注意：下电仅在伺服状态=3（运行）时有效。如果机械臂已经停止运动但仍处于
        运行状态，下电会成功。如果伺服状态不是3，下电会失败但不抛出异常。

        异常：
            如果未连接，抛出 printer.command_error
        """
        import time

        # 检查连接状态
        if self.socket_fd_6001 <= 0:
            raise self.printer.command_error(
                "Cannot power off: robot arm not connected."
            )

        logging.info("=" * 60)
        logging.info("开始机械臂下电...")

        # 检查当前伺服状态
        with self.nrc_lock:
            ret_list = self.nrc.get_servo_state(self.socket_fd_6001, 0)

        if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
            servo_state = ret_list[1]
            state_names = {0: "停止", 1: "就绪", 2: "报警", 3: "运行"}
            logging.info(f"当前伺服状态: {state_names.get(servo_state, '未知')} ({servo_state})")

            if servo_state != 3:
                logging.warning(
                    f"⚠️ 伺服状态不是运行(3)，无法下电。"
                    f"当前状态: {state_names.get(servo_state, '未知')} ({servo_state})"
                )
                logging.info("=" * 60)
                return  # 不抛出异常，只是跳过下电

        # 执行下电
        with self.nrc_lock:
            ret = self.nrc.set_servo_poweroff(self.socket_fd_6001)

        if ret == 0:
            logging.info("✅ 机械臂下电成功")
        elif ret == -1:
            logging.warning("⚠️ 下电失败: 操作不允许（可能伺服状态不正确）")
        else:
            logging.warning(f"⚠️ 下电返回非零值: ret={ret}")

        logging.info("=" * 60)

    # ========== 坐标系转换方法 ==========

    def _euler_zyx_to_rotation_matrix(self, rz, ry, rx):
        """
        将 ZYX 欧拉角转换为旋转矩阵（轻量实现，无 numpy 依赖）

        参数：
            rz, ry, rx: 绕 Z, Y, X 轴的旋转角度（弧度）

        返回：
            3x3 旋转矩阵（列表形式）
        """
        import math

        # 计算三角函数值
        cos_rx, sin_rx = math.cos(rx), math.sin(rx)
        cos_ry, sin_ry = math.cos(ry), math.sin(ry)
        cos_rz, sin_rz = math.cos(rz), math.sin(rz)

        # Rx 矩阵（绕 X 轴旋转）
        Rx = [
            [1, 0, 0],
            [0, cos_rx, -sin_rx],
            [0, sin_rx, cos_rx]
        ]

        # Ry 矩阵（绕 Y 轴旋转）
        Ry = [
            [cos_ry, 0, sin_ry],
            [0, 1, 0],
            [-sin_ry, 0, cos_ry]
        ]

        # Rz 矩阵（绕 Z 轴旋转）
        Rz = [
            [cos_rz, -sin_rz, 0],
            [sin_rz, cos_rz, 0],
            [0, 0, 1]
        ]

        # 组合旋转：R = Rz * Ry * Rx
        # 先计算 Ry * Rx
        RyRx = [[0]*3 for _ in range(3)]
        for i in range(3):
            for j in range(3):
                RyRx[i][j] = sum(Ry[i][k] * Rx[k][j] for k in range(3))

        # 再计算 Rz * (Ry * Rx)
        R = [[0]*3 for _ in range(3)]
        for i in range(3):
            for j in range(3):
                R[i][j] = sum(Rz[i][k] * RyRx[k][j] for k in range(3))

        return R

    def _build_transform_matrix(self, x, y, z, rx, ry, rz):
        """
        构建 4x4 齐次变换矩阵

        参数：
            x, y, z: 平移向量（mm）
            rx, ry, rz: 欧拉角（弧度）

        返回：
            4x4 齐次变换矩阵（列表形式）
        """
        # 计算旋转矩阵
        R = self._euler_zyx_to_rotation_matrix(rz, ry, rx)

        # 构建 4x4 齐次变换矩阵
        T = [
            [R[0][0], R[0][1], R[0][2], x],
            [R[1][0], R[1][1], R[1][2], y],
            [R[2][0], R[2][1], R[2][2], z],
            [0, 0, 0, 1]
        ]

        return T

    def _read_user_coord_transform(self):
        """
        从机械臂读取用户坐标系参数并构建变换矩阵

        返回：
            4x4 齐次变换矩阵，如果失败返回 None
        """
        if self.socket_fd_6001 <= 0:
            logging.error("Cannot read user coord: not connected")
            return None

        try:
            import math

            # 调用 NRC API 读取用户坐标系参数
            pos_vec = self.nrc.VectorDouble()
            with self.nrc_lock:
                ret = self.nrc.get_user_coord_para(
                    self.socket_fd_6001,
                    self.user_coord_number,
                    pos_vec
                )

            if ret != 0:
                logging.error(f"Failed to read user coord {self.user_coord_number}: ret={ret}")
                return None

            if len(pos_vec) < 6:
                logging.error(f"Invalid user coord params: len={len(pos_vec)} < 6")
                return None

            # 提取参数
            x, y, z = pos_vec[0], pos_vec[1], pos_vec[2]
            rx, ry, rz = pos_vec[3], pos_vec[4], pos_vec[5]

            # 构建变换矩阵
            T = self._build_transform_matrix(x, y, z, rx, ry, rz)

            # 记录日志
            logging.info("=" * 60)
            logging.info(f"User Coordinate System {self.user_coord_number} Loaded")
            logging.info(f"  Translation: X={x:.4f}mm, Y={y:.4f}mm, Z={z:.4f}mm")
            logging.info(f"  Rotation (rad): Rx={rx:.6f}, Ry={ry:.6f}, Rz={rz:.6f}")
            logging.info(f"  Rotation (deg): Rx={math.degrees(rx):.4f}°, Ry={math.degrees(ry):.4f}°, Rz={math.degrees(rz):.4f}°")
            logging.info("=" * 60)
            # 输出完整变换矩阵 (MATLAB 格式)
            logging.info("[TRANSFORM_MATRIX] 4x4 Homogeneous Transform (Base <- User):")
            logging.info(f"[TRANSFORM_MATRIX] T = [")
            logging.info(f"[TRANSFORM_MATRIX]   {T[0][0]:.10f}, {T[0][1]:.10f}, {T[0][2]:.10f}, {T[0][3]:.4f};")
            logging.info(f"[TRANSFORM_MATRIX]   {T[1][0]:.10f}, {T[1][1]:.10f}, {T[1][2]:.10f}, {T[1][3]:.4f};")
            logging.info(f"[TRANSFORM_MATRIX]   {T[2][0]:.10f}, {T[2][1]:.10f}, {T[2][2]:.10f}, {T[2][3]:.4f};")
            logging.info(f"[TRANSFORM_MATRIX]   {T[3][0]:.10f}, {T[3][1]:.10f}, {T[3][2]:.10f}, {T[3][3]:.4f}]")
            logging.info("=" * 60)

            return T

        except Exception as e:
            logging.exception(f"Error reading user coord transform: {e}")
            return None

    def _read_and_log_tcp_params(self):
        """
        读取并输出 TCP（工具坐标系）参数

        TCP 参数对于姿态调整时末端位置保持至关重要：
        - 如果 TCP 偏移不正确，姿态旋转时末端会产生位移
        - 正确的 TCP 应该指向喷嘴尖端
        """
        if self.socket_fd_6001 <= 0:
            logging.warning("Cannot read TCP params: not connected")
            return

        try:
            import math

            # 默认使用工具编号 1（大多数情况下都是用工具1）
            current_tool = 1
            logging.info(f"Reading TCP for Tool Number: {current_tool}")

            # 读取工具参数
            tcp_param = self.nrc.ToolParam()
            with self.nrc_lock:
                ret = self.nrc.get_tool_hand_param(
                    self.socket_fd_6001,
                    current_tool,
                    tcp_param
                )

            if ret != 0:
                logging.warning(f"Failed to get TCP params for tool {current_tool}: ret={ret}")
                return

            # 输出 TCP 参数
            logging.info("=" * 60)
            logging.info(f"[TCP] Tool {current_tool} Parameters (Tool Center Point):")
            logging.info(f"  Position: X={tcp_param.X:.4f}mm, Y={tcp_param.Y:.4f}mm, Z={tcp_param.Z:.4f}mm")
            logging.info(f"  Rotation: A={math.degrees(tcp_param.A):.4f}°, B={math.degrees(tcp_param.B):.4f}°, C={math.degrees(tcp_param.C):.4f}°")
            logging.info(f"  Rotation (rad): A={tcp_param.A:.6f}, B={tcp_param.B:.6f}, C={tcp_param.C:.6f}")
            logging.info(f"  Payload Mass: {tcp_param.payloadMass:.2f}kg")
            logging.info(f"  Mass Center: ({tcp_param.payloadMassCenter_X:.2f}, {tcp_param.payloadMassCenter_Y:.2f}, {tcp_param.payloadMassCenter_Z:.2f})mm")
            logging.info("=" * 60)

            # 检查 TCP 是否可能不正确
            tcp_offset = math.sqrt(tcp_param.X**2 + tcp_param.Y**2 + tcp_param.Z**2)
            if tcp_offset < 1.0:
                logging.warning(
                    f"[TCP] ⚠️ TCP 偏移量较小 ({tcp_offset:.2f}mm)，"
                    f"可能导致姿态旋转时末端位置偏移。请确认喷嘴是否在 TCP 位置。"
                )

        except Exception as e:
            logging.exception(f"Error reading TCP params: {e}")

    def _transform_user_to_base(self, x_user, y_user, z_user):
        """
        将用户坐标系中的点转换到基坐标系

        参数：
            x_user, y_user, z_user: 用户坐标系中的坐标（mm）

        返回：
            (x_base, y_base, z_base): 基坐标系中的坐标（mm）
        """
        if self.T_base_user is None:
            # 如果未读取变换矩阵，直接返回原坐标（不转换）
            logging.warning("Transform matrix not loaded, using user coords directly")
            return x_user, y_user, z_user

        try:
            # 输入验证
            if any(v != v for v in [x_user, y_user, z_user]):  # NaN check
                logging.error(f"NaN in user coords: ({x_user}, {y_user}, {z_user})")
                return x_user, y_user, z_user

            # 齐次坐标
            p_user = [x_user, y_user, z_user, 1.0]

            # 矩阵乘法：p_base = T_base_user @ p_user
            p_base = [
                sum(self.T_base_user[i][j] * p_user[j] for j in range(4))
                for i in range(3)
            ]

            # 输出验证
            if any(v != v for v in p_base):  # NaN check
                logging.error(f"NaN in transformed coords: ({p_base[0]}, {p_base[1]}, {p_base[2]})")
                logging.error(f"Input was: ({x_user}, {y_user}, {z_user})")
                return x_user, y_user, z_user

            # 首次转换时记录日志
            if not self.coord_transform_logged:
                logging.info("=" * 60)
                logging.info("Coordinate Transform Applied")
                logging.info(f"  User Coord: ({x_user:.2f}, {y_user:.2f}, {z_user:.2f})")
                logging.info(f"  Base Coord: ({p_base[0]:.2f}, {p_base[1]:.2f}, {p_base[2]:.2f})")
                logging.info("=" * 60)
                self.coord_transform_logged = True

            return p_base[0], p_base[1], p_base[2]

        except Exception as e:
            logging.exception(f"Error in coordinate transform: {e}")
            logging.error(f"Input: ({x_user}, {y_user}, {z_user})")
            # 发生错误时返回原坐标
            return x_user, y_user, z_user

    # ========== 姿态管理方法 ==========

    def get_current_orientation(self):
        """
        获取当前姿态（用于 gcode_move 拆分判断）

        返回:
            (rx, ry, rz): 当前姿态（弧度）

        修复说明（2025-01-08）：
            - 原逻辑：返回 last_orientation（最后入队的姿态）
            - 问题：如果提前入队了未来姿态，会返回错误的值
            - 修复：使用 planned_orientation 跟踪当前规划的姿态
                   只在 Move 实际添加后才更新
        """
        with self.orientation_lock:
            # 使用 planned_orientation（当前规划的姿态）
            # 而不是 last_orientation（最后入队的姿态）
            if hasattr(self, 'planned_orientation') and self.planned_orientation is not None:
                return self.planned_orientation
            elif self.last_orientation is not None:
                return (self.last_orientation.rx,
                        self.last_orientation.ry,
                        self.last_orientation.rz)
            else:
                # 返回固定姿态
                return (self.fixed_orientation_rx,
                        self.fixed_orientation_ry,
                        self.fixed_orientation_rz)

    def set_planned_orientation(self, rx, ry, rz):
        """
        设置当前规划的姿态（由 gcode_move 在 Move 添加后调用）

        参数:
            rx, ry, rz: 姿态（弧度）
        """
        with self.orientation_lock:
            self.planned_orientation = (rx, ry, rz)

    def begin_orientation_move(self):
        """
        开始姿态协调移动（阻止航点发送，直到移动完成）

        在 _execute_split_move 开始时调用，确保 Move 1 和 Move 2
        一起被提取和发送，而不是被分开。
        """
        with self.orientation_move_lock:
            self.orientation_move_pending = True
            logging.debug("[ORIENT-LOCK] 姿态协调移动开始，暂停航点发送")

    def end_orientation_move(self):
        """
        结束姿态协调移动（恢复航点发送）

        在 _execute_split_move 结束时调用，所有 Move 都已进入 TrapQ。

        关键修复：主动提取 TrapQ 并立即发送航点，确保姿态调整航点及时发送到机械臂。

        问题根源：
        - flush_step_generation() 只是将 Move 从 lookahead 队列刷新到 TrapQ
        - 但 TrapQ 的提取是异步的（每 10ms 轮询一次）
        - 当 end_orientation_move() 被调用时，Move 可能还没有被提取到 accumulated_moves
        - 导致 _flush_accumulated_moves() 什么都不做

        解决方案：
        - 使用 force_immediate_trapq_send() 主动提取 TrapQ 并立即发送
        - 确保 Move 1 和 Move 2 一起发送，时间戳不会过期
        """
        with self.orientation_move_lock:
            self.orientation_move_pending = False

        # 关键修复：主动提取 TrapQ 并立即发送
        logging.info("[ORIENT-LOCK] 姿态协调移动结束，主动提取 TrapQ 并立即发送航点")
        self.force_immediate_trapq_send()

    def force_immediate_trapq_send(self):
        """
        强制立即提取 TrapQ 数据并发送航点

        用于姿态协调移动结束后，立即发送所有累积的 waypoints，
        确保与 MCU 挤出命令同步。

        关键改进：
        1. 获取当前 print_time 作为目标结束时间
        2. 循环提取直到覆盖所有 moves
        3. 确保 Move 2 也被包含
        """
        import time

        # 获取目标结束时间（当前 print_time，即所有已加入的 moves 的结束时间）
        target_end_time = self.toolhead.get_last_move_time()
        logging.info(f"[FORCE-SEND] 目标结束时间: {target_end_time:.6f}")

        # 循环提取，最多尝试 10 次，每次等待 10ms
        max_attempts = 10
        for attempt in range(max_attempts):
            # 获取安全提取范围
            extract_start_time, extract_end_time = self._get_safe_extraction_range()

            if extract_start_time is not None:
                logging.info(f"[FORCE-SEND] 尝试 {attempt+1}: 提取范围 {extract_start_time:.6f} - {extract_end_time:.6f}")

                # 提取 moves
                new_moves = self._extract_moves_from_trapq(extract_start_time, extract_end_time)

                if new_moves:
                    # 过滤标记 move
                    new_moves = [m for m in new_moves if self._get_move_attr(m, 'move_t') > 0]
                    if new_moves:
                        # 累积 moves
                        self._accumulate_moves(new_moves)
                        logging.info(f"[FORCE-SEND] 提取并追加了 {len(new_moves)} 个 moves")

            # 检查是否已提取到目标时间
            if self.accumulated_moves:
                last_move = self.accumulated_moves[-1]
                last_move_end = (
                    self._get_move_attr(last_move, 'print_time') +
                    self._get_move_attr(last_move, 'move_t')
                )
                # 如果已经提取到目标时间（允许 10ms 误差），就停止循环
                if last_move_end >= target_end_time - 0.01:
                    logging.info(f"[FORCE-SEND] 已提取到目标时间 {last_move_end:.6f} >= {target_end_time:.6f}")
                    break
                else:
                    logging.info(f"[FORCE-SEND] 当前提取到 {last_move_end:.6f}，目标 {target_end_time:.6f}，继续等待...")

            # 等待一小段时间让 TrapQ finalize
            time.sleep(0.01)  # 10ms

        if not self.accumulated_moves:
            logging.debug("[FORCE-SEND] 没有数据需要发送")
            return

        moves_count = len(self.accumulated_moves)
        moves = self.accumulated_moves
        self.accumulated_moves = []
        self.last_move_end_time = None

        # 重置空闲计数
        self.idle_count = 0
        self.last_print_time = None

        logging.info(f"[FORCE-SEND] 强制发送 {moves_count} 个 moves（姿态协调移动后立即发送）")

        # 记录发送信息
        first_move_time = self._get_move_attr(moves[0], 'print_time')
        last_move = moves[-1]
        last_move_time = (
            self._get_move_attr(last_move, 'print_time') +
            self._get_move_attr(last_move, 'move_t')
        )
        logging.info(f"[FORCE-SEND] 时间范围: {first_move_time:.6f} - {last_move_time:.6f}")

        # 更新统计
        self.total_moves_extracted += len(moves)

        try:
            # 转换为航点
            waypoints = self._convert_moves_to_waypoints(moves)

            if not waypoints:
                logging.warning("[FORCE-SEND] 无法转换为航点")
                return

            # 添加时间戳
            timestamped_waypoints = self._add_timestamps_to_waypoints(waypoints)

            # 验证时间戳
            if not self._validate_timestamps(timestamped_waypoints):
                logging.error("[FORCE-SEND] 时间戳验证失败")
                return

            # 发送航点到机械臂
            self._send_waypoints_to_robot(timestamped_waypoints, waypoints)

            # 更新 last_sent_time
            self.last_sent_time = last_move_time

            logging.info(f"[FORCE-SEND] ✅ 成功发送 {len(waypoints)} 个航点")

        except Exception as e:
            logging.exception(f"[FORCE-SEND] 发送失败: {e}")

    def enqueue_orientation(self, print_time, rx=None, ry=None, rz=None):
        """
        将姿态数据入队

        参数：
            print_time (float): Klipper的print_time时间戳（秒）
            rx (float, optional): 绕X轴旋转角度（弧度），None表示使用固定姿态
            ry (float, optional): 绕Y轴旋转角度（弧度），None表示使用固定姿态
            rz (float, optional): 绕Z轴旋转角度（弧度），None表示使用固定姿态

        说明：
            - 如果ABC参数为None，使用配置文件中的固定姿态值
            - 入队时检测万向节死锁风险（B轴接近±90度）
            - 线程安全：使用orientation_lock保护
        """
        import math

        # 使用固定姿态填充缺失值
        if rx is None:
            rx = self.fixed_orientation_rx
        if ry is None:
            ry = self.fixed_orientation_ry
        if rz is None:
            rz = self.fixed_orientation_rz

        # 万向节死锁风险检测（仅在接近±90°时警告）
        gimbal_threshold = math.radians(85)  # 85度阈值
        # 检测是否接近+90°或-90°（不检测0°附近）
        if abs(abs(ry) - math.pi/2) < math.radians(5):  # 距离±90°在5°以内
            logging.warning(
                f"⚠️ Gimbal lock risk detected: B={math.degrees(ry):.1f}° "
                f"(close to ±90°). Consider using quaternion interpolation."
            )

        # 创建队列条目
        entry = OrientationQueueEntry(print_time, rx, ry, rz)

        # 线程安全入队
        with self.orientation_lock:
            self.orientation_queue.append(entry)
            self.last_orientation = entry  # 更新最后姿态

        logging.debug(
            f"[ORIENTATION] Enqueued: {entry}"
        )

    def _get_orientation_at_time(self, print_time):
        """
        获取指定时刻的姿态（自动选择插值方法）

        参数：
            print_time (float): 查询的时间点（秒）

        返回：
            tuple: (rx, ry, rz) 插值后的欧拉角（弧度）

        插值方法：
            - 根据配置选择：线性插值（默认）或四元数SLERP（可选）
            - use_quaternion_interpolation=true: 使用SLERP，更平滑
            - use_quaternion_interpolation=false: 使用线性插值，更快

        线程安全：
            - 使用orientation_lock保护队列读取
        """
        if self.use_quaternion_interpolation:
            return self._get_orientation_at_time_quaternion(print_time)
        else:
            return self._get_orientation_at_time_linear(print_time)

    def _get_orientation_at_time_linear(self, print_time):
        """
        获取指定时刻的姿态（线性插值，默认方法）

        参数：
            print_time (float): 查询的时间点（秒）

        返回：
            tuple: (rx, ry, rz) 插值后的欧拉角（弧度）
        """
        import math

        with self.orientation_lock:
            if not self.orientation_queue:
                return (self.fixed_orientation_rx,
                        self.fixed_orientation_ry,
                        self.fixed_orientation_rz)

            if len(self.orientation_queue) == 1:
                entry = self.orientation_queue[0]
                return (entry.rx, entry.ry, entry.rz)

            entry_before = None
            entry_after = None

            for i, entry in enumerate(self.orientation_queue):
                if entry.print_time <= print_time:
                    entry_before = entry
                    if i + 1 < len(self.orientation_queue):
                        entry_after = self.orientation_queue[i + 1]
                else:
                    if entry_before is None:
                        entry_before = entry
                    else:
                        entry_after = entry
                    break

            if entry_before is None:
                entry = self.orientation_queue[0]
                return (entry.rx, entry.ry, entry.rz)

            if entry_after is None:
                return (entry_before.rx, entry_before.ry, entry_before.rz)

            t0 = entry_before.print_time
            t1 = entry_after.print_time

            if abs(t1 - t0) < 1e-9:
                return (entry_before.rx, entry_before.ry, entry_before.rz)

            ratio = (print_time - t0) / (t1 - t0)
            ratio = max(0.0, min(1.0, ratio))

            # 使用最短路径角度插值（避免跨越 ±π 边界时绕远路）
            def lerp_angle(a0, a1, t):
                """
                最短路径角度线性插值

                确保从 a0 到 a1 走最短路径，处理 ±π 边界跨越问题。
                例如：从 170° 到 -170° 应该走 +20°（经过 180°），
                而不是 -340°（经过 0°）。
                """
                # 计算角度差，归一化到 [-π, π]
                diff = a1 - a0
                while diff > math.pi:
                    diff -= 2 * math.pi
                while diff < -math.pi:
                    diff += 2 * math.pi
                return a0 + diff * t

            rx = lerp_angle(entry_before.rx, entry_after.rx, ratio)
            ry = lerp_angle(entry_before.ry, entry_after.ry, ratio)
            rz = lerp_angle(entry_before.rz, entry_after.rz, ratio)

            return (rx, ry, rz)

    def _get_orientation_at_time_quaternion(self, print_time):
        """
        获取指定时刻的姿态（四元数SLERP插值，可选方法）

        参数：
            print_time (float): 查询的时间点（秒）

        返回：
            tuple: (rx, ry, rz) 插值后的欧拉角（弧度）

        特性：
            - 最短旋转路径
            - 角速度连续
            - 无万向节死锁
        """
        import math
        from .quaternion_utils import (
            euler_to_quaternion, quaternion_to_euler, quaternion_slerp
        )

        with self.orientation_lock:
            if not self.orientation_queue:
                return (self.fixed_orientation_rx,
                        self.fixed_orientation_ry,
                        self.fixed_orientation_rz)

            if len(self.orientation_queue) == 1:
                entry = self.orientation_queue[0]
                return (entry.rx, entry.ry, entry.rz)

            # 查找时间区间（与线性插值相同）
            entry_before = None
            entry_after = None

            for i, entry in enumerate(self.orientation_queue):
                if entry.print_time <= print_time:
                    entry_before = entry
                    if i + 1 < len(self.orientation_queue):
                        entry_after = self.orientation_queue[i + 1]
                else:
                    if entry_before is None:
                        entry_before = entry
                    else:
                        entry_after = entry
                    break

            if entry_before is None:
                entry = self.orientation_queue[0]
                return (entry.rx, entry.ry, entry.rz)

            if entry_after is None:
                return (entry_before.rx, entry_before.ry, entry_before.rz)

            # 转换为四元数
            q0 = euler_to_quaternion(entry_before.rx, entry_before.ry, entry_before.rz)
            q1 = euler_to_quaternion(entry_after.rx, entry_after.ry, entry_after.rz)

            # 计算插值参数
            t0 = entry_before.print_time
            t1 = entry_after.print_time

            if abs(t1 - t0) < 1e-9:
                return (entry_before.rx, entry_before.ry, entry_before.rz)

            ratio = (print_time - t0) / (t1 - t0)
            ratio = max(0.0, min(1.0, ratio))

            # SLERP插值
            q_interp = quaternion_slerp(q0, q1, ratio)

            # 转回欧拉角
            rx, ry, rz = quaternion_to_euler(q_interp)

            return (rx, ry, rz)

    def get_orientation_change(self, start_time, end_time):
        """
        计算时间区间内的最大姿态变化量（精确四元数方法）

        参数：
            start_time (float): 起始时间（print_time，秒）
            end_time (float): 结束时间（print_time，秒）

        返回：
            float: 最大旋转角度（度）

        用途：
            - 用于nrc_robot_arm.py::check_move()中的时间协调验证
            - 计算公式：delta_theta_max = max(各段旋转角度)
        """
        import math
        from .quaternion_utils import euler_to_quaternion, get_rotation_angle

        with self.orientation_lock:
            if not self.orientation_queue or len(self.orientation_queue) < 2:
                return 0.0

            # 查找时间区间内的姿态条目
            entries_in_range = [
                entry for entry in self.orientation_queue
                if start_time <= entry.print_time <= end_time
            ]

            # 如果区间内没有足够条目，尝试添加边界附近的条目
            if len(entries_in_range) < 2:
                # 查找起始时间前最近的条目
                before_entries = [
                    entry for entry in self.orientation_queue
                    if entry.print_time < start_time
                ]
                if before_entries:
                    entries_in_range.insert(0, before_entries[-1])

                # 查找结束时间后最近的条目
                after_entries = [
                    entry for entry in self.orientation_queue
                    if entry.print_time > end_time
                ]
                if after_entries:
                    entries_in_range.append(after_entries[0])

            if len(entries_in_range) < 2:
                return 0.0

            # 计算相邻条目间的旋转角度（使用四元数精确计算）
            max_delta_deg = 0.0
            for i in range(len(entries_in_range) - 1):
                e0, e1 = entries_in_range[i], entries_in_range[i + 1]

                # 转换为四元数
                q0 = euler_to_quaternion(e0.rx, e0.ry, e0.rz)
                q1 = euler_to_quaternion(e1.rx, e1.ry, e1.rz)

                # 计算旋转角度（弧度）
                angle_rad = get_rotation_angle(q0, q1)
                angle_deg = math.degrees(angle_rad)

                max_delta_deg = max(max_delta_deg, angle_deg)

            return max_delta_deg

    def _cleanup_old_orientations(self, current_print_time):
        """
        清理已过期的姿态数据

        参数：
            current_print_time (float): 当前print_time（秒）

        保留策略：
            - 保留当前时间前1秒内的数据（用于插值）
            - 至少保留1个条目（用于向后查询）

        调用时机：
            - 在_trapq_poll_callback()中每次轮询时调用
        """
        cutoff_time = current_print_time - 1.0  # 保留1秒数据

        with self.orientation_lock:
            # 移除过旧的条目，但保留至少一个
            removed_count = 0
            while len(self.orientation_queue) > 1:
                if self.orientation_queue[0].print_time < cutoff_time:
                    removed = self.orientation_queue.pop(0)
                    removed_count += 1
                else:
                    break

            if removed_count > 0:
                logging.debug(
                    f"[ORIENTATION] Cleaned {removed_count} old entries "
                    f"(cutoff={cutoff_time:.6f}s, remaining={len(self.orientation_queue)})"
                )

    def _adjust_gimbal_lock_threshold(self):
        """
        调整万向节死锁阈值（可选优化）

        注意：当前实现使用85度固定阈值，足够安全
        此方法预留用于未来四元数插值升级
        """
        pass

    # ========== Klipper 事件处理器 ==========


    def _handle_ready(self):
        """
        Klipper 就绪后初始化

        仅初始化状态变量，不自动连接机械臂。
        用户需要手动执行 NRC_CONNECT 命令来连接机械臂。
        """
        # 导入 NRC SDK（仅加载模块，不连接）
        self._import_nrc_sdk()

        # 初始化打印起始时间（将在首次提取时更新）
        self.start_print_time = None

        # 重置状态
        self.is_first_batch = True
        self.first_waypoint_time = None
        self.last_sent_time = 0.0
        self.total_waypoints_sent = 0
        self.total_moves_extracted = 0
        self.poll_count = 0
        self.job_started = False
        self.job_name = None

        # 重置跳过第一批标志
        self.first_batch_skipped = False

        # 初始化打印开始时刻
        self.print_start_print_time = None

        logging.info("=" * 60)
        logging.info("NRC Robot Arm Executor Ready")
        logging.info("  Status: Waiting for connection")
        logging.info("  Use 'NRC_CONNECT' command to connect to robot arm")
        logging.info("=" * 60)
    
    def _trapq_poll_callback(self, eventtime):
        """
        TrapQ 轮询回调函数（10ms 间隔）

        周期性提取 TrapQ 数据，转换为航点，并发送到机械臂。

        参数：
            eventtime: 当前事件时间（reactor.monotonic()）

        返回：
            下次唤醒时间（eventtime + POLL_INTERVAL）
        """
        callback_start_time = eventtime

        try:
            # 检查连接状态
            if self.socket_fd_7000 <= 0:
                logging.error("Servo port not connected, skipping waypoint send")
                return eventtime + POLL_INTERVAL

            # 增加轮询计数
            self.poll_count += 1

            # 检查是否暂停
            if self.is_paused:
                return eventtime + POLL_INTERVAL

            # 检查 ToolHead 是否空闲（命令是否完整）
            process_accumulated = False
            moves = None

            # 使用 check_busy() 检查 lookahead 队列是否为空
            current_print_time, est_print_time, lookahead_empty = self.toolhead.check_busy(eventtime)

            # 更新 idle_count（无论 lookahead 是否为空）
            if self.last_print_time is not None:
                if abs(current_print_time - self.last_print_time) < 0.0001:
                    # print_time 没有增长
                    self.idle_count += 1
                else:
                    # print_time 还在增长，重置计数
                    self.idle_count = 0
            else:
                self.idle_count = 0

            # 更新 last_print_time
            self.last_print_time = current_print_time

            # 判断是否应该发送累积的 moves（使用混合触发策略）
            # 注意：已移除 split_move_in_progress 保护，因为：
            # 1. flush_step_generation() 确保两段 Move 同时进入 TrapQ
            # 2. 强制发送阈值（0.5秒）足够让两段 Move 一起被提取
            moves_to_send = self._should_send_accumulated_moves(self.accumulated_moves, self.idle_count)
            if moves_to_send > 0:
                # 提取应该发送的moves
                moves = self.accumulated_moves[:moves_to_send]

                # 保留剩余的moves（如果有命令边界）
                remaining_moves = self.accumulated_moves[moves_to_send:]
                if remaining_moves:
                    logging.info(
                        f"[SPLIT] Keeping {len(remaining_moves)} moves for next command"
                    )
                    self.accumulated_moves = remaining_moves
                    # 更新last_move_end_time
                    last_remaining_move = remaining_moves[-1]
                    self.last_move_end_time = (
                        self._get_move_attr(last_remaining_move, 'print_time') +
                        self._get_move_attr(last_remaining_move, 'move_t')
                    )
                else:
                    self.accumulated_moves = []
                    self.last_move_end_time = None

                # 重置空闲计数
                self.idle_count = 0
                self.last_print_time = None

                # 注意：不重置 batch_count！批次号应该全局连续递增
                # batch_count 只在归零、连接、手动重置时重置为 0
                # self.batch_count = 0  # ❌ 已删除：会导致 count 不连续
                # 注意：不重置 is_first_batch，保持全局连续发送（避免清空之前命令的缓冲区）
                # is_first_batch 只在归零、连接、手动重置时重置为 True

                process_accumulated = True

            # 如果不是处理累积的 moves，则提取新的 moves
            if not process_accumulated:
                # 获取安全提取范围
                extract_start_time, extract_end_time = self._get_safe_extraction_range()

                # 没有新数据
                if extract_start_time is None:
                    # 每 100 次轮询记录一次（避免日志过多）
                    if self.poll_count % 100 == 0:
                        logging.debug(
                            f"Poll #{self.poll_count}: No new data to extract"
                        )
                    return eventtime + POLL_INTERVAL

                # 提取 moves
                new_moves = self._extract_moves_from_trapq(extract_start_time, extract_end_time)

                if not new_moves:
                    # 没有提取到 moves
                    return eventtime + POLL_INTERVAL

                # 检测并过滤标记 move（marker move）
                # 标记 move 的特征：move_t=0, start_v=0, accel=0
                # 这些 move 由 toolhead.set_position() 创建，用于标记位置变化
                # 注意：不在此处重置状态，状态重置由 G28 归零事件触发
                original_count = len(new_moves)
                marker_moves = [m for m in new_moves if self._get_move_attr(m, 'move_t') == 0]
                new_moves = [m for m in new_moves if self._get_move_attr(m, 'move_t') > 0]

                if marker_moves:
                    logging.debug(
                        f"[MARKER] Filtered {len(marker_moves)} marker move(s) "
                        f"(created by toolhead.set_position())"
                    )

                if not new_moves:
                    # 所有 moves 都是标记 move，跳过
                    return eventtime + POLL_INTERVAL

                # 累积 moves（不立即发送）
                self._accumulate_moves(new_moves)

                logging.debug(
                    f"Accumulated {len(new_moves)} moves "
                    f"(total: {len(self.accumulated_moves)} moves)"
                )

                # 继续累积，等待命令完整
                moves = None

            # 确保 moves 不为空（如果是累积模式，moves可能为None）
            if not moves:
                # 继续累积，等待命令完整
                return eventtime + POLL_INTERVAL

            # 跳过第一批（加速段）
            if self.skip_first_batch and not self.first_batch_skipped:
                self.first_batch_skipped = True
                logging.info(
                    f"[SKIP] Skipping first batch ({len(moves)} moves, "
                    f"likely acceleration segment)"
                )
                # 清空累积缓冲区
                self.accumulated_moves = []
                self.last_move_end_time = None
                self.last_accumulate_time = None
                # 重置空闲计数
                self.idle_count = 0
                self.last_print_time = None
                return eventtime + POLL_INTERVAL

            # 记录发送信息
            first_move_time = self._get_move_attr(moves[0], 'print_time')
            last_move = moves[-1]
            last_move_time = (
                self._get_move_attr(last_move, 'print_time') +
                self._get_move_attr(last_move, 'move_t')
            )
            logging.info(
                f"Sending {len(moves)} moves: time range {first_move_time:.6f} - {last_move_time:.6f}"
            )

            # 更新统计
            self.total_moves_extracted += len(moves)

            # 记录提取信息（使用实际 moves 的时间范围）
            actual_start = self._get_move_attr(moves[0], 'print_time')
            last_pt = self._get_move_attr(moves[-1], 'print_time')
            last_mt = self._get_move_attr(moves[-1], 'move_t')
            actual_end = last_pt + last_mt
            logging.info(
                f"Extracted {len(moves)} moves from TrapQ "
                f"(time range: {actual_start:.6f} - {actual_end:.6f})"
            )

            # 记录详细信息（调试模式）
            self._log_move_details(moves)

            # 转换为航点
            waypoints = self._convert_moves_to_waypoints(moves)

            if waypoints:
                # 记录采样信息（一次性发送）
                logging.info(
                    f"Converted to {len(waypoints)} waypoints (one-time send) "
                    f"(avg interval: {(waypoints[-1][0] - waypoints[0][0]) / max(1, len(waypoints) - 1) * 1000:.1f}ms)"
                )

                # 添加时间戳
                timestamped_waypoints = self._add_timestamps_to_waypoints(waypoints)

                # 验证时间戳
                if not self._validate_timestamps(timestamped_waypoints):
                    logging.error("Timestamp validation failed")
                    # 继续执行，但记录错误

                # 记录时间戳信息
                if timestamped_waypoints:
                    logging.info(
                        f"Timestamped {len(timestamped_waypoints)} waypoints, "
                        f"time range: {timestamped_waypoints[0][0]:.1f}ms - "
                        f"{timestamped_waypoints[-1][0]:.1f}ms"
                    )

                    # 检查是否需要启动作业（在发送数据之前）
                    if not self.job_started:
                        if self.enable_auto_job_start:
                            logging.info("=" * 60)
                            logging.info("🤖 检测到首次航点，先启动作业再发送数据")
                            logging.info(f"🚀 自动启动作业 '{self.auto_job_name}'...")
                            logging.info("=" * 60)
                            try:
                                self._auto_start_job_with_name(self.auto_job_name)
                                self.job_started = True
                                self.job_name = self.auto_job_name
                                logging.info("✅ 作业自动启动成功，准备发送数据")
                            except Exception as e:
                                logging.error(f"❌ 作业自动启动失败: {e}")
                                self.printer.invoke_shutdown(f"Job startup failed: {e}")
                                return self.reactor.NEVER
                        else:
                            # 手动模式：要求用户先启动作业
                            error_msg = (
                                "作业未启动。请先运行 'NRC_START_JOB' 命令，"
                                "或在 printer.cfg 中设置 'enable_auto_job_start: True'"
                            )
                            logging.error(f"❌ {error_msg}")
                            self.printer.invoke_shutdown(error_msg)
                            return self.reactor.NEVER

                    # 发送航点到机械臂
                    try:
                        self._send_waypoints_to_robot(timestamped_waypoints, waypoints)

                        # 发送成功后更新 last_sent_time（避免重复提取）
                        last_move = moves[-1]

                        # 清理过期的姿态数据（新增）
                        if self.last_sent_time > 0:
                            self._cleanup_old_orientations(self.last_sent_time)
                        last_move_end_time = self._get_move_attr(last_move, 'print_time') + \
                                            self._get_move_attr(last_move, 'move_t')
                        self.last_sent_time = last_move_end_time
                        logging.debug(
                            f"Updated last_sent_time to {self.last_sent_time:.6f} "
                            f"(after sending {len(moves)} moves)"
                        )

                    except Exception as e:
                        logging.exception("Failed to send waypoints to robot")
                        self.printer.invoke_shutdown(f"Robot communication error: {e}")
                        return self.reactor.NEVER

                    # 记录统计信息（每 10 次提取记录一次）
                    if self.total_moves_extracted % 10 == 0:
                        self._log_extraction_stats(moves, waypoints, timestamped_waypoints)
            else:
                logging.warning("No waypoints generated from moves")

            # 记录回调耗时（使用 reactor 时间）
            callback_end_time = self.reactor.monotonic()
            elapsed_ms = (callback_end_time - callback_start_time) * 1000.0
            if elapsed_ms > WARN_CALLBACK_TIME_MS:
                logging.warning(
                    f"Poll callback took {elapsed_ms:.2f}ms (> {WARN_CALLBACK_TIME_MS}ms)"
                )

            # 监控轮询回调耗时
            self.poll_callback_times.append(elapsed_ms)
            if elapsed_ms > self.max_poll_callback_time_ms:
                self.max_poll_callback_time_ms = elapsed_ms

            # 告警：轮询回调耗时过长
            if elapsed_ms > 50.0:
                self.warning_count += 1
                logging.warning(
                    f"[MONITOR] ⚠️  轮询回调耗时过长: {elapsed_ms:.2f}ms (> 50ms阈值)"
                )

            # 下次唤醒时间：10ms 后
            return eventtime + POLL_INTERVAL
            
        except Exception as e:
            logging.exception("TrapQ poll error")
            self.printer.invoke_shutdown(f"TrapQ poll failed: {e}")
            return self.reactor.NEVER
    
    # ========== 事件处理 ==========

    def _handle_homing_end(self, homing_state, rails):
        """
        处理归零完成事件

        在 G28 归零完成后：
        1. 发送累积的 moves（如果有）
        2. 重置运动状态（准备新的运动序列）

        参数：
            homing_state: 归零状态对象
            rails: 归零的轨道列表
        """
        logging.info("[HOMING] G28 homing completed, resetting motion state")

        # 如果有累积的 moves，先发送
        if self.accumulated_moves:
            logging.info("[HOMING] Flushing accumulated moves before reset")
            self._flush_accumulated_moves()

        # 重置运动状态
        self._reset_motion_state()

    # ========== Move 累积和命令边界检测 ==========

    def _reset_motion_state(self):
        """
        重置运动状态（用于 G28 归零等场景）

        重置内容：
        1. is_first_batch = True（下次发送清空缓冲区）
        2. last_sent_time = 0.0（重置发送时间）
        3. accumulated_moves = []（清空累积缓冲区）
        4. batch_count = 0（重置批次计数）
        5. last_sent_waypoint = None（重置批次间连续性跟踪）

        注意：不重置 print_start_print_time，保持时间连续性
        这符合传统打印机的设计，归零、暂停等操作不影响打印时间管理
        """
        self.is_first_batch = True
        # 注意：不重置 print_start_print_time，保持时间基准的连续性
        # self.print_start_print_time = None  # ❌ 已删除
        self.last_sent_time = 0.0
        self.accumulated_moves = []
        self.last_move_end_time = None
        self.batch_count = 0
        self.idle_count = 0
        self.last_print_time = None
        self.last_sent_waypoint = None  # 重置批次间连续性跟踪

        logging.debug(
            "[RESET] Motion state reset: "
            "is_first_batch=True, print_start_print_time保持不变, ready for new motion sequence"
        )

    def _flush_accumulated_moves(self):
        """
        立即发送所有累积的 moves（用于边界情况）

        场景：
        1. 检测到 marker move（G28 归零）
        2. 检测到大时间间隔（M400 等待）
        3. 打印暂停

        返回：
            bool: 是否成功发送
        """
        if not self.accumulated_moves:
            logging.debug("[FLUSH] No accumulated moves to flush")
            return True

        try:
            logging.info(f"[FLUSH] Flushing {len(self.accumulated_moves)} accumulated moves")

            # 转换为航点
            waypoints = self._convert_moves_to_waypoints(self.accumulated_moves)

            if not waypoints:
                logging.warning("[FLUSH] No waypoints generated from accumulated moves")
                return False

            # 添加时间戳
            timestamped_waypoints = self._add_timestamps_to_waypoints(waypoints)

            # 验证时间戳
            if not self._validate_timestamps(timestamped_waypoints):
                logging.error("[FLUSH] Timestamp validation failed")
                return False

            # 发送航点到机械臂
            self._send_waypoints_to_robot(timestamped_waypoints, waypoints)

            # 更新 last_sent_time
            last_move = self.accumulated_moves[-1]
            last_move_end_time = (self._get_move_attr(last_move, 'print_time') +
                                  self._get_move_attr(last_move, 'move_t'))
            self.last_sent_time = last_move_end_time

            # 清空累积缓冲区
            self.accumulated_moves = []
            self.last_move_end_time = None

            logging.info("[FLUSH] Accumulated moves flushed successfully")
            return True

        except Exception as e:
            logging.exception(f"[FLUSH] Failed to flush accumulated moves: {e}")
            return False

    def _accumulate_moves(self, new_moves):
        """
        累积 moves 并检测命令边界

        策略：
        - 累积moves直到检测到命令边界（move间隔 > 50ms）
        - 或者累积超时（100ms）
        - 这样可以等待TrapQ填充完整，一次性发送完整命令

        参数：
            new_moves: 新提取的 moves

        返回：
            bool: True 表示应该发送累积的 moves，False 表示继续累积
        """
        if not new_moves:
            return False

        # 检查命令边界
        if self.accumulated_moves and self.last_move_end_time is not None:
            first_new_move = new_moves[0]
            gap = first_new_move.print_time - self.last_move_end_time

            if gap > MOVE_GAP_THRESHOLD:
                # 检测到命令边界
                logging.info(
                    f"[BOUNDARY] Detected command boundary: gap={gap*1000:.1f}ms "
                    f"(threshold={MOVE_GAP_THRESHOLD*1000:.0f}ms)"
                )
                return True  # 发送之前累积的 moves

        # 深拷贝新 moves（避免 C FFI 对象被垃圾回收）
        for move in new_moves:
            # 创建一个简单的字典来存储 move 数据
            move_copy = {
                'print_time': move.print_time,
                'move_t': move.move_t,
                'start_v': move.start_v,
                'accel': move.accel,
                'start_x': move.start_x,
                'start_y': move.start_y,
                'start_z': move.start_z,
                'x_r': move.x_r,
                'y_r': move.y_r,
                'z_r': move.z_r,
            }
            self.accumulated_moves.append(move_copy)

        # 更新最后一个 move 的结束时间
        last_move = new_moves[-1]
        self.last_move_end_time = last_move.print_time + last_move.move_t

        logging.debug(
            f"[ACCUMULATE] Added {len(new_moves)} moves, "
            f"total accumulated: {len(self.accumulated_moves)}"
        )

        return False  # 继续累积

    def _detect_command_boundary(self, accumulated_moves):
        """
        检测累积 moves 中的命令边界

        检测策略：
        1. 时间间隔 > 50ms（MOVE_GAP_THRESHOLD）
        2. 速度方向变化（任意轴速度方向相反）

        参数：
            accumulated_moves: 累积的 moves 列表

        返回：
            int or None: 边界索引（第一个新命令的索引），None 表示无边界
        """
        if not accumulated_moves or len(accumulated_moves) < 2:
            return None

        boundary_index = None
        max_gap = 0.0
        max_gap_index = -1

        for i in range(len(accumulated_moves) - 1):
            move1 = accumulated_moves[i]
            move2 = accumulated_moves[i + 1]
            move1_end = self._get_move_attr(move1, 'print_time') + self._get_move_attr(move1, 'move_t')
            move2_start = self._get_move_attr(move2, 'print_time')
            gap = move2_start - move1_end

            # 记录最大间隔
            if gap > max_gap:
                max_gap = gap
                max_gap_index = i

            # 策略1: 检测时间间隔（50ms 为命令边界，100ms 为大间隔）
            if gap > MOVE_GAP_THRESHOLD:  # 50ms
                if gap > 0.1:  # 100ms
                    logging.warning(
                        f"[LARGE_GAP] Detected large time gap at move {i}: {gap*1000:.1f}ms "
                        f"(possible M400 or other wait command)"
                    )
                else:
                    logging.info(
                        f"[BOUNDARY] Detected command boundary (time gap) at move {i}: {gap*1000:.1f}ms"
                    )
                boundary_index = i + 1
                break

            # 策略2: 检测速度方向变化（用于连续G1命令）
            # 注意：由于 pull_move 结构没有 move_d 字段，暂时禁用此策略
            # 可以通过 axes_r 和 move_t 计算，但需要更复杂的逻辑

        # 如果没有检测到边界，记录最大间隔（用于调试）
        if boundary_index is None and len(accumulated_moves) > 1:
            logging.debug(
                f"[NO_BOUNDARY] No command boundary detected in {len(accumulated_moves)} moves, "
                f"max gap={max_gap*1000:.1f}ms at move {max_gap_index} "
                f"(threshold={MOVE_GAP_THRESHOLD*1000:.1f}ms)"
            )

        return boundary_index

    def _should_send_accumulated_moves(self, accumulated_moves, idle_count):
        """
        判断是否应该发送累积的 moves（混合触发策略）

        触发条件（优先级从高到低）：
        1. 命令边界触发：检测到时间间隔 > 50ms
        2. 时间窗口触发：累积时间 >= 200ms 且 idle_count >= 10
        3. 强制发送触发：累积时间 >= 2000ms（避免数据堆积太久）
        4. 数量触发：累积 moves >= 20 个

        参数：
            accumulated_moves: 累积的 moves 列表
            idle_count: 连续空闲计数

        返回：
            int: 应该发送的 moves 数量（0 表示继续累积）
        """
        if not accumulated_moves:
            return 0

        # 计算累积时间范围
        first_move = accumulated_moves[0]
        last_move = accumulated_moves[-1]
        first_time = self._get_move_attr(first_move, 'print_time')
        last_time = (
            self._get_move_attr(last_move, 'print_time') +
            self._get_move_attr(last_move, 'move_t')
        )
        accumulated_duration = last_time - first_time

        # 策略1: 命令边界触发（优先级最高）
        boundary_index = self._detect_command_boundary(accumulated_moves)
        if boundary_index is not None:
            logging.info(
                f"[TRIGGER] Command boundary detected at move {boundary_index}, "
                f"sending {boundary_index} moves"
            )
            return boundary_index

        # 策略2: 时间窗口触发（空闲时发送）
        # 注意：不再等待姿态协调移动完成，避免因连续姿态移动导致严重延迟
        if accumulated_duration >= SEND_TIME_WINDOW and idle_count >= SEND_IDLE_THRESHOLD:
            logging.info(
                f"[TRIGGER] Time window reached ({accumulated_duration*1000:.1f}ms, idle={idle_count*10}ms), "
                f"sending {len(accumulated_moves)} moves"
            )
            return len(accumulated_moves)

        # 策略3: 强制发送触发（避免数据堆积太久，保证流式发送）
        # 使用 FORCED_SEND_THRESHOLD（50ms）避免机械臂饥饿
        # 修复（2026-01-09）：移除对 orientation_move_pending 的等待
        # 原因：连续多个姿态协调移动会导致 pending 长时间为 True，
        #       造成严重的同步延迟（如11秒）。Move 1 和 Move 2 已通过
        #       flush_step_generation() 确保同时进入 TrapQ，不会被分开提取。
        if accumulated_duration >= FORCED_SEND_THRESHOLD:
            logging.info(
                f"[TRIGGER] Forced send ({accumulated_duration*1000:.1f}ms accumulated), "
                f"sending {len(accumulated_moves)} moves to prevent buffer overflow"
            )
            return len(accumulated_moves)

        # 策略4: 数量触发（防止内存占用过大）
        # ★ 修复（2026-01-31）：增加最小时间跨度检查，避免发送时间跨度过短的批次
        # 原因：密集填充路径可能产生大量短moves（如50个moves只有35ms时间跨度），
        #       导致生成的waypoints过少，MCU命令队列饥饿，触发"Timer too close"
        MIN_DURATION_FOR_COUNT_TRIGGER = 0.1  # 100ms 最小时间跨度
        if len(accumulated_moves) >= SEND_MOVE_COUNT_LIMIT:
            if accumulated_duration >= MIN_DURATION_FOR_COUNT_TRIGGER:
                logging.info(
                    f"[TRIGGER] Move count limit reached ({len(accumulated_moves)} moves, "
                    f"{accumulated_duration*1000:.1f}ms), sending all"
                )
                return len(accumulated_moves)
            else:
                # 数量足够但时间跨度太短，继续累积
                # 但如果数量过多（> 200），强制发送防止内存无限增长
                MAX_MOVES_HARD_LIMIT = 200
                if len(accumulated_moves) >= MAX_MOVES_HARD_LIMIT:
                    logging.warning(
                        f"[TRIGGER] Hard limit reached ({len(accumulated_moves)} moves, "
                        f"{accumulated_duration*1000:.1f}ms), force sending to prevent memory overflow"
                    )
                    return len(accumulated_moves)
                logging.debug(
                    f"[WAIT] {len(accumulated_moves)} moves but only "
                    f"{accumulated_duration*1000:.1f}ms < {MIN_DURATION_FOR_COUNT_TRIGGER*1000:.0f}ms, "
                    f"waiting for more time span..."
                )

        # 继续累积
        return 0

    # ========== TrapQ 数据提取方法 ==========

    def _get_safe_extraction_range(self):
        """
        计算安全的 TrapQ 提取时间范围

        使用极小的安全边际（10ms），依赖Klipper的trapq_finalize_moves
        确保moves已经finalize。

        返回：
            (start_time, end_time): 提取时间范围（秒）
            (None, None): 如果没有新数据
        """
        # 获取当前 print_time（下一个move的起始时间）
        current_print_time = self.toolhead.get_last_move_time()

        # 计算安全的结束时间（减去极小的安全边际）
        end_time = current_print_time - SAFETY_MARGIN

        # 起始时间是上次发送的时间
        start_time = self.last_sent_time

        # 调试日志（每100次轮询记录一次）
        if self.poll_count % 100 == 0:
            logging.debug(
                f"[EXTRACT_RANGE] current_print_time={current_print_time:.6f}, "
                f"end_time={end_time:.6f}, start_time={start_time:.6f}, "
                f"range={end_time - start_time:.6f}s"
            )

        # 检查是否有新数据
        if end_time <= start_time:
            return None, None

        return start_time, end_time

    def _extract_moves_from_trapq(self, start_time, end_time):
        """
        从 TrapQ 提取指定时间范围内的 moves

        参考 motion_report.py 的 extract_trapq 方法实现。
        使用批量提取策略，处理大量 move。

        参数：
            start_time: 起始时间（print_time，秒）
            end_time: 结束时间（print_time，秒）

        返回：
            list of struct pull_move（按时间顺序）
        """
        res = []

        # 批量提取循环
        while True:
            # 分配 pull_move 数组
            data = self.ffi_main.new('struct pull_move[128]')

            # 调用 C 层提取函数
            count = self.trapq_extract_old(
                self.trapq, data, len(data),
                start_time, end_time
            )

            # 没有数据，退出循环
            if not count:
                break

            # 保存本批次数据
            res.append((data, count))

            # 如果本批次未满，说明已提取完毕
            if count < len(data):
                break

            # 更新 end_time 继续提取更早的 move
            end_time = data[count-1].print_time

        # 反转结果以获得时间顺序（从早到晚）
        res.reverse()

        # 展开为单个 move 列表
        # 注意：trapq_extract_old 返回的每个批次内的moves是从新到旧排列的
        # 所以需要反向遍历每个批次（range(cnt-1, -1, -1)）
        # 重要：必须保存 res（包含 data 数组），防止 C 数组被垃圾回收
        moves = [data[i] for data, cnt in res for i in range(cnt-1, -1, -1)]

        # 保存 res 到实例变量，防止 data 数组被垃圾回收
        self._trapq_data_cache = res

        # 调试：验证moves的顺序（详细日志）
        # 注意：这里的moves是从TrapQ直接提取的C对象，不是字典
        if len(moves) > 1:
            logging.debug(f"[TRAPQ] Extracted {len(moves)} moves, verifying order:")
            for i in range(len(moves) - 1):
                pt_i = self._get_move_attr(moves[i], 'print_time')
                pt_i1 = self._get_move_attr(moves[i+1], 'print_time')
                if pt_i > pt_i1:
                    logging.error(
                        f"[TRAPQ] ORDER ERROR: Move {i} (pt={pt_i:.6f}) "
                        f"> Move {i+1} (pt={pt_i1:.6f})"
                    )
            first_pt = self._get_move_attr(moves[0], 'print_time')
            last_pt = self._get_move_attr(moves[-1], 'print_time')
            logging.debug(
                f"[TRAPQ] First move: pt={first_pt:.6f}, "
                f"Last move: pt={last_pt:.6f}"
            )

        return moves

    def _log_move_details(self, moves):
        """
        记录 move 详细信息（调试用）

        参数：
            moves: list of struct pull_move 或 dict
        """
        if not moves:
            return

        logging.debug(f"=== Extracted {len(moves)} moves ===")

        # 记录所有 move 的 print_time（用于调试时间戳问题）
        for i, move in enumerate(moves):
            pt = self._get_move_attr(move, 'print_time')
            mt = self._get_move_attr(move, 'move_t')
            sv = self._get_move_attr(move, 'start_v')
            a = self._get_move_attr(move, 'accel')

            # 计算移动距离（使用 axes_r 和 move_t）
            try:
                axes_r = self._get_move_attr(move, 'axes_r')
                dx = axes_r[0]
                dy = axes_r[1]
                dz = axes_r[2]
                distance = math.sqrt(dx*dx + dy*dy + dz*dz) * mt
            except (KeyError, IndexError, TypeError):
                # 如果无法获取 axes_r，使用备用方法
                distance = 0.0

            logging.debug(
                f"Move {i}: "
                f"pt={pt:.6f} "
                f"mt={mt:.6f} "
                f"end_time={pt + mt:.6f} "
                f"sv={sv:.3f} "
                f"a={a:.3f} "
                f"dist={distance:.6f}mm"
            )

            # # 特别标记微小移动（可能是姿态调整的 Move 2）
            # if distance < 0.01:  # 小于 0.01mm
            #     logging.info(
            #         f"[POLL-DEBUG] 检测到微小移动 (Move {i}): "
            #         f"距离={distance:.6f}mm, 持续时间={mt*1000:.1f}ms, "
            #         f"时间范围: {pt:.6f}s → {pt+mt:.6f}s"
            #     )多余了，不要检测

    # ========== 轨迹采样方法 ==========

    def _get_move_attr(self, move, attr):
        """获取 move 属性（支持字典和 C 对象）"""
        if isinstance(move, dict):
            return move[attr]
        else:
            return getattr(move, attr)

    def _calculate_position(self, move, sample_time):
        """
        计算 move 在指定时间的位置

        使用梯形速度曲线公式：
        - distance(t) = (start_v + 0.5 * accel * t) * t
        - position(t) = start_pos + direction * distance(t)

        参数：
            move: struct pull_move 或 dict（深拷贝的 move 数据）
            sample_time: 相对于 move.print_time 的时间偏移（秒）

        返回：
            (x, y, z): 位置坐标
        """
        # 支持字典和 C 对象
        if isinstance(move, dict):
            move_t = move['move_t']
            start_v = move['start_v']
            accel = move['accel']
            start_x = move['start_x']
            start_y = move['start_y']
            start_z = move['start_z']
            x_r = move['x_r']
            y_r = move['y_r']
            z_r = move['z_r']
        else:
            move_t = move.move_t
            start_v = move.start_v
            accel = move.accel
            start_x = move.start_x
            start_y = move.start_y
            start_z = move.start_z
            x_r = move.x_r
            y_r = move.y_r
            z_r = move.z_r

        # 限制时间范围在 [0, move.move_t]
        t = max(0.0, min(sample_time, move_t))

        # 计算距离：dist = (start_v + 0.5 * accel * t) * t
        dist = (start_v + 0.5 * accel * t) * t

        # 计算位置：pos = start_pos + direction * dist
        x = start_x + x_r * dist
        y = start_y + y_r * dist
        z = start_z + z_r * dist

        return (x, y, z)

    def _convert_moves_to_waypoints(self, moves):
        """
        将 moves 转换为固定时间步长采样的航点序列（包含姿态）

        策略（修复版 + 姿态支持）：
        - 从第一个move的起始时间开始采样，不跳过任何moves
        - 在整段连续路径上以 SAMPLING_INTERVAL (50ms) 固定间隔采样
        - 跨 move 边界连续采样，避免每个 move 只有起止两点导致的“偶发超速”
        - 必须包含最终终点（即最后一个 move 的结束时刻）
        - **新增**：每个采样点查询对应时刻的姿态（rx, ry, rz）

        参数：
            moves: list of struct pull_move 或 dict（按时间顺序）

        返回：
            list of (abs_time, x, y, z, rx, ry, rz)  # 7维航点
            abs_time 为 Klipper 的绝对 print_time（秒）
        """
        if not moves:
            logging.warning("No moves to convert")
            return []

        waypoints = []

        # 采样起点：第一个move的起始时间（不使用start_print_time，避免跳过moves）
        first_move = moves[0]
        first_pt = self._get_move_attr(first_move, 'print_time')
        base_time = first_pt

        next_sample_time = base_time
        last_added_time = None

        # === 批次级位置锁定检测 ===
        # 如果批次中包含姿态调整微移动，则整个批次的位置都锁定到第一个航点
        # 这样可以避免 Move 0/1 期间的微小位置变化导致的毛刺
        batch_has_orient_move = False
        batch_locked_pos = None  # (x_base, y_base, z_base)

        for move in moves:
            mt = self._get_move_attr(move, 'move_t')
            start_v = self._get_move_attr(move, 'start_v')
            accel = self._get_move_attr(move, 'accel')
            total_dist = (start_v + 0.5 * accel * mt) * mt

            # 检测姿态调整微移动：距离 < 0.1mm 且时间 > 100ms
            if total_dist < 0.1 and mt > 0.1:
                batch_has_orient_move = True
                break

        if batch_has_orient_move:
            # 锁定位置为第一个 move 的起点
            first_start_x = self._get_move_attr(first_move, 'start_x')
            first_start_y = self._get_move_attr(first_move, 'start_y')
            first_start_z = self._get_move_attr(first_move, 'start_z')
            batch_locked_pos = self._transform_user_to_base(first_start_x, first_start_y, first_start_z)
            logging.info(
                f"[ORIENT-LOCK-BATCH] 批次包含姿态调整移动，锁定整个批次位置到: "
                f"({batch_locked_pos[0]:.2f}, {batch_locked_pos[1]:.2f}, {batch_locked_pos[2]:.2f})"
            )

        # 遍历每个 move，按绝对时间采样（不跳过任何moves）
        for i, move in enumerate(moves):
            pt = self._get_move_attr(move, 'print_time')
            mt = self._get_move_attr(move, 'move_t')
            move_start = pt
            move_end = pt + mt

            # === 检测 Move 2（姿态调整微移动）===
            # 特征：短距离（< 0.1mm）且持续时间 > 100ms
            # 这类移动需要锁定位置，只让姿态变化
            start_x = self._get_move_attr(move, 'start_x')
            start_y = self._get_move_attr(move, 'start_y')
            start_z = self._get_move_attr(move, 'start_z')
            x_r = self._get_move_attr(move, 'x_r')
            y_r = self._get_move_attr(move, 'y_r')
            z_r = self._get_move_attr(move, 'z_r')
            start_v = self._get_move_attr(move, 'start_v')
            accel = self._get_move_attr(move, 'accel')

            # 计算移动距离
            total_dist = (start_v + 0.5 * accel * mt) * mt

            # 检测是否为姿态调整微移动
            is_orientation_move = (total_dist < 0.1) and (mt > 0.1)  # 距离 < 0.1mm 且时间 > 100ms

            if is_orientation_move:
                # 姿态调整微移动：使用批次锁定位置，只让姿态变化
                # 记录检测到的姿态调整移动
                if i == 0 or not hasattr(self, '_orient_move_logged') or not self._orient_move_logged:
                    start_orient = self._get_orientation_at_time(move_start)
                    end_orient = self._get_orientation_at_time(move_end)
                    logging.info(
                        f"[ORIENT-LOCK] 检测到姿态调整微移动: Move {i}, "
                        f"距离={total_dist*1000:.3f}μm, 时间={mt*1000:.1f}ms, "
                        f"姿态: {math.degrees(start_orient[1]):.2f}° → {math.degrees(end_orient[1]):.2f}°"
                    )
                    self._orient_move_logged = True

                # 本 move 的第一个采样时刻（绝对时间）
                t_abs = max(next_sample_time, move_start)

                # 在 move 内按固定间隔采样，但位置锁定
                while t_abs < move_end - 1e-9:
                    # 查询姿态（姿态可以变化）
                    rx, ry, rz = self._get_orientation_at_time(t_abs)

                    if last_added_time is None or abs(t_abs - last_added_time) > 1e-9:
                        # 使用批次锁定位置
                        x_base, y_base, z_base = batch_locked_pos
                        waypoints.append((t_abs, x_base, y_base, z_base, rx, ry, rz))
                        last_added_time = t_abs

                        # 调试：记录前5个采样点的姿态信息
                        if len(waypoints) <= 5:
                            logging.info(
                                f"[SAMPLE] Waypoint {len(waypoints)}: t={t_abs:.6f}s, "
                                f"orient=({math.degrees(rx):.2f}°, {math.degrees(ry):.2f}°, {math.degrees(rz):.2f}°)"
                            )

                    t_abs += SAMPLING_INTERVAL
            else:
                # 正常移动：位置和姿态都正常采样
                # 但如果批次包含姿态调整移动，则使用批次锁定位置
                # 本 move 的第一个采样时刻（绝对时间）
                t_abs = max(next_sample_time, move_start)

                # 在 move 内按固定间隔采样（不包含严格意义上的 move_end，终点在循环后确保）
                while t_abs < move_end - 1e-9:
                    # 查询姿态
                    rx, ry, rz = self._get_orientation_at_time(t_abs)

                    if batch_has_orient_move and batch_locked_pos is not None:
                        # 批次级锁定：使用锁定位置
                        x_base, y_base, z_base = batch_locked_pos
                    else:
                        # 正常计算位置
                        rel_t = t_abs - pt
                        x_user, y_user, z_user = self._calculate_position(move, rel_t)
                        x_base, y_base, z_base = self._transform_user_to_base(x_user, y_user, z_user)

                    if last_added_time is None or abs(t_abs - last_added_time) > 1e-9:
                        waypoints.append((t_abs, x_base, y_base, z_base, rx, ry, rz))
                        last_added_time = t_abs

                        # 调试：记录前5个采样点的姿态信息
                        if len(waypoints) <= 5:
                            logging.info(
                                f"[SAMPLE] Waypoint {len(waypoints)}: t={t_abs:.6f}s, "
                                f"orient=({math.degrees(rx):.2f}°, {math.degrees(ry):.2f}°, {math.degrees(rz):.2f}°)"
                            )

                    t_abs += SAMPLING_INTERVAL

            # 更新下一次采样时刻（保持全局采样网格）
            next_sample_time = max(next_sample_time, move_end)

        # 确保加入最终终点（最后一个 move 的结束时刻）
        last_move = moves[-1]
        final_time = self._get_move_attr(last_move, 'print_time') + self._get_move_attr(last_move, 'move_t')

        # 检查终点是否需要添加
        if not waypoints:
            # 如果没有任何航点，强制添加终点
            if batch_has_orient_move and batch_locked_pos is not None:
                x_base, y_base, z_base = batch_locked_pos
            else:
                x_user, y_user, z_user = self._calculate_position(last_move, self._get_move_attr(last_move, 'move_t'))
                x_base, y_base, z_base = self._transform_user_to_base(x_user, y_user, z_user)
            rx, ry, rz = self._get_orientation_at_time(final_time)
            waypoints.append((final_time, x_base, y_base, z_base, rx, ry, rz))
        else:
            # 检查终点与最后一个航点的时间间隔
            time_gap = final_time - waypoints[-1][0]

            # 只有时间间隔 >= SAMPLING_INTERVAL/2 时才添加终点
            # 这避免了生成间隔过短的航点对（如2ms）
            if time_gap >= SAMPLING_INTERVAL / 2:
                if batch_has_orient_move and batch_locked_pos is not None:
                    x_base, y_base, z_base = batch_locked_pos
                else:
                    x_user, y_user, z_user = self._calculate_position(last_move, self._get_move_attr(last_move, 'move_t'))
                    x_base, y_base, z_base = self._transform_user_to_base(x_user, y_user, z_user)
                rx, ry, rz = self._get_orientation_at_time(final_time)
                waypoints.append((final_time, x_base, y_base, z_base, rx, ry, rz))
            else:
                # 时间间隔太短，用终点数据更新最后一个航点
                logging.debug(
                    f"[SAMPLE] Final endpoint too close to last waypoint ({time_gap*1000:.1f}ms < {SAMPLING_INTERVAL*500:.1f}ms), "
                    f"updating last waypoint instead of adding new one"
                )
                if batch_has_orient_move and batch_locked_pos is not None:
                    x_base, y_base, z_base = batch_locked_pos
                else:
                    x_user, y_user, z_user = self._calculate_position(last_move, self._get_move_attr(last_move, 'move_t'))
                    x_base, y_base, z_base = self._transform_user_to_base(x_user, y_user, z_user)
                rx, ry, rz = self._get_orientation_at_time(final_time)
                waypoints[-1] = (final_time, x_base, y_base, z_base, rx, ry, rz)

        logging.debug(f"[SAMPLE] Generated {len(waypoints)} raw waypoints from {len(moves)} moves")

        # === 姿态平滑过渡后处理 ===
        # 问题：NRC 控制器在两个航点之间进行内部姿态插值时，末端位置会偏移
        # 解决：检测相邻航点的姿态变化，如果超过阈值则插入中间过渡点
        # 这样每个航点之间的姿态变化足够小，NRC 的内部插值产生的位置偏移也更小
        waypoints = self._smooth_orientation_transitions(waypoints)

        return waypoints

    def _smooth_orientation_transitions(self, waypoints):
        """
        平滑姿态过渡：在姿态变化较大的相邻航点之间插入过渡航点

        问题根因：NRC 控制器在两个航点之间进行内部姿态插值时，
        可能不是绕 TCP 旋转，导致末端位置偏移约 0.1-0.2mm。

        解决方案：将大的姿态变化拆分成多个小步骤，
        使每个航点之间的姿态变化小于阈值（如 1°），
        从而减少 NRC 内部插值产生的位置偏移。

        参数：
            waypoints: list of (t, x, y, z, rx, ry, rz) 原始航点

        返回：
            list of (t, x, y, z, rx, ry, rz) 平滑后的航点
        """
        if len(waypoints) < 2:
            return waypoints

        # 姿态变化阈值（度），超过此值则插入过渡点
        MAX_ORIENT_CHANGE_DEG = 1.0  # 1度

        smoothed = [waypoints[0]]
        orient_transitions_count = 0
        max_orient_change = 0.0  # 跟踪最大姿态变化

        # 调试：输出前5个航点的姿态
        logging.info(f"[ORIENT-SMOOTH-DEBUG] 航点总数: {len(waypoints)}")
        for i in range(min(5, len(waypoints))):
            wp = waypoints[i]
            logging.info(
                f"[ORIENT-SMOOTH-DEBUG] Waypoint {i}: t={wp[0]:.6f}s, "
                f"orient=({math.degrees(wp[4]):.2f}°, {math.degrees(wp[5]):.2f}°, {math.degrees(wp[6]):.2f}°)"
            )

        for i in range(1, len(waypoints)):
            wp_prev = smoothed[-1]  # 使用已处理的最后一个点
            wp_curr = waypoints[i]

            t0, x0, y0, z0, rx0, ry0, rz0 = wp_prev
            t1, x1, y1, z1, rx1, ry1, rz1 = wp_curr

            # 计算姿态变化（使用最短路径）
            def angle_diff(a, b):
                diff = b - a
                while diff > math.pi:
                    diff -= 2 * math.pi
                while diff < -math.pi:
                    diff += 2 * math.pi
                return diff

            drx = angle_diff(rx0, rx1)
            dry = angle_diff(ry0, ry1)
            drz = angle_diff(rz0, rz1)
            delta_orient_rad = math.sqrt(drx*drx + dry*dry + drz*drz)
            delta_orient_deg = math.degrees(delta_orient_rad)

            if delta_orient_deg > MAX_ORIENT_CHANGE_DEG:
                # 需要插入过渡点
                num_steps = int(math.ceil(delta_orient_deg / MAX_ORIENT_CHANGE_DEG))
                orient_transitions_count += 1

                for step in range(1, num_steps):
                    ratio = step / num_steps

                    # 时间线性插值
                    t_interp = t0 + (t1 - t0) * ratio

                    # 位置锁定为起点位置（关键修复！）
                    # 在姿态过渡期间，保持位置不变，只让姿态变化
                    # 这样 NRC 控制器执行时不会产生位置偏移
                    x_interp = x0
                    y_interp = y0
                    z_interp = z0

                    # 姿态线性插值（使用最短路径）
                    rx_interp = rx0 + drx * ratio
                    ry_interp = ry0 + dry * ratio
                    rz_interp = rz0 + drz * ratio

                    smoothed.append((t_interp, x_interp, y_interp, z_interp,
                                    rx_interp, ry_interp, rz_interp))

            # 添加当前航点
            smoothed.append(wp_curr)

            # 跟踪最大姿态变化
            if delta_orient_deg > max_orient_change:
                max_orient_change = delta_orient_deg

        # 总是报告最大姿态变化，便于调试
        logging.info(
            f"[ORIENT-SMOOTH] 处理完成: 航点数 {len(waypoints)} → {len(smoothed)}, "
            f"最大姿态变化={max_orient_change:.2f}°, 触发过渡={orient_transitions_count}次"
        )

        return smoothed

    # ========== 预校准方法 ==========

    def _perform_pre_calibration(self):
        """
        预校准：在打印开始前测量实际延迟

        工作流程：
        1. 查询并记录当前位置
        2. 发送 Z 轴正向移动的航点（使用 servo_move）
        3. 轮询检测机械臂开始运动
        4. 计算实际延迟 = 检测时间 - 发送时间
        5. 应用校准值到 calibrated_timestamp_offset_ms
        6. 返回原位

        返回：
            bool: 校准是否成功
        """
        import time
        import math

        logging.info("=" * 60)
        logging.info("[CALIBRATION] 🔧 开始预校准...")
        logging.info("=" * 60)

        # ★★★ 等待作业完全启动 ★★★
        # 作业刚启动后可能需要一些时间才能接收 servo_move 命令
        wait_after_job_start = 0.5  # 500ms
        logging.info(f"[CALIBRATION] 等待 {wait_after_job_start*1000:.0f}ms（确保作业完全启动）...")
        time.sleep(wait_after_job_start)

        try:
            # 1. 查询当前位置（基坐标系）
            current_pos = self.nrc.VectorDouble()
            with self.nrc_lock:
                # 使用 coord=1 获取基坐标系下的直角坐标
                ret = self.nrc.get_current_position(self.socket_fd_6001, 1, current_pos)

            if ret != 0:
                logging.warning(f"[CALIBRATION] ❌ 无法查询当前位置，返回码: {ret}，跳过预校准")
                return False

            start_x, start_y, start_z = current_pos[0], current_pos[1], current_pos[2]
            # ★★★ 关键修复：使用固定姿态（弧度制），而不是查询到的姿态（可能是角度制）★★★
            queried_rx, queried_ry, queried_rz = current_pos[3], current_pos[4], current_pos[5]
            logging.info(
                f"[CALIBRATION] 查询到的姿态（角度制）: rx={queried_rx:.2f}°, ry={queried_ry:.2f}°, rz={queried_rz:.2f}°"
            )
            
            # 使用固定姿态（弧度制）- 与正式打印一致
            start_rx = self.fixed_orientation_rx
            start_ry = self.fixed_orientation_ry
            start_rz = self.fixed_orientation_rz
            logging.info(
                f"[CALIBRATION] 使用固定姿态（弧度制）: rx={math.degrees(start_rx):.2f}°, ry={math.degrees(start_ry):.2f}°, rz={math.degrees(start_rz):.2f}°"
            )
            logging.info(
                f"[CALIBRATION] 当前位置: X={start_x:.2f}, Y={start_y:.2f}, Z={start_z:.2f}"
            )

            # 2. 计算目标位置（Z轴正向移动）
            target_z = start_z + self.pre_calibration_distance_mm
            logging.info(
                f"[CALIBRATION] 目标位置: Z={target_z:.2f} (移动 {self.pre_calibration_distance_mm:.1f}mm)"
            )

            # 3. 构造航点序列（固定时间间隔，模拟正常打印）
            waypoints_to_target = []
            waypoints_to_origin = []

            # ★★★ 关键修复：时间戳从 initial_timestamp_offset_ms 开始 ★★★
            # 正式打印的第一个航点时间戳是 initial_timestamp_offset_ms（如 220ms）
            # 预校准也必须使用相同的起始时间戳，否则机械臂会认为需要立即执行
            base_timestamp_ms = self.initial_timestamp_offset_ms
            logging.info(f"[CALIBRATION] 使用基准时间戳: {base_timestamp_ms:.1f}ms（与正式打印一致）")

            # 添加起始点（时间戳=base_timestamp_ms，当前位置）
            waypoints_to_target.append(
                (base_timestamp_ms, start_x, start_y, start_z, start_rx, start_ry, start_rz)
            )
            logging.info(f"[CALIBRATION] 添加起始点: t={base_timestamp_ms:.1f}ms, pos=({start_x:.2f}, {start_y:.2f}, {start_z:.2f})")

            # 移动到目标位置的航点（500ms 后到达）
            move_duration_ms = 500.0
            sampling_interval_ms = 50.0  # 50ms 采样间隔
            num_points = int(move_duration_ms / sampling_interval_ms)

            for i in range(1, num_points + 1):
                ratio = i / num_points
                t_ms = base_timestamp_ms + i * sampling_interval_ms
                z_interp = start_z + (target_z - start_z) * ratio
                waypoints_to_target.append(
                    (t_ms, start_x, start_y, z_interp, start_rx, start_ry, start_rz)
                )

            # 返回原位的航点（从 base+500ms 到 base+1000ms）
            for i in range(1, num_points + 1):
                ratio = i / num_points
                t_ms = base_timestamp_ms + move_duration_ms + i * sampling_interval_ms
                z_interp = target_z + (start_z - target_z) * ratio
                waypoints_to_origin.append(
                    (t_ms, start_x, start_y, z_interp, start_rx, start_ry, start_rz)
                )

            all_waypoints = waypoints_to_target + waypoints_to_origin
            logging.info(f"[CALIBRATION] 生成 {len(all_waypoints)} 个校准航点")

            # 4. 发送航点并记录发送时间
            send_time = time.perf_counter()
            self._send_calibration_waypoints(all_waypoints, is_first_batch=True)
            logging.info(f"[CALIBRATION] 航点已发送，发送时间: {send_time:.6f}")

            # 5. 轮询检测运动开始
            max_wait_time = 3.0  # 最多等待 3 秒
            position_threshold = 0.3  # 检测阈值（mm）
            poll_interval = 0.02  # 20ms 轮询

            wait_start = time.perf_counter()
            movement_detected = False
            detection_time = None

            while time.perf_counter() - wait_start < max_wait_time:
                # 查询当前位置（基坐标系）
                new_pos = self.nrc.VectorDouble()
                with self.nrc_lock:
                    ret = self.nrc.get_current_position(self.socket_fd_6001, 1, new_pos)

                if ret == 0:
                    # 计算位置变化
                    dx = new_pos[0] - start_x
                    dy = new_pos[1] - start_y
                    dz = new_pos[2] - start_z
                    distance = math.sqrt(dx*dx + dy*dy + dz*dz)

                    if distance > position_threshold:
                        movement_detected = True
                        detection_time = time.perf_counter()
                        break

                time.sleep(poll_interval)

            if not movement_detected:
                logging.warning(
                    f"[CALIBRATION] ❌ 未能在 {max_wait_time}s 内检测到运动，跳过预校准"
                )
                return False

            # 6. 计算延迟并应用
            measured_delay_s = detection_time - send_time
            measured_delay_ms = measured_delay_s * 1000.0

            logging.info("=" * 60)
            logging.info(f"[CALIBRATION] ✅ 预校准完成！")
            logging.info(f"[CALIBRATION]   • 发送时间: {send_time:.6f}s")
            logging.info(f"[CALIBRATION]   • 检测时间: {detection_time:.6f}s")
            logging.info(f"[CALIBRATION]   • 实测延迟: {measured_delay_ms:.1f}ms")
            logging.info(f"[CALIBRATION]   • 配置值:   {self.initial_timestamp_offset_ms:.1f}ms")
            logging.info(f"[CALIBRATION]   ➜ 已应用校准值: {measured_delay_ms:.1f}ms")
            logging.info("=" * 60)

            # 应用校准值
            self.calibrated_timestamp_offset_ms = measured_delay_ms

            # 7. 等待机械臂返回原位（使用轮询检测而非固定等待）
            logging.info("[CALIBRATION] 等待机械臂返回原位...")
            
            # 轮询检测机械臂是否停止（通过位置变化判断）
            max_wait_time = 3.0  # 最长等待3秒
            poll_interval = 0.1  # 100ms轮询
            stable_count = 0
            stable_threshold = 5  # 连续5次位置不变认为已停止
            last_check_pos = None
            
            wait_start = time.perf_counter()
            while time.perf_counter() - wait_start < max_wait_time:
                check_pos = self.nrc.VectorDouble()
                with self.nrc_lock:
                    ret = self.nrc.get_current_position(self.socket_fd_6001, 1, check_pos)
                
                if ret == 0:
                    if last_check_pos is not None:
                        pos_change = math.sqrt(
                            (check_pos[0] - last_check_pos[0])**2 +
                            (check_pos[1] - last_check_pos[1])**2 +
                            (check_pos[2] - last_check_pos[2])**2
                        )
                        if pos_change < 0.1:  # 位置变化小于0.1mm认为静止
                            stable_count += 1
                            if stable_count >= stable_threshold:
                                logging.info(f"[CALIBRATION] 检测到机械臂已停止（连续{stable_threshold}次位置稳定）")
                                break
                        else:
                            stable_count = 0  # 重置计数
                    
                    last_check_pos = [check_pos[i] for i in range(6)]
                
                time.sleep(poll_interval)
            
            # 验证是否回到原位
            final_pos = self.nrc.VectorDouble()
            with self.nrc_lock:
                ret = self.nrc.get_current_position(self.socket_fd_6001, 1, final_pos)

            if ret == 0:
                final_distance = math.sqrt(
                    (final_pos[0] - start_x)**2 +
                    (final_pos[1] - start_y)**2 +
                    (final_pos[2] - start_z)**2
                )
                if final_distance < 1.0:  # 允许 1mm 误差
                    logging.info(f"[CALIBRATION] ✅ 已返回原位 (误差: {final_distance:.2f}mm)")
                else:
                    logging.warning(
                        f"[CALIBRATION] ⚠️ 未完全返回原位 (偏差: {final_distance:.2f}mm)"
                    )

            # 8. 重置关键状态变量（避免与正常打印冲突）
            # is_first_batch = True：确保正常打印第一批航点清空缓冲区
            # last_sent_waypoint = None：避免批次间姿态连续性检查使用校准航点作为基准
            self.is_first_batch = True
            self.last_sent_waypoint = None
            logging.info("[CALIBRATION] 已重置状态变量：is_first_batch=True, last_sent_waypoint=None")

            return True

        except Exception as e:
            logging.error(f"[CALIBRATION] ❌ 预校准异常: {e}", exc_info=True)
            return False

    def _send_calibration_waypoints(self, waypoints, is_first_batch=True):
        """
        发送校准航点到机械臂

        参数：
            waypoints: [(t_ms, x, y, z, rx, ry, rz), ...]
            is_first_batch: 是否首批（设置 clearBuffer=True）
        """
        num_waypoints = len(waypoints)

        # 构建 ServoMovePara（与正式打印保持一致）
        param = self.nrc.ServoMovePara()
        param.clearBuffer = is_first_batch
        param.targetMode = 1  # 直角坐标系
        param.sendMode = 1
        param.runMode = 1
        param.coord = 1  # ★修复：使用笛卡尔坐标（与正式打印一致，原来是0）
        param.size = num_waypoints
        param.sum = 999  # ★修复：设为999（与正式打印一致，原来是num_waypoints）
        param.count = 1
        param.extMove = 0

        # 填充位置数据
        param.pos = self.nrc.VectorVectorDouble()
        for wp in waypoints:
            _, x, y, z, rx, ry, rz = wp
            point = self.nrc.VectorDouble()
            point.append(x)
            point.append(y)
            point.append(z)
            point.append(rx)
            point.append(ry)
            point.append(rz)
            point.append(0.0)  # 外部轴
            param.pos.append(point)

        # 填充速度和加速度（全部为 0，由时间戳控制）
        param.axisvel = self.nrc.VectorVectorDouble()
        param.axisacc = self.nrc.VectorVectorDouble()
        for _ in range(num_waypoints):
            vel = self.nrc.VectorDouble()
            acc = self.nrc.VectorDouble()
            for _ in range(7):
                vel.append(0.0)
                acc.append(0.0)
            param.axisvel.append(vel)
            param.axisacc.append(acc)

        # 填充时间戳
        param.timeStamp = self.nrc.VectorDouble()
        for wp in waypoints:
            t_ms = wp[0]
            param.timeStamp.append(t_ms)

        # 发送到机械臂
        with self.nrc_lock:
            ret = self.nrc.servo_move(self.socket_fd_7000, param)

        if ret != 0:
            logging.warning(f"[CALIBRATION] servo_move 返回非零值: {ret}")
        else:
            logging.info(f"[CALIBRATION] 已发送 {num_waypoints} 个校准航点")

    # ========== 延迟测量方法 ==========

    def _measure_first_waypoint_delay(self):
        """
        测量第一个航点的执行延迟（在后台线程中执行）

        通过轮询机械臂位置，检测何时开始移动，计算延迟。
        延迟 = 检测到运动的时间 - 第一个航点发送时间

        注意：
        - 此方法在后台线程中执行，不阻塞主流程
        - 测量结果仅用于诊断和日志输出，不影响时间戳计算
        - 如果测量失败（网络断开、超时等），不影响打印
        """
        import time
        import math

        if self.first_waypoint_send_time is None or self.initial_position is None:
            logging.warning("[DELAY] 无法测量延迟：缺少初始数据")
            return

        try:
            logging.info("[DELAY] 开始测量第一个航点执行延迟...")

            # 最大等待时间（秒）
            max_wait_time = 5.0
            # 位置变化阈值（毫米）
            position_threshold = 0.5
            # 轮询间隔（秒）
            poll_interval = 0.02  # 20ms

            start_time = time.perf_counter()
            movement_detected = False

            while time.perf_counter() - start_time < max_wait_time:
                # 查询当前位置
                current_pos = self.nrc.VectorDouble()
                ret = self.nrc.get_current_position(self.socket_fd_6001, 0, current_pos)

                if ret == 0:
                    # 计算位置变化
                    dx = current_pos[0] - self.initial_position[0]
                    dy = current_pos[1] - self.initial_position[1]
                    dz = current_pos[2] - self.initial_position[2]
                    distance = math.sqrt(dx*dx + dy*dy + dz*dz)

                    if distance > position_threshold:
                        # 检测到运动
                        movement_detected = True
                        detection_time = time.perf_counter()
                        delay = detection_time - self.first_waypoint_send_time
                        self.first_waypoint_execution_delay = delay

                        logging.info(
                            f"[DELAY] ✅ 第一个航点执行延迟测量完成: {delay*1000:.1f}ms\n"
                            f"        发送时间: {self.first_waypoint_send_time:.6f}s\n"
                            f"        检测时间: {detection_time:.6f}s\n"
                            f"        位置变化: {distance:.2f}mm"
                        )

                        # 自动延迟调整逻辑（如果启用）
                        if self.enable_auto_delay_adjustment:
                            delay_ms = delay * 1000  # 转换为毫秒
                            delay_diff_ms = abs(delay_ms - self.initial_timestamp_offset_ms)

                            if delay_diff_ms > self.delay_adjustment_threshold_ms:
                                logging.warning(
                                    f"[DELAY] ⚠️  检测到延迟差异: {delay_diff_ms:.1f}ms\n"
                                    f"        实测延迟: {delay_ms:.1f}ms\n"
                                    f"        配置补偿: {self.initial_timestamp_offset_ms:.1f}ms\n"
                                    f"        差异超过阈值: {self.delay_adjustment_threshold_ms:.1f}ms\n"
                                    f"        建议调整 initial_timestamp_offset_ms 为 {delay_ms:.0f}ms"
                                )
                            else:
                                logging.info(
                                    f"[DELAY] ✅ 延迟配置合理（差异 {delay_diff_ms:.1f}ms < 阈值 {self.delay_adjustment_threshold_ms:.1f}ms）"
                                )

                        break
                else:
                    logging.warning(f"[DELAY] 查询位置失败，返回码: {ret}")

                time.sleep(poll_interval)

            if not movement_detected:
                logging.warning(
                    f"[DELAY] ⚠️  未能在 {max_wait_time}s 内检测到机械臂运动\n"
                    f"        可能原因：机械臂未启动、网络延迟过大、或位置阈值设置不当"
                )

        except Exception as e:
            logging.error(f"[DELAY] 延迟测量过程中发生异常: {e}", exc_info=True)

    # ========== 时间戳计算方法 ==========

    def _add_timestamps_to_waypoints(self, waypoints):
        """
        为航点添加相对时间戳（单位：毫秒）

        时间戳策略（第十三次修复：纯相对时间 + 基于已发送时间戳的过期检测）：
        - 时间戳完全基于print_time计算，保持连续性（无批次间隙）
        - 在发送前检测时间戳是否过期（基于已发送的最后一个时间戳）
        - 如果过期（时间戳倒退），整体平移所有时间戳
        - **关键**：保持航点间的相对时间差不变，确保时间戳连续

        核心原理：
        - 时间戳基于print_time连续计算，无批次间隙
        - 机械臂和挤出机按相同时间线执行（姿态调整并到时间线）
        - 避免时间戳过期导致机械臂立即执行

        问题根源（日志11.md）：
        - 第十二次修复使用"实时时间"检测过期，导致误判
        - Batch 1: 时间戳 220ms → 5536.8ms
        - Batch 2: 原始时间戳 5536.8ms（应该连续）
        - 实时时间检测：6539.3ms > 5536.8ms → 误判为过期
        - 整体平移：adjustment=1102.4ms
        - 结果：6639.3ms → 9639.1ms
        - 时间戳间隙：6639.3 - 5536.8 = 1102.5ms ← 这是问题根源！

        解决方案（第十三次修复）：
        - 不使用实时时间，改用已发送的最后一个时间戳作为参考
        - 只有当新时间戳小于已发送的时间戳时，才认为过期
        - 避免因实时时间和print_time差异导致的误判

        参数：
            waypoints: list of (abs_time, x, y, z) 或 (abs_time, x, y, z, rx, ry, rz)
                       abs_time 为 Klipper 的绝对 print_time（秒）

        返回：
            list of (relative_time_ms, x, y, z, rx, ry, rz)
            relative_time_ms 为相对时间戳（毫秒），基于print_time计算，确保连续性
        """
        if not waypoints:
            return []

        # 初始化时间基准（第一次调用时）
        if not hasattr(self, 'timestamp_base_time'):
            self.timestamp_base_time = waypoints[0][0]  # 第一个航点的print_time
            # 优先使用预校准测得的值，如果未校准则使用配置值
            if self.calibrated_timestamp_offset_ms is not None:
                self.timestamp_base_ms = self.calibrated_timestamp_offset_ms
                logging.info(
                    f"[TIMESTAMP] 使用预校准值: {self.calibrated_timestamp_offset_ms:.1f}ms"
                )
            else:
                self.timestamp_base_ms = self.initial_timestamp_offset_ms
                logging.info(
                    f"[TIMESTAMP] 使用配置值: {self.initial_timestamp_offset_ms:.1f}ms (未执行预校准)"
                )
            self.last_sent_timestamp_ms = None  # 跟踪已发送的最后一个时间戳
            logging.info(
                f"[TIMESTAMP] 初始化时间戳跟踪系统（纯相对时间 + 基于已发送时间戳的过期检测）"
            )

        timestamped_waypoints = []

        for i, waypoint in enumerate(waypoints):
            # 支持4维和7维航点（向后兼容）
            if len(waypoint) == 7:
                abs_time, x, y, z, rx, ry, rz = waypoint
            elif len(waypoint) == 4:
                abs_time, x, y, z = waypoint
                rx, ry, rz = (self.fixed_orientation_rx,
                             self.fixed_orientation_ry,
                             self.fixed_orientation_rz)
            else:
                raise ValueError(f"Invalid waypoint structure at {i}: {waypoint}")

            # 基于print_time计算时间戳（保持连续性）
            time_diff_ms = (abs_time - self.timestamp_base_time) * 1000.0
            global_time_ms = self.timestamp_base_ms + time_diff_ms

            timestamped_waypoints.append((global_time_ms, x, y, z, rx, ry, rz))

        if timestamped_waypoints:
            first_ts = timestamped_waypoints[0][0]
            last_ts = timestamped_waypoints[-1][0]
            duration_ms = last_ts - first_ts

            # 发送前检测时间戳是否过期（基于已发送的最后一个时间戳）
            if self.last_sent_timestamp_ms is not None:
                if first_ts < self.last_sent_timestamp_ms:
                    # 时间戳过期（小于已发送的最后一个时间戳），整体平移
                    BUFFER_MS = 100.0  # 100ms缓冲
                    adjustment_ms = self.last_sent_timestamp_ms - first_ts + BUFFER_MS

                    # 整体平移，保持相对时间差不变
                    timestamped_waypoints = [
                        (ts + adjustment_ms, x, y, z, rx, ry, rz)
                        for ts, x, y, z, rx, ry, rz in timestamped_waypoints
                    ]

                    logging.info(
                        f"[TIMESTAMP] 检测到时间戳过期（小于已发送的最后一个时间戳），整体平移: "
                        f"原始第一个航点={first_ts:.1f}ms, "
                        f"已发送的最后一个时间戳={self.last_sent_timestamp_ms:.1f}ms, "
                        f"调整量={adjustment_ms:.1f}ms, "
                        f"新第一个航点={first_ts + adjustment_ms:.1f}ms"
                    )

                    # 关键修复：更新时间基准，确保下一批次连续
                    old_base_ms = self.timestamp_base_ms
                    self.timestamp_base_ms += adjustment_ms
                    logging.info(
                        f"[TIMESTAMP] 更新时间基准: {old_base_ms:.1f}ms → {self.timestamp_base_ms:.1f}ms"
                    )

                    # 更新first_ts和last_ts
                    first_ts = timestamped_waypoints[0][0]
                    last_ts = timestamped_waypoints[-1][0]
                else:
                    # 时间戳未过期，检查是否有间隙
                    gap_ms = first_ts - self.last_sent_timestamp_ms
                    if gap_ms > 100:  # 如果间隙超过100ms，记录警告
                        logging.warning(
                            f"[TIMESTAMP] 检测到时间戳间隙: {gap_ms:.1f}ms "
                            f"(last_sent={self.last_sent_timestamp_ms:.1f}ms, "
                            f"first_new={first_ts:.1f}ms)"
                        )
                    else:
                        logging.debug(
                            f"[TIMESTAMP] 时间戳连续: gap={gap_ms:.1f}ms "
                            f"(last_sent={self.last_sent_timestamp_ms:.1f}ms, "
                            f"first_new={first_ts:.1f}ms)"
                        )

            logging.info(
                f"[TIMESTAMP] Generated {len(timestamped_waypoints)} timestamped waypoints: "
                f"range=[{first_ts:.1f}, {last_ts:.1f}]ms, duration={duration_ms:.1f}ms"
            )

            # 更新已发送的最后一个时间戳
            self.last_sent_timestamp_ms = last_ts

        return timestamped_waypoints

    def _validate_timestamps(self, timestamped_waypoints):
        """
        验证时间戳的正确性

        检查：
        1. 时间戳非负
        2. 时间戳单调非递减（允许相等，用于短move）
        3. 时间戳间隔合理（接近采样间隔）
        4. 多批次发送时时间戳连续性（全局累积）

        参数：
            timestamped_waypoints: list of (global_time_ms, x, y, z, rx, ry, rz)
                                   global_time_ms 为全局累积时间戳（毫秒）

        返回：
            True: 验证通过
            False: 验证失败
        """
        if len(timestamped_waypoints) < 1:
            return True

        # 检查所有时间戳非负
        for i, waypoint in enumerate(timestamped_waypoints):
            ts = waypoint[0]  # 第一个元素是时间戳
            if ts < 0:
                logging.error(
                    f"Negative timestamp at index {i}: {ts:.1f}ms"
                )
                return False

        if len(timestamped_waypoints) < 2:
            return True

        expected_interval = SAMPLING_INTERVAL * 1000.0  # 转换为毫秒

        for i in range(1, len(timestamped_waypoints)):
            prev_ts = timestamped_waypoints[i-1][0]
            curr_ts = timestamped_waypoints[i][0]

            # 检查单调性（允许相等，用于非常短的move）
            if curr_ts < prev_ts:
                logging.error(
                    f"Timestamp not monotonic at index {i}: "
                    f"{prev_ts:.1f}ms -> {curr_ts:.1f}ms"
                )
                return False

            # 检查间隔（允许 10ms 误差）
            interval = curr_ts - prev_ts
            if interval > 0 and abs(interval - expected_interval) > 10.0:
                logging.warning(
                    f"Timestamp interval unusual at index {i}: "
                    f"{interval:.1f}ms (expected ~{expected_interval:.1f}ms)"
                )

        return True

    # ========== 统计和监控方法 ==========

    def _log_extraction_stats(self, moves, waypoints, timestamped_waypoints):
        """
        记录提取和转换的统计信息

        参数：
            moves: list of struct pull_move 或 dict（累积后的moves）
            waypoints: list of (abs_time, x, y, z)
            timestamped_waypoints: list of (relative_time_ms, x, y, z)
        """
        if not moves:
            return

        # 计算统计信息（支持字典和C对象）
        first_move_pt = self._get_move_attr(moves[0], 'print_time')
        last_move_pt = self._get_move_attr(moves[-1], 'print_time')
        last_move_mt = self._get_move_attr(moves[-1], 'move_t')
        time_range = last_move_pt + last_move_mt - first_move_pt
        avg_waypoints_per_move = len(waypoints) / len(moves) if moves else 0

        logging.info(
            f"=== Extraction Stats ===\n"
            f"  Moves extracted: {len(moves)}\n"
            f"  Time range: {time_range:.3f}s\n"
            f"  Waypoints generated: {len(waypoints)}\n"
            f"  Avg waypoints/move: {avg_waypoints_per_move:.1f}\n"
            f"  Timestamped waypoints: {len(timestamped_waypoints)}\n"
            f"  Total moves extracted: {self.total_moves_extracted}\n"
            f"  Total waypoints sent: {self.total_waypoints_sent}"
        )
    
    # ========== 状态查询接口 ==========

    def _status_query_callback(self, eventtime):
        """
        状态查询回调（1Hz）

        周期性查询机械臂状态并缓存到内存。

        参数：
            eventtime: 当前事件时间

        返回：
            下次唤醒时间（eventtime + 1.0）
        """
        try:
            # 查询位置（高频：1Hz）- 查询关节坐标、直角坐标和用户坐标
            if eventtime - self.last_position_query_time >= 1.0:
                # 查询关节坐标（coord=0）
                pos_vec_joint = self.nrc.VectorDouble()
                with self.nrc_lock:
                    ret_joint = self.nrc.get_current_position(
                        self.socket_fd_6001, 0, pos_vec_joint
                    )
                if ret_joint == 0:
                    self.cached_position_joint = [pos_vec_joint[i] for i in range(6)]

                # 查询直角坐标/基坐标系（coord=1）
                pos_vec_cartesian = self.nrc.VectorDouble()
                with self.nrc_lock:
                    ret_cartesian = self.nrc.get_current_position(
                        self.socket_fd_6001, 1, pos_vec_cartesian
                    )
                if ret_cartesian == 0:
                    self.cached_position_cartesian = [pos_vec_cartesian[i] for i in range(6)]

                # 查询用户坐标系（coord=3）
                pos_vec_user = self.nrc.VectorDouble()
                with self.nrc_lock:
                    ret_user = self.nrc.get_current_position(
                        self.socket_fd_6001, 3, pos_vec_user
                    )
                if ret_user == 0:
                    self.cached_position_user = [pos_vec_user[i] for i in range(6)]

                self.last_position_query_time = eventtime

            # 查询状态和速度（中频：0.5Hz）
            if eventtime - self.last_state_query_time >= 2.0:
                with self.nrc_lock:
                    # 查询伺服状态
                    ret_list = self.nrc.get_servo_state(
                        self.socket_fd_6001, 0
                    )
                    if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
                        self.cached_servo_state = ret_list[1]

                    # 查询运行状态
                    ret_list = self.nrc.get_robot_running_state(
                        self.socket_fd_6001, 0
                    )
                    if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
                        self.cached_running_state = ret_list[1]

                    # 查询实时线速度（注意：函数名拼写是 curretn 而不是 current）
                    joint_speed_vec = self.nrc.VectorDouble()
                    joint_speed_sync_vec = self.nrc.VectorDouble()
                    ret_list = self.nrc.get_curretn_line_speed_and_joint_speed(
                        self.socket_fd_6001, 0.0, joint_speed_vec, joint_speed_sync_vec
                    )
                    if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
                        self.cached_line_speed = ret_list[1]  # 实时线速度（mm/s）

                    self.last_state_query_time = eventtime

            # 查询运行模式（低频：0.2Hz）
            if eventtime - self.last_mode_query_time >= 5.0:
                with self.nrc_lock:
                    ret_list = self.nrc.get_current_mode(
                        self.socket_fd_6001, 0
                    )
                if isinstance(ret_list, list) and len(ret_list) > 1 and ret_list[0] == 0:
                    self.cached_mode = ret_list[1]
                    self.last_mode_query_time = eventtime

        except Exception:
            logging.exception("Error in status query callback")

        return eventtime + 1.0  # 下次唤醒时间

    def get_status(self, eventtime):
        """
        返回执行器状态（包含缓存的机械臂状态）

        参数：
            eventtime: 当前事件时间

        返回：
            状态字典
        """
        return {
            # 连接状态
            'connected': self.socket_fd_6001 > 0 and self.socket_fd_7000 > 0,
            'control_socket_fd': self.socket_fd_6001 if self.socket_fd_6001 else 0,
            'servo_socket_fd': self.socket_fd_7000 if self.socket_fd_7000 else 0,

            # 机械臂状态（缓存，提供默认值）
            'robot_position_joint': self.cached_position_joint if self.cached_position_joint else [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
            'robot_position_cartesian': self.cached_position_cartesian if self.cached_position_cartesian else [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
            'robot_position_user': self.cached_position_user if self.cached_position_user else [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
            'robot_mode': self.cached_mode if self.cached_mode is not None else -1,
            'servo_state': self.cached_servo_state if self.cached_servo_state is not None else -1,
            'robot_running_state': self.cached_running_state if self.cached_running_state is not None else -1,
            'robot_line_speed': self.cached_line_speed if self.cached_line_speed is not None else 0.0,

            # 执行器状态
            'is_paused': self.is_paused,
            'is_first_batch': self.is_first_batch,
            'last_sent_time': self.last_sent_time if self.last_sent_time else 0.0,
            'start_print_time': self.start_print_time if self.start_print_time else 0.0,
            'total_waypoints_sent': self.total_waypoints_sent,
            'total_moves_extracted': self.total_moves_extracted,
            'poll_count': self.poll_count,

            # 作业状态
            'job_started': self.job_started,
            'job_name': self.job_name if self.job_name else '',

            # 错误信息
            'last_error': getattr(self, 'last_error', ''),
            'error_code': getattr(self, 'error_code', 0)
        }
    
    def pause(self):
        """暂停轨迹提取和发送"""
        self.is_paused = True
        logging.info("NRC Executor paused")
    
    def resume(self):
        """恢复轨迹提取和发送"""
        self.is_paused = False
        logging.info("NRC Executor resumed")

    # ========== G-code 命令处理器 ==========

    cmd_NRC_CONNECT_help = "Connect to robot arm"
    def cmd_NRC_CONNECT(self, gcmd):
        """连接机械臂并启动定时器"""
        if self.socket_fd_6001 > 0:
            gcmd.respond_info("Already connected")
            return

        try:
            # 重置状态变量
            self.is_first_batch = True
            self.first_waypoint_time = None
            self.batch_count = 0
            self.start_print_time = None
            self.job_started = False
            self.total_waypoints_sent = 0
            self.accumulated_moves = []
            self.last_move_end_time = None
            self.last_accumulate_time = None
            self.first_batch_skipped = False
            self.last_sent_waypoint = None  # 重置批次间连续性跟踪

            # 重置打印开始时刻
            self.print_start_print_time = None

            # 重置姿态状态（确保从固定姿态开始）
            with self.orientation_lock:
                self.orientation_queue = []
                self.last_orientation = None
                self.planned_orientation = None

            logging.info("✅ 作业状态已重置（确保首次G命令会启动作业，时间基准将重新初始化）")
            logging.info("✅ 姿态状态已重置为固定姿态")

            # 确保 NRC SDK 已加载
            if self.nrc is None:
                self._import_nrc_sdk()

            # 连接双端口
            gcmd.respond_info("Connecting to robot arm...")
            self._connect_control_port()
            self._connect_servo_port()

            # 启动 TrapQ 轮询定时器
            self.poll_timer = self.reactor.register_timer(
                self._trapq_poll_callback,
                self.reactor.NOW
            )
            logging.info("TrapQ polling timer started")

            # 启动状态查询定时器
            self.status_query_timer = self.reactor.register_timer(
                self._status_query_callback,
                self.reactor.NOW
            )
            logging.info("Status query timer started")

            # 读取用户坐标系变换矩阵
            logging.info(f"Reading user coordinate system {self.user_coord_number}...")
            self.T_base_user = self._read_user_coord_transform()

            if self.T_base_user is None:
                logging.warning(
                    f"Failed to read user coord {self.user_coord_number}. "
                    f"Coordinate transform will be disabled."
                )
                gcmd.respond_info(
                    f"⚠️ Warning: Failed to read user coordinate system {self.user_coord_number}\n"
                    f"Coordinate transform disabled. Please check user coord settings."
                )
            else:
                logging.info(f"✅ User coordinate system {self.user_coord_number} loaded successfully")

            # 读取并输出 TCP（工具坐标系）参数
            self._read_and_log_tcp_params()

            # 输出连接状态
            logging.info("=" * 60)
            logging.info("NRC Robot Arm Connected")
            logging.info(f"  Control Port: {self.socket_fd_6001}")
            logging.info(f"  Servo Port: {self.socket_fd_7000}")
            logging.info(f"  User Coord: {self.user_coord_number} ({'Loaded' if self.T_base_user else 'Failed'})")
            logging.info(f"  Job Status: Not Started")
            logging.info("=" * 60)

            gcmd.respond_info(
                f"Robot arm connected successfully\n"
                f"  Control Port: {self.socket_fd_6001}\n"
                f"  Servo Port: {self.socket_fd_7000}\n"
                f"  User Coord: {self.user_coord_number} ({'Loaded' if self.T_base_user else 'Failed'})\n"
                f"  Status query: Active\n"
                f"  Job: Not started (will auto-start on first move)"
            )
        except Exception as e:
            gcmd.respond_error(f"Failed to connect: {e}")

    cmd_NRC_DISCONNECT_help = "Disconnect from robot arm"
    def cmd_NRC_DISCONNECT(self, gcmd):
        """断开机械臂连接并停止定时器"""
        try:
            # 停止定时器
            if self.poll_timer is not None:
                self.reactor.unregister_timer(self.poll_timer)
                self.poll_timer = None
                logging.info("TrapQ polling timer stopped")

            if self.status_query_timer is not None:
                self.reactor.unregister_timer(self.status_query_timer)
                self.status_query_timer = None
                logging.info("Status query timer stopped")

            # 断开连接
            self._disconnect_robot()

            # 重置状态标志
            self.job_started = False
            self.job_name = None
            self.is_first_batch = True
            self.first_waypoint_time = None
            self.last_sent_time = 0.0
            self.start_print_time = None
            self.total_waypoints_sent = 0
            self.total_moves_extracted = 0
            self.poll_count = 0

            # 重置姿态状态
            with self.orientation_lock:
                self.orientation_queue = []
                self.last_orientation = None
                self.planned_orientation = None

            logging.info("所有状态标志已重置（包括 job_started=False, 姿态状态）")

            gcmd.respond_info("✅ 机械臂已断开连接，定时器已停止")
        except Exception as e:
            gcmd.respond_error(f"Failed to disconnect: {e}")

    cmd_NRC_SHUTDOWN_help = "Shutdown robot arm"
    def cmd_NRC_SHUTDOWN(self, gcmd):
        """关闭机械臂"""
        try:
            self._disconnect_robot()
            gcmd.respond_info("Robot arm shutdown")
        except Exception as e:
            gcmd.respond_error(f"Shutdown error: {e}")

    cmd_NRC_POWER_ON_help = "Power on robot arm"
    def cmd_NRC_POWER_ON(self, gcmd):
        """机械臂上电"""
        try:
            with self.nrc_lock:
                ret = self.nrc.set_servo_poweron(self.socket_fd_6001)
            if ret != 0:
                gcmd.respond_error(f"Power on failed: ret={ret}")
            else:
                gcmd.respond_info("Robot arm powered on")
        except Exception as e:
            gcmd.respond_error(f"Power on error: {e}")

    cmd_NRC_POWER_OFF_help = "Power off robot arm"
    def cmd_NRC_POWER_OFF(self, gcmd):
        """机械臂下电"""
        try:
            with self.nrc_lock:
                ret = self.nrc.set_servo_poweroff(self.socket_fd_6001)
            if ret != 0:
                gcmd.respond_error(f"Power off failed: ret={ret}")
            else:
                gcmd.respond_info("Robot arm powered off")
        except Exception as e:
            gcmd.respond_error(f"Power off error: {e}")

    cmd_NRC_STATUS_help = "Query robot arm status"
    def cmd_NRC_STATUS(self, gcmd):
        """查询机械臂状态"""
        status = self.get_status(self.reactor.monotonic())

        mode_names = {-1: "Not Queried", 0: "Teach", 1: "Remote", 2: "Run"}
        servo_names = {-1: "Not Queried", 0: "Stop", 1: "Ready", 2: "Alarm", 3: "Running"}
        running_names = {-1: "Not Queried", 0: "Idle", 1: "Running", 2: "Paused"}

        # 格式化位置信息
        pos_joint = status['robot_position_joint']
        pos_cart = status['robot_position_cartesian']
        pos_joint_str = f"[{pos_joint[0]:.2f}, {pos_joint[1]:.2f}, {pos_joint[2]:.2f}, {pos_joint[3]:.2f}, {pos_joint[4]:.2f}, {pos_joint[5]:.2f}]"
        pos_cart_str = f"[{pos_cart[0]:.2f}, {pos_cart[1]:.2f}, {pos_cart[2]:.2f}, {pos_cart[3]:.2f}, {pos_cart[4]:.2f}, {pos_cart[5]:.2f}]"

        gcmd.respond_info(
            f"=== NRC Robot Arm Status ===\n"
            f"Connection:\n"
            f"  Connected: {status['connected']}\n"
            f"  Control Socket: {status['control_socket_fd']}\n"
            f"  Servo Socket: {status['servo_socket_fd']}\n"
            f"\n"
            f"Robot State:\n"
            f"  Position (Joint): {pos_joint_str}\n"
            f"  Position (Cartesian/Base): {pos_cart_str}\n"
            f"  Mode: {mode_names.get(status['robot_mode'], 'Unknown')}\n"
            f"  Servo State: {servo_names.get(status['servo_state'], 'Unknown')}\n"
            f"  Running State: {running_names.get(status['robot_running_state'], 'Unknown')}\n"
            f"  Line Speed: {status['robot_line_speed']:.2f} mm/s\n"
            f"\n"
            f"Job Status:\n"
            f"  Job Started: {status['job_started']}\n"
            f"  Job Name: {status['job_name'] if status['job_name'] else 'N/A'}\n"
            f"\n"
            f"Executor State:\n"
            f"  Paused: {status['is_paused']}\n"
            f"  Total Waypoints Sent: {status['total_waypoints_sent']}\n"
            f"  Total Moves Extracted: {status['total_moves_extracted']}\n"
            f"  Poll Count: {status['poll_count']}"
        )

    cmd_NRC_SET_MODE_help = "Set robot arm mode (MODE=0/1/2)"
    def cmd_NRC_SET_MODE(self, gcmd):
        """设置运行模式"""
        mode = gcmd.get_int('MODE', minval=0, maxval=2)

        try:
            with self.nrc_lock:
                ret = self.nrc.set_current_mode(self.socket_fd_6001, mode)
            if ret != 0:
                gcmd.respond_error(f"Set mode failed: ret={ret}")
            else:
                mode_names = {0: "Teach", 1: "Remote", 2: "Run"}
                gcmd.respond_info(f"Mode set to {mode_names[mode]}")
        except Exception as e:
            gcmd.respond_error(f"Set mode error: {e}")

    cmd_NRC_RESET_ERROR_help = "Reset robot arm errors"
    def cmd_NRC_RESET_ERROR(self, gcmd):
        """复位错误"""
        try:
            with self.nrc_lock:
                ret = self.nrc.clear_error(self.socket_fd_6001)
            if ret == 0 or ret == -4:
                gcmd.respond_info("Errors cleared")
            else:
                gcmd.respond_error(f"Clear error failed: ret={ret}")
        except Exception as e:
            gcmd.respond_error(f"Clear error error: {e}")

    cmd_NRC_START_JOB_help = "Start robot arm job (JOB_NAME=name)"
    def cmd_NRC_START_JOB(self, gcmd):
        """启动作业（6步流程）"""
        # 检查连接状态
        if self.socket_fd_6001 <= 0:
            gcmd.respond_error("未连接。请先运行 NRC_CONNECT 命令")
            return

        # 检查作业是否已启动
        if self.job_started:
            gcmd.respond_info(f"⚠️  作业 '{self.job_name}' 已经在运行中")
            return

        job_name = gcmd.get('JOB_NAME', self.auto_job_name)

        try:
            gcmd.respond_info(f"正在启动作业 '{job_name}'...")
            self._auto_start_job_with_name(job_name)

            # 执行预校准（如果启用）
            if self.enable_pre_calibration:
                gcmd.respond_info("正在执行预校准...")
                calibration_success = self._perform_pre_calibration()
                if calibration_success:
                    gcmd.respond_info(
                        f"✅ 预校准完成，测得延迟: {self.calibrated_timestamp_offset_ms:.1f}ms"
                    )
                else:
                    gcmd.respond_info(
                        f"⚠️ 预校准失败，将使用配置值: {self.initial_timestamp_offset_ms:.1f}ms"
                    )

            # 设置作业状态标志
            self.job_started = True
            self.job_name = job_name
            logging.info(f"作业状态已更新: job_started=True, job_name='{job_name}'")

            gcmd.respond_info(f"✅ 作业 '{job_name}' 启动成功")
        except Exception as e:
            gcmd.respond_error(f"❌ 作业启动失败: {e}")

    cmd_NRC_PAUSE_help = "Pause robot arm motion (flush pending moves)"
    def cmd_NRC_PAUSE(self, gcmd):
        """
        暂停机械臂运动

        操作：
        1. 设置 is_paused = True
        2. 发送所有累积的 moves
        3. 记录暂停时间
        """
        if self.is_paused:
            gcmd.respond_info("⚠️  已经处于暂停状态")
            return

        try:
            gcmd.respond_info("正在暂停机械臂运动...")

            # 设置暂停标志
            self.is_paused = True

            # 发送所有累积的 moves
            if self.accumulated_moves:
                gcmd.respond_info(f"正在发送 {len(self.accumulated_moves)} 个累积的 moves...")
                self._flush_accumulated_moves()

            gcmd.respond_info("✅ 机械臂运动已暂停")
            logging.info("[PAUSE] Robot arm motion paused, all pending moves sent")

        except Exception as e:
            gcmd.respond_error(f"❌ 暂停失败: {e}")

    cmd_NRC_RESUME_help = "Resume robot arm motion (continue with global timestamp)"
    def cmd_NRC_RESUME(self, gcmd):
        """
        恢复机械臂运动

        操作：
        1. 设置 is_paused = False
        2. 保持 print_start_print_time 不变（时间戳连续）
        3. 记录恢复时间
        """
        if not self.is_paused:
            gcmd.respond_info("⚠️  未处于暂停状态")
            return

        try:
            gcmd.respond_info("正在恢复机械臂运动...")

            # 清除暂停标志
            self.is_paused = False

            gcmd.respond_info("✅ 机械臂运动已恢复（时间戳保持连续）")
            logging.info("[RESUME] Robot arm motion resumed, continuing with global timestamp")

        except Exception as e:
            gcmd.respond_error(f"❌ 恢复失败: {e}")


# ========== Klipper 模块加载函数 ==========

def load_config(config):
    """
    Klipper 模块加载入口
    
    参数：
        config: Klipper 配置对象
    
    返回：
        NRCRobotArmExecutor 实例
    """
    return NRCRobotArmExecutor(config)

