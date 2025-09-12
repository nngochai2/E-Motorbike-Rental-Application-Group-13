//
// Created by Hoang Vinh Khue on 9/07/2025.
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

    // Structure to hold listing data
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

    public:
        MotorbikeManager();
        ~MotorbikeManager();

        // Registration and listing management
        bool registerMotorbike(
            const std::string& ownerUsername,
            const std::string& brand,
            const std::string& model,
            const std::string& color,
            int engineSize,
            const std::string& licensePlate,
            const std::string& city
        );

        bool listMotorbike(
            const std::string& motorbikeId,
            const DateUtil::TimePoint& startDate,
            const DateUtil::TimePoint& endDate,
            double dailyRate,
            double minRating
        );

        bool unlistMotorbike(const std::string& motorbikeId);

        // Search and retrieval methods
        std::vector<Motorbike*> searchMotorbikes(
            const std::string& city,
            const DateUtil::TimePoint& startDate,
            const DateUtil::TimePoint& endDate
        ) const;
        Motorbike* getMotorbikeById(const std::string& motorbikeId) const;
        std::vector<Motorbike*> getMotorbikesByOwner(const std::string& username) const;
        std::vector<Motorbike*> getAllMotorbikes() const;

        // Rating system
        bool updateMotorbikeRating(const std::string& motorbikeId, double rating);

        // Data persistence methods
        bool loadAllMotorbikes();
        bool saveAllMotorbikes() const;

        // Statistics and utilities
        int getTotalMotorbikeCount() const;
        int getListedMotorbikeCount() const;
        std::vector<std::string> getAllCities() const;

        // Display methods (for admin and debugging)
        void displayAllMotorbikes() const;
        void displayMotorbikeStatistics() const;

    private:
        void cleanupMemory();
    };
}
