//
// Created by Nguyen Ngoc Hai on 8/26/2025.
//

#include "ConsoleView.h"
#include "../managers/AuthManager.h"
#include <iostream>
#include <iomanip>

#include "../managers/MotorbikeManager.h"

namespace EMotoRental
{
    ConsoleView::ConsoleView() {}

    ConsoleView::~ConsoleView() {}

    // ========================================== WELCOME AND MAIN SCREENS =============================================

    void ConsoleView::displayWelcomeScreen() {
        // Remove this to ignore inconsistent performances
        // clearScreen();

        std::cout << "\n";
        std::cout << "EEET2482/EEET2653/COSC2082/COSC2721 GROUP PROJECT\n";
        std::cout << "Semester 2 2025\n";
        std::cout << "E-MOTORBIKE RENTAL APPLICATION\n";
        std::cout << "Instructor: Dr Ling Huo Chong, Dr Ushik Shrestha, Dr Tri Huynh\n";
        std::cout << "Group: Group No. 13\n";
        std::cout << "\n";
        std::cout << "Group Members: 4\n";
        std::cout << "s3978281, Nguyen Ngoc Hai\n";
        std::cout << "XXXXXXX, [Student Name 2]\n";
        std::cout << "XXXXXXX, [Student Name 3]\n";
        std::cout << "XXXXXXX, [Student Name 4]\n";
        std::cout << "\n";
        displaySeparator();
        std::cout << "\n";
    }

    int ConsoleView::getUserTypeChoice() {
        displayUserTypeOptions();
        return InputHelper::getMenuChoice(0, 3); // 0 = Exit, 1 = Guest, 2 = Member, 3 = Admin
    }

    void ConsoleView::displayMainMenu(const std::string& userType) {
        displayHeader(userType + " Main Menu");

        if (userType == "Guest") {
            displayGuestMenu();
        } else if (userType == "Member") {
            displayMemberMenu();
        } else if (userType == "Admin") {
            displayAdminMenu();
        }
    }

    // ========================================= AUTHENTICATION SCREENS ================================================

    std::string ConsoleView::getUsername() {
        return InputHelper::getStringInput("Enter username: ");
    }

    std::string ConsoleView::getPassword() {
        return InputHelper::getPasswordInput("Enter password: ");
    }

    bool ConsoleView::getRegistrationData(std::string& username, std::string& password, std::string& fullName,
                                        std::string& email, std::string& phone) {
        displayHeader("Member Registration");

        // Get username
        username = InputHelper::getStringInput("Enter username: ");
        if (username.empty()) {
            InputHelper::displayError("Username cannot be empty!");
            return false;
        }

        // Get password
        password = InputHelper::getPasswordInput("Enter password: ");
        if (password.empty()) {
            InputHelper::displayError("Password cannot be empty!");
            return false;
        }

        // Confirm password
        if (const std::string confirmPass = InputHelper::getPasswordInput("Confirm password: "); password != confirmPass) {
            InputHelper::displayError("Passwords does not match!");
            return false;
        }

        // Get full name
        fullName = InputHelper::getStringInput("Enter full name: ");
        if (fullName.empty()) {
            InputHelper::displayError("Full name cannot be empty!");
            return false;
        }

        // Get email
        email = InputHelper::getStringInput("Enter email: ");
        if (email.empty()) {
            InputHelper::displayError("Email cannot be empty!");
            return false;
        }

        // Get phone
        phone = InputHelper::getStringInput("Enter phone number: ");
        if (phone.empty()) {
            InputHelper::displayError("Phone number cannot be empty!");
            return false;
        }

        // Show summary
        displaySubHeader("Registration Summary");
        std::cout << "Username: " << username << "\n";
        std::cout << "Full Name: " << fullName << "\n";
        std::cout << "Email: " << email << "\n";
        std::cout << "Phone: " << phone << "\n\n";

        return InputHelper::confirmAction("Proceed with registration?");
    }

    // ============================================= MEMBER SCREENS ====================================================

    void ConsoleView::displayMemberDashboard(const Member* member,
                                            const RentalManager* rentalManager,
                                            const MotorbikeManager* motorbikeManager) {
        if (!member) return;

        displayHeader("Member Dashboard");
        displayAccountOverview(member);

        // Enhanced rental sections with actual data
        if (rentalManager && motorbikeManager) {
            displayActiveRentalBookings(member, rentalManager, motorbikeManager);
            displayActiveRentalRequests(member, rentalManager, motorbikeManager);
        } else {
            // Fallback to existing simple display
            displayRentalSection("Your active rental booking", "No active rentals.");
            displayRentalSection("Your active rental requests", "No pending requests.");
        }

        displayFooter();
    }

    void ConsoleView::displayMemberMenu() {
        std::cout << "1. View Dashboard\n";
        std::cout << "2. Update Profile\n";
        std::cout << "3. Change Password\n";
        std::cout << "4. Top Up Credit Points\n";
        std::cout << "5. Register Motorbike\n";
        std::cout << "6. List Motorbike for Rent\n";
        std::cout << "7. Search Available Motorbikes\n";
        std::cout << "8. Rental Management\n";  // Leads to rental submenu
        std::cout << "0. Logout\n";
    }

