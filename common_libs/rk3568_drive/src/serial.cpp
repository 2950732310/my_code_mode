#include "serial.h"
#include <iostream>
#include "fcntl.h"
#include <unistd.h>
#include <string.h>

// 构造函数，初始化成员变量
SerialPort::SerialPort() 
    : fd_(-1),      // 串口文件描述符初始化为 -1，表示未打开
      is_open_(false) // 串口未打开
{
}

// 析构函数，关闭串口，防止资源泄漏
SerialPort::~SerialPort()
{
    close();
}

/**
 * 打开串口
 * @param port 串口设备路径，例如 "/dev/ttyS5"
 * @param baudrate 波特率，例如 B115200
 * @return 打开成功返回 true，失败返回 false
 */
bool SerialPort::open(const std::string& port, int baudrate)
{
    // 使用系统调用 open 打开串口，非阻塞模式
    fd_ = ::open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd_ < 0)
    {
        std::cerr << "[ERROR] " << port << " 串口打开失败！" << std::endl;
        return false;
    }

    // 配置串口参数
    if (configureSerial(baudrate) != 0) {
        std::cerr << "[ERROR] " << port << " 串口配置失败！" << std::endl;
        ::close(fd_);   // 配置失败则关闭串口
        fd_ = -1;
        is_open_ = false;
        return false;
    }
    
    is_open_ = true; // 成功打开并配置串口
    std::cout << port << " 串口打开成功！" << std::endl;
    return true;
}

/**
 * 配置串口参数
 * @param baudrate 波特率
 * @return 成功返回 0，失败返回 -1
 */
int SerialPort::configureSerial(int baudrate) {
    struct termios options; // termios 结构体存放串口属性
    
    // 获取当前串口设置
    if (tcgetattr(fd_, &options) == -1) {
        return -1;
    }
    
    // 设置输入/输出波特率
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);
    
    // 控制模式标志：8位数据位，无校验，1停止位，无硬件流控
    options.c_cflag |= (CLOCAL | CREAD); // 本地连接并允许接收
    options.c_cflag &= ~CSIZE;           // 清除数据位掩码
    options.c_cflag |= CS8;              // 8 位数据位
    options.c_cflag &= ~PARENB;          // 无奇偶校验
    options.c_cflag &= ~CSTOPB;          // 1 个停止位
    options.c_cflag &= ~CRTSCTS;         // 无硬件流控
    
    // 本地模式标志：原始输入模式（非规范模式），关闭回显和信号
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    // 输出模式：原始输出模式
    options.c_oflag &= ~OPOST;
    
    // 输入模式：关闭软件流控
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    
    // 控制字符：非阻塞读取
    options.c_cc[VMIN] = 0;  // 最小读取字符数为 0
    options.c_cc[VTIME] = 0; // 立即返回
    
    // 设置生效
    return tcsetattr(fd_, TCSANOW, &options);
}

/**
 * 从串口读取数据
 * @param buffer 存储读取的数据
 * @param size 最大读取字节数
 * @return 读取到的字节数，失败或无数据返回 -1
 */
int SerialPort::read(uint8_t* buffer, size_t size) {
    int n = ::read(fd_, buffer, size);
    if(n <= 0)
    {
        return -1;  // 没有数据或读取失败
    }
    return n;
}

/**
 * 向串口写入数据
 * @param data 要发送的数据
 * @param size 数据长度
 * @return 实际写入字节数
 */
int SerialPort::write(const uint8_t* data, size_t size) {
    return ::write(fd_, data, size);
}

/**
 * 关闭串口
 */
void SerialPort::close() {
    if (is_open_) {
        ::close(fd_);
        is_open_ = false;
    }
}

/**
 * 检查串口是否打开
 * @return 打开返回 true，未打开返回 false
 */
bool SerialPort::isOpen() const {
    return is_open_;
}
