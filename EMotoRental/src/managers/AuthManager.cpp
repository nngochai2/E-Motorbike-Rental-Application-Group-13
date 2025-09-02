//
// Created by Nguyen Ngoc Hai on 8/25/2025.
//

#include "AuthManager.h"
#include <iostream>
#include <sstream>

#include "FileHandler.h"

namespace EMotoRental
{
    AuthManager::AuthManager() : currentUser(nullptr) {
        // Add a default admin account (in memory - Saved by DataManager for system management
        addDefaultAdmin();

        std::cout << "AuthManager initialized." << std::endl;
    }

    AuthManager::~AuthManager() {
        // Clean up memory
        cleanupMemory();

        std::cout << "AuthManager: Data saved and resources cleaned up." << std::endl;
    }

    // ================================== AUTHENTICATION AND USER MANAGEMENT ===========================================

    User* AuthManager::login(const std::string& username, const std::string& password) {
        // First check if already logged in
        if (currentUser != nullptr) {
            std::cout << "User already logged in. Please logout first." << std::endl;
            return currentUser;
        }

        // Check members
        if (Member* member = findMemberByUsername(username); member != nullptr && member->validatePassword(password)) {
            currentUser = member;
            std::cout << "Member login successful: " << username << std::endl;
            return currentUser;
        }

        // Check admins
        if (Admin* admin = findAdminByUsername(username); admin != nullptr && admin->validatePassword(password)) {
            currentUser = admin;
            std::cout << "Admin login successful: " << username << std::endl;
            return currentUser;
        }

        // Login failed
        std::cout << "Login failed: Invalid username or password." << std::endl;
        return nullptr;
    }

    void AuthManager::logout() {
        if (currentUser != nullptr) {
            std::cout << "User logged out: " << currentUser->getUsername() << std::endl;
            currentUser = nullptr;
        }
        else {
            std::cerr << "No user currently logged in." << std::endl;
        }
    }

    User* AuthManager::getCurrentUser() const {
        return currentUser;
    }

    bool AuthManager::isLoggedIn() const {
        return currentUser != nullptr;
    }

    // =============================================== REGISTRATION ====================================================

    bool AuthManager::registerMember(
        const std::string& username, const std::string& password, const std::string& fullName, const std::string& email,
        const std::string& phone
    ) {
        // Validate input params
        if (username.empty() || password.empty() || fullName.empty() || email.empty() || phone.empty()) {
            std::cout << "Registration failed: All fields are required." << std::endl;
            return false;
        }

        // Check username availability
        if (!isUsernameAvailable(username)) {
            std::cout << "Registration failed: Username is not available." << std::endl;
            return false;
        }

        // Validate password strength
        if (!validatePasswordStrength(password)) {
            std::cout << "Registration failed: Password does not meet strength requirements" << std::endl;
            std::cout << "Ensure your password has at least 8 characters and contain an uppercase character, a digit, and a special character" << std::endl;
            return false;
        }

        // Basic email formate validation
        if (email.find('@') == std::string::npos || email.find('.') == std::string::npos) {
            std::cout << "Registration failed: Invalid email format" << std::endl;
            return false;
        }

        // Basic phone number format validation
        if (phone.length() < 10) {
            std::cout << "Registration failed: Phone number must be at least 10 digits" << std::endl;
            return false;
        }

        // Create new member with default values (20 credit points, rating 3.0)
        auto* newMember = new Member(username, password, fullName, email, phone);

        // Add to members collection
        addMember(newMember);

        std::cout << "Registration successful! Member '" << username
            << "' created with 20 credit points and rating 3.0" << std::endl;

        return true;
    }

    bool AuthManager::isUsernameAvailable(const std::string& username) const {
        return !userExists(username);
    }

    // ========================================= USER MANAGEMENT =======================================================

    Member* AuthManager::findMemberByUsername(const std::string& username) const {
        for (Member* member : members) {
            if (member->getUsername() == username) {
                return member;
            }
        }
        return nullptr;
    }

    Admin* AuthManager::findAdminByUsername(const std::string& username) const {
        for (Admin* admin : admins) {
            if (admin->getUsername() == username) {
                return admin;
            }
        }
        return nullptr;
    }

    void AuthManager::addMember(Member* member) {
        if (member != nullptr) {
            members.push_back(member);
        }
    }

    void AuthManager::addAdmin(Admin* admin) {
        if (admin != nullptr) {
            admins.push_back(admin);
        }
    }

    std::vector<Member*> AuthManager::getAllMembers() const {
        return members;
    }

    std::vector<Admin*> AuthManager::getAllAdmins() const {
        return admins;
    }

    // ============================================== VALIDATION =======================================================

    bool AuthManager::validatePasswordStrength(const std::string& password) {
        // Password must be at lease 8 characters
        if (password.length() < 8) {
            std::cout << "Invalid password. Password must be at least 8 characters" << std::endl;
            return false;
        }

        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

        for (char c : password) {
            if (c >= 'A' && c <= 'Z') hasUpper = true;
            else if (c >= 'a' && c <= 'z') hasLower = true;
            else if (c >= '0' && c <= '9') hasDigit = true;
            else if (c == '!' || c == '@' || c == '#' || c == '$' ||
                     c == '%' || c == '^' || c == '&' || c == '*' ||
                     c == '(' || c == ')' || c == '-' || c == '_' ||
                     c == '+' || c == '=' || c == '[' || c == ']' ||
                     c == '{' || c == '}' || c == '|' || c == '\\' ||
                     c == ':' || c == ';' || c == '"' || c == '\'' ||
                     c == '<' || c == '>' || c == ',' || c == '.' ||
                     c == '?' || c == '/') {
                hasSpecial = true;
                     }
        }

        return hasUpper && hasLower && hasDigit && hasSpecial;
    }

    // ========================================== PRIVATE HELPER METHODS ===============================================

    /**
     * Creates a built-in admin account for system access, ensures there's always at least 1 admin available.
     */
    void AuthManager::addDefaultAdmin() {
        const auto defaultAdmin = new Admin("admin", "12345", "System Admin");
        addAdmin(defaultAdmin);

        std::cout << "Default admin created successfully." << std::endl;
    }

    bool AuthManager::userExists(const std::string& username) const {
        // Check in members
        if (findMemberByUsername(username) != nullptr) {
            return true;
        }

        // Check in admins
        if (findAdminByUsername(username) != nullptr) {
            return true;
        }

        return false;
    }

    void AuthManager::cleanupMemory() {
        // Clean up members
        for (const Member* member : members) {
            delete member;
        }
        members.clear();

        // Clean up admins
        for (const Admin* admin : admins) {
            delete admin;
        }
        admins.clear();

        // Clear current user reference (don't delete, as it's already deleted above
        currentUser = nullptr;
    }
}
