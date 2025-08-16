//
// Created by Nguyen Ngoc Hai on 8/16/2025.
//
#include "User.h"
#include <sstream>
#include <utility>

namespace EMotoRental
{
    User::User() : hasValidLicense(false), isVerified(false)
    {
    }

    User::User(std::string username, std::string password,
               std::string fullName, std::string email,
               std::string phone)
        : username(std::move(username)), password(std::move(password)), fullName(std::move(fullName)),
          email(std::move(email)),
          phoneNumber(std::move(phone)), hasValidLicense(false), isVerified(false)
    {
    }

    User::~User() = default;

    // Getters
    std::string User::getUsername() const { return username; }
    std::string User::getFullName() const { return fullName; }
    std::string User::getEmail() const { return email; }
    std::string User::getPhoneNumber() const { return phoneNumber; }
    std::string User::getIdNumber() const { return fullName; }
    std::string User::getLicenseNumber() const { return licenseNumber; }
    bool User::isLicenseValid() const { return hasValidLicense; }
    bool User::getIsVerified() const { return isVerified; }

    bool User::validatePassword(const std::string& password) const
    {
        return this->password == password;
    }

    bool User::changePassword(const std::string& oldPass, const std::string& newPass)
    {
        if (!validatePassword(oldPass))
        {
            return false;
        }
        if (!isPasswordStrong(newPass))
        {
            return false;
        }
        password = newPass;
        return true;
    }

    bool User::updateProfile(const std::string& email, const std::string& phone)
    {
        // Basic email validation
        if (email.find('@') == std::string::npos)
        {
            return false;
        }
        // Basic phone validation
        if (phone.length() < 10)
        {
            return false;
        }

        this->email = email;
        this->phoneNumber = phone;
        return true;
    }

    void User::setVerified(bool status)
    {
        isVerified = status;
    }

    void User::setLicense(const std::string& licenseNum, bool isValid)
    {
        licenseNumber = licenseNum;
        hasValidLicense = isValid;
    }

    void User::setIdNumber(const std::string& id)
    {
        idNumber = id;
    }

    bool User::isPasswordStrong(const std::string& password)
    {
        if (password.length() < 8) return false;

        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

        for (char c : password)
        {
            if (c >= 'A' && c <= 'Z') hasUpper = true;
            else if (c >= 'a' && c <= 'z') hasLower = true;
            else if (c >= '0' && c <= '9') hasDigit = true;
            else if (c == '!' || c == '@' || c == '#' || c == '$' ||
                c == '%' || c == '^' || c == '&' || c == '*')
                hasSpecial = true;
        }

        return hasUpper && hasLower && hasDigit && hasSpecial;
    }
}
