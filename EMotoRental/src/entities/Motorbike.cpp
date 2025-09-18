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
                             requiredRenterRating(1.0), isListed(false),
                             motorbikeRating(3.0) {
        // Private default constructor - only used by fromCSVString() for data loading
        // Real motorbikes should always be created with the parameterized constructor
    }

    Motorbike::Motorbike(const std::string& brand, const std::string& model,
                         const std::string& color, int engineSize, int yearMade,
                         const std::string& plate, const std::string& city,
                         const std::string& owner)
        : licensePlate(plate), brand(brand), model(model), color(color),
          engineSize(engineSize), yearMade(yearMade), city(city), dailyRate(0.0),
          requiredRenterRating(1.0), isListed(false), ownerUsername(owner),
          motorbikeRating(3.0) {
        // Input validation following requirements
        if (brand.empty() || model.empty() || color.empty()) {
            throw std::invalid_argument("Brand, model, and color cannot be empty");
        }

        if (city.empty()) {
            throw std::invalid_argument("City cannot be empty");
        }

        // Validate city is HCMC or Hanoi (project requirement)
        if (city != "HCMC" && city != "Hanoi") {
            throw std::invalid_argument("City must be either 'HCMC' or 'Hanoi'");
        }

        if (owner.empty()) {
            throw std::invalid_argument("Owner username cannot be empty");
        }

        if (engineSize <= 0) {
            throw std::invalid_argument("Engine size must be greater than 0");
        }

        // Validate year made
        if (!DateUtil::isValidYear(yearMade, 1900, 0)) {
            const int currentYear = DateUtil::getCurrentYear();
            throw std::invalid_argument("Invalid made year! Year made must be between 1900 and "
                + std::to_string(currentYear));
        }
    }

    // ========================================== CORE FUNCTIONALITIES =================================================

    bool Motorbike::listForRent(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate,
                                double dailyRate, double minRating) {
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

    bool Motorbike::unlist() {
        if (!isListed) {
            std::cout << "Motorbike is not currently listed" << std::endl;
            return false;
        }

        isListed = false;
        std::cout << "Motorbike unlisted successfully" << std::endl;
        return true;
    }

    bool Motorbike::isAvailable(const DateUtil::TimePoint& startDate, const DateUtil::TimePoint& endDate) const {
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

    // ========================================== RATING AND REQUIREMENTS ==============================================

    void Motorbike::updateRating(double newRating) {
        // Input validation
        if (newRating < 1.0 || newRating > 5.0) {
            std::cout << "Invalid rating. Must be between 1.0 and 5.0" << std::endl;
            return;
        }

        motorbikeRating = newRating;
        // Note: RentalManager will handle the display message with rating count
    }

    bool Motorbike::meetsRequirement(const Member& member) const {
        // Check if member has enough rating
        if (member.getRenterRating() < requiredRenterRating) {
            return false;
        }

        // Check license requirement for bikes > 50cc (project business rule)
        if (engineSize > 50 && !member.isLicenseValid()) {
            return false;
        }

        return true;
    }

    double Motorbike::calculateCost(const int days) const {
        if (days <= 0) {
            return 0.0;
        }
        return dailyRate * days;
    }

    // ================================================ GETTERS ========================================================

    std::string Motorbike::getLicensePlate() const {
        return licensePlate;
    }

    std::string Motorbike::getOwnerUsername() const {
        return ownerUsername;
    }

    std::string Motorbike::getBrand() const {
        return brand;
    }

    std::string Motorbike::getModel() const {
        return model;
    }

    std::string Motorbike::getColor() const {
        return color;
    }

    std::string Motorbike::getCity() const {
        return city;
    }

    int Motorbike::getEngineSize() const {
        return engineSize;
    }

    int Motorbike::getYearMade() const {
        return yearMade;
    }

    double Motorbike::getDailyRate() const {
        return dailyRate;
    }

    double Motorbike::getRequiredRenterRating() const {
        return requiredRenterRating;
    }

    double Motorbike::getMotorbikeRating() const {
        return motorbikeRating;
    }

    bool Motorbike::getIsListed() const {
        return isListed;
    }

    DateUtil::TimePoint Motorbike::getAvailableStart() const {
        return availableStart;
    }

    DateUtil::TimePoint Motorbike::getAvailableEnd() const {
        return availableEnd;
    }

    // ================================================ SETTERS ========================================================

    void Motorbike::setLicensePlate(const std::string& plate) {
        if (plate.empty()) {
            throw std::invalid_argument("License plate cannot be empty");
        }
        licensePlate = plate;
    }

    void Motorbike::setCity(const std::string& newCity) {
        if (newCity.empty()) {
            throw std::invalid_argument("City cannot be empty");
        }

        // Validate city is HCMC or Hanoi (project requirement)
        if (newCity != "HCMC" && newCity != "Hanoi") {
            throw std::invalid_argument("City must be either 'HCMC' or 'Hanoi'");
        }

        city = newCity;
    }

    void Motorbike::setOwnerUsername(const std::string& owner) {
        if (owner.empty()) {
            throw std::invalid_argument("Owner username cannot be empty");
        }
        ownerUsername = owner;
    }

    void Motorbike::setYearMade(const int year) {
        if (!DateUtil::isValidYear(year)) {
            const int currentYear = DateUtil::getCurrentYear();
            throw std::invalid_argument("Year made must be between 1900 and " +
                                      std::to_string(currentYear + 1));
        }
        yearMade = year;
    }

    // ============================================= DISPLAY METHODS ===================================================

    void Motorbike::displayDetails() const {
        std::cout << "========== MOTORBIKE DETAILS ==========" << std::endl;
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
            std::cout << "Min. Renter Rating: " << std::fixed << std::setprecision(1) << requiredRenterRating << "/5.0"
                << std::endl;
            std::cout << "Available From: " << DateUtil::formatDate(availableStart) << std::endl;
            std::cout << "Available Until: " << DateUtil::formatDate(availableEnd) << std::endl;
        }
        else {
            std::cout << "Status: Not listed for rent" << std::endl;
        }
        std::cout << "=======================================" << std::endl;
    }

    void Motorbike::displayPublicInfo() const {
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

    // ============================================ DATA PERSISTENCE ===================================================

    std::string Motorbike::toCSVString() const {
        std::ostringstream oss;
        oss << licensePlate << ","
            << brand << ","
            << model << ","
            << color << ","
            << engineSize << ","
            << yearMade << ","
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

    Motorbike* Motorbike::fromCSVString(const std::string& csvData) {
        const std::vector<std::string> tokens = FileHandler::parseCSVLine(csvData);

        if (tokens.size() < 14) {
            std::cerr << "Invalid motorbike CSV data: not enough fields" << std::endl;
            return nullptr;
        }

        try {
            auto* bike = new Motorbike();

            bike->licensePlate = tokens[0]; // 0 - License plate as primary identifier
            bike->brand = tokens[1]; // 1
            bike->model = tokens[2]; // 2
            bike->color = tokens[3]; // 3
            bike->engineSize = std::stoi(tokens[4]); // 4
            bike->yearMade = std::stoi(tokens[5]); // 5
            bike->city = tokens[6]; // 6
            bike->dailyRate = std::stod(tokens[7]); // 7
            bike->requiredRenterRating = std::stod(tokens[8]); // 8
            bike->motorbikeRating = std::stod(tokens[9]); // 9
            bike->isListed = tokens[10] == "1"; // 10
            bike->ownerUsername = tokens[11]; // 11
            bike->availableStart = DateUtil::parseDate(tokens[12]); // 12
            bike->availableEnd = DateUtil::parseDate(tokens[13]); // 13

            return bike;
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing motorbike data: " << e.what() << std::endl;
            return nullptr;
        }
    }
}
