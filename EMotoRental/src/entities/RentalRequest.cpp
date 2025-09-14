//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#include "RentalRequest.h"
#include <iostream>
#include <sstream>
#include "../utils/FileHandler.h"
#include "../utils/IdGenerator.h"

namespace EMotoRental
{
    RentalRequest::RentalRequest() : estimatedCost(0.0), status(RequestStatus::PENDING)
    {
        // Set request date to current time
        requestDate = DateUtil::getCurrentTime();

        // Generate ID with timestamp
        requestId = generateRequestId();
    }

    RentalRequest::RentalRequest(const std::string& renter, const std::string& motorbikeLicensePlate,
                                   const DateUtil::TimePoint& start, const DateUtil::TimePoint& end,
                                   double cost)
            : renterUsername(renter), motorbikeLicensePlate(motorbikeLicensePlate), requestedStart(start),
              requestedEnd(end), estimatedCost(cost), status(RequestStatus::PENDING)
    {
        requestDate = DateUtil::getCurrentTime();
        requestId = generateRequestId();
    }

    void RentalRequest::approve()
    {
        if (status != RequestStatus::PENDING) {
            std::cout << "Cannot approve request that is not pending" << std::endl;
            return;
        }
        
        status = RequestStatus::APPROVED;
        std::cout << "Rental request " << requestId << " has been approved." << std::endl;
    }

    void RentalRequest::reject()
    {
        if (status != RequestStatus::PENDING) {
            std::cout << "Cannot reject request that is not pending" << std::endl;
            return;
        }
        
        status = RequestStatus::REJECTED;
        std::cout << "Rental request " << requestId << " has been rejected." << std::endl;
    }

    bool RentalRequest::hasConflict(const RentalRequest& otherRequest) const
    {
        // Check if requests are for the same motorbike
        if (motorbikeLicensePlate != otherRequest.motorbikeLicensePlate) {
            return false; // Different motorbikes, no conflict
        }

        // Check if date ranges overlap
        if (requestedEnd < otherRequest.requestedStart || requestedStart > otherRequest.requestedEnd) {
            return false; // No overlap in dates
        }

        return true; // Same motorbike and overlapping dates = conflict
    }

    // ================================================ GETTERS ========================================================

    std::string RentalRequest::getRequestId() const { return requestId; }
    RequestStatus RentalRequest::getStatus() const { return status; }
    std::string RentalRequest::getRenterUsername() const { return renterUsername; }
    std::string RentalRequest::getMotorbikeLicensePlate() const { return motorbikeLicensePlate; }
    DateUtil::TimePoint RentalRequest::getStartDate() const { return requestedStart; }
    DateUtil::TimePoint RentalRequest::getEndDate() const { return requestedEnd; }
    double RentalRequest::getEstimatedCost() const { return estimatedCost; }
    DateUtil::TimePoint RentalRequest::getRequestDate() const { return requestDate; }

    // ============================================= DISPLAY METHODS ===================================================

    void RentalRequest::displayRequest() const
    {
        std::cout << "========== RENTAL REQUEST ==========" << std::endl;
        std::cout << "Request ID: " << requestId << std::endl;
        std::cout << "Renter: " << renterUsername << std::endl;
        std::cout << "Motorbike ID: " << motorbikeLicensePlate << std::endl;
        std::cout << "Requested Period: " << DateUtil::formatDate(requestedStart) 
                  << " to " << DateUtil::formatDate(requestedEnd) << std::endl;
        std::cout << "Estimated Cost: " << estimatedCost << " CP" << std::endl;
        
        std::cout << "Status: ";
        switch (status) {
            case RequestStatus::PENDING:
                std::cout << "PENDING";
                break;
            case RequestStatus::APPROVED:
                std::cout << "APPROVED";
                break;
            case RequestStatus::REJECTED:
                std::cout << "REJECTED";
                break;
        }
        std::cout << std::endl;
        
        std::cout << "Request Date: " << DateUtil::formatDate(requestDate) << std::endl;
        std::cout << "===================================" << std::endl;
    }

    // ============================================ DATA PERSISTENCE ===================================================

    std::string RentalRequest::toCSVString() const
    {
        std::ostringstream oss;
        oss << requestId << ","
            << renterUsername << ","
            << motorbikeLicensePlate << ","
            << DateUtil::formatDate(requestedStart) << ","
            << DateUtil::formatDate(requestedEnd) << ","
            << estimatedCost << ",";
        
        // Convert status enum to string
        switch (status) {
            case RequestStatus::PENDING:
                oss << "PENDING";
                break;
            case RequestStatus::APPROVED:
                oss << "APPROVED";
                break;
            case RequestStatus::REJECTED:
                oss << "REJECTED";
                break;
        }
        
        oss << "," << DateUtil::formatDate(requestDate);
        
        return oss.str();
    }

    RentalRequest* RentalRequest::fromCSVString(const std::string& csvData)
    {
        std::vector<std::string> tokens = FileHandler::parseCSVLine(csvData);
        
        if (tokens.size() < 8) {
            std::cerr << "Invalid rental request CSV data: not enough fields" << std::endl;
            return nullptr;
        }
        
        try {
            auto* request = new RentalRequest();
            
            request->requestId = tokens[0];
            request->renterUsername = tokens[1];
            request->motorbikeLicensePlate = tokens[2];
            request->requestedStart = DateUtil::parseDate(tokens[3]);
            request->requestedEnd = DateUtil::parseDate(tokens[4]);
            request->estimatedCost = std::stod(tokens[5]);
            
            // Convert string to status enum
            if (tokens[6] == "PENDING") {
                request->status = RequestStatus::PENDING;
            } else if (tokens[6] == "APPROVED") {
                request->status = RequestStatus::APPROVED;
            } else if (tokens[6] == "REJECTED") {
                request->status = RequestStatus::REJECTED;
            }
            
            request->requestDate = DateUtil::parseDate(tokens[7]);
            
            return request;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing rental request data: " << e.what() << std::endl;
            return nullptr;
        }
    }

    // ============================================ HELPER METHODS =====================================================

    std::string RentalRequest::generateRequestId() const {
        // Generate unique ID based on timestamp and renter
        const auto timestamp = DateUtil::getTimestamp();
        return "REQ" + std::to_string(timestamp);
    }

    std::string RentalRequest::statusToString() const {
        switch (status) {
            case RequestStatus::PENDING: return "PENDING";
            case RequestStatus::APPROVED: return "APPROVED";
            case RequestStatus::REJECTED: return "REJECTED";
            default: return "UNKNOWN";
        }
    }

    RequestStatus RentalRequest::stringToStatus(const std::string& str) {
        if (str == "PENDING") return RequestStatus::PENDING;
        if (str == "APPROVED") return RequestStatus::APPROVED;
        if (str == "REJECTED") return RequestStatus::REJECTED;

        // Default fallback - should log this as an error in production
        std::cerr << "Warning: Unknown RequestStatus string: " << str << ", defaulting to PENDING" << std::endl;
        return RequestStatus::PENDING;
    }
}
