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
        static void displayWelcomeScreen();
        static int getUserTypeChoice();
        static void displayMainMenu(const std::string& userType);

        // Authentication Screens
        static std::string getUsername();
        static std::string getPassword();
        static bool getRegistrationData(std::string& username, std::string& password, std::string& fullName,
                                        std::string& email, std::string& phone);
        static bool getVerificationData(std::string& idNumber, std::string& licenseNumber, std::string& idType);
        static void displayVerificationResult(bool success, const std::string& message = "");

        // Member Screens
        static void displayMemberDashboard(const Member* member,
                                           const RentalManager* rentalManager = nullptr,
                                           const MotorbikeManager* motorbikeManager = nullptr);
        static void displayMemberMenu();
        static void displayRentalMenu();
        static bool getRatingData(int& star, std::string& comment);

        // Admin Screens
        static void displayAdminMenu();
        static void displayAllMembers(const std::vector<Member*>& members);

        // Guest Screens
        static void displayGuestMenu();

        // Credit Management Screens
        static bool getCreditTopUpData(double& amount, std::string& password);
        static bool getProfileUpdateData(std::string& email, std::string& phone);
        static bool getPasswordChangeData(std::string& oldPass, std::string& newPass);

        // Display utilities (Pure display - No input)
        static void displaySeparator();
        static void clearScreen();
        static void displayHeader(const std::string& title);
        static void displaySubHeader(const std::string& subtitle);
        static void displayFooter();

    private:
        // Private display helpers
        static void displayUserTypeOptions();
        static void displayAccountOverview(const Member* member);
        static void displayRentalSection(const std::string& title, const std::string& message);

        static void displayActiveRentalBookings(const Member* member, const RentalManager* rentalManager,
                                                const MotorbikeManager* motorbikeManager);
        static void displayActiveRentalRequests(const Member* member, const RentalManager* rentalManager,
                                              const MotorbikeManager* motorbikeManager);
    };
}
