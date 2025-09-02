//
// Created by Nguyen Ngoc Hai on 8/25/2025.
//

#pragma once
#include <vector>
#include "../entities/Member.h"
#include "../entities/Admin.h"

namespace EMotoRental
{
    class AuthManager
    {
    private:
        std::vector<Member*> members;
        std::vector<Admin*> admins;
        User* currentUser;

    public:
        AuthManager();
        ~AuthManager();

        // Authentication
        User* login(const std::string& username, const std::string& password);
        void logout();
        [[nodiscard]] User* getCurrentUser() const;
        [[nodiscard]] bool isLoggedIn() const;

        // Registration
        bool registerMember(
            const std::string& username,
            const std::string& password,
            const std::string& fullName,
            const std::string& email,
            const std::string& phone
        );

        [[nodiscard]] bool isUsernameAvailable(const std::string& username) const;

        // User management
        [[nodiscard]] Member* findMemberByUsername(const std::string& username) const;
        Admin* findAdminByUsername(const std::string& username) const;
        void addMember(Member* member);
        void addAdmin(Admin* admin);
        [[nodiscard]] std::vector<Member*> getAllMembers() const;
        std::vector<Admin*> getAllAdmins() const;

        // Validation
        static bool validatePasswordStrength(const std::string& password) ;

    private:
        // Helper methods
        void addDefaultAdmin();
        [[nodiscard]] bool userExists(const std::string& username) const;
        void cleanupMemory();
    };
}