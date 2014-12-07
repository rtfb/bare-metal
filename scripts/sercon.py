import serial
from time import sleep


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
        nbytes = conn.write(cmd + '\r\n')
        conn.flush()
        sleep(0.05) # sleep for 50 millis, give the other end time to respond
        data = conn.read(conn.inWaiting())
        print(data)
    conn.close()


if __name__ == '__main__':
    main()
