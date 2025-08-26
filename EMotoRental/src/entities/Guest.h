//
// Created by Tran Dinh Dong on 8/26/2025.
//
#pragma once
#include "User.h"
#include <iostream>

namespace EMotoRental
{
    class Guest : public User
    {
    public:
        Guest();

        // Virtual function implementations
        std::string getUserType() const override;
        void displayInfo() const override;
        std::string toCSVString() const override;

        // Guest-specific functionality
        void browsePublicListings() const;
    };
}
