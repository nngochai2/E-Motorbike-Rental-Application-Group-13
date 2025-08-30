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
        std::string motorbikeId;
        DateUtil::TimePoint startDate;
        DateUtil::TimePoint endDate;
        double totalCost;
        RentalStatus status;
        
    public:
        Rental(const std::string& requestId, const std::string& renterUsername, 
               const std::string& motorbikeId, const DateUtil::TimePoint& startDate,
               const DateUtil::TimePoint& endDate, double totalCost);
               
        std::string getRentalId() const;
        RentalStatus getStatus() const;
        void complete();
        void displayDetails() const;
        
        std::string toCSVString() const;
        static Rental* fromCSVString(const std::string& csvData);
    };
}
