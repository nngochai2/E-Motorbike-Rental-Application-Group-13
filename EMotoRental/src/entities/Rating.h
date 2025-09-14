//
// Created by Nguyen Ngoc Hai on 9/14/2025.
//

#pragma once

#include <string>

#include "../enums/RatingType.h"
#include "../utils/DateUtil.h"

namespace EMotoRental
{
    class Rating
    {
    private:
        std::string ratingId;
        std::string reviewerId;
        std::string revieweeId; // For renter rating, or motorbike ID for motorbike rating
        int stars;
        std::string comment;
        DateUtil::TimePoint ratingDate;
        std::string rentalId;
        RatingType type;

    public:
        // Constructors
        Rating();
        Rating(const std::string& reviewer, const std::string& reviewee,
               int stars, const std::string& comment, const std::string& rentalId,
               RatingType type);

        // Getters
        std::string getRatingId() const;
        std::string getReviewerId() const;
        std::string getRevieweeId() const;
        int getStars() const;
        std::string getComment() const;
        DateUtil::TimePoint getRatingDate() const;
        std::string getRentalId() const;
        RatingType getType() const;

        // Display
        void displayRating() const;

        // Data persistence
        std::string toCSVString() const;
        static Rating* fromCSVString(const std::string& csvData);

    private:
        static std::string generateRatingId();
        std::string typeToString() const;
        static RatingType stringToType(const std::string& str);
    };
}
