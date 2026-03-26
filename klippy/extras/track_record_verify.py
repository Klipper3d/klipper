#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
轨迹记录验证脚本 - 验证姿态调整时的末端位置移动

用途：
1. 在打印过程中启动轨迹记录
2. 打印结束后保存轨迹数据
3. 分析轨迹数据，验证姿态调整时末端位置是否发生移动

使用方法：
1. 在 Mainsail 控制台执行 NRC_TRACK_RECORD_START 开始记录
2. 执行打印（使用带姿态的 GCODE）
3. 执行 NRC_TRACK_RECORD_STOP 停止记录
4. 执行 NRC_TRACK_RECORD_SAVE 保存记录
5. 或执行 NRC_TRACK_RECORD_ANALYZE 分析记录
; 1. 连接机械臂并启动作业
NRC_CONNECT
NRC_START_JOB

; 2. 开始采样（10ms 间隔）
NRC_SAMPLE_START INTERVAL=10

; 3. 执行带姿态的移动
G1 X100 Y100 Z10 A0 B10 C0 F500
G1 X100 Y110 Z10 A0 B0 C0 F500

; 4. 停止并保存
NRC_SAMPLE_STOP FILENAME=orient_test.csv

; 5. 分析结果
NRC_SAMPLE_ANALYZE FILENAME=orient_test.csv
依赖：
- NRC SDK (nrc_interface)
- Klipper

