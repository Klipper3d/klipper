#!/usr/bin/env python3
import socket, json, sys

socket_address = "/opt/printer_data/run/klipper.sock"
message = {"id": 666, "method": "info"}

# Set up socket connection
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.connect(socket_address)

# Send message and receive response
sock.sendall(json.dumps(message).encode() + b"\x03")
response = sock.recv(4096).decode("utf-8").strip("\x03")
sock.close()

# Check the result
if json.loads(response)["result"]["state"] == "ready":
    # State is ready - healthy
    sys.exit(0)
else:
    # State is not ready - unhealthy
    sys.exit(1)
