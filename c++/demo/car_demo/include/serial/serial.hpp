/**
 * @file serial.hpp
 * @author luoyebai (2112216825@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-06-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INCLUDE__SERIAL_HPP
#define INCLUDE__SERIAL_HPP

// std
#include <array>
#include <chrono>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
constexpr size_t DATA_LENGTH = 4;

/**
 * @brief serial port
 *
 */
class SerialPort {
  public:
    SerialPort() { openPort(); };
    ~SerialPort() { closePort(); };
    speed_t baud_rate_ = 9600;
    std::string uart_device_ = "/dev/ttyUSB";

    /**
     * @brief send data
     *
     */
    void send(const std::array<uint8_t, DATA_LENGTH - 2> &data);

  private:
    int serial_port_number_;
    char parit_ = 'N';
    char data_bit_ = 8;
    char stop_bit_ = 1;
    bool is_synchronizable_;
    uint8_t t_data_[DATA_LENGTH];

    bool openPort();
    void closePort();
    bool setBaudRate();
    bool setBit();
    template <typename T> void transformData(const T &data);
};

bool SerialPort::openPort() {
    for (size_t i = 0; i < 6; ++i) {
        const std::string uart_path =
            std::string(uart_device_) + std::to_string(i);
        serial_port_number_ =
            open(uart_path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

        if (serial_port_number_ == -1) {
            if (i == 5) {
                puts("\n\e[33mcan't open serial\e[0m\n");
                return false;
            }
            continue;
        }
        break;
    }

    puts("\n\e[32mopening serial port\e[0m\n");
    setBaudRate();
    return true;
}

void SerialPort::closePort() {
    close(serial_port_number_);
    return;
}

void SerialPort::send(const std::array<uint8_t, DATA_LENGTH - 2> &data) {
    transformData(data);
    if (serial_port_number_ == -1)
        printf("\e[33msend: %d %d %d %d\n\e[0m", 0xa5, data[0], data[1], 0xfe);
    else
        printf("\e[32msend: %d %d %d %d\n\e[0m", 0xa5, data[0], data[1], 0xfe);
    write(serial_port_number_, t_data_, DATA_LENGTH);
    return;
}

bool SerialPort::setBaudRate() {
    int speed_arr[] = {
        B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
        B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
    };
    int name_arr[] = {
        115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,
        115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,
    };
    size_t i;
    int status;
    struct termios Opt;
    tcgetattr(serial_port_number_, &Opt);

    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
        if (baud_rate_ == name_arr[i]) {
            tcflush(serial_port_number_, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(serial_port_number_, TCSANOW, &Opt);

            if (status != 0) {
                perror("tcsetattr fd1");
                return false;
            }

            tcflush(serial_port_number_, TCIOFLUSH);
        }
    }
    return true;
}

bool SerialPort::setBit() {
    struct termios termios_p;

    if (tcgetattr(serial_port_number_, &termios_p) != 0) {
        perror("SetupSerial 1");
        return false;
    }

    termios_p.c_cflag |= (CLOCAL | CREAD);
    termios_p.c_cflag &= ~CSIZE;

    switch (data_bit_) {
    case 7:
        termios_p.c_cflag |= CS7;
        break;

    case 8:
        termios_p.c_cflag |= CS8;
        break;

    default:
        fprintf(stderr, "Unsupported data size\n");
        return false;
    }

    switch (parit_) {
    case 'n':
    case 'N':
        termios_p.c_cflag &= ~PARENB;
        termios_p.c_iflag &= ~INPCK;
        break;

    case 'o':
    case 'O':
        termios_p.c_cflag |= (PARODD | PARENB);
        termios_p.c_iflag |= INPCK;
        break;

    case 'e':
    case 'E':
        termios_p.c_cflag |= PARENB;
        termios_p.c_cflag &= ~PARODD;
        termios_p.c_iflag |= INPCK;
        break;

    case 'S':
    case 's':
        termios_p.c_cflag &= ~PARENB;
        termios_p.c_cflag &= ~CSTOPB;
        break;

    default:
        fprintf(stderr, "Unsupported parity\n");
        return false;
    }

    /* 设置停止位*/
    switch (stop_bit_) {
    case 1:
        termios_p.c_cflag &= ~CSTOPB;
        break;

    case 2:
        termios_p.c_cflag |= CSTOPB;
        break;

    default:
        fprintf(stderr, "Unsupported stop bits\n");
        return false;
    }

    // Set input parity option
    if (parit_ != 'n')
        termios_p.c_iflag |= INPCK;

    // clear
    tcflush(serial_port_number_, TCIFLUSH);
    termios_p.c_cc[VTIME] = 150;
    termios_p.c_cc[VMIN] = 0;
    termios_p.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    termios_p.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    termios_p.c_iflag &= ~(ICRNL | IGNCR);
    termios_p.c_oflag &= ~OPOST;

    if (tcsetattr(serial_port_number_, TCSANOW, &termios_p) != 0) {
        perror("SetupSerial 3");
        return false;
    }

    return true;
}

template <typename T> void SerialPort::transformData(const T &data) {
    // Headframe
    t_data_[0] = 0xa5;
    for (size_t i = 0; i < DATA_LENGTH - 2; ++i) {
        t_data_[i + 1] = data[i];
    }
    // Endframe
    t_data_[DATA_LENGTH - 1] = 0xfe;
    return;
}

#endif //! INCLUDE__SERIAL_HPP
