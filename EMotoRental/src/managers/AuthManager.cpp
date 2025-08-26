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
        // addDefaultAdmin();

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
        }
        else
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

    // ================================== AUTHENTICATION AND USER MANAGEMENT ===========================================

    bool AuthManager::registerMember(
        const std::string& username, const std::string& password, const std::string& fullName, const std::string& email,
        const std::string& phone
    )
    {
        // Validate input params
        if (username.empty() || password.empty() || fullName.empty() || email.empty() || phone.empty())
        {
            std::cout << "Registration failed: All fields are required." << std::endl;
            return false;
        }

        // Check username availability
        if (!isUsernameAvailable(username))
        {
            std::cout << "Registration failed: Username is not available." << std::endl;
            return false;
        }

        // Validate password strength
        if (!validatePasswordStrength(password))
        {
            std::cout << "Registration failed: Password does not meet strenght requirements";
            return false;
        }

        // Basic email formate validation
        if (email.find('@') == std::string::npos || email.find('.') == std::string::npos)
        {
            std::cout << "Registration failed: Invalid email format" << std::endl;
            return false;
        }

        // Basic phone number format validation
        if (phone.length() < 10)
        {
            std::cout << "Registration failed: Phone number must be at least 10 digits" << std::endl;
            return false;
        }

        // Create new member with default values (20 credit points, rating 3.0)
        Member* newMember = new Member(username, password, fullName, email, phone);

        // Add to members collection
        addMember(newMember);

        std::cout << "Registratio successful! Member '" << username
                    << "' created with 20 credit points and rating 3.0" << std::endl;

        return true;
    }

    bool AuthManager::isUsernameAvailable(const std::string& username) const
    {
        return !userExists(username);
    }

    // ========================================= USER MANAGEMENT =======================================================

    Member* AuthManager::findMemberByUsername(const std::string& username) const
    {
        for (Member* member: members)
        {
            if (member->getUsername() == username)
            {
                return member;
            }
        }
        return nullptr;
    }

    void AuthManager::addMember(Member* member)
    {
        if (member != nullptr)
        {
            members.push_back(member);
        }
    }

    std::vector<Member*> AuthManager::getAllMembers() const
    {
        return members;
    }

    // ========================================== DATA PERSISTENCE =====================================================



}
