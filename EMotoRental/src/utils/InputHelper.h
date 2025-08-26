//
// Created by Nguyen Ngoc Hai on 8/26/2025.
//

#pragma once
#include <string>
#include "DateUtil.h"

namespace EMotoRental
{
    class InputHelper
    {
    public:
        // Basic input methods
        static std::string getStringInput(const std::string& prompt);
        static int getIntInput(const std::string& prompt);
        static double getDoubleInput(const std::string& prompt);
        static DateUtil::TimePoint getDateInput(const std::string& prompt);

        // Specialized input methods
        static std::string getPasswordInput(const std::string& prompt);
        static bool confirmAction(const std::string& message);
        static int getMenuChoice(int minChoice, int maxChoice);

        // Validation methods
        static bool validatePasswordStrength(const std::string& password);
        static bool validateEmail(const std::string& email);
        static bool validatePhoneNumber(const std::string& phoneNumber);

        // Display methods
        static void displayError(const std::string& message);
        static void displaySuccess(const std::string& message);
        static void displayMessage(const std::string& message);
        static void waitForEnter();

    private:
        // Helper methods
        static void clearInputStream();
        static std::string maskPasswordInput();
    };
}