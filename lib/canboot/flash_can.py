#!/usr/bin/env python3
# Script to upload software via Can Bootloader
#
# Copyright (C) 2022 Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from __future__ import annotations
import sys
import os
import asyncio
import socket
import struct
import logging
import errno
import argparse
import hashlib
import pathlib
from typing import Dict, List, Optional, Union

def output_line(msg: str) -> None:
    sys.stdout.write(msg + "\n")
    sys.stdout.flush()

def output(msg: str) -> None:
    sys.stdout.write(msg)
    sys.stdout.flush()

# Standard crc16 ccitt, take from msgproto.py in Klipper
def crc16_ccitt(buf: Union[bytes, bytearray]) -> int:
    crc = 0xffff
    for data in buf:
        data ^= crc & 0xff
        data ^= (data & 0x0f) << 4
        crc = ((data << 8) | (crc >> 8)) ^ (data >> 4) ^ (data << 3)
    return crc & 0xFFFF

logging.basicConfig(level=logging.INFO)
CAN_FMT = "<IB3x8s"
CAN_READER_LIMIT = 1024 * 1024

# Canboot Defs
CMD_HEADER = b'\x01\x88'
CMD_TRAILER = b'\x99\x03'
BOOTLOADER_CMDS = {
    'CONNECT': 0x11,
    'SEND_BLOCK': 0x12,
    'SEND_EOF': 0x13,
    'REQUEST_BLOCK': 0x14,
    'COMPLETE': 0x15,
    'GET_CANBUS_ID': 0x16,
}

ACK_SUCCESS = 0xa0
NACK = 0xf1

# Klipper Admin Defs (for jumping to bootloader)
KLIPPER_ADMIN_ID = 0x3f0
KLIPPER_SET_NODE_CMD = 0x01
KLIPPER_REBOOT_CMD = 0x02

# CAN Admin Defs
CANBUS_ID_ADMIN = 0x3f0
CANBUS_ID_ADMIN_RESP = 0x3f1
CANBUS_CMD_QUERY_UNASSIGNED = 0x00
CANBUS_CMD_SET_NODEID = 0x11
CANBUS_CMD_CLEAR_NODE_ID = 0x12
CANBUS_RESP_NEED_NODEID = 0x20
CANBUS_NODEID_OFFSET = 128

class FlashCanError(Exception):
    pass

