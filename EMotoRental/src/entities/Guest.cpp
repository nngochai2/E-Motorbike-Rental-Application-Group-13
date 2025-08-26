//
// Created by Tran Dinh Dong on 8/26/2025.
//
#include "Guest.h"

namespace EMotoRental
{
    Guest::Guest() : User("guest", "", "Guest User", "", "") 
    {
        // A guest won't really have a proper account
        // So we just pass dummy/empty values to User constructor
    }

    std::string Guest::getUserType() const
    {
        return "Guest";
    }

    void Guest::displayInfo() const
    {
        std::cout << "=== GUEST SESSION ===" << std::endl;
        std::cout << "You are browsing as a guest (no registered account)." << std::endl;
        std::cout << "=====================" << std::endl;
    }

    std::string Guest::toCSVString() const
    {
        // Guests don’t need to be persisted to CSV
        return "GUEST";
    }

    void Guest::browsePublicListings() const
    {
        std::cout << "Browsing public motorbike listings..." << std::endl;
        // TODO: later integrate with MotorbikeManager to show real bikes
    }
}
