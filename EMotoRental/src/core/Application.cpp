//
// Created by Nguyen Ngoc Hai on 8/31/2025.
//

#include "Application.h"
#include <iostream>
#include <iomanip>

namespace EMotoRental
{
    Application::Application() : isRunning(true) {
        std::cout << "Initializing E-Motorbike Rental Application..." << std::endl;

        view = new ConsoleView();
        dataManager = new DataManager();

        if (dataManager->getAuthManager()) {
            // Load existing data
            if (dataManager->loadAllData()) {
                std::cout << "Application initialized successfully!" << std::endl;
                displaySystemInfo();
            }
            else {
                std::cout << "Warning: Some data could not be loaded." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Failed to initialize data management system!" << std::endl;
            isRunning = false;
        }
    }

    Application::~Application() {
        std::cout << "Shutting down E-Motorbike Rental Application..." << std::endl;

        // DataManager handles all data saving and cleanup
        delete dataManager;
        delete view;

        std::cout << "Application shutdown complete. Goodbye!" << std::endl;
    }

    // ========================================== MAIN APPLICATION LOOP ================================================

    void Application::run() {
        if (!isRunning) {
            std::cerr << "Application failed to initialize. Exiting." << std::endl;
            return;
        }

        view->displayWelcomeScreen();

        while (isRunning) {
            try {
                handleUserTypeSelection();
                // The key: isRunning can be changed by exitApplication()
                // The while condition will be checked on next iteration
            } catch (const std::exception& e) {
                InputHelper::displayError("An unexpected error occurred: " + std::string(e.what()));
                InputHelper::displayMessage("Returning to main menu...");
            }
        }

        InputHelper::displayMessage("Thank you for using E-Motorbike Rental Application!");
    }

    // ========================================== MAIN FLOW CONTROLLER =================================================

    void Application::handleUserTypeSelection() {
        switch (view->getUserTypeChoice()) {
        case 1:
            handleGuestFlow();
            break;
        case 2:
            handleMemberFlow();
            break;
        case 3:
            handleAdminFlow();
            break;
        case 0:
            exitApplication();
            break;
        default:
            InputHelper::displayError("Invalid choice! Please select 0-3.");
            break;
        }
    }

    void Application::handleGuestFlow() {
        InputHelper::displayMessage("Welcome, Guest! Browse our available motorbikes or register as a member.");
        handleGuestMenu();
    }

    void Application::handleMemberFlow() {
        if (handleMemberLogin()) {
            // Member successfully logged in
            const auto member = static_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
            InputHelper::displaySuccess("Welcome back, " + member->getFullName() + "!");
            view->displayMemberDashboard(member);
            handleMemberMenu();
        }
        // If login failed, return to main menu
    }

    void Application::handleAdminFlow() {
        if (handleAdminLogin()) {
            // Admin successfully logged in
            const auto admin = static_cast<Admin*>(dataManager->getAuthManager()->getCurrentUser());
            InputHelper::displaySuccess("Admin access granted. Welcome, " + admin->getFullName() + "!");
            handleAdminMenu();
        }
    }

    // ========================================== AUTHENTICATION FLOWS =================================================

    bool Application::handleMemberLogin() const {
        const std::string username = view->getUsername();
        const std::string password = view->getPassword();

        const User* user = dataManager->getAuthManager()->login(username, password);

        if (user && user->getUserType() == "Member") {
            return true;
        }
        if (user && user->getUserType() != "Member") {
            InputHelper::displayError("Please use the correct login option for your account type.");
            dataManager->getAuthManager()->logout();
            return false;
        }
        InputHelper::displayError("Invalid username or password!");
        return false;
    }

    bool Application::handleAdminLogin() const {
        const std::string username = view->getUsername();
        const std::string password = view->getPassword();

        const User* user = dataManager->getAuthManager()->login(username, password);

        if (user && user->getUserType() == "Admin") {
            return true;
        }
        if (user && user->getUserType() != "Admin") {
            InputHelper::displayError("Admin access required!");
            dataManager->getAuthManager()->logout();
            return false;
        }
        InputHelper::displayError("Invalid admin credentials!");
        return false;
    }

    void Application::handleMemberRegistration() const {
        std::string username, password, fullName, email, phone;

        if (view->getRegistrationData(username, password, fullName, email, phone)) {
            if (AuthManager* authManager = dataManager->getAuthManager(); authManager->registerMember(username, password, fullName, email, phone)) {
                // Efficiently save the new member to an individual file
                if (const Member* newMember = authManager->findMemberByUsername(username)) {
                    dataManager->saveMember(newMember);
                }

                InputHelper::displaySuccess("Registration successful! You can now login with your credentials.");
                InputHelper::displayMessage("You have been awarded 20 credit points and a default rating of 3.0");
            } else {
                InputHelper::displayError("Registration failed! Please try again with different details.");
            }
        }
        InputHelper::waitForEnter();
    }

    void Application::handleLogout() const {
        dataManager->getAuthManager()->logout();
        InputHelper::displaySuccess("Logged out successfully!");
    }

    // ============================================= MENU HANDLERS =====================================================

    void Application::handleGuestMenu() {
        bool guestSession = true;

        while (guestSession) {
            view->displayMainMenu("Guest");

            switch (InputHelper::getMenuChoice(0, 3)) {
            case 1:
                InputHelper::displayMessage("Browse Available Motorbikes feature coming soon!");
                InputHelper::displayMessage("This will show public motorbike listings with limited details.");
                InputHelper::waitForEnter();
                break;
            case 2:
                InputHelper::displayMessage("Filter by Location feature coming soon!");
                InputHelper::displayMessage("This will allow filtering motorbikes by city (HCMC/Hanoi).");
                InputHelper::waitForEnter();
                break;
            case 3:
                handleMemberRegistration();
                break;
            case 0:
                guestSession = false;
                break;
            default:
                InputHelper::displayError("Invalid choice! Please select 0-3.");
                break;
            }
        }
    }

    void Application::handleMemberMenu() {
        bool memberSession = true;

        while (memberSession) {
            view->displayMainMenu("Member");

            switch (InputHelper::getMenuChoice(0, 8)) {
            case 1:
                handleMemberDashboard();
                break;
            case 2:
                handleProfileUpdate();
                break;
            case 3:
                handlePasswordChange();
                break;
            case 4:
                handleCreditTopUp();
                break;
            case 5:
                handleMotorbikeRegistration();
                break;
            case 6:
                handleMotorbikeListing();
                break;
            case 7:
                handleMotorbikeSearch();
                break;
            case 8:
                handleRentalHistory();
                break;
            case 0:
                handleLogout();
                memberSession = false;
                break;
            default:
                InputHelper::displayError("Invalid choice! Please select 0-8.");
                break;
            }
        }
    }

    void Application::handleAdminMenu() {
        bool adminSession = true;

        while (adminSession) {
            view->displayMainMenu("Admin");

            switch (InputHelper::getMenuChoice(0, 5)) {
            case 1:
                handleViewAllMembers();
                break;
            case 2:
                handleViewAllMotorbikes();
                break;
            case 3:
                handleViewMemberDetails();
                break;
            case 4:
                handleSystemStatistics();
                break;
            case 5:
                handleDataManagement();
                break;
            case 0:
                handleLogout();
                adminSession = false;
                break;
            default:
                InputHelper::displayError("Invalid choice! Please select 0-5.");
                break;
            }
        }
    }

    // ========================================= MEMBER FEATURE HANDLERS ===============================================

    void Application::handleMemberDashboard() const {
        if (const auto member = static_cast<Member*>(dataManager->getAuthManager()->getCurrentUser())) {
            view->displayMemberDashboard(member);
            InputHelper::waitForEnter();
        }
    }

    void Application::handleProfileUpdate() {
        const auto member = static_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        if (!member) return;

        std::string email, phone;

        if (view->getProfileUpdateData(email, phone)) {
            if (member->updateProfile(email, phone)) {
                // Update only this member's file
                dataManager->updateMember(member);
                InputHelper::displaySuccess("Profile updated successfully!");
            } else {
                InputHelper::displayError("Profile update failed! Please check your input format.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handlePasswordChange() {
        const auto member = static_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        if (!member) return;

        std::string oldPass, newPass;

        if (view->getPasswordChangeData(oldPass, newPass)) {
            // Validate new password using AuthManager
            if (!dataManager->getAuthManager()->validatePasswordStrength(newPass)) {
                InputHelper::displayError("New password does not meet strength requirements!");
            } else if (member->changePassword(oldPass, newPass)) {
                // Update this member's file only
                dataManager->updateMember(member);
                InputHelper::displaySuccess("Password updated successfully!");
            } else {
                InputHelper::displayError("Password update failed! Please check your current password.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handleCreditTopUp() {
        const auto member = static_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        if (!member) return;

        InputHelper::displayMessage("Current Credit Points: " + std::to_string(member->getCreditPoints()));

        double amount;
        std::string password;

        if (view->getCreditTopUpData(amount, password)) {
            if (member->topUpCredits(amount, password)) {
                // Update only this member's file
                dataManager->updateMember(member);
                InputHelper::displaySuccess("Successfully topped up " + std::to_string(amount) + " credit points!");
                InputHelper::displayMessage("New balance: " + std::to_string(member->getCreditPoints()) + " CP");
            } else {
                InputHelper::displayError("Top-up failed! Please check your password and amount.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeRegistration() {
        InputHelper::displayMessage("Motorbike Registration feature coming soon!");
        InputHelper::displayMessage("This will allow members to register one electric motorbike.");
        InputHelper::displayMessage("Required info: brand, model, color, engine size, plate number, location");
        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeListing() {
        InputHelper::displayMessage("Motorbike Listing feature coming soon!");
        InputHelper::displayMessage("This will allow motorbike owners to list their bike for rent.");
        InputHelper::displayMessage("Required info: availability period, daily rate, minimum renter rating");
        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeSearch() {
        InputHelper::displayMessage("Motorbike Search feature coming soon!");
        InputHelper::displayMessage("This will allow members to search for available motorbikes.");
        InputHelper::displayMessage("Search criteria: city, date range, engine size, rating requirements");
        InputHelper::waitForEnter();
    }

    void Application::handleRentalHistory() {
        if (const auto member = static_cast<Member*>(dataManager->getAuthManager()->getCurrentUser())) {
            if (const auto history = member->getRentalHistory(); history.empty()) {
                InputHelper::displayMessage("You have no rental history yet.");
            } else {
                view->displayHeader("Rental History");
                std::cout << "You have " << history.size() << " rental records:" << std::endl;
                for (size_t i = 0; i < history.size(); ++i) {
                    std::cout << (i + 1) << ". Rental ID: " << history[i] << std::endl;
                }
                InputHelper::displayMessage("Detailed rental history display coming soon!");
            }
        }
        InputHelper::waitForEnter();
    }

    // ========================================== ADMIN FEATURE HANDLERS ===============================================

    void Application::handleViewAllMembers() const {
        const auto members = dataManager->getAuthManager()->getAllMembers();
        view->displayAllMembers(members);

        InputHelper::displayMessage("Total registered members: " + std::to_string(members.size()));
        InputHelper::waitForEnter();
    }

    void Application::handleViewAllMotorbikes() {
        InputHelper::displayMessage("View All Motorbikes feature coming soon!");
        InputHelper::displayMessage("This will display all registered motorbikes in the system.");
        InputHelper::displayMessage("Including: brand, model, owner, listing status, ratings");
        InputHelper::waitForEnter();
    }

    void Application::handleViewMemberDetails() const {
        const std::string username = InputHelper::getStringInput("Enter member username: ");

        if (const Member* member = dataManager->getAuthManager()->findMemberByUsername(username)) {
            view->displayHeader("Member Details: " + username);
            member->displayInfo();
        } else {
            InputHelper::displayError("Member not found: " + username);
        }

        InputHelper::waitForEnter();
    }

    void Application::handleSystemStatistics() const {
        view->displayHeader("System Statistics");

        const auto members = dataManager->getAuthManager()->getAllMembers();
        const auto admins = dataManager->getAuthManager()->getAllAdmins();

        std::cout << "Total Members: " << members.size() << std::endl;
        std::cout << "Total Admins: " << admins.size() << std::endl;

        // Calculate statistics
        double totalCredits = 0;
        double averageRating = 0;
        int verifiedMembers = 0;
        int membersWithLicense = 0;

        for (const Member* member : members) {
            totalCredits += member->getCreditPoints();
            averageRating += member->getRenterRating();
            if (member->getIsVerified()) verifiedMembers++;
            if (member->isLicenseValid()) membersWithLicense++;
        }

        if (!members.empty()) {
            averageRating /= members.size();
        }

        std::cout << "Total Credit Points in System: " << std::fixed << std::setprecision(1) << totalCredits << std::endl;
        std::cout << "Average Member Rating: " << std::fixed << std::setprecision(2) << averageRating << std::endl;
        std::cout << "Verified Members: " << verifiedMembers << " (" <<
                     (members.empty() ? 0 : (verifiedMembers * 100 / members.size())) << "%)" << std::endl;
        std::cout << "Members with Valid License: " << membersWithLicense << " (" <<
                     (members.empty() ? 0 : (membersWithLicense * 100 / members.size())) << "%)" << std::endl;

        InputHelper::displayMessage("More detailed statistics coming soon!");
        InputHelper::waitForEnter();
    }

    void Application::handleDataManagement() {
        view->displayHeader("Data Management");

        std::cout << "1. Save All Data Now" << std::endl;
        std::cout << "2. Validate Data Consistency" << std::endl;
        std::cout << "3. Cleanup Orphaned Files" << std::endl;
        std::cout << "4. View Data Status" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;

        switch (InputHelper::getMenuChoice(0, 4)) {
            case 1:
                if (confirmCriticalAction("save all data")) {
                    if (dataManager->saveAllData()) {
                        InputHelper::displaySuccess("All data saved successfully!");
                    } else {
                        InputHelper::displayError("Some data could not be saved!");
                    }
                }
                break;
            case 2:
                if (dataManager->validateDataConsistency()) {
                    InputHelper::displaySuccess("Data consistency validation passed!");
                } else {
                    InputHelper::displayError("Data consistency issues found!");
                }
                break;
            case 3:
                if (confirmCriticalAction("cleanup orphaned files")) {
                    dataManager->cleanupOrphanedFiles();
                    InputHelper::displaySuccess("Orphaned files cleanup completed!");
                }
                break;
            case 4: {
                const auto memberFiles = dataManager->listMembers();
                const auto adminFiles = dataManager->listAdmins();
                std::cout << "Member files: " << memberFiles.size() << std::endl;
                std::cout << "Admin files: " << adminFiles.size() << std::endl;
                InputHelper::displayMessage("Data files are stored in individual files for efficient updates.");
                break;
            }
            case 0:
                return;
        }

        InputHelper::waitForEnter();
    }

    // ============================================ UTILS METHODS ======================================================

    void Application::exitApplication() {
        if (confirmCriticalAction("exit the application")) {
            isRunning = false;
        }
    }

    void Application::displaySystemInfo() const {
        const auto members = dataManager->getAuthManager()->getAllMembers();
        const auto admins = dataManager->getAuthManager()->getAllAdmins();

        std::cout << "System Status:" << std::endl;
        std::cout << "- Members loaded: " << members.size() << std::endl;
        std::cout << "- Admins available: " << admins.size() << std::endl;
        std::cout << "- Data management: Individual file strategy (efficient updates)" << std::endl;
        std::cout << "- Default admin account: username 'admin', password '12345'" << std::endl;
        std::cout << std::endl;
    }

    bool Application::confirmCriticalAction(const std::string& action) {
        return InputHelper::confirmAction("Are you sure you want to " + action + "?");
    }
}
