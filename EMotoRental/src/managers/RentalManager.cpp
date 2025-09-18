//
// Created by Tran Dinh Dong on 9/07/2025.
//

#include "RentalManager.h"

#include <iomanip>
#include <iostream>

#include "AuthManager.h"
#include "MotorbikeManager.h"

namespace EMotoRental
{
    // ============================================ CONSTRUCTOR & DESTRUCTOR ===========================================

    RentalManager::RentalManager() {
        std::cout << "RentalManager: Initialized." << std::endl;
    }

    RentalManager::~RentalManager() {
        std::cout << "RentalManager: Cleaning up..." << std::endl;
        cleanupMemory();
    }

    void RentalManager::cleanupMemory() {
        // Clean up requests
        for (const RentalRequest* request : requests) {
            delete request;
        }
        requests.clear();

        // Clean up rentals
        for (const Rental* rental : rentals) {
            delete rental;
        }
        rentals.clear();

        // Clean up ratings
        for (const Rating* rating : ratings) {
            delete rating;
        }
        ratings.clear();

        std::cout << "RentalManager: Memory cleanup completed." << std::endl;
    }

    // =========================================== RENTAL REQUEST MANAGEMENT ===========================================

    bool RentalManager::createRentalRequest(const RentalRequestData& requestData,
                                            const MotorbikeManager* motorbikeManager,
                                            const AuthManager* authManager) {
        // Validate managers
        if (!motorbikeManager || !authManager) {
            std::cout << "Request failed: System not properly initialized." << std::endl;
            return false;
        }

        // Get renter and validate
        const Member* renter = authManager->findMemberByUsername(requestData.renterUsername);
        if (!renter) {
            std::cout << "Request failed: Renter not found." << std::endl;
            return false;
        }

        // Get motorbike and validate
        const Motorbike* motorbike = motorbikeManager->getMotorbikeByLicensePlate(requestData.motorbikeLicensePlate);
        if (!motorbike) {
            std::cout << "Request failed: Motorbike not found." << std::endl;
            return false;
        }

        // Check if motorbike is available for the requested period
        if (!motorbike->isAvailable(requestData.startDate, requestData.endDate)) {
            std::cout << "Request failed: Motorbike not available for the requested period." << std::endl;
            return false;
        }

        // Check if renter meets requirements
        if (!motorbike->meetsRequirement(*renter)) {
            std::cout << "Request failed: You don't meet the requirements for this motorbike." << std::endl;
            return false;
        }

        // Check if renter already has an active rental
        if (hasActiveRental(requestData.renterUsername)) {
            std::cout << "Request failed: You already have an active rental." << std::endl;
            return false;
        }

        // Calculate cost
        const int days = DateUtil::daysBetween(requestData.startDate, requestData.endDate);
        const double cost = motorbike->calculateCost(days);

        // Check if renter has sufficient credits
        if (renter->getCreditPoints() < cost) {
            std::cout << "Request failed: Insufficient credit points. Required: " << cost
                << " CP, Available: " << renter->getCreditPoints() << " CP" << std::endl;
            return false;
        }

        // Check for rental conflicts
        if (!canCreateRental(requestData.renterUsername, requestData.motorbikeLicensePlate,
                             requestData.startDate, requestData.endDate)) {
            std::cout << "Request failed: Schedule conflict detected." << std::endl;
            return false;
        }

        // Create rental request
        auto* newRequest = new RentalRequest(requestData.renterUsername, requestData.motorbikeLicensePlate,
                                             requestData.startDate, requestData.endDate, cost);

        requests.push_back(newRequest);

        requests.push_back(newRequest);

        std::cout << "Rental request created successfully!" << std::endl;
        std::cout << "Request ID: " << newRequest->getRequestId() << std::endl;
        std::cout << "Cost: " << std::fixed << std::setprecision(2) << cost << " CP" << std::endl;
        std::cout << "The motorbike owner will review your request." << std::endl;

        return true;
    }

