//
// Created by Nguyen Ngoc Hai on 8/26/2025.
//

#pragma once
#include <string>
#include <vector>
#include "../entities/User.h"
#include "../entities/Member.h"
#include "../utils/InputHelper.h"

namespace EMotoRental
{
    class ConsoleView
    {
    public:
        ConsoleView();
        ~ConsoleView();

        // Welcome and Main Screens
        void displayWelcomeScreen() const;
        int getUserTypChoice() const;
        void displayMainMenu(const std::string& userType) const;

        // Authentication Screens
        std::string getUsername() const;
        std::string getPassword() const;
        bool getRegistrationData(std::string& username, std::string& password, std::string& fullName,
                                std::string& email, std::string& phone) const;

        // Member Screens
        void displayMemberDashboard(Member* member) const;
        void displayMemberMenu() const;

        // Admin Screens
        void displayAdminMenu() const;
        void displayAllMembers(const std::vector<Member*>& members) const;

        // Guest Screens
        void displayGuestMenu() const;

        // Credit Management Screens
        bool getCreditTopUpData(double& amount, std::string& password) const;
        bool getProfileUpdateData(std::string& email, std::string& phone) const;
        bool getPasswordChangeData(std::string& oldPass, std::string& newPass) const;

        // Display utilities (Pure display - No input)
        void displaySeparator() const;
        void clearScreen() const;
        void displayHeader(const std::string& title) const;
        void displaySubHeader(const std::string& subtitle) const;
        void displayFooter() const;

    private:
        // Private display helpers
        void displayUserTypeOptions() const;
        void displayAccountOverview() const;
        void displayRentalSection(const std::string& title, const std::string& message) const;
    };
}
