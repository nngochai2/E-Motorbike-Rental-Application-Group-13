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
        // Generate a UUID for the request with a prefix
        requestId = IdGenerator::generateId("REQ");
        
        // Set request date to current time
        requestDate = DateUtil::getCurrentTime();
    }

    RentalRequest::RentalRequest(const std::string& renterUsername, const std::string& motorbikeId,
                               const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate,
                               double estimatedCost) 
        : renterUsername(renterUsername), motorbikeId(motorbikeId),
          requestedStart(startDate), requestedEnd(endDate),
          estimatedCost(estimatedCost), status(RequestStatus::PENDING)
    {
        // Generate a UUID for the request with a prefix
        requestId = IdGenerator::generateId("REQ");
        
        // Set request date to current time
        requestDate = DateUtil::getCurrentTime();
    }

    Rental* RentalRequest::approve()
    {
        if (status != RequestStatus::PENDING) {
            std::cout << "Cannot approve request that is not pending" << std::endl;
            return nullptr;
        }
        
        status = RequestStatus::APPROVED;
        std::cout << "Rental request " << requestId << " has been approved." << std::endl;
        
        // Create a new Rental object based on this request
        // The Rental constructor will generate its own unique ID
        return new Rental(requestId, renterUsername, motorbikeId, 
                         requestedStart, requestedEnd, estimatedCost);
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

    std::string RentalRequest::getRequestId() const
    {
        return requestId;
    }

    RequestStatus RentalRequest::getStatus() const
    {
        return status;
    }

    bool RentalRequest::hasConflict(const RentalRequest& other) const
    {
        // Check if requests are for the same motorbike
        if (motorbikeId != other.motorbikeId) {
            return false; // Different motorbikes, no conflict
        }
        
        // Check if date ranges overlap
        if (requestedEnd < other.requestedStart || requestedStart > other.requestedEnd) {
            return false; // No overlap in dates
        }
        
        return true; // Same motorbike and overlapping dates = conflict
    }

    void RentalRequest::displayRequest() const
    {
        std::cout << "========== RENTAL REQUEST ==========" << std::endl;
        std::cout << "Request ID: " << requestId << std::endl;
        std::cout << "Renter: " << renterUsername << std::endl;
        std::cout << "Motorbike ID: " << motorbikeId << std::endl;
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

    std::string RentalRequest::toCSVString() const
    {
        std::ostringstream oss;
        oss << requestId << ","
            << renterUsername << ","
            << motorbikeId << ","
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
            request->motorbikeId = tokens[2];
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
}
