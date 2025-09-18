//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#include "DataManager.h"

#include <filesystem>
#include <iostream>

#include "DataInitializer.h"
#include "FileHandler.h"

namespace EMotoRental
{
    // Static member definitions
    const std::string DataManager::DATA_DIR = "EMotoRental/data/";
    const std::string DataManager::MEMBERS_DIR = "EMotoRental/data/members/";
    const std::string DataManager::ADMINS_DIR = "EMotoRental/data/admins/";
    const std::string DataManager::MOTORBIKES_DIR = "EMotoRental/data/motorbikes/";
    const std::string DataManager::RENTALS_DIR = "EMotoRental/data/rentals/";
    const std::string DataManager::REQUESTS_DIR = "EMotoRental/data/rentals/requests/";
    const std::string DataManager::RENTALS_ACTIVE_DIR = "EMotoRental/data/rentals/active/";
    const std::string DataManager::RATINGS_DIR = "EMotoRental/data/rentals/ratings/";

    DataManager::DataManager() {
        authManager = nullptr;
        motorbikeManager = nullptr;
        rentalManager = nullptr;

        if (initializeSystem()) {
            authManager = new AuthManager();
            motorbikeManager = new MotorbikeManager();
            rentalManager = new RentalManager();
            std::cout << "DataManager: System initialized successfully." << std::endl;
        }
        else {
            std::cerr << "DataManager: Failed to initialize system." << std::endl;
        }
    }

    DataManager::~DataManager() {
        shutdown();
    }

    // ============================================= MANAGER ACCESS ====================================================

    AuthManager* DataManager::getAuthManager() const {
        return authManager;
    }

    MotorbikeManager* DataManager::getMotorbikeManager() const {
        return motorbikeManager;
    }

    RentalManager* DataManager::getRentalManager() const {
        return rentalManager;
    }

    // ============================================= DATA LIFECYCLE ====================================================

    bool DataManager::initializeSystem() {
        std::cout << "DataManager: Initializing system." << std::endl;

        // Create directory structure
        if (!createDirectoryStructure()) {
            std::cerr << "DataManager: Failed to create directory structure." << std::endl;
            return false;
        }

        // Initialize default data on first run
        if (DataInitializer::isFirstRun()) {
            std::cout << "DataManager: First run detected. Setting up demonstration data..." << std::endl;
            if (!DataInitializer::initializeDefaultData()) {
                std::cerr << "DataManager: Failed to initialize default data." << std::endl;
                return false;
            }
        }

        std::cout << "DataManager: Directory structure created successfully." << std::endl;
        return true;
    }

