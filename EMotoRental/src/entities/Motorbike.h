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
        // Basic motorbike information
        std::string licensePlate;       // Primary identifier
        std::string brand;
        std::string model;
        std::string color;
        int engineSize;                 // in cc
        int yearMade;
        std::string city;               // Location (HCMC or Hanoi)

        // Rental information
        double dailyRate;               // Cost per day in credit points
        double requiredRenterRating;    // Minimum renter rating required
        bool isListed;                  // Whether the motorbike is listed for rent
        std::string ownerUsername;      // Owner of the motorbike

        // Available period
        DateUtil::TimePoint availableStart;
        DateUtil::TimePoint availableEnd;

        // Rating system
        double motorbikeRating;

    public:
        Motorbike();
        Motorbike(const std::string& brand, const std::string& model,
                 const std::string& color, int engineSize, int yearMade,
                 const std::string& plate, const std::string& city,
                 const std::string& owner);

        virtual ~Motorbike() = default;

        // Core functionalities
        bool listForRent(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate,
                        double dailyRate, double minRating);
        bool unlist();
        bool isAvailable(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate) const;

        // Rating and requirements
        void updateRating(double newRating);
        bool meetsRequirement(const Member& member) const;
        double calculateCost(int days) const;

        // Getters
        std::string getLicensePlate() const;
        std::string getOwnerUsername() const;
        std::string getBrand() const;
        std::string getModel() const;
        std::string getColor() const;
        std::string getCity() const;
        int getEngineSize() const;
        int getYearMade() const;
        double getDailyRate() const;
        double getRequiredRenterRating() const;
        double getMotorbikeRating() const;
        bool getIsListed() const;
        DateUtil::TimePoint getAvailableStart() const;
        DateUtil::TimePoint getAvailableEnd() const;

        // Setters
        void setLicensePlate(const std::string& plate);
        void setCity(const std::string& newCity);
        void setOwnerUsername(const std::string& owner);
        void setYearMade(int year);

        // Display methods
        void displayDetails() const;
        void displayPublicInfo() const;

        // Data persistence
        std::string toCSVString() const;
        static Motorbike* fromCSVString(const std::string& csvData);
    };
}