    bool RentalManager::approveRequest(const std::string& requestId, MotorbikeManager* motorbikeManager,
                                       const AuthManager* authManager) {
        // Find the request
        RentalRequest* request = getRequestById(requestId);
        if (!request) {
            std::cout << "Approval failed: Request not found." << std::endl;
            return false;
        }

        if (request->getStatus() != RequestStatus::PENDING) {
            std::cout << "Approval failed: Request is not pending." << std::endl;
            return false;
        }

        // Get renter and motorbike
        Member* renter = authManager->findMemberByUsername(request->getRenterUsername());
        const Motorbike* motorbike = motorbikeManager->getMotorbikeByLicensePlate(request->getMotorbikeLicensePlate());

        if (!renter || !motorbike) {
            std::cout << "Approval failed: Renter or motorbike not found." << std::endl;
            return false;
        }

        // Deduct credits from renter
        if (!renter->deductCredits(request->getEstimatedCost())) {
            std::cout << "Approval failed: Could not deduct credits." << std::endl;
            return false;
        }

        // Approve the request
        request->approve();

        // Create active rental
        auto* rental = new Rental(*request, motorbike->getOwnerUsername());
        rentals.push_back(rental);

        // Reject all conflicting requests for the same motorbike
        for (RentalRequest* otherRequest : requests) {
            if (otherRequest != request &&
                otherRequest->getStatus() == RequestStatus::PENDING &&
                otherRequest->hasConflict(*request)) {
                otherRequest->reject();
                std::cout << "Automatically rejected conflicting request: "
                    << otherRequest->getRequestId() << std::endl;
            }
        }

        std::cout << "Rental request approved successfully!" << std::endl;
        std::cout << "Rental ID: " << rental->getRentalId() << std::endl;
        std::cout << "Credits deducted: " << std::fixed << std::setprecision(2)
            << request->getEstimatedCost() << " CP" << std::endl;

        return true;
    }

    void RentalManager::rejectRequest(const std::string& requestId) {
        RentalRequest* request = getRequestById(requestId);
        if (request && request->getStatus() == RequestStatus::PENDING) {
            request->reject();
            std::cout << "Request " << requestId << " has been rejected." << std::endl;
        }
    }

    // ============================================ RENTAL LIFECYCLE MANAGEMENT ========================================

    void RentalManager::completeRental(const std::string& rentalId) {
        if (Rental* rental = getRentalById(rentalId); rental && rental->isActive()) {
            rental->complete();
            std::cout << "Rental " << rentalId << " has been marked as completed." << std::endl;
            std::cout << "Please rate your experience!" << std::endl;
        }
        else {
            std::cout << "Cannot complete rental: Rental not found or already completed." << std::endl;
        }
    }

    bool RentalManager::canCreateRental(const std::string& renterUsername, const std::string& motorbikeLicensePlate,
                                        const DateUtil::TimePoint& startDate,
                                        const DateUtil::TimePoint& endDate) const {
        // Check if renter already has an active rental during this period
        for (const Rental* rental : rentals) {
            if (rental->getRenterUsername() == renterUsername && rental->isActive()) {
                // Check if there's any date overlap with existing rental
                if (!(endDate <= rental->getStartDate() || startDate >= rental->getEndDate())) {
                    return false; // Renter already has a conflicting rental
                }
            }
        }

        // Check for conflicts with existing rentals
        for (const Rental* rental : rentals) {
            if (rental->getMotorbikeLicensePlate() == motorbikeLicensePlate && rental->isActive()) {
                // Check date overlap
                if (!(endDate <= rental->getStartDate() || startDate >= rental->getEndDate())) {
                    return false; // Conflict found
                }
            }
        }

        // Check for conflicts with pending/approved requests
        for (const RentalRequest* request : requests) {
            if (request->getMotorbikeLicensePlate() == motorbikeLicensePlate &&
                request->getStatus() == RequestStatus::PENDING) {
                if (!(endDate <= request->getStartDate() || startDate >= request->getEndDate())) {
                    return false; // Conflict found
                }
            }
        }

        return true;
    }

