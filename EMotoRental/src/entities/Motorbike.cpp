//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#include "Motorbike.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../utils/FileHandler.h"
#include "../utils/IdGenerator.h"

namespace EMotoRental
{
    Motorbike::Motorbike() : engineSize(0), yearMade(2025), dailyRate(0.0), 
                            requiredRenterRating(0.0), motorbikeRating(3.0), 
                            isListed(false)
    {
        // Generate a UUID for the motorbike with a prefix
        motorbikeId = IdGenerator::generateId("MB");
    }

    Motorbike::Motorbike(const std::string& brand, const std::string& model, const std::string& color, 
                        int engineSize, const std::string& plate, const std::string& city, 
                        const std::string& owner) 
        : brand(brand), model(model), color(color), engineSize(engineSize),
          yearMade(2025), licensePlate(plate), city(city), dailyRate(0.0),
          requiredRenterRating(0.0), motorbikeRating(3.0), isListed(false),
          ownerUsername(owner)
    {
        // Generate a UUID for the motorbike with a prefix
        motorbikeId = IdGenerator::generateId("MB");
    }

    bool Motorbike::listForRent(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate, 
                              double dailyRate, double minRating)
    {
        // Validate inputs
        if (dailyRate <= 0.0) {
            std::cout << "Daily rate must be greater than zero" << std::endl;
            return false;
        }

        if (minRating < 0.0 || minRating > 5.0) {
            std::cout << "Required rating must be between 0 and 5" << std::endl;
            return false;
        }

        // Ensure start date is before end date
        if (DateUtil::daysBetween(startDate, endDate) < 0) {
            std::cout << "End date must be after start date" << std::endl;
            return false;
        }

        // Update motorbike listing information
        this->availableStart = startDate;
        this->availableEnd = endDate;
        this->dailyRate = dailyRate;
        this->requiredRenterRating = minRating;
        this->isListed = true;

        std::cout << "Motorbike listed successfully" << std::endl;
        return true;
    }

    bool Motorbike::unlist()
    {
        if (!isListed) {
            std::cout << "Motorbike is not currently listed" << std::endl;
            return false;
        }

        isListed = false;
        std::cout << "Motorbike unlisted successfully" << std::endl;
        return true;
    }

    bool Motorbike::isAvailable(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate) const
    {
        if (!isListed) {
            return false;
        }

        // Check if dates are valid (start before end)
        if (DateUtil::daysBetween(startDate, endDate) < 0) {
            return false;
        }

        // Check if requested date range falls within available date range
        if (DateUtil::daysBetween(availableStart, startDate) < 0 || 
            DateUtil::daysBetween(endDate, availableEnd) < 0) {
            return false;
        }

        return true;
    }

    void Motorbike::updateRating(double newRating)
    {
        if (newRating < 1.0 || newRating > 5.0) {
            std::cout << "Invalid rating. Must be between 1.0 and 5.0" << std::endl;
            return;
        }

        // Calculate the new average rating
        motorbikeRating = (motorbikeRating + newRating) / 2.0;
        std::cout << "Motorbike rating updated to " << motorbikeRating << std::endl;
    }

    bool Motorbike::meetsRequirement(const Member& member) const
    {
        // Check if member has enough rating
        if (member.getRenterRating() < requiredRenterRating) {
            return false;
        }

        return true;
    }

    double Motorbike::calculateCost(int days) const
    {
        if (days <= 0) {
            return 0.0;
        }
        return dailyRate * days;
    }

    std::string Motorbike::getMotorbikeId() const
    {
        return motorbikeId;
    }

    std::string Motorbike::getOwnerUsername() const
    {
        return ownerUsername;
    }
    
    std::string Motorbike::getLicensePlate() const
    {
        return licensePlate;
    }
    
    std::string Motorbike::getCity() const
    {
        return city;
    }
    
    bool Motorbike::getIsListed() const
    {
        return isListed;
    }

