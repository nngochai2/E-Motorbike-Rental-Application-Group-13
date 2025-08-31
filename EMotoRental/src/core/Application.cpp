//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#include "Application.h"
#include <iostream>
#include <iomanip>

namespace EMotoRental
{
    Application::Application() : isRunning(true) {
        std::cout << "Initializing E-Motorbike Rental Application..." << std::endl;

        view = new ConsoleView();

    }

}
