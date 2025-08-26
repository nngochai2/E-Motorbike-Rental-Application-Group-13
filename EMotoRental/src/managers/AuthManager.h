//
// Created by Nguyen Ngoc Hai on 8/25/2025.
//

#pragma once
#include <vector>
#include "../entities/Member.h"

namespace EMotoRental
{
    class AuthManager
    {
    private:
        std::vector<Member*> members;
        User* currentUser;

    public:
        AuthManager();
        ~AuthManager();

        // Authentication
        User* login(const std::string& username, const std::string& password);
        void logout();
        User* getCurrentUser() const;
        bool isLoggedIn() const;

        // Registration
        bool registerMember(
            const std::string& username,
            const std::string& password,
            const std::string& fullName,
            const std::string& email,
            const std::string& phone
        );

        bool isUsernameAvailable(const std::string& username) const;

        // User management
        Member* findMemberByUsername(const std::string& username) const;
        // Admin* findAdminByUsername(const std::string& username) const;
        void addMember(Member* member);
        // void addAdmin(Admin* admin);
        std::vector<Member*> getAllMembers() const;
        // std::vector<Admin*> getAllAdmins() const;

        // Date persistence (using CSV methods)
        bool loadAllMembers();
        bool saveAllMembers();
        bool loadAllAdmins();
        bool saveAllAdmins();

        // Validation
        bool validatePasswordStrength(const std::string& password) const;

    private:
        // Helper methods
        // void addDefaultAdmin();
        bool userExists(const std::string& username) const;
    };
}