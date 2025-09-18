//
// Created by Tran Dinh Dong on 9/07/2025.
//

#pragma once

#include <vector>
#include <string>
#include "../entities/RentalRequest.h"
#include "../entities/Rental.h"
#include "../entities/Rating.h"
#include "../utils/DateUtil.h"

namespace EMotoRental
{
    // Forward declarations
    class Member;
    class Motorbike;
    class MotorbikeManager;
    class AuthManager;

    // Structure for rental request data
    struct RentalRequestData
    {
        std::string renterUsername;
        std::string motorbikeLicensePlate;
        DateUtil::TimePoint startDate;
        DateUtil::TimePoint endDate;

        RentalRequestData(const std::string& renter, const std::string& motorbikeId,
                          const DateUtil::TimePoint& start, const DateUtil::TimePoint& end)
            : renterUsername(renter), motorbikeLicensePlate(motorbikeId), startDate(start), endDate(end) {}
    };

    class RentalManager
    {
    private:
        std::vector<RentalRequest*> requests;
        std::vector<Rental*> rentals;
        std::vector<Rating*> ratings;
        std::vector<std::string> recentlyCompletedRentals; // Store rental IDs

        // Helper methods
        void cleanupMemory();

    public:
        // Constructor
        RentalManager();

        // Destructor
        ~RentalManager();

        // Rental request management
        bool createRentalRequest(const RentalRequestData& requestData, const MotorbikeManager* motorbikeManager,
                                 const AuthManager* authManager);
        bool approveRequest(const std::string& requestId,
                            MotorbikeManager* motorbikeManager,
                            const AuthManager* authManager);
        void rejectRequest(const std::string& requestId);

        // Rental lifecycle management
        void completeRental(const std::string& rentalId);
        bool canCreateRental(const std::string& renterUsername,
                             const std::string& motorbikeLicensePlate,
                             const DateUtil::TimePoint& startDate,
                             const DateUtil::TimePoint& endDate) const;
        void processOverdueRental();
        std::vector<std::string> getAndClearRecentCompletions();

        // Rating system
        bool createRating(const std::string& reviewerUsername,
                          const std::string& revieweeId,
                          int stars,
                          const std::string& comment,
                          const std::string& rentalId,
                          RatingType type,
                          const MotorbikeManager* motorbikeManager,
                          const AuthManager* authManager);

        // Query methods
        std::vector<RentalRequest*> getRequestsForMotorbike(const std::string& motorbikeLicensePlate) const;
        std::vector<RentalRequest*> getRequestsFromRenter(const std::string& renterUsername) const;
        std::vector<Rental*> getActiveRentalsForMember(const std::string& username) const;
        std::vector<Rental*> getRentalHistory(const std::string& username) const;
        std::vector<Rating*> getRatingsForEntity(const std::string& entityId, RatingType type) const;

        // Rating calculations
        double calculateAverageRating(const std::string& entityId, RatingType type) const;
        int getRatingCount(const std::string& entityId, RatingType type) const;

        // Utility methods
        RentalRequest* getRequestById(const std::string& requestId) const;
        Rental* getRentalById(const std::string& rentalId) const;
        bool hasActiveRental(const std::string& renterUsername) const;
        std::vector<Rental*> getOverdueRentals() const;

        // Data management (for DataManager to use)
        void addRentalRequest(RentalRequest* request);
        void addRental(Rental* rental);
        void addRating(Rating* rating);
        std::vector<RentalRequest*> getAllRequests() const;
        std::vector<Rental*> getAllRentals() const;
        std::vector<Rating*> getAllRatings() const;

        // Display methods
        void displayRequestsForOwner(const std::string& ownerUsername,
                                     const MotorbikeManager* motorbikeManager) const;
        void displayRentalHistory(const std::string& username) const;
        void displayRatingsForEntity(const std::string& entityId, RatingType type) const;
    };
}
