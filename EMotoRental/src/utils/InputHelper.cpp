//
// Created by Nguyen Ngoc Hai on 8/26/2025.
//

#include "InputHelper.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <cctype>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace EMotoRental
{
    // ========================================= BASIC INPUT METHODS ===================================================

    std::string InputHelper::getStringInput(const std::string& prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        // Trim whitespaces
        while (!input.empty() && std::isspace(input.front())) {
            input.erase(input.begin());
        }
        while (!input.empty() && std::isspace(input.back())) {
            input.pop_back();
        }

        return input;
    }

    int InputHelper::getIntInput(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;

            if (!(std::cin >> value)) {
                displayError("Invalid input! Please enter a valid number.");
                clearInputStream();
            } else {
                clearInputStream();
                return value;
            }
        }
    }

    double InputHelper::getDoubleInput(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;

            if (!(std::cin >> value)) {
                displayError("Invalid input! Please enter a valid number.");
                clearInputStream();
            } else {
                clearInputStream();
                return value;
            }
        }
    }

    DateUtil::TimePoint InputHelper::getDateInput(const std::string& prompt) {
        while (true) {
            std::string dateStr = getStringInput(prompt + " (DD/MM/YYYY");

            if (dateStr.empty()) {
                displayError("Data cannot be empty!");
                continue;
            }

            try {
                const DateUtil::TimePoint date = DateUtil::parseDate(dateStr);
                return date;
            } catch ([[maybe_unused]] const std::exception& e) {
                displayError("Invalid date format! Please use DD/MM/YYYY format.");
            }
        }
    }

    // ========================================= SPECIALIZED INPUT METHODS =============================================

    std::string InputHelper::getPasswordInput(const std::string& prompt) {
        std::cout << prompt;
        return maskPasswordInput();
    }

    bool InputHelper::confirmAction(const std::string& message) {
         while (true) {
             std::cout << message << " (y/n): ";
             std::string input;
             std::getline(std::cin, input);

             if (input.length() == 1) {
                 const char response = std::tolower(input[0]);
                 if (response == 'y') return true;
                 if (response == 'n') return false;
             }

             displayError("Please enter 'y' for yes or 'n' for no.");
         }
    }

    int InputHelper::getMenuChoice(const int minChoice, const int maxChoice) {
        while (true) {
            if (const int choice = getIntInput("Enter your choice: "); choice >= minChoice && choice <= maxChoice) {
                return choice;
            }
            displayError("Invalid choice! Please enter a number between " + std::to_string(minChoice)
                + " and " + std::to_string(maxChoice));
        }
    }

    // ============================================== DISPLAY METHODS ==================================================

    void InputHelper::displayError(const std::string& message) {
        std::cout << "\n*** ERROR: " << message << " ***\n" << std::endl;
    }

    void InputHelper::displaySuccess(const std::string& message) {
        std::cout << "\n*** SUCCESS: " << message << " ***\n" << std::endl;
    }

    void InputHelper::displayMessage(const std::string& message) {
        std::cout << "\n" << message << "\n" << std::endl;
    }

    void InputHelper::waitForEnter() {
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // ========================================== PRIVATE HELPER METHODS ===============================================

    void InputHelper::clearInputStream() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string InputHelper::maskPasswordInput() {
        std::string password;

        #ifdef _WIN32
            // Windows implementation using conio.h
            char ch;
            while ((ch = _getch()) != 13) { // 13 is the ASCII code for Enter
                if (ch == 8) { // 8 is the ASCII code for Backspace
                    if (!password.empty()) {
                        password.pop_back();
                        std::cout << "\b \b"; // Move back, print space, move back
                        std::cout.flush();
                    }
                } else {  // Printable characters
                    password.push_back(ch);
                    std::cout << "*";
                    std::cout.flush();
                }
            }
            std::cout << std::endl;

        #else
            // Linux/Mac implementation using termios.h
            struct termios oldTermios, newTermios;
            tcgetattr(STDIN_FILENO, &oldTermios);

            newTermios = oldTermios;
            newTermios.c_lflag &= ~(ECHO | ICANON);
            tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

            char ch;
            while ((ch = getchar()) != '\n' && ch != '\r') {
                if (ch == 127 || ch == '\b') {  // Backspace
                    if (!password.empty()) {
                        password.pop_back();
                        std::cout << "\b \b";
                        std::cout.flush();
                    }
                } else if (ch >= 32 && ch <= 126) {  // Printable characters
                    password += ch;
                    std::cout << '*';
                    std::cout.flush();
                }
            }

            tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldTermios);
            std::cout << std::endl;
        #endif

        return password;
    }
}