    void RentalManager::processOverdueRental() {
        auto overdueRentals = getOverdueRentals();

        if (overdueRentals.empty()) {
            return; // No overdue rentals to process
        }

        // Process silently - no console output for normal operation
        for (Rental* rental : overdueRentals) {
            if (rental->isActive()) {
                rental->complete(); // Direct completion without console output
            }
        }
    }

    // =============================================== RATING SYSTEM ===================================================

    bool RentalManager::createRating(const std::string& reviewerUsername, const std::string& revieweeId, int stars,
                                     const std::string& comment, const std::string& rentalId, RatingType type,
                                     const MotorbikeManager* motorbikeManager, const AuthManager* authManager) {
        // Validate input
        if (stars < 1 || stars > 5) {
            std::cout << "Rating failed: Stars must be between 1 and 5." << std::endl;
            return false;
        }

        // Find the rental
        const Rental* rental = getRentalById(rentalId);
        if (!rental) {
            std::cout << "Rating failed: Rental not found." << std::endl;
            return false;
        }

        if (rental->getStatus() != RentalStatus::COMPLETED) {
            std::cout << "Rating failed: Can only rate completed rentals." << std::endl;
            return false;
        }

        // Validate reviewer
        Member* reviewer = authManager->findMemberByUsername(reviewerUsername);
        if (!reviewer) {
            std::cout << "Rating failed: Reviewer not found." << std::endl;
            return false;
        }

        // Check if reviewer is part of this rental
        if (rental->getRenterUsername() != reviewerUsername &&
            rental->getOwnerUsername() != reviewerUsername) {
            std::cout << "Rating failed: You can only rate rentals you participated in." << std::endl;
            return false;
        }

        // Check if rating already exists
        for (const Rating* existingRating : ratings) {
            if (existingRating->getRentalId() == rentalId &&
                existingRating->getReviewerId() == reviewerUsername &&
                existingRating->getType() == type) {
                std::cout << "Rating failed: You have already rated this "
                    << (type == RatingType::RENTER_RATING ? "renter" : "motorbike") << "." << std::endl;
                return false;
            }
        }

        // Validate reviewee based on type
        if (type == RatingType::RENTER_RATING) {
            // Owner rating renter
            if (reviewerUsername != rental->getOwnerUsername()) {
                std::cout << "Rating failed: Only the owner can rate the renter." << std::endl;
                return false;
            }
            if (revieweeId != rental->getRenterUsername()) {
                std::cout << "Rating failed: Invalid reviewee for renter rating." << std::endl;
                return false;
            }
        }
        else if (type == RatingType::MOTORBIKE_RATING) {
            // Renter rating motorbike
            if (reviewerUsername != rental->getRenterUsername()) {
                std::cout << "Rating failed: Only the renter can rate the motorbike." << std::endl;
                return false;
            }
            if (revieweeId != rental->getMotorbikeLicensePlate()) {
                std::cout << "Rating failed: Invalid reviewee for motorbike rating." << std::endl;
                return false;
            }
        }

        // Create the rating
        auto* newRating = new Rating(reviewerUsername, revieweeId, stars, comment, rentalId, type);
        ratings.push_back(newRating);

        // Update the reviewee's rating
        if (type == RatingType::RENTER_RATING) {
            // Update renter's rating
            if (Member* renter = authManager->findMemberByUsername(revieweeId)) {
                const double newRatingDouble = calculateAverageRating(revieweeId, RatingType::RENTER_RATING);
                renter->updateRenterRating(newRatingDouble);
                std::cout << "Updated motorbike " << revieweeId << "'s rating to: "
                    << std::fixed << std::setprecision(1) << newRating << std::endl;
            }
        }
        else if (type == RatingType::MOTORBIKE_RATING) {
            if (Motorbike* motorbike = motorbikeManager->getMotorbikeByLicensePlate(revieweeId)) {
                const double newRatingDouble = calculateAverageRating(revieweeId, RatingType::MOTORBIKE_RATING);
                motorbike->updateRating(newRatingDouble);
                std::cout << "Updated motorbike " << revieweeId << "'s rating to: "
                    << std::fixed << std::setprecision(1) << newRating << std::endl;
            }
        }

        std::cout << "Rating submitted successfully!" << std::endl;
        std::cout << "Rating ID: " << newRating->getRatingId() << std::endl;
        return true;
    }

