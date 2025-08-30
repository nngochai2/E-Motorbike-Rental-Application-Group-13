//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#include "Rental.h"
#include <iostream>
#include <sstream>
#include "../utils/FileHandler.h"
#include "../utils/IdGenerator.h"

namespace EMotoRental
{
    Rental::Rental(const std::string& requestId, const std::string& renterUsername, 
                 const std::string& motorbikeId, const DateUtil::TimePoint& startDate,
                 const DateUtil::TimePoint& endDate, double totalCost)
        : renterUsername(renterUsername), motorbikeId(motorbikeId),
          startDate(startDate), endDate(endDate), totalCost(totalCost),
          status(RentalStatus::ACTIVE)
    {
        // Generate a UUID for the rental with a prefix, different from requestId
        rentalId = IdGenerator::generateId("RNT");
    }
    
    std::string Rental::getRentalId() const
    {
        return rentalId;
    }
    
    RentalStatus Rental::getStatus() const
    {
        return status;
    }
    
    void Rental::complete()
    {
        status = RentalStatus::COMPLETED;
        std::cout << "Rental " << rentalId << " has been marked as completed." << std::endl;
    }
    
    void Rental::displayDetails() const
    {
        std::cout << "========== RENTAL DETAILS ==========" << std::endl;
        std::cout << "Rental ID: " << rentalId << std::endl;
        std::cout << "Renter: " << renterUsername << std::endl;
        std::cout << "Motorbike ID: " << motorbikeId << std::endl;
        std::cout << "Start Date: " << DateUtil::formatDate(startDate) << std::endl;
        std::cout << "End Date: " << DateUtil::formatDate(endDate) << std::endl;
        std::cout << "Total Cost: " << totalCost << " CP" << std::endl;
        std::cout << "Status: " << (status == RentalStatus::ACTIVE ? "ACTIVE" : "COMPLETED") << std::endl;
        std::cout << "===================================" << std::endl;
    }
    
    std::string Rental::toCSVString() const
    {
        std::ostringstream oss;
        oss << rentalId << ","
            << renterUsername << ","
            << motorbikeId << ","
            << DateUtil::formatDate(startDate) << ","
            << DateUtil::formatDate(endDate) << ","
            << totalCost << ","
            << (status == RentalStatus::ACTIVE ? "ACTIVE" : "COMPLETED");
            
        return oss.str();
    }
    
    Rental* Rental::fromCSVString(const std::string& csvData)
    {
        std::vector<std::string> tokens = FileHandler::parseCSVLine(csvData);
        
        if (tokens.size() < 7) {
            std::cerr << "Invalid rental CSV data: not enough fields" << std::endl;
            return nullptr;
        }
        
        try {
            std::string rentalId = tokens[0];
            std::string renterUsername = tokens[1];
            std::string motorbikeId = tokens[2];
            DateUtil::TimePoint startDate = DateUtil::parseDate(tokens[3]);
            DateUtil::TimePoint endDate = DateUtil::parseDate(tokens[4]);
            double totalCost = std::stod(tokens[5]);
            
            auto* rental = new Rental(rentalId, renterUsername, motorbikeId, startDate, endDate, totalCost);
            
            // Set status
            if (tokens[6] == "COMPLETED") {
                rental->status = RentalStatus::COMPLETED;
            }
            
            return rental;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing rental data: " << e.what() << std::endl;
            return nullptr;
        }
    }
}