    void ConsoleView::displayRentalMenu() {
        displayHeader("Rental Management");
        std::cout << "1. Search & Request Rental\n";
        std::cout << "2. View My Rental Requests\n";
        std::cout << "3. Approve Rental Requests\n";
        std::cout << "4. Complete Rental\n";
        std::cout << "5. Rate Rental Experience\n";
        std::cout << "0. Back to Main Menu\n";
    }

    // ============================================== ADMIN SCREENS ====================================================

    void ConsoleView::displayAdminMenu() {
        std::cout << "1. View All Members\n";
        std::cout << "2. View All Motorbikes\n";
        std::cout << "3. View Member Details\n";
        std::cout << "4. System Statistics\n";
        std::cout << "5. Data Management\n";
        std::cout << "0. Logout\n";
    }

    void ConsoleView::displayAllMembers(const std::vector<Member*>& members) {
        if (members.empty()) {
            InputHelper::displayError("No members found in the system");
        }

        displayHeader("All Members (" + std::to_string(members.size()) + ")");

        // Table header
        std::cout << std::left
                    << std::setw(15) << "Username"
                    << std::setw(25) << "Full Name"
                    << std::setw(25) << "Email"
                    << std::setw(10) << "Credits"
                    << std::setw(8) << "Rating"
                    << std::setw(8) << "License"
                    << std::setw(10) << "Verified" << std::endl;
        displaySeparator();

        // Member data
        for (const Member* member : members) {
            std::cout << std::left
                        << std::setw(15) << member->getUsername()
                        << std::setw(25) << member->getFullName()
                        << std::setw(25) << member->getEmail()
                        << std::setw(10) << std::fixed << std::setprecision(1) << member->getCreditPoints()
                        << std::setw(8) << std::fixed << std::setprecision(1) << member->getRenterRating()
                        << std::setw(8) << (member->isLicenseValid() ? "Valid" : "None")
                        << std::setw(10) << (member->getIsVerified() ? "Yes" : "No") << std::endl;
        }

        displayFooter();
    }

    bool ConsoleView::getRatingData(int& stars, std::string& comment) {
        displayHeader("Rate Rental Experience");

        stars = InputHelper::getIntInput("Enter rating (1-5 stars): ");
        if (stars < 1 || stars > 5) {
            InputHelper::displayError("Rating must be between 1 and 5!");
            return false;
        }

        comment = InputHelper::getStringInput("Enter comment (optional): ");

        // Show summary
        displaySubHeader("Rating Summary");
        std::cout << "Stars: " << stars << "/5\n";
        std::cout << "Comment: " << (comment.empty() ? "(No comment)" : comment) << "\n";

        return InputHelper::confirmAction("Submit this rating?");
    }

    // ============================================== GUEST SCREENS ====================================================
    void ConsoleView::displayGuestMenu() {
        std::cout << "1. Browse Available Motorbikes\n";
        std::cout << "2. Filter by Location\n";
        std::cout << "3. Register as Member\n";
        std::cout << "0. Exit\n";
    }

    // ======================================== CREDIT MANAGEMENT SCREENS ==============================================

    bool ConsoleView::getCreditTopUpData(double& amount, std::string& password) {
        displayHeader("Credit Point Top-Up");

        // Get amount
        amount = InputHelper::getDoubleInput("Enter amount to top up ($1 = 1 CP): ");
        if (amount <= 0) {
            InputHelper::displayError("Amount must be greater than 0!");
            return false;
        }

        // Get password for validation
        password = InputHelper::getPasswordInput("Enter password for verification: ");
        if (password.empty()) {
            InputHelper::displayError("Password cannot be empty!");
            return false;
        }

        // Show summary
        displayHeader("Top-Up Summary");
        std::cout << "Amount: $" << std::fixed << std::setprecision(2) << amount
                    << " (" << amount << "Credit Points)\n\n";

        return InputHelper::confirmAction("Proceed with top-up?");
    }

    bool ConsoleView::getProfileUpdateData(std::string& email, std::string& phone) {
        displayHeader("Update Profile Information");

        // Get email (no validation here - AuthManager will validate)
        email = InputHelper::getStringInput("Enter new email address: ");
        if (email.empty()) {
            InputHelper::displayError("New email address cannot be empty!");
            return false;
        }

        // Get phone (no validation here - AuthManager will validate
        phone = InputHelper::getStringInput("Enter new phone number: ");
        if (phone.empty()) {
            InputHelper::displayError("New phone number cannot be empty!");
            return false;
        }

        // Show summary
        displayHeader("Update Summary");
        std::cout << "New email address: " << email << std::endl;
        std::cout << "New phone number: " << phone << std::endl;

        return InputHelper::confirmAction("Proceed with update?");
    }

