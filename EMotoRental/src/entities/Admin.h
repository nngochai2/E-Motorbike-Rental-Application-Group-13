//
// Created by Tran Dinh Dong on 8/26/2025.
//
#pragma once
#include "User.h"

namespace EMotoRental {
    class Admin : public User {

    public:
        Admin();
        Admin(const std::string& username, const std::string& password,
              const std::string& fullName);

        // Virtual function implementations
        std::string getUserType() const override;
        void displayInfo() const override;
        std::string toCSVString() const override;
        static Admin* fromCSVString(const std::string& csvData);
    };
}
