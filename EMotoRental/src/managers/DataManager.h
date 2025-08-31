//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#pragma once
#include "AuthManager.h"
#include "../utils/FileHandler.h"
#include <string>
#include <vector>

namespace EMotoRental
{
    class DataManager
    {
    private:
        AuthManager* authManager;
        // MotorbikeManager* motorbikeManager;  // TODO: Add when implemented
        // RentalManager* rentalManager; // TODO: Add when implemented

        // File paths
        static const std::string DATA_DIR;
        static const std::string MEMBERS_DIR;
        static const std::string ADMINS_DIR;
        static const std::string MOTORBIKES_DIR;
        static const std::string RENTALS_DIR;

    public:
        DataManager();
        ~DataManager();

        // Manager access
        AuthManager* getAuthManager() const;
        // MotorbikeManager* getMotorbikeManager() const;  // TODO
        // RentalManager* getRentalManager() const; // TODO

        // Data lifecycle
        bool initializeSystem();
        bool loadAllData();
        bool saveAllData();
        bool shutdown();

        // Individual entity operations (efficient updates)
        bool saveMember(Member* member);
        bool loadMember(const std::string& username, Member*& member);
        bool deleteMember(const std::string& username);
        bool updateMember(Member* member);

        bool saveAdmin(Admin admin);
        bool loadAdmin(const std::string& username, Admin*& admin);

        // Data integrity
        bool validateDataConsistency();
        void cleanupOrphanedFiles();

        // System utils
        bool createDirectoryStructure();
        std::vector<std::string> listMembers() const;
        std::vector<std::string> listAdmins() const;

    private:
        // Helper methods
        std::string getMemberFilePath(const std::string& username) const;
        std::string getAdminFilePath(const std::string& username) const;
        bool fileExists(const std::string& filename) const;
        void logDataOperation(const std::string& operation, const std::string details) const;
    };
}
