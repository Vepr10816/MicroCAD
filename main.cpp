#include <iostream>
#include "Point.h"

int main() {

    Point testPoint(1, 2.5, -1.3);

    std::cout << testPoint.getInfo() << std::endl;

    return 0;
}