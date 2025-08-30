//
// Created by Khue Vinh Hoang on 8/29/2025.
//

#pragma once
#include <random>
#include <sstream>
#include <iomanip>

namespace EMotoRental
{
    class IdGenerator
    {
    public:
        // Generate a UUID v4 (random) - implementation inspired by RFC 4122
        static std::string generateUUID()
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 15);
            std::uniform_int_distribution<> dis2(8, 11);

            std::stringstream ss;
            ss << std::hex;

            // Generate 8 random hex digits
            for (int i = 0; i < 8; i++) {
                ss << dis(gen);
            }
            
            // Add hyphen and 4 random hex digits
            ss << "-";
            for (int i = 0; i < 4; i++) {
                ss << dis(gen);
            }
            
            // Add hyphen and 4 random hex digits, with version number in first position
            ss << "-4";  // Version 4 UUID
            for (int i = 0; i < 3; i++) {
                ss << dis(gen);
            }
            
            // Add hyphen and 4 random hex digits, with variant in first position
            ss << "-";
            ss << dis2(gen);  // Variant bits
            for (int i = 0; i < 3; i++) {
                ss << dis(gen);
            }
            
            // Add hyphen and 12 random hex digits
            ss << "-";
            for (int i = 0; i < 12; i++) {
                ss << dis(gen);
            }

            return ss.str();
        }

        // Generate a prefixed UUID for entities
        static std::string generateId(const std::string& prefix)
        {
            return prefix + "-" + generateUUID();
        }
    };
}
