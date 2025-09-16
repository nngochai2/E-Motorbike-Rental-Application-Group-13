//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#pragma once
#include "AuthManager.h"
#include "MotorbikeManager.h"
#include "RentalManager.h"
#include <string>
#include <vector>

namespace EMotoRental
{
    class DataManager
    {
    private:
        AuthManager* authManager;
        MotorbikeManager* motorbikeManager;
        RentalManager* rentalManager;

        // File paths
        static const std::string DATA_DIR;
        static const std::string MEMBERS_DIR;
        static const std::string ADMINS_DIR;
        static const std::string MOTORBIKES_DIR;
        static const std::string RENTALS_DIR;
        static const std::string REQUESTS_DIR;
        static const std::string RENTALS_ACTIVE_DIR;
        static const std::string RATINGS_DIR;

    public:
        DataManager();
        ~DataManager();

        // Manager access
        AuthManager* getAuthManager() const;
        MotorbikeManager* getMotorbikeManager() const;
        RentalManager* getRentalManager() const;

        // Data lifecycle
        static bool initializeSystem();
        bool loadAllData() const;
        bool saveAllData() const;
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
        static bool deleteMotorbike(const std::string& licensePlate);
        static bool updateMotorbike(const Motorbike* motorbike);

        // Data integrity
        static bool validateDataConsistency();
        static void cleanupOrphanedFiles();

        // System utils
        static bool createDirectoryStructure();
        static std::vector<std::string> listMembers();
        static std::vector<std::string> listAdmins();
        static std::vector<std::string> listMotorbikes();

        static std::vector<std::string> listRentalRequests();
        static std::vector<std::string> listRentals();
        static std::vector<std::string> listRatings();

        // Convenience methods
        bool registerAndSaveMotorbike(const MotorbikeRegistrationData& registrationData);
        bool listAndSaveMotorbike(const std::string& licensePlate, const MotorbikeListingData& listingData);
        bool unlistAndSaveMotorbike(const std::string& licensePlate);

    private:
        // Individual file operations for rental entities
        static bool saveRentalRequest(const RentalRequest* request);
        static bool loadRentalRequest(const std::string& requestId, RentalRequest*& request);
        static bool saveRental(const Rental* rental);
        static bool loadRental(const std::string& rentalId, Rental*& rental);
        static bool saveRating(const Rating* rating);
        static bool loadRating(const std::string& ratingId, Rating*& rating);

        // File path helper methods
        static std::string getMemberFilePath(const std::string& username);
        static std::string getAdminFilePath(const std::string& username);
        static std::string getMotorbikeFilePath(const std::string& licensePlate);

        static std::string getRequestFilePath(const std::string& requestId);
        static std::string getRentalFilePath(const std::string& rentalId);
        static std::string getRatingFilePath(const std::string& ratingId);

        static bool fileExists(const std::string& filename);
        static void logDataOperation(const std::string& operation, const std::string& details);
    };
}
