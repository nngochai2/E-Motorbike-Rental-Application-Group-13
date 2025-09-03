//
// Created by Nguyen Ngoc Hai on 9/3/2025.
//

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../entities/Motorbike.h"
#include "../utils/DateUtil.h"

namespace EMotoRental
{
    // Structure to hold motorbike registration data
    struct MotorbikeRegistrationData
    {
        std::string brand;
        std::string model;
        std::string color;
        int engineSize;
        int yearMade;
        std::string licensePlate;
        std::string city;
        std::string ownerUsername;

        MotorbikeRegistrationData(const std::string& brand, const std::string& model,
                                 const std::string& color, int engineSize, int yearMade,
                                 const std::string& plate, const std::string& city,
                                 const std::string& owner)
            : brand(brand), model(model), color(color), engineSize(engineSize),
              yearMade(yearMade), licensePlate(plate), city(city), ownerUsername(owner) {}
    };

    // Structure to hold listing dat
    struct MotorbikeListingData
    {
        DateUtil::TimePoint startDate;
        DateUtil::TimePoint endDate;
        double dailyRate;
        double minRenterRating;

        MotorbikeListingData(const DateUtil::TimePoint& start, const DateUtil::TimePoint& end,
                           double rate, double minRating)
            : startDate(start), endDate(end), dailyRate(rate), minRenterRating(minRating) {}
    };

    class MotorbikeManager
    {
    private:
        std::vector<Motorbike*> motorbikes;

        // File paths for data persistence
        static const std::string MOTORBIKES_FILE;

        // Helper methods for data management
        std::string formatMotorbikesAsCSV() const;
        std::vector<Motorbike*> parseMotorbikesFromCSV(const std::string& content) const;
        void cleanupMemory();

    public:
        MotorbikeManager();
        ~MotorbikeManager();

        
    };
}