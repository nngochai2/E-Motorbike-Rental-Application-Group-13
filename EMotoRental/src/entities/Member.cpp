//
// Created by Nguyen Ngoc Hai on 8/16/2025.
//
#include "Member.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace EMotoRental
{
    Member::Member() : User(), creditPoints(20.0), renterRating(3.0) {}

    Member::Member(const std::string& username, const std::string& password,
               const std::string& fullName, const std::string& email,
               const std::string& phone)
    : User(username, password, fullName, email, phone),
      creditPoints(20.0), renterRating(3.0) {}

    // Credit system
    bool Member::topUpCredits(double amount, const std::string& password)
    {
        if (!validatePassword(password))
        {
            return false;
        }
        if (amount <= 0)
        {
            return false;
        }
        creditPoints += amount;
        return true;
    }

    bool Member::deductCredits(double amount)
    {
        if (amount <= 0 || creditPoints < amount)
        {
            return false;
        }
        creditPoints -= amount;
        return true;
    }

    double Member::getCreditPoints() const
    {
        return creditPoints;
    }

    // Rating system
    double Member::getRenterRating() const
    {
        return renterRating;
    }

    void Member::updateRenterRating(double newRating)
    {
        if (newRating >= 1.0 && newRating <= 5.0)
        {
            renterRating = newRating;
        }
    }

    // Motorbike ownership
    std::string Member::getOwnedMotorbikeId() const
    {
        return ownedMotorbikeId;
    }

    void Member::setOwnedMotorbikeId(const std::string& id)
    {
        ownedMotorbikeId = id;
    }

    bool Member::hasOwnedMotorbike() const
    {
        return !ownedMotorbikeId.empty();
    }

    // Virtual implementations
    std::string Member::getUserType() const
    {
        return "Member";
    }

    void Member::displayInfo() const {
        std::cout << "=== MEMBER INFORMATION ===" << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Full Name: " << fullName << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Phone: " << phoneNumber << std::endl;
        std::cout << "Credit Points: " << std::fixed << std::setprecision(1) << creditPoints << std::endl;
        std::cout << "Renter Rating: " << std::fixed << std::setprecision(1) << renterRating << std::endl;
        std::cout << "License Valid: " << (hasValidLicense ? "Yes" : "No") << std::endl;
        std::cout << "Verified: " << (isVerified ? "Yes" : "No") << std::endl;
        if (hasOwnedMotorbike()) {
            std::cout << "Owned Motorbike ID: " << ownedMotorbikeId << std::endl;
        }
        std::cout << "=========================" << std::endl;
    }

    std::string Member::toCSVString() const {
        std::ostringstream oss;
        oss << username << "," << password << "," << fullName << ","
            << email << "," << phoneNumber << "," << idNumber << ","
            << licenseNumber << "," << (hasValidLicense ? "1" : "0") << ","
            << (isVerified ? "1" : "0") << "," << creditPoints << ","
            << renterRating << "," << ownedMotorbikeId;
        return oss.str();
    }

   Member* Member::fromCSVString(const std::string& csvData) {
        std::istringstream iss(csvData);
        std::string token;
        std::vector<std::string> tokens;

        // Parse CSV line
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 12) {
            return nullptr; // Invalid data
        }

        auto* member = new Member(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
        member->setIdNumber(tokens[5]);
        member->setLicense(tokens[6], tokens[7] == "1");
        member->setVerified(tokens[8] == "1");
        member->creditPoints = std::stod(tokens[9]);
        member->renterRating = std::stod(tokens[10]);
        member->ownedMotorbikeId = tokens[11];

        return member;
    }

    void Member::addRentalToHistory(const std::string& rentalId)
    {
        rentalHistory.push_back(rentalId);
    }

    std::vector<std::string> Member::getRentalHistory() const
    {
        return rentalHistory;
    }
}
