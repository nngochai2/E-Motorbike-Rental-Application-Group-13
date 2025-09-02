cmake_minimum_required(VERSION 3.31)
project(E_Motorbike_Rental_Application_Group_13)

set(CMAKE_CXX_STANDARD 23)

# Include all necessary directories
include_directories(EMotoRental/src/entities)
include_directories(EMotoRental/src/enums)
include_directories(EMotoRental/src/utils)
include_directories(EMotoRental/src/managers)
include_directories(EMotoRental/src/core)

add_executable(E_Motorbike_Rental_Application_Group_13
                # Main
                EMotoRental/src/main.cpp

                # Core Application
                EMotoRental/src/core/Application.cpp
                EMotoRental/src/core/Application.h
                EMotoRental/src/core/ConsoleView.cpp
                EMotoRental/src/core/ConsoleView.h

                # Entities
                EMotoRental/src/entities/User.cpp
                EMotoRental/src/entities/User.h
                EMotoRental/src/entities/Member.cpp
                EMotoRental/src/entities/Member.h
                EMotoRental/src/entities/Admin.cpp
                EMotoRental/src/entities/Admin.h

                # Managers
                EMotoRental/src/managers/AuthManager.cpp
                EMotoRental/src/managers/AuthManager.h
                EMotoRental/src/managers/DataManager.cpp
                EMotoRental/src/managers/DataManager.h

                # Utils
                EMotoRental/src/utils/DateUtil.cpp
                EMotoRental/src/utils/DateUtil.h
                EMotoRental/src/utils/FileHandler.cpp
                EMotoRental/src/utils/FileHandler.h
                EMotoRental/src/utils/InputHelper.cpp
                EMotoRental/src/utils/InputHelper.h

                # Enums (headers only)
                EMotoRental/src/enums/RatingType.h
                EMotoRental/src/enums/RentalStatus.h
                EMotoRental/src/enums/RequestStatus.h
)