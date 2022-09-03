# query local ip address

import socket
import time
import traceback

def get_host_ip():

    ip='0.0.0.0'
    try:
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        s.connect(('8.8.8.8',80))
        ip=s.getsockname()[0]
    except:
        traceback.print_exc()
    finally:
        s.close()

    return ip
