#ifndef SERIAL_H
#define SERIAL_H

#include <string>
#include <termios.h>

class SerialPort {
public:
    SerialPort();
    ~SerialPort();

    bool open(const std::string& port, int baudrate = B115200);
    void close();
    int read(uint8_t* buffer, size_t size);
    int write(const uint8_t* data, size_t size);
    bool isOpen() const;

private:
    int configureSerial(int baudrate);
    
    int fd_;
    std::string port_;
    bool is_open_;
};

#endif // SERIAL_H
