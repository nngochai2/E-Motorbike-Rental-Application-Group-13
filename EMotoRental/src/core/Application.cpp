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

        ConsoleView::displayWelcomeScreen();

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
        switch (ConsoleView::getUserTypeChoice()) {
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
            const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
            InputHelper::displaySuccess("Welcome back, " + member->getFullName() + "!");
            ConsoleView::displayMemberDashboard(member);
            handleMemberMenu();
        }
        // If login failed, return to main menu
    }

    void Application::handleAdminFlow() {
        if (handleAdminLogin()) {
            // Admin successfully logged in
            const auto admin = dynamic_cast<Admin*>(dataManager->getAuthManager()->getCurrentUser());
            InputHelper::displaySuccess("Admin access granted. Welcome, " + admin->getFullName() + "!");
            handleAdminMenu();
        }
    }

    // ========================================== AUTHENTICATION FLOWS =================================================

    bool Application::handleMemberLogin() const {
        const std::string username = ConsoleView::getUsername();
        const std::string password = ConsoleView::getPassword();

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
        const std::string username = ConsoleView::getUsername();
        const std::string password = ConsoleView::getPassword();

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

        if (ConsoleView::getRegistrationData(username, password, fullName, email, phone)) {
            if (AuthManager* authManager = dataManager->getAuthManager(); authManager->registerMember(username, password, fullName, email, phone)) {
                // Efficiently save the new member to an individual file
                if (const Member* newMember = authManager->findMemberByUsername(username)) {
                    DataManager::saveMember(newMember);
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

    void Application::handleGuestMenu() const {
        bool guestSession = true;

        while (guestSession) {
            ConsoleView::displayMainMenu("Guest");

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
            ConsoleView::displayMainMenu("Member");

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
            ConsoleView::displayMainMenu("Admin");

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
        if (const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser())) {
            ConsoleView::displayMemberDashboard(member);
            InputHelper::waitForEnter();
        }
    }

    void Application::handleProfileUpdate() {
        const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        if (!member) return;

        std::string email, phone;

        if (ConsoleView::getProfileUpdateData(email, phone)) {
            if (member->updateProfile(email, phone)) {
                // Update only this member's file
                DataManager::updateMember(member);
                InputHelper::displaySuccess("Profile updated successfully!");
            } else {
                InputHelper::displayError("Profile update failed! Please check your input format.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handlePasswordChange() {
        const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        if (!member) return;

        std::string oldPass, newPass;

        if (ConsoleView::getPasswordChangeData(oldPass, newPass)) {
            // Validate new password using AuthManager
            if (!AuthManager::validatePasswordStrength(newPass)) {
                InputHelper::displayError("New password does not meet strength requirements!");
            } else if (member->changePassword(oldPass, newPass)) {
                // Update this member's file only
                DataManager::updateMember(member);
                InputHelper::displaySuccess("Password updated successfully!");
            } else {
                InputHelper::displayError("Password update failed! Please check your current password.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handleCreditTopUp() {
        const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        if (!member) return;

        InputHelper::displayMessage("Current Credit Points: " + std::to_string(member->getCreditPoints()));

        double amount;
        std::string password;

        if (ConsoleView::getCreditTopUpData(amount, password)) {
            if (member->topUpCredits(amount, password)) {
                // Update only this member's file
                DataManager::updateMember(member);
                InputHelper::displaySuccess("Successfully topped up " + std::to_string(amount) + " credit points!");
                InputHelper::displayMessage("New balance: " + std::to_string(member->getCreditPoints()) + " CP");
            } else {
                InputHelper::displayError("Top-up failed! Please check your password and amount.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeRegistration() {
        if (!isUserLoggedIn() || isMember()) {
            InputHelper::displayError("Please log in as a member to register a motorbike.");
            return;
        }

        const auto currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());

        // Check if member already owns a motorbike
        if (!dataManager->getMotorbikeManager()->canOwnerRegisterMotorbike(currentMember->getUsername())) {
            InputHelper::displayError("You already own a motorbike. Each member can only register one motorbike.");
            InputHelper::waitForEnter();
            return;
        }

        ConsoleView::displayHeader("Motorbike Registration");

        try {
            // Get motorbike details from user
            std::string brand = InputHelper::getStringInput("Enter brand: ");
            std::string model = InputHelper::getStringInput("Enter model: ");
            std::string color = InputHelper::getStringInput("Enter color: ");

            int engineSize =InputHelper::getIntInput("Enter engine size (cc): ");
            // Engine size validation
            if (engineSize <= 0) {
                InputHelper::displayError("Engine size must be greater than 0.");
                InputHelper::waitForEnter();
                return;
            }

            int yearMade = InputHelper::getIntInput("Enter year made: ");

            std::string licensePlate = InputHelper::getStringInput("Enter license plate: ");

            // City validation (project requirement: HCMC or Hanoi only)
            std::string city;
            while (true) {
                city = InputHelper::getStringInput("Enter city (HCMC/Hanoi): ");
                if (city == "HCMC" || city == "Hanoi") {
                    break;
                }
                InputHelper::displayError("City must be either 'HCMC' or 'Hanoi'.");
            }

            // Create registration data
            MotorbikeRegistrationData regData(brand, model, color, engineSize, yearMade, licensePlate, city,
                                        currentMember->getUsername());

            // Register and save through DataManager
            if (dataManager->registerAndSaveMotorbike(regData)) {
                InputHelper::displaySuccess("Motorbike registered successfully");

                // Update member's owned motorbike license plate
                currentMember->setOwnedMotorbikeId(licensePlate);
                dataManager->updateMember(currentMember);

                // Display registered motorbike details
                if (const Motorbike* newBike = dataManager->getMotorbikeManager()->getMotorbikeByLicensePlate(licensePlate)) {
                    newBike->displayDetails();
                }
            } else {
                InputHelper::displayError("Failed to register motorbike. Please try again.");
            }

        } catch (const std::exception& e) {
            InputHelper::displayError("Failed to register motorbike. Please try again.");
        }

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

    void Application::handleRentalHistory() const {
        if (const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser())) {
            if (const auto history = member->getRentalHistory(); history.empty()) {
                InputHelper::displayMessage("You have no rental history yet.");
            } else {
                ConsoleView::displayHeader("Rental History");
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
        ConsoleView::displayAllMembers(members);

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
            ConsoleView::displayHeader("Member Details: " + username);
            member->displayInfo();
        } else {
            InputHelper::displayError("Member not found: " + username);
        }

        InputHelper::waitForEnter();
    }

    void Application::handleSystemStatistics() const {
        ConsoleView::displayHeader("System Statistics");

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

    void Application::handleDataManagement() const {
        ConsoleView::displayHeader("Data Management");

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
                if (DataManager::validateDataConsistency()) {
                    InputHelper::displaySuccess("Data consistency validation passed!");
                } else {
                    InputHelper::displayError("Data consistency issues found!");
                }
                break;
            case 3:
                if (confirmCriticalAction("cleanup orphaned files")) {
                    DataManager::cleanupOrphanedFiles();
                    InputHelper::displaySuccess("Orphaned files cleanup completed!");
                }
                break;
            case 4: {
                const auto memberFiles = DataManager::listMembers();
                const auto adminFiles = DataManager::listAdmins();
                std::cout << "Member files: " << memberFiles.size() << std::endl;
                std::cout << "Admin files: " << adminFiles.size() << std::endl;
                InputHelper::displayMessage("Data files are stored in individual files for efficient updates.");
                break;
            }
            case 0:
                return;
            default: ;
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

    // =========================================== HELPER METHODS ======================================================

    bool Application::isUserLoggedIn() const {
        return dataManager->getAuthManager()->getCurrentUser() != nullptr;
    }

    bool Application::isMember() const {
        if (!isUserLoggedIn()) return false;
        return dataManager->getAuthManager()->getCurrentUser()->getUserType() == "Member";
    }

    bool Application::isAdmin() const {
        if (!isUserLoggedIn()) return false;
        return dataManager->getAuthManager()->getCurrentUser()->getUserType() == "Admin";
    }
}
