//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#include "DataManager.h"

#include <filesystem>
#include <iostream>
#include <bits/ostream.tcc>

#include "FileHandler.h"

namespace EMotoRental
{
    // Static member definitions
    const std::string DataManager::DATA_DIR = "EMotoRental/data/";
    const std::string DataManager::MEMBERS_DIR = "EMotoRental/data/members/";
    const std::string DataManager::ADMINS_DIR = "EMotoRental/data/admins/";
    const std::string DataManager::MOTORBIKES_DIR = "EMotoRental/data/motorbikes/";
    const std::string DataManager::RENTALS_DIR = "EMotoRental/data/rentals/";

    DataManager::DataManager() {
        authManager = nullptr;
        motorbikeManager = nullptr;

        if (initializeSystem()) {
            authManager = new AuthManager();
            motorbikeManager = new MotorbikeManager();
            std::cout << "DataManager: System initialized successfully." << std::endl;
        } else {
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

    // ============================================= DATA LIFECYCLE ====================================================

    bool DataManager::initializeSystem() {
        std::cout << "DataManager: Initializing system." << std::endl;

        // Create directory structure
        if (!createDirectoryStructure()) {
            std::cerr << "DataManager: Failed to create directory structure." << std::endl;
            return false;
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

        bool success = true;

        // Load all members
        const auto memberFiles = listMembers();
        int loadedMembers = 0;

        for (const std::string& username : memberFiles) {
            if (Member* member = nullptr; loadMember(username, member)) {
                authManager->addMember(member);
                loadedMembers++;
            } else {
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
            } else {
                std::cout << "DataManager: Warning - Failed to load admin: " << username << std::endl;
                success = false;
            }
        }

        std::cout << "DataManager: Loaded " << loadedMembers << " members and "
                          << loadedAdmins << " admins." << std::endl;

        // Validate data consistency
        if (success) {
            success = validateDataConsistency();
        }

        logDataOperation("LOAD_ALL", "Members: " + std::to_string(loadedMembers) +
                        ", Admins: " + std::to_string(loadedAdmins));

        return success;
    }

    bool DataManager::saveAllData() {
        std::cout << "DataManager: Saving all system data..." << std::endl;

        if (!authManager) {
            std::cerr << "DataManager: AuthManager not initialized!" << std::endl;
            return false;
        }

        bool success = true;
        int savedMembers = 0, savedAdmins = 0;

        // Save all members
        const auto members = authManager->getAllMembers();
        for (const auto& member : members) {
            if (saveMember(member)) {
                savedMembers++;
            } else {
                std::cout << "DataManager: Warning - Failed to save member: " << member->getUsername() << std::endl;
                success = false;
            }
        }

        // Save all admins
        for (const auto admins = authManager->getAllAdmins(); Admin* admin : admins) {
            if (saveAdmin(admin)) {
                savedAdmins++;
            } else {
                std::cout << "DataManager: Warning - Failed to save admin: " << admin->getUsername() << std::endl;
                success = false;
            }
        }

        // Save all motorbikes
        if (motorbikeManager) {
            const auto motorbikes = motorbikeManager->getAllMotorbikes();
            for (const auto& motorbike : motorbikes) {

            }
        }

        std::cout << "DataManager: Saved " << savedMembers << " members and " << savedAdmins << " admins." << std::endl;

        logDataOperation("SAVE_ALL", "Members: " + std::to_string(savedMembers) + ", Admins: "
            + std::to_string(savedAdmins));

        return success;
    }

    void DataManager::shutdown() {
        std::cout << "DataManager: Shutting down system..." << std::endl;

        // Save all data before cleanup
        saveAllData();

        // Cleanup managers
        if (authManager) {
            delete authManager;
            authManager = nullptr;
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

        } catch (const std::exception& e) {
            std::cerr << "DataManager: Error in saving member " << member->getUsername() << ":" << e.what() << std::endl;
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

        } catch (const std::exception& e) {
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

        } catch (const std::exception& e) {
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

        } catch (const std::exception& e) {
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

        } catch (const std::exception& e) {
            std::cerr << "DataManager: Error loading admin " << username << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool DataManager::saveMotorbike(const Motorbike* motorbike) {
        if (!motorbike) return false;

        try {
            const std::string filePath =
        }
    }


    // ============================================ DATA INTEGRITY =====================================================

    bool DataManager::validateDataConsistency() {
        std::cout << "DataManager: Validating data consistency..." << std::endl;

        bool isConsistent = true;

        // Check for duplicate usernames across member and admin
        auto members = listMembers();
        auto admins = listAdmins();

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
                std::cout << "DataManager: Warning - Username '" << memberName << "' exists in both members and admins!" << std::endl;
                isConsistent = false;
            }
        }

        // TODO: Add more consistency checks when other managers are implemented
        // - Check motorbike owners exist as members
        // - Check rental participants exist
        // - Check rating references are valid

        if (isConsistent) {
            std::cout << "DataManager: Data consistency validation passed." << std::endl;
        } else {
            std::cout << "DataManager: Data consistency issues found!" << std::endl;
        }

        return isConsistent;
    }

    void DataManager::cleanupOrphanedFiles() {
        std::cout << "DataManager: Cleaning up orphaned files..." << std::endl;

        // TODO: Implement cleanup logic
        // - Remove member files for deleted accounts
        // - Remove motorbike files for non-existent owners
        // - Remove rental files for deleted members

        logDataOperation("CLEANUP", "Orphaned files cleaned");
    }

    // ============================================== SYSTEM UTILS =====================================================

    bool DataManager::createDirectoryStructure() {
        try {
            std::filesystem::create_directories(DATA_DIR);
            std::filesystem::create_directories(MEMBERS_DIR);
            std::filesystem::create_directories(ADMINS_DIR);
            std::filesystem::create_directories(MOTORBIKES_DIR);
            std::filesystem::create_directories(RENTALS_DIR);

            return true;

        } catch (const std::exception& e) {
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
        } catch (const std::exception& e) {
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
        } catch (const std::exception& e) {
            std::cerr << "DataManager: Error listing admins: " << e.what() << std::endl;
        }

        return usernames;
    }

    // ========================================== PRIVATE HELPER METHODS ===============================================

    std::string DataManager::getMemberFilePath(const std::string& username) {
        return MEMBERS_DIR + username + ".csv";
    }

    std::string DataManager::getAdminFilePath(const std::string& username) {
        return ADMINS_DIR + username + ".csv";
    }

    bool DataManager::fileExists(const std::string& filename) {
        return std::filesystem::exists(filename);
    }

    void DataManager::logDataOperation(const std::string& operation, const std::string& details) {
        // Simple logging - could be enhanced to log file
        std::cout << "DataManager LOG: " << operation << " - " << details << std::endl;
    }

}
