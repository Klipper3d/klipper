import logging
import json
import time
import socket
import threading
# from . import probe

class LaserCFG:
    def __init__(self, config):
        self.serial = config.get("serial", "/tmp/ai_server_uds")
        self.x_offset = config.getfloat('x_offset', 0.)
        self.y_offset = config.getfloat('y_offset', 0.)
        self.z_offset = config.getfloat('z_offset', 10.)


class LaserVAL:
    def __init__(self, config):
        self.g29_cnt = int(0)
        self.middle_point_z = 0.0
        self.leveling = False


class LaserOBJ:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.toolhead = None
        self.bed_mesh = None
        self.gcode = self.printer.lookup_object('gcode')

    def find_objs(self):
        self.bed_mesh = self.printer.lookup_object('bed_mesh')
        self.toolhead = self.printer.lookup_object('toolhead')


class LaserCmd:
    def __init__(self, req):
        self.req = req
        self.res = None


class LaserClient:
    def __init__(self, serial='/tmp/ai_server_uds'):
        self.serial = serial
        self.conn = None
        self.send_queue = []
        self.thread = threading.Thread(target=self.run_thread)

    def connect(self):
        conn = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        conn.connect(self.serial)
        self.conn = conn
        self.thread.start()

    def _send(self, cmd):
        msg = json.dumps(cmd.req).encode() + b'\x03'
        self.conn.send(msg)
        data = b''
        while 1:
            data += self.conn.recv(1024)
            logging.info("---------------data:%s" % data)
            if data and data[-1] == 3:
                res = json.loads(data[:-1])
                logging.info("---------------json data:%s" % res)
                if res.get("control") == cmd.req.get("control"):
                    logging.info("---------------json res:%s" % res)
                    cmd.res = res
                    return True
                return False

    def run_thread(self):
        while 1:
            for i in self.send_queue:
                self._send(i)
            self.send_queue = []
            time.sleep(0.05)

    def open_flow(self, wait_func, wait_timeout):
        cmd = LaserCmd({"control": "open_flow"})
        self.send_queue.append(cmd)
        wait_func(wait_timeout)
        # logging.info("++++++++++++++++++res:%s" % cmd.res)
        # if cmd.res:
        #     if cmd.res.get("result").get("code") == 0:
        #         return
        # wait_func(wait_timeout)
        # raise Exception("open_flow fail")

    def close_flow(self, wait_func, wait_timeout):
        cmd = LaserCmd({"control": "close_flow"})
        self.send_queue.append(cmd)
        # wait_func(wait_timeout)
        # logging.info("++++++++++++++++++res:%s" % cmd.res)
        # if cmd.res:
        #     if cmd.res.get("result").get("code") == 0:
        #         return
        # wait_func(wait_timeout)
        # raise Exception("close_flow fail")

    def get_point_cloud(self, wait_func, wait_timeout):
        cmd = LaserCmd({"control": "get_point_cloud"})
        self.send_queue.append(cmd)
        wait_func(wait_timeout)
        logging.info("++++++++++++++++++res:%s" % cmd.res)
        if cmd.res:
            if cmd.res.get("result").get("code") == 0:
                point_cloud = cmd.res.get("result").get("point_cloud")
                if point_cloud:
                    return point_cloud
            else:
                raise Exception("get_point_cloud fail")
        # wait_func(wait_timeout)
        # raise Exception("get_point_cloud fail")

    def get_level_value(self, wait_func, wait_timeout, point_cloud):
        cmd = LaserCmd({"control": "get_level_value", "point_cloud": point_cloud})
        self.send_queue.append(cmd)
        wait_func(wait_timeout)
        logging.info("++++++++++++++++++res:%s" % cmd.res)
        if cmd.res:
            if cmd.res.get("result").get("code") == 0:
                value = cmd.res.get("result").get("value")
                if value:
                    return value
        # wait_func(wait_timeout)
        raise Exception("get_level_value fail")


class LaserEndstopWrapper:
    def __init__(self, config):
        self.cfg = LaserCFG(config)
        self.val = LaserVAL(config)
        self.obj = LaserOBJ(config)
        self.laser_client = LaserClient(self.cfg.serial)
        self.obj.printer.register_event_handler('klippy:mcu_identify', self._handle_mcu_identify)
        self.obj.printer.register_event_handler("klippy:shutdown", self._handle_shutdown)

    def _handle_mcu_identify(self):
        self.obj.find_objs()
        try:
            self.laser_client.connect()
        except Exception as e:
            error = "laser serial: %s, is not connected, please check device" % self.laser_client.serial
            logging.error("return error:%s, try e: %s" % (error, e))
            raise self.obj.gcode.error(error)

    def _handle_shutdown(self):
        self.val.leveling = False
        try:
            self.laser_client.close_flow(self.obj.toolhead.dwell, 1)
        except Exception as e:
            logging.exception(e)


    def _move(self, pos, speed, wait=True):
        self.obj.toolhead.manual_move(pos, speed)
        if wait:
            self.obj.toolhead.wait_moves()

    def laser_measure(self):
        """
        return laser measure z result
        """
        for i in range(3):
            point_cloud = self.laser_client.get_point_cloud(self.obj.toolhead.dwell, 3)
            if point_cloud:
                value = abs(self.laser_client.get_level_value(self.obj.toolhead.dwell, 3, point_cloud))
                self.obj.gcode.respond_info("laser_measure z:%.3f" % value)
                return value
            else:
                # if failed will retry
                self.obj.gcode.respond_info("laser_measure z: failed")
        return self.val.middle_point_z

    def run_G29_Z(self):
        self.val.leveling = True
        x_cnt = self.obj.bed_mesh.bmc.mesh_config['x_count']
        y_cnt = self.obj.bed_mesh.bmc.mesh_config['y_count']
        min_x, min_y = self.obj.bed_mesh.bmc.mesh_min
        max_x, max_y = self.obj.bed_mesh.bmc.mesh_max
        self.obj.toolhead.wait_moves()
        now_pos = self.obj.toolhead.get_position()
        if (int(self.val.g29_cnt) % int(x_cnt)) == 0:
            self.laser_client.open_flow(self.obj.toolhead.dwell, 1)
            # open flow            # self._move(now_pos[:2] + [now_pos[2] + 2, now_pos[3]], 30)
        if self.val.g29_cnt == 0:
            pass
            # now_pos[2] = self.get_best_rdy_z(min_x, min_y, self.val.rdy_pos) + self.cfg.best_above_z
        self.val.g29_cnt += 1
        if self.val.g29_cnt == x_cnt * y_cnt:
            self.val.g29_cnt = 0
            now_pos[2] = self.laser_measure()
            self.laser_client.close_flow(self.obj.toolhead.dwell, 1)
            self._move([(max_x - min_x) / 2 + min_x, (max_y - min_y) / 2 + min_y, 10], 30)
            self.val.leveling = False
        else:
            # laser measure
            now_pos[2] = self.laser_measure()

        if int((x_cnt * y_cnt)/2) + 1 == self.val.g29_cnt:
            self.val.middle_point_z = now_pos[2]

        return now_pos

    def is_leveling(self):
        return self.val.leveling


def load_config(config):
    laser = LaserEndstopWrapper(config)
    config.get_printer().add_object('laser', laser)
    return laser
