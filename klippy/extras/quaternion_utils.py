# 四元数工具库 - 姿态插值与转换
#
# 功能：
# 1. 欧拉角与四元数互转（无numpy依赖）
# 2. SLERP球面线性插值
# 3. 旋转角度计算
#
# 设计原则：
# - 轻量级实现，避免外部依赖
# - 数值稳定性优先
# - 清晰的文档和注释

import math

class Quaternion:
    """
    四元数类：表示三维旋转
    
    属性：
        w (float): 实部
        x, y, z (float): 虚部（i, j, k分量）
    
    约定：
        - 单位四元数：w² + x² + y² + z² = 1
        - 旋转顺序：ZYX（Yaw-Pitch-Roll）
    """
    
    def __init__(self, w, x, y, z):
        self.w = w
        self.x = x
        self.y = y
        self.z = z
    
    def normalize(self):
        """归一化为单位四元数"""
        norm = math.sqrt(self.w**2 + self.x**2 + self.y**2 + self.z**2)
        if norm < 1e-10:
            # 零四元数，返回单位四元数
            return Quaternion(1.0, 0.0, 0.0, 0.0)
        return Quaternion(self.w/norm, self.x/norm, self.y/norm, self.z/norm)
    
    def conjugate(self):
        """共轭四元数（逆旋转）"""
        return Quaternion(self.w, -self.x, -self.y, -self.z)
    
    def multiply(self, q):
        """四元数乘法（旋转复合）"""
        w = self.w*q.w - self.x*q.x - self.y*q.y - self.z*q.z
        x = self.w*q.x + self.x*q.w + self.y*q.z - self.z*q.y
        y = self.w*q.y - self.x*q.z + self.y*q.w + self.z*q.x
        z = self.w*q.z + self.x*q.y - self.y*q.x + self.z*q.w
        return Quaternion(w, x, y, z)
    
    def dot(self, q):
        """点积（用于计算夹角）"""
        return self.w*q.w + self.x*q.x + self.y*q.y + self.z*q.z
    
    def __repr__(self):
        return f"Quaternion(w={self.w:.4f}, x={self.x:.4f}, y={self.y:.4f}, z={self.z:.4f})"


def euler_to_quaternion(rx, ry, rz):
    """
    ZYX欧拉角转四元数（内旋序列：先绕Z，再绕Y，最后绕X）
    
    参数：
        rx (float): 绕X轴旋转角度（弧度，Roll）
        ry (float): 绕Y轴旋转角度（弧度，Pitch）
        rz (float): 绕Z轴旋转角度（弧度，Yaw）
    
    返回：
        Quaternion: 单位四元数
    
    注意：
        - 旋转顺序与NRC机械臂坐标系一致
        - 输入弧度，非角度
    """
    cy = math.cos(rz * 0.5)
    sy = math.sin(rz * 0.5)
    cp = math.cos(ry * 0.5)
    sp = math.sin(ry * 0.5)
    cr = math.cos(rx * 0.5)
    sr = math.sin(rx * 0.5)
    
    w = cr * cp * cy + sr * sp * sy
    x = sr * cp * cy - cr * sp * sy
    y = cr * sp * cy + sr * cp * sy
    z = cr * cp * sy - sr * sp * cy
    
    return Quaternion(w, x, y, z)


def quaternion_to_euler(q):
    """
    四元数转ZYX欧拉角
    
    参数：
        q (Quaternion): 输入四元数
    
    返回：
        tuple: (rx, ry, rz) 弧度
    
    注意：
        - 万向节死锁处理：当pitch=±90°时，无法唯一确定roll和yaw
        - 本函数返回一个有效解
    """
    # Roll (绕X轴旋转)
    sinr_cosp = 2 * (q.w * q.x + q.y * q.z)
    cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y)
    rx = math.atan2(sinr_cosp, cosr_cosp)
    
    # Pitch (绕Y轴旋转)
    sinp = 2 * (q.w * q.y - q.z * q.x)
    if abs(sinp) >= 1:
        ry = math.copysign(math.pi / 2, sinp)  # 限制在±90°
    else:
        ry = math.asin(sinp)
    
    # Yaw (绕Z轴旋转)
    siny_cosp = 2 * (q.w * q.z + q.x * q.y)
    cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z)
    rz = math.atan2(siny_cosp, cosy_cosp)
    
    return (rx, ry, rz)


def quaternion_slerp(q0, q1, t):
    """
    球面线性插值（Spherical Linear Interpolation）
    
    参数：
        q0 (Quaternion): 起始姿态
        q1 (Quaternion): 目标姿态
        t (float): 插值参数 [0, 1]，0返回q0，1返回q1
    
    返回：
        Quaternion: 插值结果
    
    特性：
        - 最短旋转路径
        - 角速度恒定
        - 数值稳定（自动处理接近平行的情况）
    """
    # 限制t在[0,1]
    t = max(0.0, min(1.0, t))
    
    # 计算夹角余弦
    dot = q0.dot(q1)
    
    # 确保最短路径（夹角<180°）
    if dot < 0:
        q1 = Quaternion(-q1.w, -q1.x, -q1.y, -q1.z)
        dot = -dot
    
    # 接近平行时使用线性插值（数值稳定性）
    if dot > 0.9995:
        result = Quaternion(
            q0.w + t * (q1.w - q0.w),
            q0.x + t * (q1.x - q0.x),
            q0.y + t * (q1.y - q0.y),
            q0.z + t * (q1.z - q0.z)
        )
        return result.normalize()
    
    # 标准SLERP公式
    theta_0 = math.acos(dot)  # 夹角
    sin_theta_0 = math.sin(theta_0)
    theta = theta_0 * t
    sin_theta = math.sin(theta)
    
    s0 = math.cos(theta) - dot * sin_theta / sin_theta_0
    s1 = sin_theta / sin_theta_0
    
    return Quaternion(
        s0 * q0.w + s1 * q1.w,
        s0 * q0.x + s1 * q1.x,
        s0 * q0.y + s1 * q1.y,
        s0 * q0.z + s1 * q1.z
    )


def get_rotation_angle(q0, q1):
    """
    计算两个姿态间的实际旋转角度
    
    参数：
        q0, q1 (Quaternion): 起止姿态
    
    返回：
        float: 旋转角度（弧度）
    
    用途：
        - 计算姿态变化量
        - 用于时间协调验证
    """
    # 计算相对旋转四元数
    q0_conj = q0.conjugate()
    q_delta = q0_conj.multiply(q1).normalize()
    
    # 旋转角度 = 2 * arccos(w)
    # 处理数值误差
    w_clamped = max(-1.0, min(1.0, q_delta.w))
    angle = 2.0 * math.acos(w_clamped)
    
    return angle

