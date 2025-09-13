//
// Created by Hoang Vinh Khue on 9/07/2025.
//

#include "MotorbikeManager.h"
#include "../utils/FileHandler.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

namespace EMotoRental
{
    // ============================================ CONSTRUCTOR & DESTRUCTOR ===========================================

    MotorbikeManager::MotorbikeManager() {
        std::cout << "MotorbikeManager: Initialized." << std::endl;
    }

    MotorbikeManager::~MotorbikeManager() {
        std::cout << "MotorbikeManager: Cleaning up..." << std::endl;
        cleanupMemory();
    }

    // ============================================ CORE MOTORBIKE MANAGEMENT ==========================================

    bool MotorbikeManager::registerMotorbike(const MotorbikeRegistrationData& data) {
        // Validate input data
        if (data.brand.empty() || data.model.empty() || data.licensePlate.empty()) {
            std::cout << "Registration failed: Brand, model, and license plate are required." << std::endl;
            return false;
        }

        // Check if license plate is unique
        if (!isLicensePlateUnique(data.licensePlate)) {
            std::cout << "Registration failed: License plate '" << data.licensePlate
                << "' is already registered." << std::endl;
            return false;
        }

        try {
            // Create new motorbike
            auto* newMotorbike = new Motorbike(data.brand, data.model, data.color, data.engineSize, data.yearMade,
                                               data.licensePlate, data.city, data.ownerUsername);

            // Add to collection
            motorbikes.push_back(newMotorbike);

            std::cout << "Motorbike registered successfully!" << std::endl;
            std::cout << "License Plate: " << data.licensePlate << std::endl;
            std::cout << "Owner: " << data.ownerUsername << std::endl;

            // DataManager will handle data persistence externally
            return true;
        }
        catch (const std::exception& e) {
            std::cout << "Registration failed: " << e.what() << std::endl;
            return false;
        }
    }

    bool MotorbikeManager::listMotorbike(const std::string& plate, const MotorbikeListingData& listingData) const {
        // Find the motorbike by license plate
        auto* motorbike = getMotorbikeByLicensePlate(plate);
        if (!motorbike) {
            std::cout << "Listing failed: Motorbike with license plate '" << plate << "' not found." << std::endl;
            return false;
        }

        // Use the motorbike's own listing logic
        const bool success = motorbike->listForRent(listingData.startDate, listingData.endDate, listingData.dailyRate,
                                                    listingData.minRenterRating);

        return success;
    }

    bool MotorbikeManager::unlistMotorbike(const std::string& plate) const {
        // Find the motorbike by license plate
        auto* motorbike = getMotorbikeByLicensePlate(plate);
        if (!motorbike) {
            std::cout << "Unlisting failed: Motorbike with ID '" << plate << "' not found." << std::endl;
            return false;
        }

        // Use the motorbike's own unlisting logic
        const bool success = motorbike->unlist();

        return success;
    }

    // ========================================== SEARCH AND RETRIEVAL METHODS =========================================

    std::vector<Motorbike*> MotorbikeManager::searchMotorbikes(const std::string& city,
                                                               const DateUtil::TimePoint& startDate,
                                                               const DateUtil::TimePoint& endDate) const {
        std::vector<Motorbike*> results;

        for (auto* motorbike : motorbikes) {
            // Check if motorbike matches search criteria
            if (motorbike->getCity() == city && motorbike->isAvailable(startDate, endDate)) {
                results.push_back(motorbike);
            }

            // Sort results by rating (highest first)
            std::ranges::sort(results,
                              [](const Motorbike* a, const Motorbike* b) {
                                  return a->getMotorbikeRating() > b->getMotorbikeRating();
                              });

            return results;
        }

        std::cout << "Found " << results.size() << " matching motorbikes." << std::endl;
        return results;
    }

    std::vector<Motorbike*> MotorbikeManager::getAllListedMotorbikes() const {
        std::vector<Motorbike*> listedMotorbikes;

        for (Motorbike* motorbike : motorbikes) {
            if (motorbike->getIsListed()) {
                listedMotorbikes.push_back(motorbike);
            }
        }

        return listedMotorbikes;
    }

    // ======================================= INDIVIDUAL MOTORBIKE RETRIEVAL ==========================================

    Motorbike* MotorbikeManager::getMotorbikeByLicensePlate(const std::string& plate) const {
        for (auto* motorbike : motorbikes) {
            if (motorbike->getLicensePlate() == plate) {
                return motorbike;
            }
        }

        return nullptr;
    }

