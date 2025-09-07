//
// Created by Tran Dinh Dong on 9/07/2025.
//

#include "RentalManager.h"
#include <iostream>
#include <sstream>
#include "../utils/FileHandler.h"
#include "../utils/Logger.h"

namespace EMotoRental
{
    RentalManager::RentalManager() {}

    bool RentalManager::createRentalRequest(RentalRequest* request)
    {
        if (!request) return false;
        requests.push_back(request);
        Logger::logDataOperation("Created rental request: " + request->getRequestId());
        return true;
    }

    bool RentalManager::approveRequest(const std::string& requestId)
    {
        for (auto* req : requests)
        {
            if (req->getRequestId() == requestId && req->getStatus() == RequestStatus::PENDING)
            {
                Rental* rental = req->approve();
                if (rental)
                {
                    rentals.push_back(rental);
                    Logger::logDataOperation("Approved request " + requestId + ", created rental " + rental->getRentalId());
                    return true;
                }
            }
        }
        return false;
    }

    bool RentalManager::rejectRequest(const std::string& requestId)
    {
        for (auto* req : requests)
        {
            if (req->getRequestId() == requestId && req->getStatus() == RequestStatus::PENDING)
            {
                req->reject();
                Logger::logDataOperation("Rejected request: " + requestId);
                return true;
            }
        }
        return false;
    }

    void RentalManager::completeRental(const std::string& rentalId)
    {
        for (auto* rental : rentals)
        {
            if (rental->getRentalId() == rentalId && rental->isActive())
            {
                rental->complete();
                Logger::logDataOperation("Completed rental: " + rentalId);
                return;
            }
        }
    }

    bool RentalManager::createRating(Rating* rating)
    {
        if (!rating) return false;
        ratings.push_back(rating);
        Logger::logDataOperation("Created rating: " + rating->getRatingId());
        return true;
    }

    std::vector<RentalRequest*> RentalManager::getRequestsForMotorbike(const std::string& motorbikeId) const
    {
        std::vector<RentalRequest*> result;
        for (auto* req : requests)
        {
            if (req->getMotorbikeId() == motorbikeId)
                result.push_back(req);
        }
        return result;
    }

    std::vector<Rental*> RentalManager::getActiveRentalsForMember(const std::string& username) const
    {
        std::vector<Rental*> result;
        for (auto* rental : rentals)
        {
            if (rental->isActive() && rental->getRenterUsername() == username)
                result.push_back(rental);
        }
        return result;
    }

    std::vector<Rating*> RentalManager::getRatingsForEntity(const std::string& entityId, RatingType type) const
    {
        std::vector<Rating*> result;
        for (auto* rating : ratings)
        {
            if (rating->getType() == type && rating->getRevieweeId() == entityId)
                result.push_back(rating);
        }
        return result;
    }

    double RentalManager::calculateAverageRating(const std::string& entityId, RatingType type) const
    {
        auto entityRatings = getRatingsForEntity(entityId, type);
        if (entityRatings.empty()) return 0.0;

        double sum = 0.0;
        for (auto* rating : entityRatings)
        {
            sum += rating->getStars();
        }
        return sum / entityRatings.size();
    }

    bool RentalManager::loadAllRentalData()
    {
        try
        {
            auto rentalLines = FileHandler::readAllLines("data/rentals.csv");
            for (auto& line : rentalLines)
            {
                if (auto* rental = Rental::fromCSVString(line))
                    rentals.push_back(rental);
            }

            auto requestLines = FileHandler::readAllLines("data/requests.csv");
            for (auto& line : requestLines)
            {
                if (auto* request = RentalRequest::fromCSVString(line))
                    requests.push_back(request);
            }

            auto ratingLines = FileHandler::readAllLines("data/ratings.csv");
            for (auto& line : ratingLines)
            {
                if (auto* rating = Rating::fromCSVString(line))
                    ratings.push_back(rating);
            }
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading rental data: " << e.what() << std::endl;
            return false;
        }
    }

    bool RentalManager::saveAllRentalData() const
    {
        try
        {
            std::vector<std::string> rentalLines;
            for (auto* rental : rentals)
                rentalLines.push_back(rental->toCSVString());
            FileHandler::writeAllLines("data/rentals.csv", rentalLines);

            std::vector<std::string> requestLines;
            for (auto* req : requests)
                requestLines.push_back(req->toCSVString());
            FileHandler::writeAllLines("data/requests.csv", requestLines);

            std::vector<std::string> ratingLines;
            for (auto* rating : ratings)
                ratingLines.push_back(rating->toCSVString());
            FileHandler::writeAllLines("data/ratings.csv", ratingLines);

            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error saving rental data: " << e.what() << std::endl;
            return false;
        }
    }
}
