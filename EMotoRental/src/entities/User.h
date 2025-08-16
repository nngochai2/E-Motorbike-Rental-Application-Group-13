#pragma once
#include <string>

namespace EMotoRental
{
    class User
    {
    protected:
        std::string username;
        std::string password;
        std::string fullName;
        std::string email;
        std::string phoneNumber;
        std::string idNumber;
        std::string licenseNumber;
        bool hasValidLicense;
        bool isVerified;

    public:
        User();
        User(const std::string& username, const std::string& password,
             const std::string& fullName, const std::string& email,
             const std::string& phone);
        virtual ~User();

        // Getters
        std::string getUsername() const;
        std::string getFullName() const;
        std::string getEmail() const;
        std::string getPhoneNumber() const;
        bool isLicenseValid() const;
        bool getIsVerified() const;

        // Authentication
        bool validatePassword(const std::string& password) const;
        bool changePassword(std::string oldPass, const std::string& newPass);

        // Profile Management
        bool updateProfile(const std::string& email, const std::string& phone);
        void setVerified(bool status);
        void setLicense(const std::string& licenseNum, bool isValid);

        // Pure virtual functions
        virtual std::string getUserType() const = 0;
        virtual void displayInfo() const = 0;
        virtual std::string toCSVString() const = 0;
    };
}
