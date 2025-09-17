cmake_minimum_required(VERSION 3.31)
project(E_Motorbike_Rental_Application_Group_13)

set(CMAKE_CXX_STANDARD 23)

include_directories(EMotoRental/src/entities)
include_directories(EMotoRental/src/enums)
include_directories(EMotoRental/src/utils)

add_executable(E_Motorbike_Rental_Application_Group_13
        EMotoRental/src/entities/User.h
        EMotoRental/src/enums/RatingType.h
        EMotoRental/src/enums/RentalStatus.h
        EMotoRental/src/enums/RequestStatus.h
        EMotoRental/src/utils/DateUtil.cpp
        EMotoRental/src/utils/DateUtil.h
        EMotoRental/src/main.cpp
        EMotoRental/src/entities/Member.h
        EMotoRental/src/entities/User.cpp
        EMotoRental/src/entities/Member.cpp
        EMotoRental/src/managers/AuthManager.h
        EMotoRental/src/managers/AuthManager.cpp
        EMotoRental/src/utils/FileHandler.cpp
        EMotoRental/src/utils/FileHandler.h
        EMotoRental/src/utils/IdGenerator.h
        EMotoRental/src/entities/Motorbike.h
        EMotoRental/src/entities/Motorbike.cpp
        EMotoRental/src/entities/Rental.h
        EMotoRental/src/entities/Rental.cpp
        EMotoRental/src/entities/RentalRequest.h
        EMotoRental/src/entities/RentalRequest.cpp
        EMotoRental/src/utils/InputHelper.cpp
        EMotoRental/src/utils/InputHelper.h
        EMotoRental/src/core/ConsoleView.cpp
        EMotoRental/src/core/ConsoleView.h
        EMotoRental/src/core/Application.cpp
        EMotoRental/src/core/Application.h
        EMotoRental/src/managers/DataManager.cpp
        EMotoRental/src/managers/DataManager.h
        EMotoRental/src/entities/Admin.cpp
        EMotoRental/src/entities/Admin.h
        EMotoRental/src/managers/MotorbikeManager.cpp
        EMotoRental/src/managers/MotorbikeManager.h
        EMotoRental/src/entities/Rating.cpp
        EMotoRental/src/managers/RentalManager.cpp
        EMotoRental/src/managers/RentalManager.h
)
