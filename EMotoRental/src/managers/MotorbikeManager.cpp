//
// Created by Nguyen Ngoc Hai on 9/3/2025.
//

#include "MotorbikeManager.h"
#include "../utils/FileHandler.h"
#include <iostream>
#include <sstream>
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
        // Clean up memory
        cleanupMemory();
        std::cout << "AuthManager: Data saved and resources cleaned up." << std::endl;
    }

    // ============================================ CORE MOTORBIKE MANAGEMENT ==========================================

    bool MotorbikeManager::registerMotorbike(const MotorbikeRegistrationData &data) {
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

        // Check if owner can register a motorbike (project requirement: one motorbike per member)
        if (!canOwnerRegisterMotorbike(data.ownerUsername)) {
            std::cout << "Registration failed: Member '" << data.ownerUsername
                        << "' already owns a motorbike." << std::endl;
            return false;
        }

        try {
            // Create new motorbike
            auto* newMotorbike = new Motorbike(data.brand, data.model, data.color,
                                             data.engineSize, data.yearMade,
                                             data.licensePlate, data.city,
                                             data.ownerUsername);

            // Add to collection
            motorbikes.push_back(newMotorbike);

            std::cout << "Motorbike registered successfully!" << std::endl;
            std::cout << "License Plate: " << data.licensePlate << std::endl;
            std::cout << "Owner: " << data.ownerUsername << std::endl;

            // Note: DataManager will handle persistence externally
            return true;

        } catch (const std::exception& e) {
            std::cout << "Registration failed: " << e.what() << std::endl;
            return false;
        }
    }

    bool MotorbikeManager::listMotorbike(const std::string &licensePlate, const MotorbikeListingData &listingData) const {
        Motorbike* motorbike = findMotorbikeByLicensePlate(licensePlate);
        if (!motorbike) {
            std::cout << "Listing failed: Motorbike with license plate '" << licensePlate << "' not found." << std::endl;
            return false;
        }

        // Use the motorbike's own listing logic
        const bool success = motorbike->listForRent(listingData.startDate, listingData.endDate, listingData.dailyRate,
                                                listingData.minRenterRating);

        // DataManager will handle saving the updates
        return success;
    }

    bool MotorbikeManager::unlistMotorbike(const std::string &licensePlate) const {
        Motorbike* motorbike = findMotorbikeByLicensePlate(licensePlate);
        if (!motorbike) {
            std::cout << "Unlisting failed: Motorbike with plate '" << licensePlate << "' not found." << std::endl;
            return false;
        }

        // Use the motorbike's own unlisting logic
        bool success = motorbike->unlist();

        // Note: DataManager will handle saving the updates
        return success;
    }


    // ================================================= HELPER METHODS ================================================

    void MotorbikeManager::cleanupMemory() {
        for (const Motorbike* motorbike : motorbikes) {
            delete motorbike;
        }
        motorbikes.clear();
    }
}
