//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#pragma once
#include <string>
#include "../utils/DateUtil.h"
#include "../enums/RequestStatus.h"
#include "Rental.h"

namespace EMotoRental
{
    class RentalRequest
    {
    private:
        std::string requestId;
        std::string renterUsername;
        std::string motorbikeId;
        DateUtil::TimePoint requestedStart;
        DateUtil::TimePoint requestedEnd;
        double estimatedCost;
        RequestStatus status;
        DateUtil::TimePoint requestDate;

    public:
        RentalRequest();
        RentalRequest(const std::string& renterUsername, const std::string& motorbikeId,
                     const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate,
                     double estimatedCost);

        Rental* approve();
        void reject();
        std::string getRequestId() const;
        RequestStatus getStatus() const;
        bool hasConflict(const RentalRequest& other) const;
        void displayRequest() const;
        
        std::string toCSVString() const;
        static RentalRequest* fromCSVString(const std::string& csvData);
    };
}
