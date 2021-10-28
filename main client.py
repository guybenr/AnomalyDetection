import socket
import sys


def convert(num):
    length = len(str(num))
    num_string = str(num)
    for i in range(4 - length):
        num_string = '0' + num_string
    return num_string


if __name__ == '__main__':
    port = int(sys.argv[1])
    ip = sys.argv[2]
    file = open(sys.argv[3], "r")
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.settimeout(12)
    i = 0
    packet = convert(i) + str(file.read(80))
    flag = True
    while flag:
        s.sendto(packet.encode(), (ip, port))
        try:
            data, address = s.recvfrom(100)
            i = i + 1
            packet = convert(i) + str(file.read(91))
        except socket.timeout:
            continue
        if packet[7:] == '':
            flag = False
    s.close()
