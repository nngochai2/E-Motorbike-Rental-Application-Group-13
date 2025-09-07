//
// Created by Tran Dinh Dong on 9/07/2025.
//

#pragma once
#include <vector>
#include <string>
#include "Rental.h"
#include "RentalRequest.h"
#include "Rating.h"
#include "../enums/RatingType.h"

namespace EMotoRental
{
    class RentalManager
    {
    private:
        std::vector<RentalRequest*> requests;
        std::vector<Rental*> rentals;
        std::vector<Rating*> ratings;

        std::string formatRentalAsCSV(const Rental* rental) const;
        std::string formatRatingAsCSV(const Rating* rating) const;
        std::vector<Rental*> parseRentalsFromCSV(const std::string& content) const;
        std::vector<Rating*> parseRatingsFromCSV(const std::string& content) const;
    public:
        RentalManager();

        bool createRentalRequest(RentalRequest* request);
        bool approveRequest(const std::string& requestId);
        bool rejectRequest(const std::string& requestId);

        void completeRental(const std::string& rentalId);

        bool createRating(Rating* rating);
        std::vector<Rating*> getRatingsForEntity(const std::string& entityId, RatingType type) const;
        double calculateAverageRating(const std::string& entityId, RatingType type) const;

        std::vector<RentalRequest*> getRequestsForMotorbike(const std::string& motorbikeId) const;
        std::vector<Rental*> getActiveRentalsForMember(const std::string& username) const;

        bool loadAllRentalData();
        bool saveAllRentalData() const;
    };
}
