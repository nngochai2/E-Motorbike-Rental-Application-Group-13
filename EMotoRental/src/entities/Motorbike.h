//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#pragma once
#include <string>
#include <vector>
#include "Member.h"
#include "../utils/DateUtil.h"

namespace EMotoRental
{
    class Motorbike
    {
    private:
        std::string motorbikeId;
        std::string brand;
        std::string model;
        std::string color;
        int engineSize;
        int yearMade;
        std::string licensePlate;
        std::string city;
        double dailyRate;
        double requiredRenterRating;
        double motorbikeRating;
        bool isListed;
        std::string ownerUsername;
        std::vector<std::string> ratingIds;
        DateUtil::TimePoint availableStart;
        DateUtil::TimePoint availableEnd;

    public:
        Motorbike();
        Motorbike(const std::string& brand, const std::string& model, const std::string& color, 
                 int engineSize, const std::string& plate, const std::string& city, 
                 const std::string& owner);

        bool listForRent(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate, 
                        double dailyRate, double minRating);
        bool unlist();
        bool isAvailable(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate) const;
        void updateRating(double newRating);
        bool meetsRequirement(const Member& member) const;
        double calculateCost(int days) const;

        std::string getMotorbikeId() const;
        std::string getOwnerUsername() const;
        void displayDetails() const;
        void displayPublicInfo() const;
        std::string toCSVString() const;
        static Motorbike* fromCSVString(const std::string& csvData);
    };
}