class CanFlasher:
    def __init__(
        self,
        node: CanNode,
        fw_file: pathlib.Path
    ) -> None:
        self.node = node
        self.fw_name = fw_file
        self.fw_sha = hashlib.sha1()
        self.file_size = 0
        self.block_size = 64
        self.block_count = 0
        self.app_start_addr = 0

    async def connect_btl(self):
        output_line("Attempting to connect to bootloader")
        ret = await self.send_command('CONNECT')
        pinfo = ret[:12]
        mcu_type = ret[12:]
        ver_bytes, start_addr, self.block_size = struct.unpack("<4sII", pinfo)
        self.app_start_addr = start_addr
        proto_version = ".".join([str(v) for v in reversed(ver_bytes[:3])])
        if self.block_size not in [64, 128, 256, 512]:
            raise FlashCanError("Invalid Block Size: %d" % (self.block_size,))
        while mcu_type and mcu_type[-1] == 0x00:
            mcu_type = mcu_type[:-1]
        mcu_type = mcu_type.decode()
        output_line(
            f"CanBoot Connected\nProtocol Version: {proto_version}\n"
            f"Block Size: {self.block_size} bytes\n"
            f"Application Start: 0x{self.app_start_addr:4X}\n"
            f"MCU type: {mcu_type}"
        )

    async def verify_canbus_uuid(self, uuid):
        output_line("Verifying canbus connection")
        ret = await self.send_command('GET_CANBUS_ID')
        mcu_uuid = sum([v << ((5 - i) * 8) for i, v in enumerate(ret[:6])])
        if mcu_uuid != uuid:
            raise FlashCanError("UUID mismatch (%s vs %s)" % (uuid, mcu_uuid))

    async def send_command(
        self,
        cmdname: str,
        payload: bytes = b"",
        tries: int = 5
    ) -> bytearray:
        word_cnt = (len(payload) // 4) & 0xFF
        cmd = BOOTLOADER_CMDS[cmdname]
        out_cmd = bytearray(CMD_HEADER)
        out_cmd.append(cmd)
        out_cmd.append(word_cnt)
        if payload:
            out_cmd.extend(payload)
        crc = crc16_ccitt(out_cmd[2:])
        out_cmd.extend(struct.pack("<H", crc))
        out_cmd.extend(CMD_TRAILER)
        while tries:
            data = bytearray()
            recd_len = 0
            try:
                self.node.write(out_cmd)
                read_done = False
                while not read_done:
                    ret = await self.node.readuntil()
                    data.extend(ret)
                    while len(data) > 7:
                        if data[:2] != CMD_HEADER:
                            data = data[1:]
                            continue
                        recd_len = data[3] * 4
                        read_done = len(data) == recd_len + 8
                        break

            except Exception:
                logging.exception("Can Read Error")
            else:
                trailer = data[-2:]
                recd_crc, = struct.unpack("<H", data[-4:-2])
                calc_crc = crc16_ccitt(data[2:-4])
                recd_ack = data[2]
                cmd_response = 0
                if recd_len:
                    cmd_response, = struct.unpack("<I", data[4:8])
                if trailer != CMD_TRAILER:
                    logging.info(
                        f"Command '{cmdname}': Invalid Trailer Received "
                        f"0x{trailer.hex()}"
                    )
                elif recd_crc != calc_crc:
                    logging.info(
                        f"Command '{cmdname}': Frame CRC Mismatch, expected: "
                        f"{calc_crc}, received {recd_crc}"
                    )
                elif recd_ack != ACK_SUCCESS:
                    logging.info(f"Command '{cmdname}': Received NACK")
                elif cmd_response != cmd:
                    logging.info(
                        f"Command '{cmdname}': Acknowledged wrong command, "
                        f"expected: {cmd:2x}, received: {cmd_response:2x}"
                    )
                else:
                    # Validation passed, return payload sans command
                    if recd_len <= 4:
                        return bytearray()
                    return data[8:recd_len + 4]
            tries -= 1
            # clear the read buffer
            try:
                await self.node.read(1024, timeout=.1)
            except asyncio.TimeoutError:
                pass
            await asyncio.sleep(.1)
        raise FlashCanError("Error sending command [%s] to Can Device"
                            % (cmdname))

    async def send_file(self):
        last_percent = 0
        output_line("Flashing '%s'..." % (self.fw_name))
        output("\n[")
        with open(self.fw_name, 'rb') as f:
            f.seek(0, os.SEEK_END)
            self.file_size = f.tell()
            f.seek(0)
            flash_address = self.app_start_addr
            while True:
                buf = f.read(self.block_size)
                if not buf:
                    break
                if len(buf) < self.block_size:
                    buf += b"\xFF" * (self.block_size - len(buf))
                self.fw_sha.update(buf)
                prefix = struct.pack("<I", flash_address)
                for _ in range(3):
                    resp = await self.send_command('SEND_BLOCK', prefix + buf)
                    recd_addr, = struct.unpack("<I", resp)
                    if recd_addr == flash_address:
                        break
                    logging.info(
                        f"Block write mismatch: expected: {flash_address:4X}, "
                        f"received: {recd_addr:4X}"
                    )
                    await asyncio.sleep(.1)
                else:
                    raise FlashCanError(
                        f"Flash write failed, block address 0x{recd_addr:4X}"
                    )
                flash_address += self.block_size
                self.block_count += 1
                uploaded = self.block_count * self.block_size
                pct = int(uploaded / float(self.file_size) * 100 + .5)
                if pct >= last_percent + 2:
                    last_percent += 2.
                    output("#")
            resp = await self.send_command('SEND_EOF')
            page_count, = struct.unpack("<I", resp)
            output_line("]\n\nWrite complete: %d pages" % (page_count))

    async def verify_file(self):
        last_percent = 0
        output_line("Verifying (block count = %d)..." % (self.block_count,))
        output("\n[")
        ver_sha = hashlib.sha1()
        for i in range(self.block_count):
            flash_address = i * self.block_size + self.app_start_addr
            for _ in range(3):
                payload = struct.pack("<I", flash_address)
                resp = await self.send_command("REQUEST_BLOCK", payload)
                recd_addr, = struct.unpack("<I", resp[:4])
                if recd_addr == flash_address:
                    break
                logging.info(
                    f"Block read mismatch: expected: 0x{flash_address:4X}, "
                    f"received: 0x{recd_addr}"
                )
                await asyncio.sleep(.1)
            else:
                output_line("Error")
                raise FlashCanError("Block Request Error, block: %d" % (i,))
            ver_sha.update(resp[4:])
            pct = int(i * self.block_size / float(self.file_size) * 100 + .5)
            if pct >= last_percent + 2:
                last_percent += 2
                output("#")
        ver_hex = ver_sha.hexdigest().upper()
        fw_hex = self.fw_sha.hexdigest().upper()
        if ver_hex != fw_hex:
            raise FlashCanError("Checksum mismatch: Expected %s, Received %s"
                                % (fw_hex, ver_hex))
        output_line("]\n\nVerification Complete: SHA = %s" % (ver_hex))

    async def finish(self):
        await self.send_command("COMPLETE")


class CanNode:
    def __init__(self, node_id: int, cansocket: CanSocket) -> None:
        self.node_id = node_id
        self._reader = asyncio.StreamReader(CAN_READER_LIMIT)
        self._cansocket = cansocket

    async def read(
        self, n: int = -1, timeout: Optional[float] = 2
    ) -> bytes:
        return await asyncio.wait_for(self._reader.read(n), timeout)

    async def readexactly(
        self, n: int, timeout: Optional[float] = 2
    ) -> bytes:
        return await asyncio.wait_for(self._reader.readexactly(n), timeout)

    async def readuntil(
        self, sep: bytes = b"\x03", timeout: Optional[float] = 2
    ) -> bytes:
        return await asyncio.wait_for(self._reader.readuntil(sep), timeout)

    def write(self, payload: Union[bytes, bytearray]) -> None:
        if isinstance(payload, bytearray):
            payload = bytes(payload)
        self._cansocket.send(self.node_id, payload)

    async def write_with_response(
        self,
        payload: Union[bytearray, bytes],
        resp_length: int,
        timeout: Optional[float] = 2.
    ) -> bytes:
        self.write(payload)
        return await self.readexactly(resp_length, timeout)

    def feed_data(self, data: bytes) -> None:
        self._reader.feed_data(data)

    def close(self) -> None:
        self._reader.feed_eof()

class CanSocket:
    def __init__(self, loop: asyncio.AbstractEventLoop):
        self._loop = loop
        self.cansock = socket.socket(socket.PF_CAN, socket.SOCK_RAW,
                                     socket.CAN_RAW)
        self.admin_node = CanNode(CANBUS_ID_ADMIN, self)
        self.nodes: Dict[int, CanNode] = {
            CANBUS_ID_ADMIN_RESP: self.admin_node
        }

        self.input_buffer = b""
        self.output_packets: List[bytes] = []
        self.input_busy = False
        self.output_busy = False
        self.closed = True

    def _handle_can_response(self) -> None:
        try:
            data = self.cansock.recv(4096)
        except socket.error as e:
            # If bad file descriptor allow connection to be
            # closed by the data check
            if e.errno == errno.EBADF:
                logging.exception("Can Socket Read Error, closing")
                data = b''
            else:
                return
        if not data:
            # socket closed
            self.close()
            return
        self.input_buffer += data
        if self.input_busy:
            return
        self.input_busy = True
        while len(self.input_buffer) >= 16:
            packet = self.input_buffer[:16]
            self._process_packet(packet)
            self.input_buffer = self.input_buffer[16:]
        self.input_busy = False

    def _process_packet(self, packet: bytes) -> None:
        can_id, length, data = struct.unpack(CAN_FMT, packet)
        can_id &= socket.CAN_EFF_MASK
        payload = data[:length]
        node = self.nodes.get(can_id)
        if node is not None:
            node.feed_data(payload)

    def send(self, can_id: int, payload: bytes = b"") -> None:
        if can_id > 0x7FF:
            can_id |= socket.CAN_EFF_FLAG
        if not payload:
            packet = struct.pack(CAN_FMT, can_id, 0, b"")
            self.output_packets.append(packet)
        else:
            while payload:
                length = min(len(payload), 8)
                pkt_data = payload[:length]
                payload = payload[length:]
                packet = struct.pack(
                    CAN_FMT, can_id, length, pkt_data)
                self.output_packets.append(packet)
        if self.output_busy:
            return
        self.output_busy = True
        asyncio.create_task(self._do_can_send())

    async def _do_can_send(self):
        while self.output_packets:
            packet = self.output_packets.pop(0)
            try:
                await self._loop.sock_sendall(self.cansock, packet)
            except socket.error:
                logging.info("Socket Write Error, closing")
                self.close()
                break
        self.output_busy = False

    def _jump_to_bootloader(self, uuid: int):
        # TODO: Send Klipper Admin command to jump to bootloader.
        # It will need to be implemented
        output_line("Sending bootloader jump command...")
        plist = [(uuid >> ((5 - i) * 8)) & 0xFF for i in range(6)]
        plist.insert(0, KLIPPER_REBOOT_CMD)
        self.send(KLIPPER_ADMIN_ID, bytes(plist))

    async def _query_uuids(self) -> List[int]:
        output_line("Checking for canboot nodes...")
        payload = bytes([CANBUS_CMD_QUERY_UNASSIGNED])
        self.admin_node.write(payload)
        curtime = self._loop.time()
        endtime = curtime + 2.
        self.uuids: List[int] = []
        while curtime < endtime:
            diff = endtime - curtime
            try:
                resp = await self.admin_node.readexactly(8, diff)
            except asyncio.TimeoutError:
                break
            finally:
                curtime = self._loop.time()
            if resp[0] != CANBUS_RESP_NEED_NODEID:
                continue
            app_names = {
                KLIPPER_SET_NODE_CMD: "Klipper",
                CANBUS_CMD_SET_NODEID: "CanBoot"
            }
            app = app_names.get(resp[7], "Unknown")
            data = resp[1:7]
            output_line(f"Detected UUID: {data.hex()}, Application: {app}")
            uuid = sum([v << ((5 - i) * 8) for i, v in enumerate(data)])
            if uuid not in self.uuids and app == "CanBoot":
                self.uuids.append(uuid)
        return self.uuids

    def _reset_nodes(self) -> None:
        output_line("Resetting all bootloader node IDs...")
        payload = bytes([CANBUS_CMD_CLEAR_NODE_ID])
        self.admin_node.write(payload)

    def _set_node_id(self, uuid: int) -> CanNode:
        # Convert ID to a list
        plist = [(uuid >> ((5 - i) * 8)) & 0xFF for i in range(6)]
        plist.insert(0, CANBUS_CMD_SET_NODEID)
        node_id = len(self.nodes) + CANBUS_NODEID_OFFSET
        plist.append(node_id)
        payload = bytes(plist)
        self.admin_node.write(payload)
        decoded_id = node_id * 2 + 0x100
        node = CanNode(decoded_id, self)
        self.nodes[decoded_id + 1] = node
        return node

    async def run(self, intf: str, uuid: int, fw_path: pathlib.Path) -> None:
        if not fw_path.is_file():
            raise FlashCanError("Invalid firmware path '%s'" % (fw_path))
        try:
            self.cansock.bind((intf,))
        except Exception:
            raise FlashCanError("Unable to bind socket to can0")
        self.closed = False
        self.cansock.setblocking(False)
        self._loop.add_reader(
            self.cansock.fileno(), self._handle_can_response)
        self._jump_to_bootloader(uuid)
        await asyncio.sleep(.5)
        self._reset_nodes()
        await asyncio.sleep(.5)
        id_list = await self._query_uuids()
        if uuid not in id_list:
            raise FlashCanError(
                f"Unable to find node matching UUID: {uuid:06x}"
            )
        node = self._set_node_id(uuid)
        flasher = CanFlasher(node, fw_path)
        await asyncio.sleep(.5)
        try:
            await flasher.connect_btl()
            await flasher.verify_canbus_uuid(uuid)
            await flasher.send_file()
            await flasher.verify_file()
        finally:
            # always attempt to send the complete command. If
            # there is an error it will exit the bootloader
            # unless comms were broken
            await flasher.finish()

    async def run_query(self, intf: str):
        try:
            self.cansock.bind((intf,))
        except Exception:
            raise FlashCanError("Unable to bind socket to can0")
        self.closed = False
        self.cansock.setblocking(False)
        self._loop.add_reader(
            self.cansock.fileno(), self._handle_can_response)
        self._reset_nodes()
        await asyncio.sleep(.5)
        await self._query_uuids()

    def close(self):
        if self.closed:
            return
        self.closed = True
        for node in self.nodes.values():
            node.close()
        self._loop.remove_reader(self.cansock.fileno())
        self.cansock.close()

class SerialSocket:
    def __init__(self, loop: asyncio.AbstractEventLoop):
        self._loop = loop
        self.serial = self.serial_error = None
        self.node = CanNode(0, self)

    def _handle_response(self) -> None:
        try:
            data = self.serial.read(4096)
        except self.serial_error as e:
            logging.exception("Error on serial read")
            self.close()
        self.node.feed_data(data)

    def send(self, can_id: int, payload: bytes = b"") -> None:
        try:
            self.serial.write(payload)
        except self.serial_error as e:
            logging.exception("Error on serial write")
            self.close()

    async def run(self, intf: str, baud: int, fw_path: pathlib.Path) -> None:
        if not fw_path.is_file():
            raise FlashCanError("Invalid firmware path '%s'" % (fw_path))
        import serial
        self.serial_error = serial.SerialException
        try:
            serial_dev = serial.Serial(baudrate=baud, timeout=0,
                                       exclusive=True)
            serial_dev.port = intf
            serial_dev.open()
        except (OSError, IOError, self.serial_error) as e:
            raise FlashCanError("Unable to open serial port: %s" % (e,))
        self.serial = serial_dev
        self._loop.add_reader(self.serial.fileno(), self._handle_response)
        flasher = CanFlasher(self.node, fw_path)
        try:
            await flasher.connect_btl()
            await flasher.send_file()
            await flasher.verify_file()
        finally:
            # always attempt to send the complete command. If
            # there is an error it will exit the bootloader
            # unless comms were broken
            await flasher.finish()

    def close(self):
        if self.serial is None:
            return
        self._loop.remove_reader(self.serial.fileno())
        self.serial.close()
        self.serial = None

def main():
    parser = argparse.ArgumentParser(
        description="Can Bootloader Flash Utility")
    parser.add_argument(
        "-d", "--device", metavar='<serial device>',
        help="Serial Device"
    )
    parser.add_argument(
        "-b", "--baud", default=250000, metavar='<baud rate>',
        help="Serial baud rate"
    )
    parser.add_argument(
        "-i", "--interface", default="can0", metavar='<can interface>',
        help="Can Interface"
    )
    parser.add_argument(
        "-f", "--firmware", metavar="<klipper.bin>",
        default="~/klipper/out/klipper.bin",
        help="Path to Klipper firmware file")
    parser.add_argument(
        "-u", "--uuid", metavar="<uuid>", default=None,
        help="Can device uuid"
    )
    parser.add_argument(
        "-q", "--query", action="store_true",
        help="Query Bootloader Device IDs"
    )
    parser.add_argument(
        "-v", "--verbose", action="store_true",
        help="Enable verbose responses"
    )

    args = parser.parse_args()
    if not args.verbose:
        logging.getLogger().setLevel(logging.ERROR)
    intf = args.interface
    fpath = pathlib.Path(args.firmware).expanduser().resolve()
    loop = asyncio.get_event_loop()
    iscan = args.device is None
    sock = None
    try:
        if iscan:
            sock = CanSocket(loop)
            if args.query:
                loop.run_until_complete(sock.run_query(intf))
            else:
                if args.uuid is None:
                    raise FlashCanError(
                        "The 'uuid' option must be specified to flash a device"
                    )
                uuid = int(args.uuid, 16)
                loop.run_until_complete(sock.run(intf, uuid, fpath))
        else:
            if args.device is None:
                raise FlashCanError(
                    "The 'device' option must be specified to flash a device"
                )
            sock = SerialSocket(loop)
            loop.run_until_complete(sock.run(args.device, args.baud, fpath))
    except Exception as e:
        logging.exception("Can Flash Error")
        sys.exit(-1)
    finally:
        if sock is not None:
            sock.close()
    if args.query:
        output_line("Query Complete")
    else:
        output_line("CAN Flash Success")


if __name__ == '__main__':
    main()
