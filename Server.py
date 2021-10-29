import socket
import sys

# the program gets a port number from the main arguments and check if its valid
# then binding the port to the new socket of the program and receiving the massages from foo
# and responding to them. if a massage has been read for the first time, the server printing it to the screen.
if __name__ == '__main__':
    # gets the port and check if its valid
    if len(sys.argv) < 2:
        print("Not enough arguments")
        sys.exit(1)
    MY_PORT = int(sys.argv[1])
    if MY_PORT >= 65536 or MY_PORT < 0:
        print("Port not valid")
        sys.exit(1)
    # creating a new socket and binding the port with it
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', MY_PORT))
    current = 0
    while True:
        data, addr = s.recvfrom(100)
        numOfPacket = data[0:4]
        # if the current == numOfPacket then its the first time the the server read the massage
        if current == int(numOfPacket):
            print(str(data[4:].decode('utf-8')), end='')
            current += 1
        s.sendto(data, addr)
