import serial
from time import sleep
import base64


def encode_file(cmd):
    parts = cmd.split()
    with open(parts[1], 'rb') as image_file:
        encoded_string = base64.b64encode(image_file.read())
    return 'b64 ' + encoded_string


def main():
    conn = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)
    conn.close()
    conn.open()
    data = conn.read(7)
    print(data)
    cmd = ''
    while True:
        cmd = raw_input('> ')
        #print('(%s) %d' % (cmd, len(cmd)))
        if cmd == 'quit':
            break
        elif cmd.startswith('file '):
            cmd = encode_file(cmd)
        nbytes = conn.write(cmd + '\r')
        conn.flush()
        sleep(0.05) # sleep for 50 millis, give the other end time to respond
        data = conn.read(conn.inWaiting())
        print(data)
    conn.close()


if __name__ == '__main__':
    main()