    bool DataManager::loadAllData() const {
        std::cout << "DataManager: Loading all system data..." << std::endl;

        if (!authManager) {
            std::cerr << "DataManager: AuthManager not initialized!" << std::endl;
            return false;
        }

        if (!motorbikeManager) {
            std::cerr << "DataManager: MotorbikeManager not initialized!" << std::endl;
            return false;
        }

        if (!rentalManager) {
            std::cerr << "DataManager: RentalManager not initialized!" << std::endl;
            return false;
        }

        bool success = true;

        // Load all members
        const auto memberFiles = listMembers();
        int loadedMembers = 0;

        for (const std::string& username : memberFiles) {
            if (Member* member = nullptr; loadMember(username, member)) {
                authManager->addMember(member);
                loadedMembers++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to load member: " << username << std::endl;
                success = false;
            }
        }

        // Load all admins
        const auto adminFiles = listAdmins();
        int loadedAdmins = 0;

        for (const std::string& username : adminFiles) {
            if (Admin* admin = nullptr; loadAdmin(username, admin)) {
                authManager->addAdmin(admin);
                loadedAdmins++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to load admin: " << username << std::endl;
                success = false;
            }
        }

        // Load all motorbikes
        const auto motorbikeFiles = listMotorbikes();
        int loadedMotorbikes = 0;
        for (const std::string& licensePlate : motorbikeFiles) {
            if (Motorbike* motorbike = nullptr; loadMotorbike(licensePlate, motorbike)) {
                motorbikeManager->addMotorbike(motorbike);
                loadedMotorbikes++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to load motorbike: " << licensePlate << std::endl;
                success = false;
            }
        }

        // Load all rental requests
        const auto requestFiles = listRentalRequests();
        int loadedRequests = 0;
        for (const std::string& requestId : requestFiles) {
            if (RentalRequest* request = nullptr; loadRentalRequest(requestId, request)) {
                rentalManager->addRentalRequest(request);
                loadedRequests++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to load rental request: " << requestId << std::endl;
                success = false;
            }
        }

        // Load all rentals
        const auto rentalFiles = listRentals();
        int loadedRentals = 0;
        for (const std::string& rentalId : rentalFiles) {
            if (Rental* rental = nullptr; loadRental(rentalId, rental)) {
                rentalManager->addRental(rental);
                loadedRentals++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to load rental: " << rentalId << std::endl;
                success = false;
            }
        }

        // Load all ratings
        const auto ratingFiles = listRatings();
        int loadedRatings = 0;
        for (const std::string& ratingId : ratingFiles) {
            if (Rating* rating = nullptr; loadRating(ratingId, rating)) {
                rentalManager->addRating(rating);
                loadedRatings++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to load rating: " << ratingId << std::endl;
                success = false;
            }
        }

        std::cout << "DataManager: Loaded " << loadedMembers << " members, "
            << loadedAdmins << " admins, " << loadedMotorbikes << " motorbikes, "
            << loadedRequests << " requests, " << loadedRentals << " rentals, "
            << loadedRatings << " ratings." << std::endl;

        // Validate data consistency
        if (success) {
            success = validateDataConsistency();
        }

        logDataOperation("LOAD_ALL",
                         "Members: " + std::to_string(loadedMembers) +
                         ", Admins: " + std::to_string(loadedAdmins) +
                         ", Motorbikes: " + std::to_string(loadedMotorbikes) +
                         ", Requests: " + std::to_string(loadedRequests) +
                         ", Rentals: " + std::to_string(loadedRentals) +
                         ", Ratings: " + std::to_string(loadedRatings));

        return success;
    }

    bool DataManager::saveAllData() const {
        std::cout << "DataManager: Saving all system data..." << std::endl;

        if (!authManager) {
            std::cerr << "DataManager: AuthManager not initialized!" << std::endl;
            return false;
        }

        if (!motorbikeManager) {
            std::cerr << "DataManager: MotorbikeManager not initialized!" << std::endl;
            return false;
        }

        bool success = true;
        int savedMembers = 0, savedAdmins = 0, savedMotorbikes = 0;

        // Save all members
        for (const auto members = authManager->getAllMembers(); const auto& member : members) {
            if (saveMember(member)) {
                savedMembers++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to save member: " << member->getUsername() << std::endl;
                success = false;
            }
        }

        // Save all admins
        for (const auto admins = authManager->getAllAdmins(); Admin* admin : admins) {
            if (saveAdmin(admin)) {
                savedAdmins++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to save admin: " << admin->getUsername() << std::endl;
                success = false;
            }
        }

        // Save all motorbikes
        for (const auto motorbikes = motorbikeManager->getAllMotorbikes(); const auto& motorbike : motorbikes) {
            if (saveMotorbike(motorbike)) {
                savedMotorbikes++;
            }
            else {
                std::cout << "DataManager: Warning - Failed to save motorbike: "
                    << motorbike->getLicensePlate() << std::endl;
                success = false;
            }
        }

        std::cout << "DataManager: Saved " << savedMembers << " members, "
            << savedAdmins << " admins, and " << savedMotorbikes << " motorbikes." << std::endl;

        logDataOperation("SAVE_ALL", "Members: " + std::to_string(savedMembers) + ", Admins: "
                         + std::to_string(savedAdmins));

        return success;
    }

    void DataManager::shutdown() {
        std::cout << "DataManager: Shutting down system..." << std::endl;

        // Save all data before cleanup
        if (saveAllData()) {
            // Cleanup managers
            delete authManager;
            authManager = nullptr;

            // Cleanup MotorbikeManager
            delete motorbikeManager;
            motorbikeManager = nullptr;
        }

        std::cout << "DataManager: System shutdown successfully." << std::endl;
    }