    void Motorbike::displayDetails() const
    {
        std::cout << "========== MOTORBIKE DETAILS ==========" << std::endl;
        std::cout << "ID: " << motorbikeId << std::endl;
        std::cout << "Brand: " << brand << std::endl;
        std::cout << "Model: " << model << std::endl;
        std::cout << "Color: " << color << std::endl;
        std::cout << "Engine Size: " << engineSize << " cc" << std::endl;
        std::cout << "Year Made: " << yearMade << std::endl;
        std::cout << "License Plate: " << licensePlate << std::endl;
        std::cout << "City: " << city << std::endl;
        std::cout << "Rating: " << std::fixed << std::setprecision(1) << motorbikeRating << "/5.0" << std::endl;
        std::cout << "Owner: " << ownerUsername << std::endl;

        if (isListed) {
            std::cout << "Status: Listed for rent" << std::endl;
            std::cout << "Daily Rate: " << std::fixed << std::setprecision(2) << dailyRate << " CP" << std::endl;
            std::cout << "Min. Renter Rating: " << std::fixed << std::setprecision(1) << requiredRenterRating << "/5.0" << std::endl;
            std::cout << "Available From: " << DateUtil::formatDate(availableStart) << std::endl;
            std::cout << "Available Until: " << DateUtil::formatDate(availableEnd) << std::endl;
        } else {
            std::cout << "Status: Not listed for rent" << std::endl;
        }
        std::cout << "=======================================" << std::endl;
    }

    void Motorbike::displayPublicInfo() const
    {
        if (!isListed) {
            return; // Don't display info for unlisted bikes
        }

        std::cout << "--------- MOTORBIKE LISTING ---------" << std::endl;
        std::cout << "Brand: " << brand << " | Model: " << model << std::endl;
        std::cout << "Color: " << color << " | Engine: " << engineSize << "cc" << std::endl;
        std::cout << "City: " << city << std::endl;
        std::cout << "Rating: " << std::fixed << std::setprecision(1) << motorbikeRating << "/5.0" << std::endl;
        std::cout << "Daily Rate: " << std::fixed << std::setprecision(2) << dailyRate << " CP" << std::endl;
        std::cout << "Available: " << DateUtil::formatDate(availableStart) 
                  << " to " << DateUtil::formatDate(availableEnd) << std::endl;
        std::cout << "Min. Renter Rating: " << requiredRenterRating << "/5.0" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }

    std::string Motorbike::toCSVString() const
    {
        std::ostringstream oss;
        oss << motorbikeId << ","
            << brand << ","
            << model << ","
            << color << ","
            << engineSize << ","
            << yearMade << ","
            << licensePlate << ","
            << city << ","
            << dailyRate << ","
            << requiredRenterRating << ","
            << motorbikeRating << ","
            << (isListed ? "1" : "0") << ","
            << ownerUsername << ","
            << DateUtil::formatDate(availableStart) << ","
            << DateUtil::formatDate(availableEnd);
            
        return oss.str();
    }

    Motorbike* Motorbike::fromCSVString(const std::string& csvData)
    {
        std::vector<std::string> tokens = FileHandler::parseCSVLine(csvData);
        
        if (tokens.size() < 15) {
            std::cerr << "Invalid motorbike CSV data: not enough fields" << std::endl;
            return nullptr;
        }
        
        try {
            auto* bike = new Motorbike();
            
            bike->motorbikeId = tokens[0];
            bike->brand = tokens[1];
            bike->model = tokens[2];
            bike->color = tokens[3];
            bike->engineSize = std::stoi(tokens[4]);
            bike->yearMade = std::stoi(tokens[5]);
            bike->licensePlate = tokens[6];
            bike->city = tokens[7];
            bike->dailyRate = std::stod(tokens[8]);
            bike->requiredRenterRating = std::stod(tokens[9]);
            bike->motorbikeRating = std::stod(tokens[10]);
            bike->isListed = (tokens[11] == "1");
            bike->ownerUsername = tokens[12];
            bike->availableStart = DateUtil::parseDate(tokens[13]);
            bike->availableEnd = DateUtil::parseDate(tokens[14]);
            
            return bike;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing motorbike data: " << e.what() << std::endl;
            return nullptr;
        }
    }
}
