import socket
import sys

MY_PORT = int(sys.argv[1])
if __name__ == '__main__':

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', MY_PORT))
    current = 0
    while True:
        data, addr = s.recvfrom(100)
        numOfPacket = data[0:4]
        if current == int(numOfPacket):
            print(str(data[4:].decode('utf-8')) , end='')
            current += 1
        s.sendto(data, addr)