    // ======================================= INDIVIDUAL ENTITY OPERATIONS ============================================

    bool DataManager::saveMember(const Member* member) {
        if (!member) return false;

        try {
            const std::string filePath = getMemberFilePath(member->getUsername());
            const std::string csvData = member->toCSVString();

            const bool success = FileHandler::writeToFile(csvData, filePath);

            if (success) {
                logDataOperation("SAVE_MEMBER", member->getUsername());
            }

            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error in saving member " << member->getUsername() << ":" << e.what() <<
                std::endl;
            return false;
        }
    }

    bool DataManager::loadMember(const std::string& username, Member*& member) {
        try {
            const std::string filePath = getMemberFilePath(username);

            if (!fileExists(filePath)) {
                return false;
            }

            const std::string csvData = FileHandler::readFromFile(filePath);
            if (csvData.empty()) {
                return false;
            }

            member = Member::fromCSVString(csvData);

            if (member) {
                logDataOperation("LOAD_MEMBER", username);
                return true;
            }

            return false;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading member " << username << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::deleteMember(const std::string& username) {
        try {
            const std::string filePath = getMemberFilePath(username);

            if (!fileExists(filePath)) {
                return false; // File does not exist, consider as success
            }

            const bool success = std::filesystem::remove(filePath);

            if (success) {
                logDataOperation("DELETE_MEMBER", username);
            }

            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error deleting member " << username << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::updateMember(const Member* member) {
        // Update is same as save
        return saveMember(member);
    }

    bool DataManager::saveAdmin(const Admin* admin) {
        if (!admin) return false;

        try {
            const std::string filePath = getAdminFilePath(admin->getUsername());
            const std::string csvData = admin->toCSVString();

            const bool success = FileHandler::writeToFile(csvData, filePath);

            if (success) {
                logDataOperation("SAVE_ADMIN", admin->getUsername());
            }

            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error saving admin " << admin->getUsername() << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::loadAdmin(const std::string& username, Admin*& admin) {
        try {
            const std::string filePath = getAdminFilePath(username);

            if (!fileExists(filePath)) {
                return false;
            }

            const std::string csvData = FileHandler::readFromFile(filePath);
            if (csvData.empty()) {
                return false;
            }

            admin = Admin::fromCSVString(csvData);

            if (admin) {
                logDataOperation("LOAD_ADMIN", username);
                return true;
            }

            return false;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading admin " << username << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::saveMotorbike(const Motorbike* motorbike) {
        if (!motorbike) return false;

        try {
            const std::string filePath = getMotorbikeFilePath(motorbike->getLicensePlate());
            const std::string csvData = motorbike->toCSVString();

            const bool success = FileHandler::writeToFile(csvData, filePath);

            if (success) {
                logDataOperation("SAVE_MOTORBIKE", motorbike->getLicensePlate());
            }

            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error saving motorbike " << motorbike->getLicensePlate()
                << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::loadMotorbike(const std::string& licensePlate, Motorbike*& motorbike) {
        try {
            const std::string filePath = getMotorbikeFilePath(licensePlate);

            if (!fileExists(filePath)) {
                return false;
            }

            const std::string csvData = FileHandler::readFromFile(filePath);
            if (csvData.empty()) {
                return false;
            }

            motorbike = Motorbike::fromCSVString(csvData);

            if (motorbike) {
                logDataOperation("LOAD_MOTORBIKE", licensePlate);
                return true;
            }

            return false;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading motorbike " << licensePlate
                << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::deleteMotorbike(const std::string& licensePlate) {
        try {
            const std::string filePath = getMotorbikeFilePath(licensePlate);

            if (!fileExists(filePath)) {
                return false; // File does not exist, consider as success
            }

            const bool success = std::filesystem::remove(filePath);

            if (success) {
                logDataOperation("DELETE_MOTORBIKE", licensePlate);
            }

            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error deleting motorbike " << licensePlate
                << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::updateMotorbike(const Motorbike* motorbike) {
        // Update is same as save
        return saveMotorbike(motorbike);
    }

    // ============================================ DATA INTEGRITY =====================================================

    bool DataManager::validateDataConsistency() {
        std::cout << "DataManager: Validating data consistency..." << std::endl;

        bool isConsistent = true;

        // Check for duplicate usernames across member and admin
        const auto members = listMembers();
        const auto admins = listAdmins();

        for (const std::string& memberName : members) {
            // Check if memberName exists in admins vector
            bool foundInAdmins = false;
            for (const std::string& adminName : admins) {
                if (memberName == adminName) {
                    foundInAdmins = true;
                    break;
                }
            }

            if (foundInAdmins) {
                std::cout << "DataManager: Warning - Username '" << memberName << "' exists in both members and admins!"
                    << std::endl;
                isConsistent = false;
            }
        }

        // Check motorbike owners exist as members
        const auto motorbikeFiles = listMotorbikes();
        for (const std::string& licensePlate : motorbikeFiles) {
            Motorbike* motorbike = nullptr;
            if (loadMotorbike(licensePlate, motorbike) && motorbike) {
                const std::string ownerUsername = motorbike->getOwnerUsername();

                // Check if owner exists in members list
                bool ownerExists = false;
                for (const std::string& memberName : members) {
                    if (memberName == ownerUsername) {
                        ownerExists = true;
                        break;
                    }
                }

                if (!ownerExists) {
                    std::cout << "DataManager: Warning - Motorbike '" << licensePlate
                        << "' has non-existent owner: " << ownerUsername << std::endl;
                    isConsistent = false;
                }

                delete motorbike; // Clean up temp motorbike object
            }
        }

        // Check for duplicate license plates
        std::vector<std::string> allLicensePlates;
        for (const std::string& licensePlate : motorbikeFiles) {
            // Check if license plate already seen
            if (std::ranges::find(allLicensePlates, licensePlate) != allLicensePlates.end()) {
                std::cout << "DataManager: Warning - Duplicate license plate found: " << licensePlate << std::endl;
                isConsistent = false;
            }
            else {
                allLicensePlates.push_back(licensePlate);
            }
        }

        // NEW: Validate motorbike data integrity
        for (const std::string& licensePlate : motorbikeFiles) {
            if (Motorbike* motorbike = nullptr; loadMotorbike(licensePlate, motorbike) && motorbike) {
                // Check if motorbike ID matches filename
                if (motorbike->getLicensePlate() != licensePlate) {
                    std::cout << "DataManager: Warning - Motorbike ID mismatch for file '" << licensePlate
                        << "' (contains ID: " << motorbike->getLicensePlate() << ")" << std::endl;
                    isConsistent = false;
                }

                // Check if city is valid
                const std::string city = motorbike->getCity();
                if (city != "HCMC" && city != "Hanoi") {
                    std::cout << "DataManager: Warning - Invalid city '" << city
                        << "' for motorbike " << licensePlate << std::endl;
                    isConsistent = false;
                }

                delete motorbike; // Clean up temporary motorbike object
            }
        }

        // TODO: Add more consistency checks when other managers are implemented
        // - Check rental participants exist
        // - Check rating references are valid
        // - Validate rental date ranges
        // - Check if motorbike rentals reference existing motorbikes

        if (isConsistent) {
            std::cout << "DataManager: Data consistency validation passed." << std::endl;
        }
        else {
            std::cout << "DataManager: Data consistency issues found!" << std::endl;
        }

        return isConsistent;
    }

    void DataManager::cleanupOrphanedFiles() {
        std::cout << "DataManager: Cleaning up orphaned files..." << std::endl;

        int cleanedFiles = 0;

        // Clea up motorbike files for non-existent owners
        try {
            const auto members = listMembers();
            const auto motorbikeFiles = listMotorbikes();

            for (const std::string& licensePlate : motorbikeFiles) {
                if (Motorbike* motorbike = nullptr; loadMotorbike(licensePlate, motorbike) && motorbike) {
                    const std::string ownerUsername = motorbike->getOwnerUsername();

                    // Check if owner exists in members list
                    bool ownerExists = false;
                    for (const std::string& memberName : members) {
                        if (memberName == ownerUsername) {
                            ownerExists = true;
                            break;
                        }
                    }

                    if (!ownerExists) {
                        std::cout << "DataManager: Removing orphaned motorbike file for non-existent owner: "
                            << licensePlate << " (owner: " << ownerUsername << ")" << std::endl;

                        if (deleteMotorbike(licensePlate)) {
                            cleanedFiles++;
                            std::cout << "DataManager: Successfully removed orphaned motorbike: " << licensePlate <<
                                std::endl;
                        }
                        else {
                            std::cout << "DataManager: Failed to remove orphaned motorbike: " << licensePlate <<
                                std::endl;
                        }
                    }

                    delete motorbike; // Clean up temp motorbike object
                }
            }

            // Clean up any corrupted motorbike files that can't be loaded
            for (const auto& entry : std::filesystem::directory_iterator(MOTORBIKES_DIR)) {
                if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                    std::string fileName = entry.path().filename().string();
                    std::string licensePlate = fileName.substr(0, fileName.size() - 4);

                    // Convert back any escaped characters
                    std::replace(licensePlate.begin(), licensePlate.end(), '_', '-');

                    Motorbike* testMotorbike = nullptr;
                    if (!loadMotorbike(licensePlate, testMotorbike)) {
                        std::cout << "DataManager: Found corrupted motorbike file: " << fileName << std::endl;

                        if (std::filesystem::remove(entry.path())) {
                            cleanedFiles++;
                            std::cout << "DataManager: Removed corrupted file: " << fileName << std::endl;
                        }
                        else {
                            std::cout << "DataManager: Failed to remove corrupted file: " << fileName << std::endl;
                        }
                    }

                    if (testMotorbike) {
                        delete testMotorbike;
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error during orphaned file cleanup: " << e.what() << std::endl;
        }

        // TODO: Clean up rental files for deleted members when RentalManager is implemented
        // - Remove rental files where renter or owner no longer exists
        // - Clean up rating files for non-existent users
        // - Remove booking files for deleted motorbikes

        std::cout << "DataManager: Cleanup completed. Removed " << cleanedFiles << " orphaned files." << std::endl;
        logDataOperation("CLEANUP", "Orphaned files cleaned: " + std::to_string(cleanedFiles));
    }

    // ============================================== SYSTEM UTILS =====================================================

    bool DataManager::createDirectoryStructure() {
        try {
            std::filesystem::create_directories(DATA_DIR);
            std::filesystem::create_directories(MEMBERS_DIR);
            std::filesystem::create_directories(ADMINS_DIR);
            std::filesystem::create_directories(MOTORBIKES_DIR);
            std::filesystem::create_directories(RENTALS_DIR);
            std::filesystem::create_directories(REQUESTS_DIR);
            std::filesystem::create_directories(RENTALS_ACTIVE_DIR);
            std::filesystem::create_directories(RATINGS_DIR);

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error creating directory structure: " << e.what() << std::endl;
            return false;
        }
    }

    std::vector<std::string> DataManager::listMembers() {
        std::vector<std::string> usernames;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(MEMBERS_DIR)) {
                if (entry.is_regular_file()) {
                    // Extract username from filename (remove .csv extension)
                    if (std::string fileName = entry.path().filename().string(); fileName.ends_with(".csv")) {
                        std::string username = fileName.substr(0, fileName.size() - 4);
                        usernames.push_back(username);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing members: " << e.what() << std::endl;
        }

        return usernames;
    }

    std::vector<std::string> DataManager::listAdmins() {
        std::vector<std::string> usernames;
        try {
            for (const auto& entry : std::filesystem::directory_iterator(ADMINS_DIR)) {
                if (entry.is_regular_file()) {
                    // Extract username from filename (remove .csv extension)
                    if (std::string fileName = entry.path().filename().string(); fileName.ends_with(".csv")) {
                        std::string username = fileName.substr(0, fileName.size() - 4);
                        usernames.push_back(username);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing admins: " << e.what() << std::endl;
        }

        return usernames;
    }

    std::vector<std::string> DataManager::listMotorbikes() {
        std::vector<std::string> licensePlates;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(MOTORBIKES_DIR)) {
                if (entry.is_regular_file()) {
                    if (std::string fileName = entry.path().filename().string();
                        fileName.ends_with(".csv")) {
                        std::string licensePlate = fileName.substr(0, fileName.size() - 4);
                        licensePlates.push_back(licensePlate);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing motorbikes" << e.what() << std::endl;
        }

        return licensePlates;
    }

    std::vector<std::string> DataManager::listRentalRequests() {
        std::vector<std::string> requestIds;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(REQUESTS_DIR)) {
                if (entry.is_regular_file()) {
                    if (std::string fileName = entry.path().filename().string(); fileName.ends_with(".csv")) {
                        std::string requestId = fileName.substr(0, fileName.length() - 4);
                        requestIds.push_back(requestId);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing rental requests: " << e.what() << std::endl;
        }

        return requestIds;
    }

    std::vector<std::string> DataManager::listRentals() {
        std::vector<std::string> rentalIds;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(RENTALS_ACTIVE_DIR)) {
                if (entry.is_regular_file()) {
                    if (std::string fileName = entry.path().filename().string(); fileName.ends_with(".csv")) {
                        std::string rentalId = fileName.substr(0, fileName.length() - 4);
                        rentalIds.push_back(rentalId);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing rentals: " << e.what() << std::endl;
        }

        return rentalIds;
    }

    std::vector<std::string> DataManager::listRatings() {
        std::vector<std::string> ratingIds;

        try {
            for (const auto& entry : std::filesystem::directory_iterator(RATINGS_DIR)) {
                if (entry.is_regular_file()) {
                    std::string fileName = entry.path().filename().string();
                    if (fileName.ends_with(".csv")) {
                        std::string ratingId = fileName.substr(0, fileName.length() - 4);
                        ratingIds.push_back(ratingId);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing ratings: " << e.what() << std::endl;
        }

        return ratingIds;
    }

    // ================================ CONVENIENCE METHODS FOR APPLICATION TO USE =====================================

    // High-level motorbike operations for Application
    bool DataManager::registerAndSaveMotorbike(const MotorbikeRegistrationData& registrationData) {
        if (motorbikeManager->registerMotorbike(registrationData)) {
            if (const Motorbike* newBike = motorbikeManager->getMotorbikeByLicensePlate(registrationData.licensePlate);
                newBike && saveMotorbike(newBike)) {
                std::cout << "DataManager: Motorbike '" << registrationData.licensePlate
                    << "' registered and saved successfully." << std::endl;
                return true;
            }
            std::cout << "DataManager: Failed to save registered motorbike '" << registrationData.licensePlate
                << "'." << std::endl;
            return false;
        }
        return false;
    }

    bool DataManager::listAndSaveMotorbike(const std::string& licensePlate, const MotorbikeListingData& listingData) {
        if (motorbikeManager->listMotorbike(licensePlate, listingData)) {
            if (const Motorbike* bike = motorbikeManager->getMotorbikeByLicensePlate(licensePlate);
                bike && updateMotorbike(bike)) {
                std::cout << "DataManager: Motorbike '" << licensePlate
                    << "' listed and saved successfully." << std::endl;
                return true;
            }
            std::cout << "DataManager: Failed to save listed motorbike '" << licensePlate
                << "'." << std::endl;
            return false;
        }
        return false;
    }

    bool DataManager::unlistAndSaveMotorbike(const std::string& licensePlate) {
        if (motorbikeManager->unlistMotorbike(licensePlate)) {
            if (const Motorbike* bike = motorbikeManager->getMotorbikeByLicensePlate(licensePlate);
                bike && updateMotorbike(bike)) {
                std::cout << "DataManager: Motorbike unlisted and saved successfully." << std::endl;
                return true;
            }
            std::cout << "DataManager: Failed to save unlisted motorbike." << std::endl;
            return false;
        }
        return false;
    }

    bool DataManager::saveRentalRequest(const RentalRequest* request) {
        if (!request) return false;

        try {
            const std::string filePath = getRequestFilePath(request->getRequestId());
            const std::string csvData = request->toCSVString();
            const bool success = FileHandler::writeToFile(csvData, filePath);

            if (success) {
                logDataOperation("SAVE_REQUEST", request->getRequestId());
            }
            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error saving request " << request->getRequestId() << ": " << e.what() <<
                std::endl;
            return false;
        }
    }

    bool DataManager::loadRentalRequest(const std::string& requestId, RentalRequest*& request) {
        try {
            const std::string filePath = getRequestFilePath(requestId);

            if (!fileExists(filePath)) {
                return false;
            }

            const std::string csvData = FileHandler::readFromFile(filePath);
            if (csvData.empty()) {
                return false;
            }

            request = RentalRequest::fromCSVString(csvData);

            if (request) {
                logDataOperation("LOAD_REQUEST", requestId);
                return true;
            }
            return false;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading request " << requestId << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::saveRental(const Rental* rental) {
        if (!rental) return false;

        try {
            const std::string filePath = getRentalFilePath(rental->getRentalId());
            const std::string csvData = rental->toCSVString();
            const bool success = FileHandler::writeToFile(csvData, filePath);

            if (success) {
                logDataOperation("SAVE_RENTAL", rental->getRentalId());
            }
            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error saving rental " << rental->getRentalId() << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::loadRental(const std::string& rentalId, Rental*& rental) {
        try {
            const std::string filePath = getRentalFilePath(rentalId);

            if (!fileExists(filePath)) {
                return false;
            }

            const std::string csvData = FileHandler::readFromFile(filePath);
            if (csvData.empty()) {
                return false;
            }

            rental = Rental::fromCSVString(csvData);

            if (rental) {
                logDataOperation("LOAD_RENTAL", rentalId);
                return true;
            }
            return false;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading rental " << rentalId << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::saveRating(const Rating* rating) {
        if (!rating) return false;

        try {
            const std::string filePath = getRatingFilePath(rating->getRatingId());
            const std::string csvData = rating->toCSVString();
            const bool success = FileHandler::writeToFile(csvData, filePath);

            if (success) {
                logDataOperation("SAVE_RATING", rating->getRatingId());
            }
            return success;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error saving rating " << rating->getRatingId() << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::loadRating(const std::string& ratingId, Rating*& rating) {
        try {
            const std::string filePath = getRatingFilePath(ratingId);

            if (!fileExists(filePath)) {
                return false;
            }

            const std::string csvData = FileHandler::readFromFile(filePath);
            if (csvData.empty()) {
                return false;
            }

            rating = Rating::fromCSVString(csvData);

            if (rating) {
                logDataOperation("LOAD_RATING", ratingId);
                return true;
            }
            return false;
        }
        catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading rating " << ratingId << ": " << e.what() << std::endl;
            return false;
        }
    }

    // ========================================== PRIVATE HELPER METHODS ===============================================

    std::string DataManager::getMemberFilePath(const std::string& username) {
        return MEMBERS_DIR + username + ".csv";
    }

    std::string DataManager::getAdminFilePath(const std::string& username) {
        return ADMINS_DIR + username + ".csv";
    }

    std::string DataManager::getMotorbikeFilePath(const std::string& licensePlate) {
        // Replace invalid filename characters in license plate
        std::string safePlate = licensePlate;
        std::replace(safePlate.begin(), safePlate.end(), '/', '_');
        std::replace(safePlate.begin(), safePlate.end(), '-', '_');
        std::replace(safePlate.begin(), safePlate.end(), ' ', '_');
        return MOTORBIKES_DIR + licensePlate + ".csv";
    }

    std::string DataManager::getRequestFilePath(const std::string& requestId) {
        return REQUESTS_DIR + requestId + ".csv";
    }

    std::string DataManager::getRentalFilePath(const std::string& rentalId) {
        return RENTALS_ACTIVE_DIR + rentalId + ".csv";
    }

    std::string DataManager::getRatingFilePath(const std::string& ratingId) {
        return RATINGS_DIR + ratingId + ".csv";
    }

    bool DataManager::fileExists(const std::string& filename) {
        return std::filesystem::exists(filename);
    }

    void DataManager::logDataOperation(const std::string& operation, const std::string& details) {
        // Simple logging - could be enhanced to log file
        std::cout << "DataManager LOG: " << operation << " - " << details << std::endl;
    }
}