    Motorbike* MotorbikeManager::getMotorbikeByOwner(const std::string& ownerUsername) const {
        std::vector<Motorbike*> results;

        for (auto* motorbike : motorbikes) {
            if (motorbike->getOwnerUsername() == ownerUsername) {
                return motorbike;
            }
        }

        return nullptr;
    }

    // ============================================ RATING MANAGEMENT ==================================================

    void MotorbikeManager::updateMotorbikeRating(const std::string& plate, const double newRating) {
        // Find the motorbike by ID
        if (auto* motorbike = getMotorbikeByLicensePlate(plate)) {
            motorbike->updateRating(newRating);
            std::cout << "MotorbikeManager: Rating updated for motorbike " << plate << std::endl;
        } else {
            std::cout << "Warning: Cannot update rating for motorbike '" << plate
                      << "' - motorbike not found." << std::endl;
        }
    }

    // ========================================== VALIDATION METHODS ===================================================

    bool MotorbikeManager::isLicensePlateUnique(const std::string& licensePlate) const {
        for (const Motorbike* motorbike : motorbikes) {
            if (motorbike->getLicensePlate() == licensePlate) {
                return false;
            }
        }

        return true;
    }

    bool MotorbikeManager::canOwnerRegisterMotorbike(const std::string& ownerUsername) const {
        // Project requirement: Each member can register only one motorbike
        for (const Motorbike* motorbike : motorbikes) {
            if (motorbike->getOwnerUsername() == ownerUsername) {
                return false;
            }
        }

        return true;
    }

    // =========================================== DATA MANAGEMENT =====================================================

    void MotorbikeManager::addMotorbike(Motorbike* motorbike) {
        if (motorbike) {
            motorbikes.push_back(motorbike);
        }
    }

    // ======================================= STATISTICS AND UTILITIES ================================================

    int MotorbikeManager::getTotalMotorbikeCount() const {
        return static_cast<int>(motorbikes.size());
    }

    int MotorbikeManager::getListedMotorbikeCount() const {
        int count = 0;
        for (const Motorbike* motorbike : motorbikes) {
            if (motorbike->getIsListed()) {
                count++;
            }
        }
        return count;
    }

    std::vector<std::string> MotorbikeManager::getAllCities() const {
        std::vector<std::string> cities;

        for (const Motorbike* motorbike : motorbikes) {
            if (std::string city = motorbike->getCity(); std::ranges::find(cities, city) == cities.end()) {
                cities.push_back(city);
            }
        }

        return cities;
    }

    // =========================================== DISPLAY METHODS =====================================================

    void MotorbikeManager::displayAllMotorbikes() const
    {
        std::cout << "\n========== ALL REGISTERED MOTORBIKES ==========" << std::endl;

        if (motorbikes.empty()) {
            std::cout << "No motorbikes registered in the system." << std::endl;
            std::cout << "===============================================" << std::endl;
            return;
        }

        int index = 1;
        for (const Motorbike* motorbike : motorbikes) {
            std::cout << "\n--- Motorbike #" << index++ << " ---" << std::endl;
            motorbike->displayDetails();
        }

        std::cout << "\nTotal: " << motorbikes.size() << " motorbikes registered." << std::endl;
        std::cout << "===============================================" << std::endl;
    }

    void MotorbikeManager::displayMotorbikeStatistics() const
    {
        std::cout << "\n========== MOTORBIKE STATISTICS ==========" << std::endl;

        int totalCount = getTotalMotorbikeCount();
        int listedCount = getListedMotorbikeCount();
        int unlistedCount = totalCount - listedCount;

        std::cout << "Total Motorbikes: " << totalCount << std::endl;
        std::cout << "Listed for Rent: " << listedCount << std::endl;
        std::cout << "Not Listed: " << unlistedCount << std::endl;

        if (totalCount > 0) {
            double listedPercentage = (static_cast<double>(listedCount) / totalCount) * 100;
            std::cout << "Listing Rate: " << std::fixed << std::setprecision(1)
                      << listedPercentage << "%" << std::endl;
        }

        // City distribution
        const std::vector<std::string> cities = getAllCities();
        std::cout << "\nCity Distribution:" << std::endl;
        for (const std::string& city : cities) {
            int cityCount = 0;
            for (const Motorbike* motorbike : motorbikes) {
                if (motorbike->getCity() == city) {
                    cityCount++;
                }
            }
            std::cout << "  " << city << ": " << cityCount << " motorbikes" << std::endl;
        }

        std::cout << "===========================================" << std::endl;
    }

    // =========================================== HELPER METHODS ======================================================

    void MotorbikeManager::cleanupMemory() {
        for (const Motorbike* motorbike : motorbikes) {
            delete motorbike;
        }
        motorbikes.clear();
    }

}
