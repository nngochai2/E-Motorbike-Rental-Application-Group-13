//
// Created by Hoang Vinh Khue on 9/07/2025.
//

#include "MotorbikeManager.h"
#include "../utils/FileHandler.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "../utils/FileHandler.h"

namespace EMotoRental
{
    // ============================================ CONSTRUCTOR & DESTRUCTOR ===========================================

    MotorbikeManager::MotorbikeManager() {
        // Load motorbikes from file during initialization
        loadAllMotorbikes();
        std::cout << "MotorbikeManager initialized with " << motorbikes.size() << " motorbikes." << std::endl;
    }

    MotorbikeManager::~MotorbikeManager() {

        std::cout << "MotorbikeManager: Cleaning up..." << std::endl;
        // Clean up memory
        cleanupMemory();
        std::cout << "AuthManager: Data saved and resources cleaned up." << std::endl;
    }

    // ============================================ CORE MOTORBIKE MANAGEMENT ==========================================

    // ========================================= REGISTRATION AND LISTING MANAGEMENT =================================

    bool MotorbikeManager::registerMotorbike(
        const std::string& ownerUsername,
        const std::string& brand,
        const std::string& model,
        const std::string& color,
        int engineSize,
        const std::string& licensePlate,
        const std::string& city)
    {
        // Validate inputs
        if (ownerUsername.empty() || brand.empty() || model.empty() ||
            color.empty() || engineSize <= 0 || licensePlate.empty() || city.empty())
        {
            std::cout << "Registration failed: All fields are required." << std::endl;
            return false;
        }

        // Check if a motorbike with the same license plate already exists
        for (auto motorbike : motorbikes) {
            if (motorbike->getLicensePlate() == licensePlate) {
                std::cout << "Registration failed: License plate already registered." << std::endl;
                return false;
            }
        }

        // Create a new Motorbike
        auto* newMotorbike = new Motorbike(brand, model, color, engineSize, licensePlate, city, ownerUsername);
        motorbikes.push_back(newMotorbike);

        // Save the updated list to file
        saveAllMotorbikes();

        std::cout << "Motorbike registration successful: " << newMotorbike->getMotorbikeId() << std::endl;
        return true;
    }

    bool MotorbikeManager::listMotorbike(
        const std::string& motorbikeId,
        const DateUtil::TimePoint& startDate,
        const DateUtil::TimePoint& endDate,
        double dailyRate,
        double minRating)
    {
        // Find the motorbike by ID
        auto* motorbike = getMotorbikeById(motorbikeId);
        if (!motorbike) {
            std::cout << "Listing failed: Motorbike not found." << std::endl;
            return false;
        }

        // Try to list the motorbike for rent
        bool result = motorbike->listForRent(startDate, endDate, dailyRate, minRating);

        // Save the updated list to file if successful
        if (result) {
            saveAllMotorbikes();
            std::cout << "Motorbike successfully listed for rent." << std::endl;
        }

        return result;
    }

    bool MotorbikeManager::unlistMotorbike(const std::string& motorbikeId)
    {
        // Find the motorbike by ID
        auto* motorbike = getMotorbikeById(motorbikeId);
        if (!motorbike) {
            std::cout << "Unlisting failed: Motorbike not found." << std::endl;
            return false;
        }

        // Try to unlist the motorbike
        bool result = motorbike->unlist();

        // Save the updated list to file if successful
        if (result) {
            saveAllMotorbikes();
            std::cout << "Motorbike successfully unlisted." << std::endl;
        }

        return result;
    }

    // ========================================= SEARCH AND RETRIEVAL METHODS =======================================

