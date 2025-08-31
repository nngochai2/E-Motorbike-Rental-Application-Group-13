//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#pragma once
#include "ConsoleView.h"
#include "../managers/AuthManager.h"

namespace EMotoRental
{
    class Application
    {
    private:
        ConsoleView* view;
        AuthManager* authManager;
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
        bool handleMemberLogin();
        bool handleAdminLogin();
        bool handleMemberRegistration();
        void handleLogout();

        // Menu handlers
        void handleGuestMenu();
        void handleMemberMenu();
        void handleAdminMenu();

        // Member feature handlers
        void handleMemberDashboard();
        void handleProfileUpdate();
        void handlePasswordChange();
        void handleCreditTopUp();
        void handleMotorbikeRegistration();
        void handleMotorbikeListing();
        void handleMotorbikeSearch();
        void handleRentalHistory();

        // Admin feature handlers
        void handleViewAllMembers();
        void handleViewAllMotorbikes();
        void handleViewMemberDetails();
        void handleSystemStatistics();
        void handleDataManagement();

        // Utility methods
        void exitApplication();
        void displaySystemInfo();
        bool confirmCriticalAction(const std::string& action);
    };
}
