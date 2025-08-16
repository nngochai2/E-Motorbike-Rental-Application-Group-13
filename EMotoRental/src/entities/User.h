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
        User(std::string  username, std::string  password,
             std::string  fullName, std::string  email,
             std::string  phone);
        virtual ~User();

        // Getters
        std::string getUsername() const;
        std::string getFullName() const;
        std::string getEmail() const;
        std::string getPhoneNumber() const;
        std::string getIdNumber() const;
        std::string getLicenseNumber() const;
        bool isLicenseValid() const;
        bool getIsVerified() const;

        // Authentication
        bool validatePassword(const std::string& password) const;
        bool changePassword(const std::string& oldPass, const std::string& newPass);

        // Profile Management
        bool updateProfile(const std::string& email, const std::string& phone);
        void setVerified(bool status);
        void setLicense(const std::string& licenseNum, bool isValid);
        void setIdNumber(const std::string& id);

        // Pure virtual functions
        virtual std::string getUserType() const = 0;
        virtual void displayInfo() const = 0;
        virtual std::string toCSVString() const = 0;

    protected:
        // Helper for password validation
        static bool isPasswordStrong(const std::string& password) ;
    };
}