    // ============================================== QUERY METHODS ====================================================

    std::vector<RentalRequest*> RentalManager::getRequestsForMotorbike(const std::string& motorbikeLicensePlate) const {
        std::vector<RentalRequest*> result;
        for (RentalRequest* req : requests) {
            if (req->getMotorbikeLicensePlate() == motorbikeLicensePlate)
                result.push_back(req);
        }
        return result;
    }

    std::vector<RentalRequest*> RentalManager::getRequestsFromRenter(const std::string& renterUsername) const {
        std::vector<RentalRequest*> result;
        for (RentalRequest* req : requests) {
            if (req->getRenterUsername() == renterUsername) {
                result.push_back(req);
            }
        }

        return result;
    }

    std::vector<Rental*> RentalManager::getActiveRentalsForMember(const std::string& username) const {
        std::vector<Rental*> result;
        for (Rental* rental : rentals) {
            if ((rental->getRenterUsername() == username || rental->getOwnerUsername() == username) &&
                rental->isActive()) {
                result.push_back(rental);
            }
        }
        return result;
    }

    std::vector<Rental*> RentalManager::getRentalHistory(const std::string& username) const {
        std::vector<Rental*> result;
        for (Rental* rental : rentals) {
            if (rental->getRenterUsername() == username || rental->getOwnerUsername() == username) {
                result.push_back(rental);
            }
        }
        return result;
    }

    std::vector<Rating*> RentalManager::getRatingsForEntity(const std::string& entityId, RatingType type) const {
        std::vector<Rating*> result;
        for (Rating* rating : ratings) {
            if (rating->getRevieweeId() == entityId && rating->getType() == type) {
                result.push_back(rating);
            }
        }
        return result;
    }

    // =========================================== RATING CALCULATIONS =================================================

    double RentalManager::calculateAverageRating(const std::string& entityId, RatingType type) const {
        const std::vector<Rating*> entityRatings = getRatingsForEntity(entityId, type);

        if (entityRatings.empty()) {
            return 3.0; // Default rating if no ratings exist
        }

        int totalStars = 0;
        for (const Rating* rating : entityRatings) {
            totalStars += rating->getStars();
        }

        return static_cast<double>(totalStars) / entityRatings.size();
    }

    int RentalManager::getRatingCount(const std::string& entityId, RatingType type) const {
        return static_cast<int>(getRatingsForEntity(entityId, type).size());
    }

    // ============================================= UTILITY METHODS ===================================================

    RentalRequest* RentalManager::getRequestById(const std::string& requestId) const {
        for (RentalRequest* req : requests) {
            if (req->getRequestId() == requestId) {
                return req;
            }
        }
        return nullptr;
    }

    Rental* RentalManager::getRentalById(const std::string& rentalId) const {
        for (Rental* rental : rentals) {
            if (rental->getRentalId() == rentalId) {
                return rental;
            }
        }
        return nullptr;
    }

    bool RentalManager::hasActiveRental(const std::string& renterUsername) const {
        for (const Rental* rental : rentals) {
            if (rental->getRenterUsername() == renterUsername && rental->isActive()) {
                return true;
            }
        }
        return false;
    }

