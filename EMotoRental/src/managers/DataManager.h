//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#pragma once
#include "AuthManager.h"
#include "MotorbikeManager.h"
#include <string>
#include <vector>

namespace EMotoRental
{
    class DataManager
    {
    private:
        AuthManager* authManager;
        MotorbikeManager* motorbikeManager;
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
        MotorbikeManager* getMotorbikeManager() const;
        // RentalManager* getRentalManager() const; // TODO

        // Data lifecycle
        static bool initializeSystem();
        bool loadAllData();
        bool saveAllData();
        void shutdown();

        // Individual entity operations (efficient updates)
        static bool saveMember(const Member* member);
        static bool loadMember(const std::string& username, Member*& member);
        static bool deleteMember(const std::string& username);
        static bool updateMember(const Member* member);

        static bool saveAdmin(const Admin* admin);
        static bool loadAdmin(const std::string& username, Admin*& admin);

        static bool saveMotorbike(const Motorbike* motorbike);
        static bool loadMotorbike(const std::string& licensePlate, Motorbike*& motorbike);

        // Data integrity
        static bool validateDataConsistency();
        static void cleanupOrphanedFiles();

        // System utils
        static bool createDirectoryStructure();
        static std::vector<std::string> listMembers();
        static std::vector<std::string> listAdmins();
        static std::vector<std::string> listMotorbikes();

    private:
        // Helper methods
        static std::string getMemberFilePath(const std::string& username);
        static std::string getAdminFilePath(const std::string& username);
        static std::string getMotorbikeFilePath(const std::string& licensePlate);
        static bool fileExists(const std::string& filename);
        static void logDataOperation(const std::string& operation, const std::string& details);
    };
}
