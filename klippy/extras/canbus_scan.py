# canbus_scan.py - Klipper extra for scanning CAN bus nodes
# Place in ~/klipper/klippy/extras/canbus_scan.py
#
# Usage in printer.cfg:
#   [canbus_scan]
#
# Then run gcode:
#   SCAN_CANBUS          - scan and report all UUIDs
#   SCAN_CANBUS IFACE=can0 TIMEOUT=3.0

import socket
import struct
import select
import logging
import time
import sys

# SocketCAN constants
CAN_RAW = 1
PF_CAN = 29
SOL_CAN_BASE = 100
SOL_CAN_RAW = SOL_CAN_BASE + CAN_RAW
CAN_RAW_FILTER = 1

# Frame format: id (4B), dlc (1B), pad (3B), data (8B)
CAN_FRAME_FMT = "<IB3x8s"
CAN_FRAME_SIZE = struct.calcsize(CAN_FRAME_FMT)

# Klipper CAN admin protocol
CANBUS_ID_ADMIN = 0x00   # broadcast query goes to ID 0
CANBUS_ID_ADMIN_RESP = 0x01   # nodes respond on ID 1
KLIPPER_ADMIN_QUERY = 0x00   # query command byte


def _to_bytes(data):
    if sys.version_info[0] >= 3:
        return bytes(data)
    return ''.join([chr(b) for b in data])


class CanbusScan:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.iface = config.get('interface', 'can0')
        self.timeout = config.getfloat('timeout', 2.0, minval=0.5, maxval=10.0)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'SCAN_CANBUS',
            self.cmd_SCAN_CANBUS,
            desc="Scan CAN bus and report all Klipper node UUIDs"
        )

    # ------------------------------------------------------------------ #
    #  Low-level SocketCAN helpers                                         #
    # ------------------------------------------------------------------ #

    def _build_frame(self, can_id, data):
        """Pack a CAN frame."""
        data = bytearray(data)
        dlc = len(data)
        if dlc < 8:
            data.extend([0] * (8 - dlc))
        return struct.pack(CAN_FRAME_FMT, can_id, dlc, _to_bytes(data[:8]))

    def _parse_frame(self, raw):
        """Unpack a CAN frame to (can_id, dlc, data_bytes)."""
        can_id, dlc, data = struct.unpack(CAN_FRAME_FMT, raw)
        can_id &= 0x1FFFFFFF          # strip flags
        return can_id, dlc, bytearray(data[:dlc])

    def _set_filter(self, sock, can_id, mask=0x7FF):
        """Apply a receive filter so we only get frames matching can_id."""
        filt = struct.pack("=II", can_id, mask)
        sock.setsockopt(SOL_CAN_RAW, CAN_RAW_FILTER, filt)

    # ------------------------------------------------------------------ #
    #  Core scanner                                                        #
    # ------------------------------------------------------------------ #

    def _scan(self, iface, timeout):
        """
        Send Klipper admin query, collect UUID responses.
        Returns list of UUID hex strings.
        """
        found = {}

        try:
            sock = socket.socket(PF_CAN, socket.SOCK_RAW, CAN_RAW)
        except OSError as e:
            raise Exception("Cannot open CAN socket: %s" % e)

        try:
            sock.bind((iface,))
            self._set_filter(sock, CANBUS_ID_ADMIN_RESP)
            sock.setblocking(False)

            # Send query frame to admin ID 0x00
            query_frame = self._build_frame(
                CANBUS_ID_ADMIN, bytearray([KLIPPER_ADMIN_QUERY]))
            sock.send(query_frame)
            logging.info("canbus_scan: query sent on %s", iface)

            # Collect responses until timeout
            monotonic = getattr(time, 'monotonic', time.time)
            start = monotonic()

            while True:
                remaining = timeout - (monotonic() - start)
                if remaining <= 0:
                    break

                ready = select.select([sock], [], [], remaining)
                if not ready[0]:
                    break

                raw = sock.recv(CAN_FRAME_SIZE)
                if len(raw) < CAN_FRAME_SIZE:
                    continue

                can_id, dlc, data = self._parse_frame(raw)

                # Klipper UUID response: ID=0x01, 6 bytes UUID
                if can_id == CANBUS_ID_ADMIN_RESP and dlc >= 6:
                    uuid_bytes = data[:6]
                    uuid_str = ''.join('%02x' % b for b in uuid_bytes)
                    if uuid_str not in found:
                        found[uuid_str] = True
                        logging.info("canbus_scan: found UUID %s", uuid_str)

        finally:
            sock.close()

        return list(found.keys())

    # ------------------------------------------------------------------ #
    #  GCode command                                                       #
    # ------------------------------------------------------------------ #

    def cmd_SCAN_CANBUS(self, gcmd):
        iface = gcmd.get('IFACE', self.iface)
        timeout = gcmd.get_float('TIMEOUT', self.timeout,
                                 minval=0.5, maxval=10.0)

        self.gcode.respond_info(
            "Scanning CAN bus '%s' (timeout %.1fs)..." % (iface, timeout)
        )

        try:
            uuids = self._scan(iface, timeout)
        except Exception as e:
            raise self.gcode.error("SCAN_CANBUS failed: %s" % e)

        if not uuids:
            self.gcode.respond_info(
                "No CAN nodes found.\n"
                "Check wiring, power, and that the CAN "
                "interface '%s' is correct." %
                iface)
        else:
            lines = ["Found %d CAN node(s):" % len(uuids)]
            for i, uid in enumerate(uuids, 1):
                lines.append("  Node %d: canbus_uuid=%s" % (i, uid))
            lines.append("")
            lines.append("Add to printer.cfg:")
            for uid in uuids:
                lines.append("  [mcu my_node]")
                lines.append("  canbus_uuid: %s" % uid)
            self.gcode.respond_info('\n'.join(lines))


def load_config(config):
    return CanbusScan(config)
