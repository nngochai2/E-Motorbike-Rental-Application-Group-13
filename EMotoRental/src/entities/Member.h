//
// Created by Nguyen Ngoc Hai on 8/16/2025.
//

#pragma once
#include "User.h"
#include <vector>

#include "../managers/RentalManager.h"

namespace EMotoRental
{
    class Member : public User
    {
    private:
        double creditPoints{};
        double renterRating{};
        std::string ownedMotorbikeId;
        std::vector<std::string> rentalHistory;

    public:
        Member();
        Member(const std::string& username, const std::string& password,
               const std::string& fullName, const std::string& email,
               const std::string& phone);

        // Credit system
        bool topUpCredits(double amount, const std::string& password);
        bool deductCredits(double amount);
        double getCreditPoints() const;

        // Rating system
        double getRenterRating() const;
        void updateRenterRating(double newRating);

        // Motorbike ownership
        std::string getOwnedMotorbikeId() const;
        void setOwnedMotorbikeId(const std::string& id);
        bool hasOwnedMotorbike() const;

        // Virtual function implementations
        std::string getUserType() const override;
        void displayInfo() const override;
        std::string toCSVString() const override;
        static Member* fromCSVString(const std::string& csvData);

        // Rental history
        void addRentalToHistory(const std::string& rentalId);
        std::vector<std::string> getRentalHistory() const;
    };
}