作者：AI Agent
日期：2026-02-02
"""

import sys
import os
import time
import math
import logging

# 添加 NRC SDK 路径
sys.path.insert(0, '/home/hxcunix/klipper/lib/nrc_sdk')

# 默认参数
DEFAULT_MAX_SAMPLING_NUM = 10000    # 最大采样点数（约 100 秒 @ 100Hz）
DEFAULT_SAMPLING_INTERVAL = 10     # 采样间隔（毫秒）


class TrackRecordVerifier:
    """轨迹记录验证器"""
    
    def __init__(self, printer):
        self.printer = printer
        self.nrc = None
        self.socket_fd = -1
        self.is_recording = False
        self.record_start_time = None
        
        # 尝试获取 executor 中的连接
        self.executor = printer.lookup_object('nrc_robot_arm_executor', None)
        if self.executor and hasattr(self.executor, 'nrc'):
            self.nrc = self.executor.nrc
            self.socket_fd = self.executor.socket_fd_6001
        
        # 注册 G-code 命令
        gcode = printer.lookup_object('gcode')
        gcode.register_command(
            'NRC_TRACK_RECORD_START',
            self.cmd_NRC_TRACK_RECORD_START,
            desc="开始轨迹记录"
        )
        gcode.register_command(
            'NRC_TRACK_RECORD_STOP',
            self.cmd_NRC_TRACK_RECORD_STOP,
            desc="停止轨迹记录"
        )
        gcode.register_command(
            'NRC_TRACK_RECORD_SAVE',
            self.cmd_NRC_TRACK_RECORD_SAVE,
            desc="保存轨迹记录"
        )
        gcode.register_command(
            'NRC_TRACK_RECORD_STATUS',
            self.cmd_NRC_TRACK_RECORD_STATUS,
            desc="查询轨迹记录状态"
        )
        gcode.register_command(
            'NRC_TRACK_RECORD_PLAYBACK',
            self.cmd_NRC_TRACK_RECORD_PLAYBACK,
            desc="回放轨迹记录"
        )
        gcode.register_command(
            'NRC_TRACK_RECORD_DELETE',
            self.cmd_NRC_TRACK_RECORD_DELETE,
            desc="删除轨迹记录"
        )
        
        logging.info("TrackRecordVerifier initialized")
    
    def _check_connection(self):
        """检查连接状态"""
        if self.nrc is None or self.socket_fd <= 0:
            # 尝试重新获取
            if self.executor:
                self.nrc = self.executor.nrc
                self.socket_fd = self.executor.socket_fd_6001
        
        if self.nrc is None:
            raise self.printer.command_error("NRC SDK 未初始化，请先执行 NRC_CONNECT")
        if self.socket_fd <= 0:
            raise self.printer.command_error("未连接机械臂，请先执行 NRC_CONNECT")
    
    def cmd_NRC_TRACK_RECORD_START(self, gcmd):
        """
        开始轨迹记录
        
        参数：
            MAX_SAMPLES: 最大采样点数（默认 10000）
            INTERVAL: 采样间隔（毫秒，默认 10）
        """
        self._check_connection()
        
        max_samples = gcmd.get_int('MAX_SAMPLES', DEFAULT_MAX_SAMPLING_NUM)
        interval = gcmd.get_int('INTERVAL', DEFAULT_SAMPLING_INTERVAL)
        
        if self.is_recording:
            gcmd.respond_info("⚠️ 轨迹记录已在进行中")
            return
        
        gcmd.respond_info(f"🎬 开始轨迹记录: 最大采样数={max_samples}, 间隔={interval}ms")
        
        try:
            ret = self.nrc.track_record_start(self.socket_fd, max_samples, interval)
            if ret == 0:
                self.is_recording = True
                self.record_start_time = time.time()
                gcmd.respond_info("✅ 轨迹记录已开始")
                gcmd.respond_info(f"   预计最大记录时间: {max_samples * interval / 1000:.1f} 秒")
            else:
                gcmd.respond_info(f"❌ 启动轨迹记录失败: ret={ret}")
        except Exception as e:
            gcmd.respond_info(f"❌ 启动轨迹记录异常: {e}")
    
    def cmd_NRC_TRACK_RECORD_STOP(self, gcmd):
        """停止轨迹记录"""
        self._check_connection()
        
        if not self.is_recording:
            gcmd.respond_info("⚠️ 轨迹记录未在进行中")
            return
        
        gcmd.respond_info("⏹️ 停止轨迹记录...")
        
        try:
            ret = self.nrc.track_record_stop(self.socket_fd)
            if ret == 0:
                duration = time.time() - self.record_start_time if self.record_start_time else 0
                self.is_recording = False
                gcmd.respond_info(f"✅ 轨迹记录已停止")
                gcmd.respond_info(f"   记录时长: {duration:.1f} 秒")
                gcmd.respond_info("   使用 NRC_TRACK_RECORD_SAVE TRAJ_NAME=xxx 保存记录")
            else:
                gcmd.respond_info(f"❌ 停止轨迹记录失败: ret={ret}")
        except Exception as e:
            gcmd.respond_info(f"❌ 停止轨迹记录异常: {e}")
    
    def cmd_NRC_TRACK_RECORD_SAVE(self, gcmd):
        """
        保存轨迹记录
        
        参数：
            TRAJ_NAME: 轨迹名称（默认 "verify_orient"）
        """
        self._check_connection()
        
        traj_name = gcmd.get('TRAJ_NAME', 'verify_orient')
        
        gcmd.respond_info(f"💾 保存轨迹记录: {traj_name}")
        
        try:
            ret = self.nrc.track_record_save(self.socket_fd, traj_name)
            if ret == 0:
                gcmd.respond_info(f"✅ 轨迹记录已保存: {traj_name}")
                gcmd.respond_info("   可在示教器上查看轨迹")
                gcmd.respond_info(f"   使用 NRC_TRACK_RECORD_PLAYBACK VEL=50 回放")
            else:
                gcmd.respond_info(f"❌ 保存轨迹记录失败: ret={ret}")
        except Exception as e:
            gcmd.respond_info(f"❌ 保存轨迹记录异常: {e}")
    
    def cmd_NRC_TRACK_RECORD_STATUS(self, gcmd):
        """查询轨迹记录状态"""
        self._check_connection()
        
        gcmd.respond_info("📊 查询轨迹记录状态...")
        
        try:
            # recordStart 参数：用于返回记录是否开始
            record_start = self.nrc.VectorInt()
            ret = self.nrc.get_track_record_status(self.socket_fd, record_start)
            
            if ret == 0:
                status = "正在记录" if (len(record_start) > 0 and record_start[0] == 1) else "未在记录"
                gcmd.respond_info(f"✅ 轨迹记录状态: {status}")
                
                if self.is_recording and self.record_start_time:
                    duration = time.time() - self.record_start_time
                    gcmd.respond_info(f"   已记录时长: {duration:.1f} 秒")
            else:
                gcmd.respond_info(f"❌ 查询状态失败: ret={ret}")
        except Exception as e:
            gcmd.respond_info(f"❌ 查询状态异常: {e}")
    
    def cmd_NRC_TRACK_RECORD_PLAYBACK(self, gcmd):
        """
        回放轨迹记录
        
        参数：
            VEL: 回放速度百分比（1-100，默认 50）
        """
        self._check_connection()
        
        vel = gcmd.get_int('VEL', 50)
        vel = max(1, min(100, vel))
        
        gcmd.respond_info(f"▶️ 回放轨迹记录: 速度={vel}%")
        gcmd.respond_info("⚠️ 注意安全！机械臂将开始运动")
        
        try:
            ret = self.nrc.track_record_playback(self.socket_fd, vel)
            if ret == 0:
                gcmd.respond_info("✅ 轨迹回放已开始")
            else:
                gcmd.respond_info(f"❌ 轨迹回放失败: ret={ret}")
        except Exception as e:
            gcmd.respond_info(f"❌ 轨迹回放异常: {e}")
    
    def cmd_NRC_TRACK_RECORD_DELETE(self, gcmd):
        """删除轨迹记录"""
        self._check_connection()
        
        gcmd.respond_info("🗑️ 删除轨迹记录...")
        
        try:
            ret = self.nrc.track_record_delete(self.socket_fd)
            if ret == 0:
                gcmd.respond_info("✅ 轨迹记录已删除")
            else:
                gcmd.respond_info(f"❌ 删除轨迹记录失败: ret={ret}")
        except Exception as e:
            gcmd.respond_info(f"❌ 删除轨迹记录异常: {e}")


class PositionSampler:
    """
    本地位置采样器
    
    使用 get_current_position 高频采样，保存到本地 CSV 文件
    可用于分析姿态调整时的末端位置变化
    """
    
    def __init__(self, printer):
        self.printer = printer
        self.nrc = None
        self.socket_fd = -1
        self.executor = None
        
        # 采样状态
        self.is_sampling = False
        self.sample_thread = None
        self.samples = []
        self.sample_start_time = None
        self.sample_interval_ms = 10  # 默认 10ms 采样
        
        # 获取 executor
        self.executor = printer.lookup_object('nrc_robot_arm_executor', None)
        
        # 注册 G-code 命令
        gcode = printer.lookup_object('gcode')
        gcode.register_command(
            'NRC_SAMPLE_START',
            self.cmd_NRC_SAMPLE_START,
            desc="开始本地位置采样"
        )
        gcode.register_command(
            'NRC_SAMPLE_STOP',
            self.cmd_NRC_SAMPLE_STOP,
            desc="停止本地位置采样并保存"
        )
        gcode.register_command(
            'NRC_SAMPLE_ANALYZE',
            self.cmd_NRC_SAMPLE_ANALYZE,
            desc="分析采样数据"
        )
        
        logging.info("PositionSampler initialized")
    
    def _check_connection(self):
        """检查连接状态"""
        # 每次调用时动态获取 executor（因为初始化时可能还没连接）
        self.executor = self.printer.lookup_object('nrc_robot_arm_executor', None)
        if self.executor:
            self.nrc = getattr(self.executor, 'nrc', None)
            self.socket_fd = getattr(self.executor, 'socket_fd_6001', -1)
        
        if self.nrc is None:
            raise self.printer.command_error("NRC SDK 未初始化，请检查 nrc_robot_arm_executor 是否已加载")
        if self.socket_fd <= 0:
            raise self.printer.command_error("未连接机械臂，请先执行 NRC_CONNECT")
    
    def _sampling_thread(self):
        """采样线程"""
        import time
        
        while self.is_sampling:
            try:
                # 查询当前位置（基坐标系）
                pos = self.nrc.VectorDouble()
                ret = self.nrc.get_current_position(self.socket_fd, 1, pos)
                
                if ret == 0 and len(pos) >= 6:
                    elapsed_ms = (time.time() - self.sample_start_time) * 1000
                    self.samples.append({
                        'time_ms': elapsed_ms,
                        'x': pos[0],
                        'y': pos[1],
                        'z': pos[2],
                        'rx': pos[3],
                        'ry': pos[4],
                        'rz': pos[5]
                    })
                
                time.sleep(self.sample_interval_ms / 1000.0)
            except Exception as e:
                logging.error(f"Sampling error: {e}")
                break
    
    def cmd_NRC_SAMPLE_START(self, gcmd):
        """
        开始本地位置采样
        
        参数：
            INTERVAL: 采样间隔（毫秒，默认 10）
        """
        import threading
        import time
        
        self._check_connection()
        
        if self.is_sampling:
            gcmd.respond_info("⚠️ 采样已在进行中")
            return
        
        self.sample_interval_ms = gcmd.get_int('INTERVAL', 10)
        self.samples = []
        self.sample_start_time = time.time()
        self.is_sampling = True
        
        self.sample_thread = threading.Thread(target=self._sampling_thread, daemon=True)
        self.sample_thread.start()
        
        gcmd.respond_info(f"🎬 开始位置采样: 间隔={self.sample_interval_ms}ms")
        gcmd.respond_info("   执行打印命令后，使用 NRC_SAMPLE_STOP 停止并保存")
    
    def cmd_NRC_SAMPLE_STOP(self, gcmd):
        """
        停止采样并保存到 CSV 文件
        
        参数：
            FILENAME: 文件名（默认 position_samples.csv）
        """
        import os
        import time
        
        if not self.is_sampling:
            gcmd.respond_info("⚠️ 采样未在进行中")
            return
        
        self.is_sampling = False
        if self.sample_thread:
            self.sample_thread.join(timeout=1.0)
        
        filename = gcmd.get('FILENAME', 'position_samples.csv')
        filepath = os.path.expanduser(f'~/printer_data/gcodes/{filename}')
        
        # 保存到 CSV
        try:
            with open(filepath, 'w') as f:
                f.write("time_ms,x,y,z,rx,ry,rz\n")
                for s in self.samples:
                    f.write(f"{s['time_ms']:.1f},{s['x']:.4f},{s['y']:.4f},{s['z']:.4f},"
                           f"{s['rx']:.4f},{s['ry']:.4f},{s['rz']:.4f}\n")
            
            gcmd.respond_info(f"✅ 采样完成: {len(self.samples)} 个数据点")
            gcmd.respond_info(f"   已保存到: {filepath}")
            gcmd.respond_info("   使用 NRC_SAMPLE_ANALYZE 分析数据")
        except Exception as e:
            gcmd.respond_info(f"❌ 保存失败: {e}")
    
    def cmd_NRC_SAMPLE_ANALYZE(self, gcmd):
        """
        分析采样数据，检测姿态调整期间的位置变化
        
        参数：
            FILENAME: 文件名（默认 position_samples.csv）
        """
        import os
        import math
        
        filename = gcmd.get('FILENAME', 'position_samples.csv')
        filepath = os.path.expanduser(f'~/printer_data/gcodes/{filename}')
        
        if not os.path.exists(filepath):
            gcmd.respond_info(f"❌ 文件不存在: {filepath}")
            return
        
        # 读取数据
        samples = []
        try:
            with open(filepath, 'r') as f:
                lines = f.readlines()[1:]  # 跳过表头
                for line in lines:
                    parts = line.strip().split(',')
                    if len(parts) >= 7:
                        samples.append({
                            'time_ms': float(parts[0]),
                            'x': float(parts[1]),
                            'y': float(parts[2]),
                            'z': float(parts[3]),
                            'rx': float(parts[4]),
                            'ry': float(parts[5]),
                            'rz': float(parts[6])
                        })
        except Exception as e:
            gcmd.respond_info(f"❌ 读取失败: {e}")
            return
        
        if len(samples) < 2:
            gcmd.respond_info("❌ 数据点不足")
            return
        
        # 分析：检测姿态变化期间的位置变化
        gcmd.respond_info(f"📊 分析 {len(samples)} 个数据点...")
        
        # 计算每个时刻的位置变化和姿态变化
        orientation_change_events = []
        position_drift_total = 0.0
        
        for i in range(1, len(samples)):
            prev = samples[i-1]
            curr = samples[i]
            
            # 位置变化（mm）
            dx = curr['x'] - prev['x']
            dy = curr['y'] - prev['y']
            dz = curr['z'] - prev['z']
            pos_change = math.sqrt(dx*dx + dy*dy + dz*dz)
            
            # 姿态变化（度）
            drx = abs(curr['rx'] - prev['rx'])
            dry = abs(curr['ry'] - prev['ry'])
            drz = abs(curr['rz'] - prev['rz'])
            orient_change = math.sqrt(drx*drx + dry*dry + drz*drz)
            
            # 检测：姿态变化大（>0.5°/采样）但位置也变化了（>0.1mm）
            if orient_change > 0.5 and pos_change > 0.1:
                orientation_change_events.append({
                    'time_ms': curr['time_ms'],
                    'orient_change_deg': orient_change,
                    'pos_change_mm': pos_change
                })
                position_drift_total += pos_change
        
        # 输出分析结果
        gcmd.respond_info("=" * 50)
        gcmd.respond_info("📋 分析结果：姿态调整期间的位置变化")
        gcmd.respond_info("=" * 50)
        
        if orientation_change_events:
            gcmd.respond_info(f"⚠️ 检测到 {len(orientation_change_events)} 次姿态调整期间位置变化")
            gcmd.respond_info(f"   累计位置漂移: {position_drift_total:.3f} mm")
            
            # 显示前 5 个事件
            for i, evt in enumerate(orientation_change_events[:5]):
                gcmd.respond_info(
                    f"   [{i+1}] t={evt['time_ms']:.0f}ms: "
                    f"姿态Δ={evt['orient_change_deg']:.2f}°, "
                    f"位置Δ={evt['pos_change_mm']:.3f}mm"
                )
            
            if len(orientation_change_events) > 5:
                gcmd.respond_info(f"   ... 还有 {len(orientation_change_events)-5} 个事件")
        else:
            gcmd.respond_info("✅ 未检测到姿态调整期间的异常位置变化")
        
        gcmd.respond_info("=" * 50)


def load_config(config):
    """Klipper 配置加载入口"""
    printer = config.get_printer()
    TrackRecordVerifier(printer)
    return PositionSampler(printer)
