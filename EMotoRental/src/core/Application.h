//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#pragma once
#include "ConsoleView.h"
#include "../managers/DataManager.h"

namespace EMotoRental
{
    class Application
    {
    private:
        ConsoleView* view;
        DataManager* dataManager;
        bool isRunning;

    public:
        Application();
        ~Application();

        // Main application loop
        void run();

    private:
        // Main flow controllers
        void handleUserTypeSelection();
        void handleGuestFlow();
        void handleMemberFlow();
        void handleAdminFlow();

        // Authentication flows
        bool handleMemberLogin() const;
        bool handleAdminLogin() const;
        void handleMemberRegistration() const;
        void handleLogout() const;

        // Menu handlers
        void handleGuestMenu() const;
        void handleMemberMenu();
        void handleAdminMenu();

        // Member feature handlers
        void handleMemberDashboard() const;
        void handleProfileUpdate();
        void handlePasswordChange();
        void handleCreditTopUp();
        void handleMotorbikeRegistration();
        void handleRentalHistory() const;

        // Motorbike feature handlers
        void handleMotorbikeListing() const;
        void handleMotorbikeSearch() const;
        void displayGuestMotorbikeInfo(const Motorbike* bike) const;
        void handleMotorbikeManagement();

        // Admin feature handlers
        void handleViewAllMembers() const;
        void handleViewAllMotorbikes() const;
        void handleViewMemberDetails() const;
        void handleSystemStatistics() const;
        void handleDataManagement() const;

        // Utility methods
        void exitApplication();
        void displaySystemInfo() const;
        static bool confirmCriticalAction(const std::string& action);

        // Helper methods
        bool isUserLoggedIn() const;
        bool isMember() const;
        bool isAdmin() const;
    };
}
