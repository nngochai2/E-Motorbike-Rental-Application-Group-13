//
// Created by Nguyen Ngoc Hai on 9/18/2025.
//

#include "DataInitializer.h"
#include "../managers/DataManager.h"
#include "FileHandler.h"
#include <iostream>

namespace EMotoRental {

    bool DataInitializer::initializeDefaultData() {
        std::cout << "DataInitializer: Setting up default system data..." << std::endl;

        bool success = true;
        success &= createDefaultAdmins();
        success &= createDefaultMembers();
        success &= createDefaultMotorbikes();
        success &= createDefaultRentalsActive();
        success &= createDefaultRentalsRatings();
        success &= createDefaultRentalsRequests();

        if (success) {
            std::cout << "DataInitializer: Default data setup completed successfully!" << std::endl;
            std::cout << "DataInitializer: System is ready with test data for demonstration." << std::endl;
        } else {
            std::cerr << "DataInitializer: Some default data could not be created." << std::endl;
        }

        return success;
    }

    bool DataInitializer::isFirstRun() {
        // Check if any data files exist
        return DataManager::listMembers().empty() &&
               DataManager::listAdmins().empty() &&
               DataManager::listMotorbikes().empty();
    }

    bool DataInitializer::createDefaultAdmins() {
        std::cout << "DataInitializer: Creating default admin accounts..." << std::endl;

        // Ensure directory exists
        if (!FileHandler::createDirectory("EMotoRental/data/admins")) {
            std::cerr << "DataInitializer: Failed to create admins directory." << std::endl;
            return false;
        }

        auto adminData = getDefaultAdminData();

        for (const auto& csvData : adminData) {
            std::string fileName = "EMotoRental/data/admins/" + extractUsernameFromCSV(csvData) + ".csv";
            if (!FileHandler::writeToFile(csvData, fileName)) {
                std::cerr << "DataInitializer: Failed to create admin file: " << fileName << std::endl;
                return false;
            }
        }

        std::cout << "DataInitializer: Created " << adminData.size() << " admin accounts." << std::endl;
        return true;
    }

    bool DataInitializer::createDefaultMembers() {
        std::cout << "DataInitializer: Creating default member accounts..." << std::endl;

        // Ensure directory exists
        if (!FileHandler::createDirectory("EMotoRental/data/members")) {
            std::cerr << "DataInitializer: Failed to create members directory." << std::endl;
            return false;
        }

        auto memberData = getDefaultMemberData();

        for (const auto& csvData : memberData) {
            std::string fileName = "EMotoRental/data/members/" + extractUsernameFromCSV(csvData) + ".csv";
            if (!FileHandler::writeToFile(csvData, fileName)) {
                std::cerr << "DataInitializer: Failed to create member file: " << fileName << std::endl;
                return false;
            }
        }

        std::cout << "DataInitializer: Created " << memberData.size() << " member accounts." << std::endl;
        return true;
    }

    bool DataInitializer::createDefaultMotorbikes() {
        std::cout << "DataInitializer: Creating default motorbike listings..." << std::endl;

        // Ensure directory exists
        if (!FileHandler::createDirectory("EMotoRental/data/motorbikes")) {
            std::cerr << "DataInitializer: Failed to create motorbikes directory." << std::endl;
            return false;
        }

        auto motorbikeData = getDefaultMotorbikeData();

        for (const auto& csvData : motorbikeData) {
            std::string bikeId = extractBikeIdFromCSV(csvData);
            std::string fileName = "EMotoRental/data/motorbikes/" + bikeId + ".csv";
            if (!FileHandler::writeToFile(csvData, fileName)) {
                std::cerr << "DataInitializer: Failed to create motorbike file: " << fileName << std::endl;
                return false;
            }
        }

        std::cout << "DataInitializer: Created " << motorbikeData.size() << " motorbike listings." << std::endl;
        return true;
    }

    bool DataInitializer::createDefaultRentalsActive() {
        std::cout << "DataInitializer: Creating default active rentals..." << std::endl;

        // Ensure directory exists
        if (!FileHandler::createDirectory("EMotoRental/data/rentals")) {
            std::cerr << "DataInitializer: Failed to create rentals directory." << std::endl;
            return false;
        }
        if (!FileHandler::createDirectory("EMotoRental/data/rentals/active")) {
            std::cerr << "DataInitializer: Failed to create rentals/active directory." << std::endl;
            return false;
        }

        auto rentalData = getDefaultActiveRentalData();

        for (const auto& csvData : rentalData) {
            std::string rentalId = extractRentalIdFromCSV(csvData);
            std::string fileName = "EMotoRental/data/rentals/active/" + rentalId + ".csv";
            if (!FileHandler::writeToFile(csvData, fileName)) {
                std::cerr << "DataInitializer: Failed to create active rental file: " << fileName << std::endl;
                return false;
            }
        }

        std::cout << "DataInitializer: Created " << rentalData.size() << " active rentals." << std::endl;
        return true;
    }

