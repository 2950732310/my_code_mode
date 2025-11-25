#include "hello.h"
#include "serial.h"

int main() {
    Hello hello;
    hello.sayHello();
    
    SerialPort serial;
    serial.open("/dev/ttyUSB0", 115200);
    
    return 0;
}