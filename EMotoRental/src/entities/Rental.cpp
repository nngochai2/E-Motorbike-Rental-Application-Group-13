//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#include "Rental.h"
#include <iostream>
#include <sstream>

#include "RentalRequest.h"
#include "../utils/FileHandler.h"
#include "../utils/IdGenerator.h"

namespace EMotoRental
{
    Rental::Rental() : totalCost(0.0), status(RentalStatus::ACTIVE) {
        rentalId = generateRentalId();
    }

    Rental::Rental(const RentalRequest& request, const std::string& ownerUsername)
        : renterUsername(request.getRenterUsername()),
          ownerUsername(ownerUsername),
          motorbikeLicensPlate(request.getMotorbikeLicensePlate()),
          startDate(request.getStartDate()),
          endDate(request.getEndDate()),
          totalCost(request.getEstimatedCost()),
          status(RentalStatus::ACTIVE) {
        rentalId = generateRentalId();
    }

    // ========================================== CORE FUNCTIONALITIES =================================================

    void Rental::complete() {
        status = RentalStatus::COMPLETED;
    }

    bool Rental::isActive() const {
        return status == RentalStatus::ACTIVE;
    }

    bool Rental::isOverdue() const {
        return isActive() && DateUtil::getCurrentTime() > endDate;
    }

    // ================================================ GETTERS ========================================================

    std::string Rental::getRentalId() const { return rentalId; }
    RentalStatus Rental::getStatus() const { return status; }
    std::string Rental::getRenterUsername() const { return renterUsername; }
    std::string Rental::getOwnerUsername() const { return ownerUsername; }
    std::string Rental::getMotorbikeLicensePlate() const { return motorbikeLicensPlate; }
    DateUtil::TimePoint Rental::getStartDate() const { return startDate; }
    DateUtil::TimePoint Rental::getEndDate() const { return endDate; }
    double Rental::getTotalCost() const { return totalCost; }

    // ============================================= DISPLAY METHODS ===================================================

    void Rental::displayInfo() const {
        std::cout << "Rental ID: " << rentalId << std::endl;
        std::cout << "Renter: " << renterUsername << std::endl;
        std::cout << "Owner: " << ownerUsername << std::endl;
        std::cout << "Motorbike: " << motorbikeLicensPlate << std::endl;
        std::cout << "Period: " << DateUtil::formatDate(startDate)
                  << " to " << DateUtil::formatDate(endDate) << std::endl;
        std::cout << "Total Cost: " << std::fixed << std::setprecision(2) << totalCost << " CP" << std::endl;
        std::cout << "Status: " << statusToString() << std::endl;

        if (isOverdue()) {
            std::cout << "OVERDUE - Please return the motorbike!" << std::endl;
        }
    }

    // ============================================ DATA PERSISTENCE ===================================================

    std::string Rental::toCSVString() const {
        std::ostringstream oss;
        oss << rentalId << ","
            << renterUsername << ","
            << ownerUsername << ","
            << motorbikeLicensPlate << ","
            << DateUtil::formatDate(startDate) << ","
            << DateUtil::formatDate(endDate) << ","
            << std::fixed << std::setprecision(2) << totalCost << ","
            << statusToString();
        return oss.str();
    }

    Rental* Rental::fromCSVString(const std::string& csvData) {
        std::istringstream iss(csvData);
        std::string token;
        std::vector<std::string> tokens;

        // Parse CSV line
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 8) {
            std::cerr << "Invalid Rental CSV data: insufficient fields (" << tokens.size() << " < 8)" << std::endl;
            return nullptr;
        }

        try {
            auto* rental = new Rental();

            // Parse each field
            rental->rentalId = tokens[0];
            rental->renterUsername = tokens[1];
            rental->ownerUsername = tokens[2];
            rental->motorbikeLicensPlate = tokens[3];
            rental->startDate = DateUtil::parseDate(tokens[4]);
            rental->endDate = DateUtil::parseDate(tokens[5]);
            rental->totalCost = std::stod(tokens[6]);
            rental->status = stringToStatus(tokens[7]);

            return rental;
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing Rental CSV data: " << e.what() << std::endl;
            return nullptr;
        }
    }

    // ============================================ HELPER METHODS =====================================================

    std::string Rental::generateRentalId() {
        // Generate unique ID based on timestamp
        const auto timestamp = DateUtil::getTimestamp();
        return "RNT" + std::to_string(timestamp);
    }

    std::string Rental::statusToString() const {
        switch (status) {
            case RentalStatus::ACTIVE: return "ACTIVE";
            case RentalStatus::COMPLETED: return "COMPLETED";
            default: return "UNKNOWN";
        }
    }

    RentalStatus Rental::stringToStatus(const std::string& str) {
        if (str == "ACTIVE") return RentalStatus::ACTIVE;
        if (str == "COMPLETED") return RentalStatus::COMPLETED;

        std::cerr << "Warning: Unknown RentalStatus string: '" << str << "', defaulting to ACTIVE" << std::endl;
        return RentalStatus::ACTIVE;
    }
}
