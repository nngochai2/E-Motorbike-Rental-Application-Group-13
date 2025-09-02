//
// Created by Nguyen Ngoc Hai on 8/15/2025.
//

#include <iostream>
#include "core/Application.h"

using namespace EMotoRental;

int main() {
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        std::cerr << "Application terminated unexpectedly." << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Critical Error: Unknown Error." << std::endl;
        std::cerr << "Application terminated unexpectedly." << std::endl;
        return 1;
    }

    return 0;
}