    bool ConsoleView::getPasswordChangeData(std::string& oldPass, std::string& newPass) {
        displayHeader("Change Password");

        // Get the current password
        oldPass = InputHelper::getPasswordInput("Enter current password: ");
        if (oldPass.empty()) {
            InputHelper::displayError("Current password cannot be empty.");
            return false;
        }

        // Get new password
        newPass = InputHelper::getPasswordInput("Enter new new password: ");
        if (newPass.empty()) {
            InputHelper::displayError("New password cannot be empty.");
            return false;
        }

        // Confirm new password
        if (const std::string confirmPass = InputHelper::getPasswordInput("Confirm password?");
            newPass != confirmPass) {
            InputHelper::displayError("New passwords do not match!");
            return false;
        }

        return InputHelper::confirmAction("Proceed with password change?");
    }

    // ============================================= DISPLAY UTILS =====================================================

    void ConsoleView::displaySeparator() {
        std::cout << std::string(70, '-') << "\n";
    }

    void ConsoleView::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

    void ConsoleView::displayHeader(const std::string& title) {
        std::cout << "\n";
        displaySeparator();
        std::cout << title << "\n";
        displaySeparator();
    }

    void ConsoleView::displaySubHeader(const std::string& subtitle) {
        std::cout << "\n" << subtitle << "\n";
        std::cout << std::string(subtitle.length(), '=') << "\n";
    }

    void ConsoleView::displayFooter() {
        std::cout << "\n";
    }

    // ========================================= PRIVATE DISPLAY HEADERS ===============================================

    void ConsoleView::displayUserTypeOptions() {
        std::cout << "Use the app as:\n";
        std::cout << "1. Guest\n";
        std::cout << "2. Member\n";
        std::cout << "3. Admin\n";
        std::cout << "0. Exit\n\n";
    }

    void ConsoleView::displayAccountOverview(const Member* member) {
        std::cout << "Account Overview: " << member->getUsername() << "\n";
        displaySeparator();
        std::cout << "Current Credit Points: " << std::fixed << std::setprecision(1)
                  << member->getCreditPoints() << "\n";
        std::cout << "Renter Rating: " << std::fixed << std::setprecision(1)
                  << member->getRenterRating() << "\n";

        if (member->hasOwnedMotorbike()) {
            std::cout << "Owned Motorbike ID: " << member->getOwnedMotorbikeId() << "\n";
        } else {
            std::cout << "No motorbike registered\n";
        }

        std::cout << "License Status: " << (member->isLicenseValid() ? "Valid" : "Invalid/None") << "\n";
        std::cout << "Verification Status: " << (member->getIsVerified() ? "Verified" : "Not Verified") << "\n\n";
    }

    void ConsoleView::displayRentalSection(const std::string& title, const std::string& message) {
        std::cout << title << "\n";
        displaySeparator();
        std::cout << message << "\n\n";
    }

    void ConsoleView::displayActiveRentalBookings(const Member* member,
                                             const RentalManager* rentalManager,
                                             const MotorbikeManager* motorbikeManager) {
        std::cout << "Your active rental booking\n";
        displaySeparator();

        const auto activeRentals = rentalManager->getActiveRentalsForMember(member->getUsername());

        if (activeRentals.empty()) {
            std::cout << "No active rental bookings.\n\n";
            return;
        }

        std::cout << "Rent Period | Brand | Model | Color | Size | Plate No. | Owner | Status\n";
        for (const Rental* rental : activeRentals) {
            if (rental->getRenterUsername() == member->getUsername()) {
                if (const Motorbike* bike = motorbikeManager->getMotorbikeByLicensePlate(rental->getMotorbikeLicensePlate())) {
                    std::cout << DateUtil::formatDate(rental->getStartDate()) << "–"
                              << DateUtil::formatDate(rental->getEndDate()) << " | "
                              << bike->getBrand() << " | " << bike->getModel() << " | "
                              << bike->getColor() << " | " << bike->getEngineSize() << "cc | "
                              << bike->getLicensePlate() << " | " << rental->getOwnerUsername()
                              << " | Active\n";
                }
            }
        }
        std::cout << "\n";
    }

    void ConsoleView::displayActiveRentalRequests(const Member* member,
                                                const RentalManager* rentalManager,
                                                const MotorbikeManager* motorbikeManager) {
        std::cout << "Your active rental requests\n";
        displaySeparator();

        // Check if member owns a motorbike
        Motorbike* ownedBike = motorbikeManager->getMotorbikeByOwner(member->getUsername());
        if (!ownedBike) {
            std::cout << "No motorbike registered for rental requests.\n\n";
            return;
        }

        auto requests = rentalManager->getRequestsForMotorbike(ownedBike->getLicensePlate());

        // Filter for pending requests only
        std::vector<RentalRequest*> pendingRequests;
        for (RentalRequest* request : requests) {
            if (request->getStatus() == RequestStatus::PENDING) {
                pendingRequests.push_back(request);
            }
        }

        if (pendingRequests.empty()) {
            std::cout << "No pending rental requests.\n\n";
            return;
        }

        std::cout << "Rent period | Renter rating | Renter\n";
        for (RentalRequest* request : pendingRequests) {
            std::cout << DateUtil::formatDate(request->getStartDate()) << "–"
                      << DateUtil::formatDate(request->getEndDate()) << " | "
                      << "N/A | " << request->getRenterUsername() << "\n";
        }
        std::cout << "\n";
    }
}
