//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#pragma once
#include <string>
#include "../utils/DateUtil.h"
#include "../enums/RentalStatus.h"

// WIP: still missing some fields from the diagram
namespace EMotoRental
{
    class Rental
    {
    private:
        std::string rentalId;
        std::string renterUsername;
        std::string ownerUsername;
        std::string motorbikeId;
        DateUtil::TimePoint startDate;
        DateUtil::TimePoint endDate;
        double totalCost;
        RentalStatus status;
        std::string renterRatingId;
        std::string motorbikeRatingId;
        
    public:
        Rental();
        Rental(const std::string& requestId, const std::string& renterUsername, 
               const std::string& motorbikeId, const DateUtil::TimePoint& startDate,
               const DateUtil::TimePoint& endDate, double totalCost);
               
        std::string getRentalId() const;
        RentalStatus getStatus() const;

        void approve();
        void complete();

        void addRenterRating(const std::string& ratingId);
        void addMotorbikeRating(const std::string& ratingId);

        bool isActive() const;
        double calculateCost() const;

        void displayInfo() const;
        
        std::string toCSVString() const;
        static Rental* fromCSVString(const std::string& csvData);
    };
}
