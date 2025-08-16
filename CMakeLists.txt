cmake_minimum_required(VERSION 3.31)
project(E_Motorbike_Rental_Application_Group_13)

set(CMAKE_CXX_STANDARD 14)

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
        EMotoRental/src/entities/User.cpp)
