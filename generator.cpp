#include <random>
#include <limits>
#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char **argv ) {

    // std::ofstream file("generator.txt");

    std::srand( std::stoi(argv[1]) );

    for(int i = 0; i < std::rand() %  1000 + 1; i++) {
        int x = std::rand() % INT32_MAX;
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << std::uppercase << x << std::endl;
    }

    return 0;
}