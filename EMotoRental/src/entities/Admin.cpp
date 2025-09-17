//
// Created by Tran Dinh Dong on 8/26/2025.
//
#include "Admin.h"
#include <iostream>
#include <sstream>
#include <vector>

namespace EMotoRental {

    Admin::Admin() : User() {}

    Admin::Admin(const std::string& username, const std::string& password,
                 const std::string& fullName)
        : User(username, password, fullName, "", "") {}

    std::string Admin::getUserType() const {
        return "Admin";
    }

    void Admin::displayInfo() const {
        std::cout << "=== ADMIN INFORMATION ===" << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Full Name: " << fullName << std::endl;
        std::cout << "=========================" << std::endl;
    }

    std::string Admin::toCSVString() const {
        std::ostringstream oss;
        oss << username << "," << password << "," << fullName;
        return oss.str();
    }

    Admin* Admin::fromCSVString(const std::string& csvData) {
        std::istringstream iss(csvData);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 3) {
            return nullptr;
        }

        Admin* admin = new Admin(tokens[0], tokens[1], tokens[2]);
        return admin;
    }
}