    std::vector<Motorbike*> MotorbikeManager::searchMotorbikes(
        const std::string& city,
        const DateUtil::TimePoint& startDate,
        const DateUtil::TimePoint& endDate) const
    {
        std::vector<Motorbike*> results;

        for (auto motorbike : motorbikes) {
            // Filter by city if specified
            if (!city.empty() && motorbike->getCity() != city) {
                continue;
            }

            // Check availability for the specified date range
            if (motorbike->isAvailable(startDate, endDate)) {
                results.push_back(motorbike);
            }
        }

        std::cout << "Found " << results.size() << " matching motorbikes." << std::endl;
        return results;
    }

    Motorbike* MotorbikeManager::getMotorbikeById(const std::string& motorbikeId) const
    {
        for (auto motorbike : motorbikes) {
            if (motorbike->getMotorbikeId() == motorbikeId) {
                return motorbike;
            }
        }
        return nullptr; // Not found
    }

    std::vector<Motorbike*> MotorbikeManager::getMotorbikesByOwner(const std::string& username) const
    {
        std::vector<Motorbike*> results;

        for (auto motorbike : motorbikes) {
            if (motorbike->getOwnerUsername() == username) {
                results.push_back(motorbike);
            }
        }

        return results;
    }

    std::vector<Motorbike*> MotorbikeManager::getAllMotorbikes() const
    {
        return motorbikes;
    }

    // ========================================= RATING SYSTEM ======================================================

    bool MotorbikeManager::updateMotorbikeRating(const std::string& motorbikeId, double rating)
    {
        // Find the motorbike by ID
        auto* motorbike = getMotorbikeById(motorbikeId);
        if (!motorbike) {
            std::cout << "Rating update failed: Motorbike not found." << std::endl;
            return false;
        }

        // Update the rating
        motorbike->updateRating(rating);

        // Save the updated list to file
        saveAllMotorbikes();

        std::cout << "Motorbike rating updated successfully." << std::endl;
        return true;
    }

    // ========================================= DATA PERSISTENCE METHODS ============================================

    bool MotorbikeManager::loadAllMotorbikes()
    {
        try {
            // Check if file exists
            if (!FileHandler::fileExists("data/motorbikes.csv")) {
                std::cout << "Motorbikes file not found. Starting with empty motorbike list." << std::endl;
                return true; // Not an error, just an empty system
            }

            // Read file content
            const std::string content = FileHandler::readFromFile("data/motorbikes.csv");
            if (content.empty()) {
                std::cout << "Motorbikes file is empty." << std::endl;
                return true;
            }

            // Clear existing motorbikes
            for (auto motorbike : motorbikes) {
                delete motorbike;
            }
            motorbikes.clear();

            // Parse CSV content
            std::istringstream iss(content);
            std::string line;
            int loadedCount = 0;

            while (std::getline(iss, line)) {
                if (!line.empty()) {
                    if (Motorbike* motorbike = Motorbike::fromCSVString(line); motorbike != nullptr) {
                        motorbikes.push_back(motorbike);
                        loadedCount++;
                    } else {
                        std::cout << "Warning: Failed to parse motorbike data: " << line << std::endl;
                    }
                }
            }

            std::cout << "Loaded " << loadedCount << " motorbikes from file." << std::endl;
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error loading motorbikes: " << e.what() << std::endl;
            return false;
        }
    }

    bool MotorbikeManager::saveAllMotorbikes() const
    {
        try {
            if (motorbikes.empty()) {
                std::cout << "No motorbikes to save" << std::endl;
                return true;
            }

            // Create CSV content
            std::ostringstream oss;

            // Add motorbike data
            for (const Motorbike* motorbike : motorbikes) {
                oss << motorbike->toCSVString() << "\n";
            }

            // Write to file
            bool success = FileHandler::writeToFile(oss.str(), "data/motorbikes.csv");

            if (success) {
                std::cout << "Successfully saved " << motorbikes.size() << " motorbikes to file." << std::endl;
            } else {
                std::cout << "Failed to save motorbikes to file." << std::endl;
            }

            return success;
        } catch (std::exception& e) {
            std::cout << "Error saving motorbikes: " << e.what() << std::endl;
            return false;
        }
    }
}