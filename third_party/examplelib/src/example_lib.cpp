#include "example_lib.h"
#include <iostream>

std::string ExampleLib::getMessage() {
    return "This is a message from third-party library!";
}

void ExampleLib::printMessage() {
    std::cout << getMessage() << std::endl;
}