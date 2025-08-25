//
// Created by Nguyen Ngoc Hai on 8/25/2025.
//

#include "AuthManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace EMotoRental
{
    AuthManager::AuthManager() : currentUser(nullptr)
    {
        // Add default admin account for system management
        addDefaultAdmin();

        // Try to load existing data
        loadAllMembers();
        loadAllAdmins();

        std::cout << "AuthManager initialized with " << members.size() << " members" << std::endl;
    }

    AuthManager::~AuthManager()
    {
        // Save all data before destruction
        saveAllMembers();
        // saveAllAdmins();

        std::cout << "AuthManager: Data saved and resources cleaned up." << std::endl;
    }

    // ================================== AUTHENTICATION AND USER MANAGEMENT ===========================================

    User* AuthManager::login(const std::string& username, const std::string& password)
    {
        // First check if already logged in
        if (currentUser != nullptr)
        {
            std::cout << "User already logged in. Please logout first." << std::endl;
            return currentUser;
        }

        // Check members
        Member* member = findMemberByUsername(username);
        if (member != nullptr && member->validatePassword(password))
        {
            currentUser = member;
            std::cout << "Member login successful: " << username << std::endl;
            return currentUser;
        }

        // Check admins (later)

        // Login failed
        std::cout << "Login failed: Invalid username or password." << std::endl;
        return nullptr;
    }

    void AuthManager::logout()
    {
        if (currentUser != nullptr)
        {
            std::cout << "User logged out: " << currentUser->getUsername() << std::endl;
            currentUser = nullptr;
        } else
        {
            std::cerr << "No user currently logged in." << std::endl;
        }
    }

    User* AuthManager::getCurrentUser() const
    {
        return currentUser;
    }

    bool AuthManager::isLoggedIn() const
    {
        return currentUser != nullptr;
    }


}