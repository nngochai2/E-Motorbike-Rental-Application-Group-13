//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#pragma once
#include <string>
#include "../utils/DateUtil.h"
#include "../enums/RentalStatus.h"

namespace EMotoRental
{
    class RentalRequest;

    class Rental
    {
    private:
        std::string rentalId;
        std::string renterUsername;
        std::string ownerUsername;
        std::string motorbikeLicensPlate;
        DateUtil::TimePoint startDate;
        DateUtil::TimePoint endDate;
        double totalCost;
        RentalStatus status;
        // std::string renterRatingId;
        // std::string motorbikeRatingId;
        
    public:
        // Constructors
        Rental();
        Rental(const RentalRequest& request, const std::string& ownerUsername);

        // Core functionalities
        void complete();
        bool isActive() const;
        bool isOverdue() const;

        // Getters
        std::string getRentalId() const;
        RentalStatus getStatus() const;
        std::string getRenterUsername() const;
        std::string getOwnerUsername() const;
        std::string getMotorbikeLicensePlate() const;
        DateUtil::TimePoint getStartDate() const;
        DateUtil::TimePoint getEndDate() const;
        double getTotalCost() const;

        // Display
        void displayInfo() const;

        // Data persistence
        std::string toCSVString() const;
        static Rental* fromCSVString(const std::string& csvData);

    private:
        static std::string generateRentalId();
        std::string statusToString() const;
        static RentalStatus stringToStatus(const std::string& str);
    };
}
