//
// Created by Hoang Vinh Khue on 9/07/2025.
//

#pragma once
#include <vector>
#include "../entities/Motorbike.h"

namespace EMotoRental
{
    class MotorbikeManager
    {
    private:
        std::vector<Motorbike*> motorbikes;

    public:
        MotorbikeManager();
        ~MotorbikeManager();

        // Registration and listing management
        bool registerMotorbike(
            const std::string& ownerUsername,
            const std::string& brand,
            const std::string& model, 
            const std::string& color,
            int engineSize, 
            const std::string& licensePlate,
            const std::string& city
        );

        bool listMotorbike(
            const std::string& motorbikeId,
            const DateUtil::TimePoint& startDate,
            const DateUtil::TimePoint& endDate,
            double dailyRate,
            double minRating
        );

        bool unlistMotorbike(const std::string& motorbikeId);

        // Search and retrieval methods
        std::vector<Motorbike*> searchMotorbikes(
            const std::string& city,
            const DateUtil::TimePoint& startDate,
            const DateUtil::TimePoint& endDate
        ) const;
        Motorbike* getMotorbikeById(const std::string& motorbikeId) const;
        std::vector<Motorbike*> getMotorbikesByOwner(const std::string& username) const; 
        std::vector<Motorbike*> getAllMotorbikes() const;

        // Rating system
        bool updateMotorbikeRating(const std::string& motorbikeId, double rating);

        // Data persistence methods
        bool loadAllMotorbikes();
        bool saveAllMotorbikes() const;
    };
}
