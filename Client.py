import socket
import sys


# parameters: int 0 =< num <= 9999
# return: string that represents num with 4 chars
# for example if num == 5 then return "0005"
def convert(num):
    length = len(str(num))
    num_string = str(num)
    for i in range(4 - length):
        num_string = '0' + num_string
    return num_string


# the main function of the client.
# first, gets all of the arguments entered:
# 1. the port of foo 2. the ip of foo 3. the name of the file to read from
# then the program creates a protocol of which every packet's first 4 bytes is his number
# after that the program send the packet and wait for a response.
# if there wasn't a response the client will send the package again until all of the packages sent.
if __name__ == '__main__':
    # arguments initializing
    if len(sys.argv) < 4:
        print("Not enough arguments")
        sys.exit(1)
    port = int(sys.argv[1])
    if port >= 65536 or port < 0:
        print("Port not valid")
        sys.exit(1)
    ip = sys.argv[2]
    file = open(sys.argv[3], "r")
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # set a 12 second time out
    s.settimeout(12)
    numOfPacket = 0
    # creating a first packet with id 0
    packet = convert(numOfPacket) + str(file.read(80))
    flag = True
    while flag:
        # send the packet to foo
        s.sendto(packet.encode(), (ip, port))
        try:
            # if the response came then creating the next packet
            data, address = s.recvfrom(100)
            numOfPacket = numOfPacket + 1
            packet = convert(numOfPacket) + str(file.read(91))
        except socket.timeout:
            continue
        if packet[7:] == '':
            flag = False
    s.close()