    bool DataInitializer::createDefaultRentalsRatings() {
        std::cout << "DataInitializer: Creating default rental ratings..." << std::endl;

        // Ensure directory exists
        if (!FileHandler::createDirectory("EMotoRental/data/rentals/ratings")) {
            std::cerr << "DataInitializer: Failed to create rentals/ratings directory." << std::endl;
            return false;
        }

        auto ratingData = getDefaultRatingData();

        for (const auto& csvData : ratingData) {
            std::string ratingId = extractRatingIdFromCSV(csvData);
            std::string fileName = "EMotoRental/data/rentals/ratings/" + ratingId + ".csv";
            if (!FileHandler::writeToFile(csvData, fileName)) {
                std::cerr << "DataInitializer: Failed to create rating file: " << fileName << std::endl;
                return false;
            }
        }

        std::cout << "DataInitializer: Created " << ratingData.size() << " ratings." << std::endl;
        return true;
    }

    bool DataInitializer::createDefaultRentalsRequests() {
        std::cout << "DataInitializer: Creating default rental requests..." << std::endl;

        // Ensure directory exists
        if (!FileHandler::createDirectory("EMotoRental/data/rentals/requests")) {
            std::cerr << "DataInitializer: Failed to create rentals/requests directory." << std::endl;
            return false;
        }

        auto requestData = getDefaultRequestData();

        for (const auto& csvData : requestData) {
            std::string requestId = extractRequestIdFromCSV(csvData);
            std::string fileName = "EMotoRental/data/rentals/requests/" + requestId + ".csv";
            if (!FileHandler::writeToFile(csvData, fileName)) {
                std::cerr << "DataInitializer: Failed to create request file: " << fileName << std::endl;
                return false;
            }
        }

        std::cout << "DataInitializer: Created " << requestData.size() << " rental requests." << std::endl;
        return true;
    }

    // ============================================ DEFAULT DATA GENERATORS ============================================

    std::vector<std::string> DataInitializer::getDefaultAdminData() {
        return {
            // Format: username,password,fullName
            "admin01,adminpass01,Michael Scott",
            "admin02,adminpass02,Dwight Schrute",
            "admin03,adminpass03,Jim Halpert"
        };
    }

    std::vector<std::string> DataInitializer::getDefaultMemberData() {
        return {
            // Format: username,password,fullName,email,phone,idNumber,licenseNumber,hasValidLicense,isVerified,creditPoints,renterRating,ownedMotorbikeId

            // Primary test members with motorbikes
            "alice01,alicepass,Alice Smith,alice.smith@email.com,0123456789,ID001,LIC001,1,1,500,4.5,MB001",
            "bob22,bobpass22,Bob Johnson,bob.johnson@email.com,0987654321,ID002,LIC002,1,1,300,4.2,MB002",
            "charlie33,charliepass33,Charlie Brown,charlie.brown@email.com,0456789123,ID003,LIC003,1,0,450,3.8,MB003",
            "diana44,dianapass44,Diana Prince,diana.prince@email.com,0321654987,ID004,LIC004,1,1,600,4.7,MB004",

            // Members without motorbikes (renters only) - use "NONE" placeholder
            "eve55,evepass55,Eve Wilson,eve.wilson@email.com,0789123456,ID005,LIC005,1,1,250,3.9,NONE",
            "frank66,frankpass66,Frank Miller,frank.miller@email.com,0654321987,ID006,LIC006,1,0,180,3.2,NONE",
            "grace77,gracepass77,Grace Kelly,grace.kelly@email.com,0147258369,ID007,NONE,0,1,120,3.0,NONE",

            // Members with different license states and credit levels
            "henry88,henrypass88,Henry Ford,henry.ford@email.com,0963852741,ID008,LIC008,1,1,750,4.8,MB005",
            "iris99,irispass99,Iris Watson,iris.watson@email.com,0852741963,ID009,LIC009,1,1,80,3.1,NONE",
            "jack00,jackpass00,Jack Sparrow,jack.sparrow@email.com,0741852963,ID010,NONE,0,0,50,2.8,NONE"
        };
    }

