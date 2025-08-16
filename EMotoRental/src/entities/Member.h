//
// Created by Nguyen Ngoc Hai on 8/16/2025.
//

#pragma once
#include "User.h"
#include <vector>

namespace EMotoRental
{
    class Member : public User
    {
    private:
        double creditPoint;
        double renterRating;
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
        std::string getOwnerMotorbikeId() const;
        void setOwnerMotorbikeId(const std::string& id);
        bool hasOwedMotorbikeId() const;

        // Rental eligibility
        bool canRentMotorbike(double cost, double requiredRating, int engineSize) const;
        bool hasActiveRental() const;

        // Virtual function implementations
        std::string getUserType() const override;
        void displayInfo() const override;
        std::string toCSVString() const override;
        static Member* fromCSVSString(const std::string& csvData);

        // Rental history
        void addRentalToHistory(const std::string& rentalId);
        std::vector<std::string> getRentalHistory() const;
    };
}
