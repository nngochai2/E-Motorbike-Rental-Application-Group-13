//
// Created by Tran Dinh Dong on 8/26/2025.
//
#include "Admin.h"

namespace EMotoRental
{
    Admin::Admin() : User("admin", "", "Administrator", "", "")
    {
        // Default admin with placeholder values
        setVerified(true); // Admin is always considered verified
    }

    Admin::Admin(const std::string& username, const std::string& password,
                 const std::string& fullName)
        : User(username, password, fullName, "", "")
    {
        setVerified(true); // Admin accounts should always be verified
    }

    std::string Admin::getUserType() const
    {
        return "Admin";
    }

    void Admin::displayInfo() const
    {
        std::cout << "=== ADMIN INFORMATION ===" << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Full Name: " << fullName << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Phone: " << phoneNumber << std::endl;
        std::cout << "Verified: " << (isVerified ? "Yes" : "No") << std::endl;
        std::cout << "=========================" << std::endl;
    }

    std::string Admin::toCSVString() const
    {
        // Not really necessary? but here for measure I guess?
        return username + "," + password + "," + fullName + "," + email + "," + phoneNumber + ",ADMIN";
    }

    void Admin::viewAllMembers() const
    {
        std::cout << "[Admin] Viewing all registered members..." << std::endl;
        // TODO: integrate with MemberManager to list all members
    }

    void Admin::viewAllMotorbikes() const
    {
        std::cout << "[Admin] Viewing all motorbikes in the system..." << std::endl;
        // TODO: integrate with MotorbikeManager to list all bikes
    }
}