    std::vector<std::string> DataInitializer::getDefaultMotorbikeData() {
        return {
            // Format: id,brand,model,color,engineSize,year,location,dailyRate,ownerRating,motorbikeRating,isAvailable,ownerId,availableFrom,availableTo

            // Alice's motorbike (currently rented to bob22)
            "MB001,Honda,CBR500R,Red,500,2020,Hanoi,50,4.5,4.0,0,alice01,01/09/2025,31/12/2025",

            // Bob's motorbike (available for rent)
            "MB002,Yamaha,YZF-R3,Blue,321,2021,HCMC,45,4.2,4.3,1,bob22,10/09/2025,30/11/2025",

            // Charlie's motorbike (available, lower engine size for license-restricted members)
            "MB003,VinFast,Klara S,White,50,2022,Hanoi,35,3.8,4.1,1,charlie33,15/09/2025,31/10/2025",

            // Diana's motorbike (available, premium option)
            "MB004,Kawasaki,Ninja 400,Green,399,2021,Hanoi,55,4.7,4.6,1,diana44,18/09/2025,15/12/2025",

            // Henry's motorbike (available, high-end)
            "MB005,BMW,C400X,Black,350,2023,HCMC,60,4.8,4.8,1,henry88,20/09/2025,20/12/2025"
        };
    }

    std::vector<std::string> DataInitializer::getDefaultActiveRentalData() {
        return {
            // Format: rentalId,renterId,ownerId,motorbikeId,startDate,endDate,totalCost,status

            // Bob is currently renting Alice's motorbike (ongoing rental)
            "RNT001,bob22,alice01,MB001,14/09/2025,16/09/2025,100,ACTIVE",

            // Past rental that's completed (for rating demonstration)
            "RNT002,eve55,bob22,MB002,10/09/2025,12/09/2025,90,COMPLETED"
        };
    }

    std::vector<std::string> DataInitializer::getDefaultRatingData() {
        return {
            // Format: ratingId,fromUserId,toUserId,rating,comment,date,rentalId,ratingType
            // Note: Use enum values RENTER_RATING and MOTORBIKE_RATING

            // Rating from the completed rental RNT002
            "RAT001,eve55,bob22,5,Excellent motorbike and very responsive owner!,12/09/2025,RNT002,MOTORBIKE_RATING",
            "RAT002,bob22,eve55,4,Great renter took good care of the bike,12/09/2025,RNT002,RENTER_RATING",

            // Some historical ratings to establish member credibility
            "RAT003,frank66,alice01,5,Amazing bike smooth ride!,08/09/2025,RNT003,MOTORBIKE_RATING",
            "RAT004,alice01,frank66,4,Reliable renter returned on time,08/09/2025,RNT003,RENTER_RATING",

            "RAT005,iris99,diana44,5,Perfect motorbike for city riding,05/09/2025,RNT004,MOTORBIKE_RATING",
            "RAT006,diana44,iris99,3,Good renter but returned bike with low battery,05/09/2025,RNT004,RENTER_RATING"
        };
    }

    std::vector<std::string> DataInitializer::getDefaultRequestData() {
        return {
            // Format: requestId,ownerId,motorbikeId,startDate,endDate,totalCost,status,requestDate

            // Pending request for Alice's motorbike (will conflict with current rental)
            "REQ001,alice01,MB001,20/09/2025,22/09/2025,100,PENDING,16/09/2025",

            // Pending requests for Diana's available motorbike (multiple requests to choose from)
            "REQ002,diana44,MB004,25/09/2025,27/09/2025,110,PENDING,17/09/2025",
            "REQ003,diana44,MB004,26/09/2025,28/09/2025,110,PENDING,17/09/2025",

            // Pending request for Charlie's small bike (accessible to users without license)
            "REQ004,charlie33,MB003,22/09/2025,24/09/2025,70,PENDING,18/09/2025",

            // Rejected request (for demonstration)
            "REQ005,bob22,MB002,19/09/2025,21/09/2025,90,REJECTED,16/09/2025"
        };
    }

    // ============================================ UTILITY METHODS ============================================

    std::string DataInitializer::extractUsernameFromCSV(const std::string& csvData) {
        size_t firstComma = csvData.find(',');
        if (firstComma != std::string::npos) {
            return csvData.substr(0, firstComma);
        }
        return "unknown";
    }

    std::string DataInitializer::extractBikeIdFromCSV(const std::string& csvData) {
        size_t firstComma = csvData.find(',');
        if (firstComma != std::string::npos) {
            return csvData.substr(0, firstComma);
        }
        return "unknown";
    }

    std::string DataInitializer::extractRentalIdFromCSV(const std::string& csvData) {
        size_t firstComma = csvData.find(',');
        if (firstComma != std::string::npos) {
            return csvData.substr(0, firstComma);
        }
        return "unknown";
    }

    std::string DataInitializer::extractRatingIdFromCSV(const std::string& csvData) {
        size_t firstComma = csvData.find(',');
        if (firstComma != std::string::npos) {
            return csvData.substr(0, firstComma);
        }
        return "unknown";
    }

    std::string DataInitializer::extractRequestIdFromCSV(const std::string& csvData) {
        size_t firstComma = csvData.find(',');
        if (firstComma != std::string::npos) {
            return csvData.substr(0, firstComma);
        }
        return "unknown";
    }
}
