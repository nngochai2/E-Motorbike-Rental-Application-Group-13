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
        std::string motorbikeLicensePlate;
        DateUtil::TimePoint requestedStart;
        DateUtil::TimePoint requestedEnd;
        double estimatedCost;
        RequestStatus status;
        DateUtil::TimePoint requestDate;

    public:
        RentalRequest();
        RentalRequest(const std::string& renterUsername, const std::string& motorbikeLicensePlate,
                     const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate,
                     double estimatedCost);

        // Core functionalities
        void approve();
        void reject();

        // Conflict checking
        bool hasConflict(const RentalRequest& otherRequest) const;

        // Getters
        std::string getRequestId() const;
        RequestStatus getStatus() const;
        std::string getRenterUsername() const;
        std::string getMotorbikeLicensePlate() const;
        DateUtil::TimePoint getStartDate() const;
        DateUtil::TimePoint getEndDate() const;
        double getEstimatedCost() const;
        DateUtil::TimePoint getRequestDate() const;

        // Display
        void displayRequest() const;

        // Data persistence
        std::string toCSVString() const;
        static RentalRequest* fromCSVString(const std::string& csvData);

    private:
        std::string generateRequestId() const;
        std::string statusToString() const;
        static RequestStatus stringToStatus(const std::string& str);
    };
}