    std::vector<Rental*> RentalManager::getOverdueRentals() const {
        std::vector<Rental*> result;
        for (Rental* rental : rentals) {
            if (rental->isOverdue()) {
                result.push_back(rental);
            }
        }
        return result;
    }

    // ========================================== DATA MANAGEMENT METHODS ==============================================

    void RentalManager::addRentalRequest(RentalRequest* request) {
        if (request) {
            requests.push_back(request);
        }
    }

    void RentalManager::addRental(Rental* rental) {
        if (rental) {
            rentals.push_back(rental);
        }
    }

    void RentalManager::addRating(Rating* rating) {
        if (rating) {
            ratings.push_back(rating);
        }
    }

    std::vector<RentalRequest*> RentalManager::getAllRequests() const {
        return requests;
    }

    std::vector<Rental*> RentalManager::getAllRentals() const {
        return rentals;
    }

    std::vector<Rating*> RentalManager::getAllRatings() const {
        return ratings;
    }

    // =========================================== DISPLAY METHODS =====================================================

    void RentalManager::displayRequestsForOwner(const std::string& ownerUsername,
                                                const MotorbikeManager* motorbikeManager) const {
        if (!motorbikeManager) {
            std::cout << "Cannot display requests: MotorbikeManager not available." << std::endl;
            return;
        }

        std::cout << "\n========== RENTAL REQUESTS FOR YOUR MOTORBIKES ==========" << std::endl;

        bool hasRequests = false;
        for (const RentalRequest* request : requests) {
            // Get the motorbike to check ownership
            if (const Motorbike* motorbike = motorbikeManager->
                    getMotorbikeByLicensePlate(request->getMotorbikeLicensePlate()); motorbike && motorbike->
                getOwnerUsername() == ownerUsername) {
                if (!hasRequests) {
                    hasRequests = true;
                }

                std::cout << "\n--- Request Details ---" << std::endl;
                request->displayRequest();
                std::cout << "Motorbike: " << motorbike->getBrand() << " " << motorbike->getModel() << std::endl;
                std::cout << "------------------------" << std::endl;
            }
        }

        if (!hasRequests) {
            std::cout << "No rental requests found for your motorbikes." << std::endl;
        }
        std::cout << "=========================================================" << std::endl;
    }

    void RentalManager::displayRentalHistory(const std::string& username) const {
        std::cout << "\n========== RENTAL HISTORY FOR " << username << " ==========" << std::endl;

        if (const std::vector<Rental*> userRentals = getRentalHistory(username); userRentals.empty()) {
            std::cout << "No rental history found." << std::endl;
        }
        else {
            for (size_t i = 0; i < userRentals.size(); ++i) {
                std::cout << "\n--- Rental #" << (i + 1) << " ---" << std::endl;
                userRentals[i]->displayInfo();
                std::cout << "Role: " << (userRentals[i]->getRenterUsername() == username ? "Renter" : "Owner") <<
                    std::endl;
            }
        }
        std::cout << "============================================================" << std::endl;
    }

    void RentalManager::displayRatingsForEntity(const std::string& entityId, RatingType type) const {
        std::cout << "\n========== RATINGS FOR " << entityId << " ==========" << std::endl;

        if (const std::vector<Rating*> entityRatings = getRatingsForEntity(entityId, type); entityRatings.empty()) {
            std::cout << "No ratings found." << std::endl;
        }
        else {
            double avgRating = calculateAverageRating(entityId, type);
            std::cout << "Average Rating: " << std::fixed << std::setprecision(1)
                << avgRating << "/5.0 (" << entityRatings.size() << " reviews)" << std::endl;
            std::cout << "\n--- Individual Reviews ---" << std::endl;

            for (size_t i = 0; i < entityRatings.size(); ++i) {
                std::cout << "\nReview #" << (i + 1) << ":" << std::endl;
                entityRatings[i]->displayRating();
            }
        }
        std::cout << "===============================================" << std::endl;
    }
}
