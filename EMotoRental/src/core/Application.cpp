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

        // Process any overdue rentals from previous sessions
        checkAndProcessOverdueRentals();

        while (isRunning) {
            try {
                handleUserTypeSelection();
                // The key: isRunning can be changed by exitApplication()
                // The while condition will be checked on next iteration
            }
            catch (const std::exception& e) {
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
            if (AuthManager* authManager = dataManager->getAuthManager(); authManager->registerMember(
                username, password, fullName, email, phone)) {
                // Efficiently save the new member to an individual file
                if (const Member* newMember = authManager->findMemberByUsername(username)) {
                    DataManager::saveMember(newMember);
                }

                InputHelper::displaySuccess("Registration successful! You can now login with your credentials.");
                InputHelper::displayMessage("You have been awarded 20 credit points and a default rating of 3.0");
            }
            else {
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
                handleGuestMotorbikeBrowsing();
                break;
            case 2:
                handleGuestLocationFilter();
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
        bool hasCheckedCompletions = false; // Flag

        while (memberSession) {
            ConsoleView::displayMainMenu("Member");

            if (!hasCheckedCompletions) { // Only check once per session
                checkForRentalCompletions();
                hasCheckedCompletions = true;
            }

            switch (InputHelper::getMenuChoice(0, 9)) {
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
                handleIdentityVerification();
                break;
            case 6:
                handleMotorbikeManagement();
                break;
            case 7:
                handleMotorbikeListing();
                break;
            case 8:
                handleMotorbikeSearch();
                break;
            case 9:
                handleRentalMenu();
                break;
            case 0:
                handleLogout();
                memberSession = false;
                break;
            default:
                InputHelper::displayError("Invalid choice! Please select 0-9.");
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

    // ========================================= GUEST FEATURE HANDLERS ================================================

    void Application::handleGuestMotorbikeBrowsing() const {
        ConsoleView::displayMainMenu("Available Motorbikes");

        // Get all listed motorbikes from MotorbikeManager
        if (!dataManager->getMotorbikeManager()) {
            InputHelper::displayError("Motorbike system not available.");
            InputHelper::waitForEnter();
            return;
        }

        auto listedMotorbikes = dataManager->getMotorbikeManager()->getAllListedMotorbikes();

        if (listedMotorbikes.empty()) {
            InputHelper::displayMessage("No motorbikes currently available for rent.");
            InputHelper::waitForEnter();
            return;
        }

        std::cout << "Available Motorbikes (" << listedMotorbikes.size() << " listings):" << std::endl;
        std::cout << "Note: Register as a member to see detailed information and make bookings." << std::endl;
        std::cout << std::endl;

        // Display limited info for guests
        for (size_t i = 0; i < listedMotorbikes.size(); ++i) {
            std::cout << "--- Listing #" << (i + 1) << " ---" << std::endl;
            displayGuestMotorbikeInfo(listedMotorbikes[i]);
            std::cout << std::endl;
        }

        InputHelper::displayMessage("To see full details, rates, and availability, please register as a member.");
        InputHelper::waitForEnter();
    }

    void Application::handleGuestLocationFilter() const {
        ConsoleView::displayHeader("Filter by Location");

        if (!dataManager->getMotorbikeManager()) {
            InputHelper::displayError("Motorbike system not available.");
            InputHelper::waitForEnter();
            return;
        }

        std::cout << "Available Cities:" << std::endl;
        std::cout << "1. HCMC" << std::endl;
        std::cout << "2. Hanoi" << std::endl;
        std::cout << "0. Back to Guest Menu" << std::endl;

        int choice = InputHelper::getMenuChoice(0, 2);

        std::string selectedCity;
        switch (choice) {
        case 1:
            selectedCity = "HCMC";
            break;
        case 2:
            selectedCity = "Hanoi";
            break;
        case 0:
            return;
        default:
            InputHelper::displayError("Invalid choice.");
            InputHelper::waitForEnter();
            return;
        }

        // Get all motorbikes in the selected city
        const auto allMotorbikes = dataManager->getMotorbikeManager()->getAllListedMotorbikes();
        std::vector<Motorbike*> cityMotorbikes;

        for (Motorbike* bike : allMotorbikes) {
            if (bike->getCity() == selectedCity) {
                cityMotorbikes.push_back(bike);
            }
        }

        std::cout << std::endl;
        std::cout << "Motorbikes in " << selectedCity << ":" << std::endl;

        if (cityMotorbikes.empty()) {
            InputHelper::displayMessage("No motorbikes available in " + selectedCity + ".");
        }
        else {
            std::cout << "Found " << cityMotorbikes.size() << " motorbike(s) in " << selectedCity << ":" << std::endl;
            std::cout << std::endl;

            for (size_t i = 0; i < cityMotorbikes.size(); ++i) {
                std::cout << "--- " << selectedCity << " Listing #" << (i + 1) << " ---" << std::endl;
                displayGuestMotorbikeInfo(cityMotorbikes[i]);
                std::cout << std::endl;
            }

            InputHelper::displayMessage("Register as a member to see full details and make bookings!");
        }

        InputHelper::waitForEnter();
    }

    // ========================================= MEMBER FEATURE HANDLERS ===============================================

    void Application::handleMemberDashboard() {
        if (const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser())) {
            ConsoleView::displayMemberDashboard(member,
                                                dataManager->getRentalManager(),
                                                dataManager->getMotorbikeManager());
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
            }
            else {
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
            }
            else if (member->changePassword(oldPass, newPass)) {
                // Update this member's file only
                DataManager::updateMember(member);
                InputHelper::displaySuccess("Password updated successfully!");
            }
            else {
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
            }
            else {
                InputHelper::displayError("Top-up failed! Please check your password and amount.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeRegistration() {
        if (!isUserLoggedIn() || !isMember()) {
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

            int engineSize = InputHelper::getIntInput("Enter engine size (cc): ");
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
            const MotorbikeRegistrationData regData(brand, model, color, engineSize, yearMade, licensePlate, city,
                                                    currentMember->getUsername());

            // Register and save through DataManager
            if (dataManager->registerAndSaveMotorbike(regData)) {
                InputHelper::displaySuccess("Motorbike registered successfully");

                // Update member's owned motorbike license plate
                currentMember->setOwnedMotorbikeId(licensePlate);
                DataManager::updateMember(currentMember);

                // Display registered motorbike details
                if (const Motorbike* newBike = dataManager->getMotorbikeManager()->getMotorbikeByLicensePlate(
                    licensePlate)) {
                    newBike->displayDetails();
                }
            }
            else {
                InputHelper::displayError("Failed to register motorbike. Please try again.");
            }
        }
        catch ([[maybe_unused]] const std::exception& e) {
            InputHelper::displayError("Failed to register motorbike. Please try again.");
        }

        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeListing() const {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member to list a motorbike.");
            return;
        }

        const auto currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());

        // Check if member owns a motorbike
        const Motorbike* ownedBike = dataManager->getMotorbikeManager()->getMotorbikeByOwner(
            currentMember->getUsername());
        if (!ownedBike) {
            InputHelper::displayError("You don't own a motorbike. Please register one first.");
            InputHelper::waitForEnter();
            return;
        }

        ConsoleView::displayHeader("List Motorbike for Rent");

        // Display current motorbike details
        std::cout << "Your motorbike: " << std::endl;
        ownedBike->displayDetails();

        if (ownedBike->getIsListed()) {
            std::cout << "This motorbike is already listed for rent." << std::endl;
            if (InputHelper::confirmAction("Do you want to update the listing?")) {
                // Unlist first, then re-list with new data
                dataManager->unlistAndSaveMotorbike(ownedBike->getLicensePlate());
            }
            else {
                InputHelper::waitForEnter();
                return;
            }
        }

        try {
            // Get listing details from user
            DateUtil::TimePoint startDate = InputHelper::getDateInput("Enter availability start date");
            DateUtil::TimePoint endDate = InputHelper::getDateInput("Enter availability end date");

            double dailyRate = InputHelper::getDoubleInput("Enter daily rate (in CP): ");
            if (dailyRate <= 0) {
                InputHelper::displayError("Daily rate must be greater than 0.");
                InputHelper::waitForEnter();
                return;
            }

            double minRating = InputHelper::getDoubleInput("Enter minimum rating (1.0-5.0): ");
            if (minRating < 1.0 || minRating > 5.0) {
                InputHelper::displayError("Minimum rating must be between 1.0 and 5.0.");
                InputHelper::waitForEnter();
                return;
            }

            // Create listing data
            MotorbikeListingData listingData(startDate, endDate, dailyRate, minRating);

            // List and save through DataManager
            if (dataManager->listAndSaveMotorbike(ownedBike->getLicensePlate(), listingData)) {
                InputHelper::displaySuccess("Motorbike listed for rent successfully!");
                ownedBike->displayPublicInfo();
            }
            else {
                InputHelper::displayError("Failed to list motorbike. Please try again.");
            }
        }
        catch (const std::exception& e) {
            InputHelper::displayError("Listing failed: " + std::string(e.what()));
        }

        InputHelper::waitForEnter();
    }

    void Application::handleMotorbikeSearch() const {
        ConsoleView::displayHeader("Search Motorbikes");

        try {
            // Get search criteria from user
            std::string city;
            while (true) {
                city = InputHelper::getStringInput("Enter city to search (HCMC/Hanoi): ");
                if (city == "HCMC" || city == "Hanoi") {
                    break;
                }
                InputHelper::displayError("City must be either 'HCMC' or 'Hanoi'.");
            }

            DateUtil::TimePoint startDate = InputHelper::getDateInput("Enter rental start date");
            DateUtil::TimePoint endDate = InputHelper::getDateInput("Enter rental end date");

            // Search motorbikes
            auto results = dataManager->getMotorbikeManager()->searchMotorbikes(city, startDate, endDate);

            if (results.empty()) {
                InputHelper::displayMessage("No motorbikes found matching your search criteria.");
                InputHelper::waitForEnter();
                return;
            }

            std::cout << "\n========== SEARCH RESULTS ==========" << std::endl;
            std::cout << "Found " << results.size() << " available motorbikes:" << std::endl;

            for (size_t i = 0; i < results.size(); i++) {
                std::cout << "\n--- Result #" << (i + 1) << " ---" << std::endl;

                if (isUserLoggedIn() && isMember()) {
                    // Members see full info
                    results[i]->displayPublicInfo();

                    // Check if member meets requirements
                    if (auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser()); results[i]->meetsRequirement(*currentMember)) {
                        std::cout << "✓ You meet the requirements for this motorbike." << std::endl;
                    }
                    else {
                        std::cout << "✗ You don't meet the requirements for this motorbike." << std::endl;
                    }
                }
                else {
                    // Guest see limited info only
                    displayGuestMotorbikeInfo(results[i]);
                }
            }

            std::cout << "====================================" << std::endl;
        }
        catch (const std::exception& e) {
            InputHelper::displayError("Search failed: " + std::string(e.what()));
        }

        InputHelper::waitForEnter();
    }

    void Application::displayGuestMotorbikeInfo(const Motorbike* bike) {
        // Project requirement: Guests can only see brand, model, engine size, location
        std::cout << "Brand: " << bike->getBrand() << " | Model: " << bike->getModel() << std::endl;
        std::cout << "Engine: " << bike->getEngineSize() << "cc | City: " << bike->getCity() << std::endl;
        std::cout << "Year: " << bike->getYearMade() << std::endl;
        std::cout << "Status: Available for rent" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        // Guests CANNOT see:
        // - Ratings and reviews
        // - Daily rate
        // - Owner information
        // - Availability dates
        // - Minimum renter rating requirements
    }

    void Application::handleMotorbikeManagement() {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member.");
            return;
        }

        const auto currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        const Motorbike* ownedBike = dataManager->getMotorbikeManager()->getMotorbikeByOwner(
            currentMember->getUsername());

        ConsoleView::displayHeader("Motorbike Management");

        if (!ownedBike) {
            std::cout << "You don't own a motorbike yet." << std::endl;
            std::cout << "1. Register Motorbike" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;

            if (int choice = InputHelper::getMenuChoice(0, 1); choice == 1) {
                handleMotorbikeRegistration();
            }
            return;
        }

        // Show current motorbike status
        std::cout << "Your Motorbike:" << std::endl;
        ownedBike->displayDetails();

        std::cout << "\nManagement Options:" << std::endl;
        std::cout << "1. " << (ownedBike->getIsListed() ? "Update Listing" : "List for Rent") << std::endl;
        if (ownedBike->getIsListed()) {
            std::cout << "2. Unlist Motorbike" << std::endl;
        }
        std::cout << "3. View Motorbike Details" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;

        int maxChoice = 3; // FIXED: Always allow up to option 3
        int choice = InputHelper::getMenuChoice(0, maxChoice);

        switch (choice) {
        case 1:
            handleMotorbikeListing();
            break;
        case 2:
            if (ownedBike->getIsListed()) {
                if (InputHelper::confirmAction("Are you sure you want to unlist your motorbike?")) {
                    if (dataManager->unlistAndSaveMotorbike(ownedBike->getLicensePlate())) {
                        InputHelper::displaySuccess("Motorbike unlisted successfully!");
                    }
                    else {
                        InputHelper::displayError("Failed to unlist motorbike.");
                    }
                    InputHelper::waitForEnter();
                }
            }
            else {
                // ADDED: Error message for invalid choice
                InputHelper::displayError("Cannot unlist - motorbike is not currently listed.");
                InputHelper::waitForEnter();
            }
            break;
        case 3:
            ownedBike->displayDetails();
            InputHelper::waitForEnter();
            break;
        default:
            // Default case handles invalid choices automatically by InputHelper::getMenuChoice
            break;
        }
    }

    void Application::handleRentalHistory() const {
        if (const auto member = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser())) {
            // Use RentalManager
            if (const auto rentalHistory = dataManager->getRentalManager()->getRentalHistory(member->getUsername());
                rentalHistory.empty()) {
                InputHelper::displayMessage("You have no rental history yet.");
            }
            else {
                dataManager->getRentalManager()->displayRentalHistory(member->getUsername());
            }
        }
        InputHelper::waitForEnter();
    }

    void Application::handleIdentityVerification() const {
        const auto* currentUser = dataManager->getAuthManager()->getCurrentUser();
        const auto* member = dynamic_cast<const Member*>(currentUser);

        if (!member) {
            InputHelper::displayError("Member authentication required!");
            return;
        }

        if (member->getIsVerified()) {
            ConsoleView::displayVerificationResult(true, "Account already verified.");
            InputHelper::waitForEnter();
            return;
        }

        std::string idNumber, licenseNumber, idType;

        if (ConsoleView::getVerificationData(idNumber, licenseNumber, idType)) {
            bool success = dataManager->getAuthManager()->verifyMemberIdentity(
                member->getUsername(), idNumber, licenseNumber, idType);

            ConsoleView::displayVerificationResult(success);

            if (success) {
                // Save updated member data
                DataManager::saveMember(member);
                std::cout << "Your verification status has been saved.\n";
            }
        }

        InputHelper::waitForEnter();
    }

    // ========================================= RENTAL FEATURE HANDLERS ===============================================

    void Application::handleRentalMenu() {
        bool rentalMenuActive = true;
        while (rentalMenuActive) {
            ConsoleView::displayRentalMenu();

            switch (InputHelper::getMenuChoice(0, 5)) {
            case 1:
                handleCreateRentalRequest();
                break;
            case 2:
                handleViewRentalRequests();
                break;
            case 3:
                handleApproveRentalRequest();
                break;
            case 4:
                handleViewActiveRentals();
                break;
            case 5:
                handleRateRental();
                break;
            case 0:
                rentalMenuActive = false;
                break;
            default:
                InputHelper::displayError("Invalid choice! Please select 0-5.");
                break;
            }
        }
    }

    void Application::handleCreateRentalRequest() {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member.");
            return;
        }

        auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());

        // ENFORCE RATING REQUIREMENT
        if (!checkRatingRequirements(currentMember)) {
            InputHelper::waitForEnter();
            return;
        }

        ConsoleView::displayHeader("Create Rental Request");

        try {
            // Get search criteria
            std::string city;
            while (true) {
                city = InputHelper::getStringInput("Enter city (HCMC/Hanoi): ");
                if (city == "HCMC" || city == "Hanoi") {
                    break;
                }
                InputHelper::displayError("City must be either 'HCMC' or 'Hanoi'.");
            }

            DateUtil::TimePoint startDate = InputHelper::getDateInput("Enter rental start date");
            DateUtil::TimePoint endDate = InputHelper::getDateInput("Enter rental end date");

            if (startDate >= endDate) {
                InputHelper::displayError("End date must be after start date.");
                return;
            }

            // Search motorbikes using existing logic
            auto results = dataManager->getMotorbikeManager()->searchMotorbikes(city, startDate, endDate);

            if (results.empty()) {
                InputHelper::displayMessage("No motorbikes available for your criteria.");
                InputHelper::waitForEnter();
                return;
            }

            // Display results using existing display logic (similar to handleMotorbikeSearch)
            std::cout << "\n========== AVAILABLE MOTORBIKES ==========" << std::endl;
            std::cout << "Found " << results.size() << " available motorbikes:" << std::endl;

            for (size_t i = 0; i < results.size(); i++) {
                std::cout << "\n--- Option #" << (i + 1) << " ---" << std::endl;
                results[i]->displayDetails();

                // Show cost calculation
                int days = DateUtil::daysBetween(startDate, endDate);
                double cost = results[i]->calculateCost(days);
                std::cout << "Total Cost for " << days << " days: " << std::fixed
                    << std::setprecision(2) << cost << " CP" << std::endl;
            }
            std::cout << "===========================================" << std::endl;

            // Let user select
            std::string selectedPlate = InputHelper::getStringInput("Enter license plate to rent: ");

            // Validate selection
            Motorbike* selectedBike = nullptr;
            for (Motorbike* bike : results) {
                if (bike->getLicensePlate() == selectedPlate) {
                    selectedBike = bike;
                    break;
                }
            }

            if (!selectedBike) {
                InputHelper::displayError("Invalid license plate.");
                return;
            }

            // Create and submit request
            RentalRequestData requestData(currentMember->getUsername(), selectedPlate, startDate, endDate);

            if (dataManager->getRentalManager()->createRentalRequest(requestData,
                                                                     dataManager->getMotorbikeManager(),
                                                                     dataManager->getAuthManager())) {
                InputHelper::displaySuccess("Rental request submitted successfully!");
                InputHelper::displayMessage("The motorbike owner will review your request.");
            }
            else {
                InputHelper::displayError("Failed to create rental request.");
            }
        }
        catch (const std::exception& e) {
            InputHelper::displayError("Request failed: " + std::string(e.what()));
        }

        InputHelper::waitForEnter();
    }

    void Application::handleViewRentalRequests() const {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member.");
            return;
        }

        const auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());

        // Show requests BY this member
        const auto myRequests = dataManager->getRentalManager()->getRequestsFromRenter(currentMember->getUsername());
        ConsoleView::displayHeader("Your Rental Requests");

        if (myRequests.empty()) {
            std::cout << "You have no rental requests." << std::endl;
        }
        else {
            for (size_t i = 0; i < myRequests.size(); ++i) {
                std::cout << "\n--- Request #" << (i + 1) << " ---" << std::endl;
                myRequests[i]->displayRequest();
            }
        }

        // Show requests FOR this member's motorbike
        if (dataManager->getMotorbikeManager()->getMotorbikeByOwner(currentMember->getUsername())) {
            std::cout << "\n" << std::endl;
            dataManager->getRentalManager()->displayRequestsForOwner(currentMember->getUsername(),
                                                                     dataManager->getMotorbikeManager());
        }

        InputHelper::waitForEnter();
    }

    void Application::handleApproveRentalRequest() {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member.");
            return;
        }

        const auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());

        // Check if member owns a motorbike
        Motorbike* ownedBike = dataManager->getMotorbikeManager()->getMotorbikeByOwner(currentMember->getUsername());
        if (!ownedBike) {
            InputHelper::displayError("You don't own a motorbike.");
            return;
        }

        ConsoleView::displayHeader("Approve Rental Requests");

        // Show pending requests using RentalManager's display method
        dataManager->getRentalManager()->displayRequestsForOwner(currentMember->getUsername(),
                                                                 dataManager->getMotorbikeManager());

        std::string requestId = InputHelper::getStringInput("Enter Request ID to approve (or 'cancel'): ");

        if (requestId == "cancel") {
            return;
        }

        if (InputHelper::confirmAction("Do you want to approve this request?")) {
            if (dataManager->getRentalManager()->approveRequest(requestId,
                                                                dataManager->getMotorbikeManager(),
                                                                dataManager->getAuthManager())) {
                InputHelper::displaySuccess("Request approved successfully!");
            }
            else {
                InputHelper::displayError("Failed to approve request.");
            }
        }

        InputHelper::waitForEnter();
    }

    void Application::handleViewActiveRentals() {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member.");
            return;
        }

        const auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        ConsoleView::displayHeader("Active Rentals");

        // Show active rentals (read-only)

        if (auto activeRentals = dataManager->getRentalManager()->getActiveRentalsForMember(
            currentMember->getUsername()); activeRentals.empty()) {
            InputHelper::displayMessage("You have no active rentals.");
        }
        else {
            std::cout << "Your active rentals:" << std::endl;
            for (size_t i = 0; i < activeRentals.size(); ++i) {
                std::cout << "\n--- Rental #" << (i + 1) << " ---" << std::endl;
                activeRentals[i]->displayInfo();

                // Show if overdue
                if (activeRentals[i]->isOverdue()) {
                    std::cout << "OVERDUE - Rental period has ended!" << std::endl;
                }
            }

            InputHelper::displayMessage("Note: Rentals automatically complete when the rental period ends.");
        }

        InputHelper::waitForEnter();
    }

    void Application::handleRateRental() {
        if (!isUserLoggedIn() || !isMember()) {
            InputHelper::displayError("Please log in as a member.");
            return;
        }

        const auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());
        ConsoleView::displayHeader("Rate Rental Experience");

        // Get completed rentals
        const auto rentalHistory = dataManager->getRentalManager()->getRentalHistory(currentMember->getUsername());

        std::vector<Rental*> completedRentals;
        for (Rental* rental : rentalHistory) {
            if (rental->getStatus() == RentalStatus::COMPLETED) {
                completedRentals.push_back(rental);
            }
        }

        if (completedRentals.empty()) {
            InputHelper::displayMessage("No completed rentals to rate.");
            InputHelper::waitForEnter();
            return;
        }

        // Show unrated rentals first
        std::vector<Rental*> unratedRentals;
        for (Rental* rental : completedRentals) {
            bool needsRating = false;

            if (rental->getRenterUsername() == currentMember->getUsername()) {
                // Renter needs to rate motorbike
                auto existingRatings = dataManager->getRentalManager()->getRatingsForEntity(
                    rental->getMotorbikeLicensePlate(), RatingType::MOTORBIKE_RATING);

                bool alreadyRated = false;
                for (const Rating* rating : existingRatings) {
                    if (rating->getRentalId() == rental->getRentalId() &&
                        rating->getReviewerId() == currentMember->getUsername()) {
                        alreadyRated = true;
                        break;
                    }
                }
                if (!alreadyRated) needsRating = true;
            }

            if (rental->getOwnerUsername() == currentMember->getUsername()) {
                // Owner needs to rate renter
                auto existingRatings = dataManager->getRentalManager()->getRatingsForEntity(
                    rental->getRenterUsername(), RatingType::RENTER_RATING);

                bool alreadyRated = false;
                for (Rating* rating : existingRatings) {
                    if (rating->getRentalId() == rental->getRentalId() &&
                        rating->getReviewerId() == currentMember->getUsername()) {
                        alreadyRated = true;
                        break;
                    }
                }
                if (!alreadyRated) needsRating = true;
            }

            if (needsRating) {
                unratedRentals.push_back(rental);
            }
        }

        if (unratedRentals.empty()) {
            InputHelper::displayMessage("All completed rentals have been rated. Thank you!");
            InputHelper::waitForEnter();
            return;
        }

        // Show warning if there are unrated rentals
        if (!unratedRentals.empty()) {
            std::cout << "You have " << unratedRentals.size()
                << " completed rental(s) that require rating!" << std::endl;
            std::cout << "You must rate all completed rentals to continue using the system." << std::endl;
            std::cout << std::endl;
        }

        // Display rentals that need rating
        std::cout << "Completed rentals requiring your rating:" << std::endl;
        for (size_t i = 0; i < unratedRentals.size(); ++i) {
            std::cout << "\n--- Rental #" << (i + 1) << " (NEEDS RATING) ---" << std::endl;
            unratedRentals[i]->displayInfo();
        }

        std::string rentalId = InputHelper::getStringInput("Enter Rental ID to rate: ");

        // Find the rental
        Rental* rental = dataManager->getRentalManager()->getRentalById(rentalId);
        if (!rental || rental->getStatus() != RentalStatus::COMPLETED) {
            InputHelper::displayError("Invalid rental or rental not completed.");
            return;
        }

        // Determine what needs to be rated
        bool isRenter = (rental->getRenterUsername() == currentMember->getUsername());
        bool isOwner = (rental->getOwnerUsername() == currentMember->getUsername());

        if (!isRenter && !isOwner) {
            InputHelper::displayError("You are not part of this rental.");
            return;
        }

        int stars;
        std::string comment;
        if (ConsoleView::getRatingData(stars, comment)) {
            bool success = false;

            if (isRenter) {
                // Renter rates the motorbike
                success = dataManager->getRentalManager()->createRating(currentMember->getUsername(),
                                                                        rental->getMotorbikeLicensePlate(), stars,
                                                                        comment,
                                                                        rentalId,
                                                                        RatingType::MOTORBIKE_RATING,
                                                                        dataManager->getMotorbikeManager(),
                                                                        dataManager->getAuthManager());

                if (success) {
                    InputHelper::displaySuccess("Motorbike rating submitted!");
                }
            }

            if (isOwner) {
                // Owner rates the renter
                success = dataManager->getRentalManager()->createRating(currentMember->getUsername(),
                                                                        rental->getRenterUsername(), stars, comment,
                                                                        rentalId,
                                                                        RatingType::RENTER_RATING,
                                                                        dataManager->getMotorbikeManager(),
                                                                        dataManager->getAuthManager());

                if (success) {
                    InputHelper::displaySuccess("Renter rating submitted!");
                }
            }

            if (!success) {
                InputHelper::displayError("Failed to submit rating.");
            }
        }


        InputHelper::waitForEnter();
    }

    void Application::checkAndProcessOverdueRentals() {
        if (dataManager && dataManager->getRentalManager()) {
            dataManager->getRentalManager()->processOverdueRental();
        }
    }

    bool Application::checkRatingRequirements(Member* member) {
        if (!member) return false;

        // Get all completed rentals for this member

        for (const auto rentalHistory = dataManager->getRentalManager()->getRentalHistory(member->getUsername()); const Rental
             * rental : rentalHistory) {
            if (rental->getStatus() != RentalStatus::COMPLETED) continue;

            bool needsRating = false;

            if (rental->getRenterUsername() == member->getUsername()) {
                // Check if renter rated the motorbike
                auto existingRatings = dataManager->getRentalManager()->getRatingsForEntity(
                    rental->getMotorbikeLicensePlate(), RatingType::MOTORBIKE_RATING);

                bool alreadyRated = false;
                for (Rating* rating : existingRatings) {
                    if (rating->getRentalId() == rental->getRentalId() &&
                        rating->getReviewerId() == member->getUsername()) {
                        alreadyRated = true;
                        break;
                    }
                }
                if (!alreadyRated) needsRating = true;
            }

            if (rental->getOwnerUsername() == member->getUsername()) {
                // Check if owner rated the renter
                auto existingRatings = dataManager->getRentalManager()->getRatingsForEntity(
                    rental->getRenterUsername(), RatingType::RENTER_RATING);

                bool alreadyRated = false;
                for (const Rating* rating : existingRatings) {
                    if (rating->getRentalId() == rental->getRentalId() &&
                        rating->getReviewerId() == member->getUsername()) {
                        alreadyRated = true;
                        break;
                    }
                }
                if (!alreadyRated) needsRating = true;
            }

            if (needsRating) {
                InputHelper::displayError("You have unrated completed rentals!");
                InputHelper::displayMessage("Please rate all completed rentals before creating new requests.");
                InputHelper::displayMessage("Go to: Rental Management > Rate Rental Experience");
                return false;
            }
        }

        return true;
    }

    void Application::checkForRentalCompletions() {
        if (!isUserLoggedIn() || !isMember()) return;

        auto completedRentals = dataManager->getRentalManager()->getAndClearRecentCompletions();

        if (!completedRentals.empty()) {
            const auto* currentMember = dynamic_cast<Member*>(dataManager->getAuthManager()->getCurrentUser());

            for (const std::string& rentalId : completedRentals) {
                // Check if this member was involved in the rental
                if (const Rental* rental = dataManager->getRentalManager()->getRentalById(rentalId)) {
                    if (rental->getRenterUsername() == currentMember->getUsername() ||
                        rental->getOwnerUsername() == currentMember->getUsername()) {

                        std::cout << "Rental " << rentalId << " has been completed." << std::endl;
                        }
                }
            }
        }
    }

    // ========================================== ADMIN FEATURE HANDLERS ===============================================

    void Application::handleViewAllMembers() const {
        const auto members = dataManager->getAuthManager()->getAllMembers();
        ConsoleView::displayAllMembers(members);

        InputHelper::displayMessage("Total registered members: " + std::to_string(members.size()));
        InputHelper::waitForEnter();
    }

    void Application::handleViewAllMotorbikes() const {
        if (!isUserLoggedIn() || !isAdmin()) {
            InputHelper::displayError("Admin access required.");
            return;
        }

        ConsoleView::displayHeader("All Registered Motorbikes");

        dataManager->getMotorbikeManager()->displayAllMotorbikes();
        dataManager->getMotorbikeManager()->displayMotorbikeStatistics();

        InputHelper::waitForEnter();
    }

    void Application::handleViewMemberDetails() const {
        const std::string username = InputHelper::getStringInput("Enter member username: ");

        if (const Member* member = dataManager->getAuthManager()->findMemberByUsername(username)) {
            ConsoleView::displayHeader("Member Details: " + username);
            member->displayInfo();
        }
        else {
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

        std::cout << "Total Credit Points in System: " << std::fixed << std::setprecision(1) << totalCredits <<
            std::endl;
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
                }
                else {
                    InputHelper::displayError("Some data could not be saved!");
                }
            }
            break;
        case 2:
            if (DataManager::validateDataConsistency()) {
                InputHelper::displaySuccess("Data consistency validation passed!");
            }
            else {
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
