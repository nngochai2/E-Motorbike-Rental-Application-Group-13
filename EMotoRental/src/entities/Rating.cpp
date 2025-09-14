//
// Created by Nguyen Ngoc Hai on 9/14/2025.
//

#include "Rating.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

namespace EMotoRental
{
    Rating::Rating() : stars(0), type(RatingType::RENTER_RATING) {
        ratingDate = DateUtil::getCurrentTime();
        ratingId = generateRatingId();
    }

    Rating::Rating(const std::string& reviewer, const std::string& reviewee,
                  int stars, const std::string& comment, const std::string& rentalId,
                  RatingType type)
        : reviewerId(reviewer), revieweeId(reviewee), stars(stars),
          comment(comment), rentalId(rentalId), type(type)
    {
        ratingDate = DateUtil::getCurrentTime();
        ratingId = generateRatingId();
    }

    // ================================================ GETTERS ========================================================

    std::string Rating::getRatingId() const { return ratingId; }
    std::string Rating::getReviewerId() const { return reviewerId; }
    std::string Rating::getRevieweeId() const { return revieweeId; }
    int Rating::getStars() const { return stars; }
    std::string Rating::getComment() const { return comment; }
    DateUtil::TimePoint Rating::getRatingDate() const { return ratingDate; }
    std::string Rating::getRentalId() const { return rentalId; }
    RatingType Rating::getType() const { return type; }

    // ============================================= DISPLAY METHODS ===================================================

    void Rating::displayRating() const
    {
        std::cout << "Rating ID: " << ratingId << std::endl;
        std::cout << "From: " << reviewerId << std::endl;
        std::cout << "To: " << revieweeId << std::endl;
        std::cout << "Stars: " << stars << "/5" << std::endl;
        std::cout << "Comment: " << comment << std::endl;
        std::cout << "Type: " << typeToString() << std::endl;
        std::cout << "Date: " << DateUtil::formatDate(ratingDate) << std::endl;
    }


    // ============================================ DATA PERSISTENCE ===================================================

    std::string Rating::toCSVString() const
    {
        std::ostringstream oss;
        oss << ratingId << ","
            << reviewerId << ","
            << revieweeId << ","
            << stars << ","
            << comment << ","
            << DateUtil::formatDate(ratingDate) << ","
            << rentalId << ","
            << typeToString();
        return oss.str();
    }

    Rating* Rating::fromCSVString(const std::string& csvData) {
        std::istringstream iss(csvData);
        std::string token;
        std::vector<std::string> tokens;

        // Parse CSV line
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 8) {
            std::cerr << "Invalid Rating CSV data: insufficient fields (" << tokens.size() << " < 8)" << std::endl;
            return nullptr;
        }

        try {
            auto* rating = new Rating();

            // Parse each field
            rating->ratingId = tokens[0];
            rating->reviewerId = tokens[1];
            rating->revieweeId = tokens[2];
            rating->stars = std::stoi(tokens[3]);
            rating->comment = tokens[4];
            rating->ratingDate = DateUtil::parseDate(tokens[5]);
            rating->rentalId = tokens[6];
            rating->type = stringToType(tokens[7]);

            return rating;
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing Rating CSV data: " << e.what() << std::endl;
            return nullptr;
        }
    }

    // ============================================ HELPER METHODS =====================================================

    std::string Rating::generateRatingId() {
        const auto now = std::chrono::system_clock::now();
        const auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        return "RAT" + std::to_string(timestamp);
    }

    std::string Rating::typeToString() const
    {
        switch (type) {
            case RatingType::RENTER_RATING: return "RENTER_RATING";
            case RatingType::MOTORBIKE_RATING: return "MOTORBIKE_RATING";
            default: return "UNKNOWN";
        }
    }

    RatingType Rating::stringToType(const std::string& str) {
        if (str == "RENTER_RATING") return RatingType::RENTER_RATING;
        if (str == "MOTORBIKE_RATING") return RatingType::MOTORBIKE_RATING;

        std::cerr << "Warning: Unknown RatingType string: '" << str << "', defaulting to RENTER_RATING" << std::endl;
        return RatingType::RENTER_RATING;
    }
}