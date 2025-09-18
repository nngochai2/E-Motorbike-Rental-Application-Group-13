//
// Created by Nguyen Ngoc Hai on 9/18/2025.
//

#pragma once
#include <string>
#include <vector>

namespace EMotoRental {
    /**
     * DataInitializer creates default placeholder data for the E-Motorbike Rental System
     * This ensures the application has realistic test data for demonstration purposes
     * and can run immediately after installation without requiring manual data entry.
     */
    class DataInitializer {
    public:
        /**
         * Initializes all default data for the system
         * @return true if all data was created successfully, false otherwise
         */
        static bool initializeDefaultData();

        /**
         * Checks if this is the first run of the application (no data files exist)
         * @return true if no data files are found, false otherwise
         */
        static bool isFirstRun();

    private:
        // Data creation methods
        static bool createDefaultAdmins();
        static bool createDefaultMembers();
        static bool createDefaultMotorbikes();
        static bool createDefaultRentalsActive();
        static bool createDefaultRentalsRatings();
        static bool createDefaultRentalsRequests();

        // Placeholder data generators
        static std::vector<std::string> getDefaultAdminData();
        static std::vector<std::string> getDefaultMemberData();
        static std::vector<std::string> getDefaultMotorbikeData();
        static std::vector<std::string> getDefaultActiveRentalData();
        static std::vector<std::string> getDefaultRatingData();
        static std::vector<std::string> getDefaultRequestData();

        // Utility methods for extracting IDs from CSV data
        static std::string extractUsernameFromCSV(const std::string& csvData);
        static std::string extractBikeIdFromCSV(const std::string& csvData);
        static std::string extractRentalIdFromCSV(const std::string& csvData);
        static std::string extractRatingIdFromCSV(const std::string& csvData);
        static std::string extractRequestIdFromCSV(const std::string& csvData);
    };
}