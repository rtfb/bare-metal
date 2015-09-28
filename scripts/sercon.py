import serial
from time import sleep
import base64


def encode_file(cmd):
    parts = cmd.split()
    with open(parts[1], 'rb') as image_file:
        encoded_string = base64.b64encode(image_file.read())
    return 'b64 ' + encoded_string


def main():
    device = '/dev/ttyUSB0'
    print('Connecting to %s...' % device)
    conn = serial.Serial(port=device, baudrate=115200)
    print('OK')
    cmd = ''
    while True:
        cmd = raw_input('> ')
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
