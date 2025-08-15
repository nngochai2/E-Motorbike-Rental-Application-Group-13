#pragma once
#include <string>

namespace EMotoRental {
	class User {
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

			// Authentication
			bool validatePassword(const std::string& password) const;
			bool changePassword(const std::string oldPass, const std::string& newPass);

			// Profile Management
			bool updateProfile(const std::string& email, const std::string& phone);
			void setVerified(bool status);
			bool isLicenseValid() const;

			// Pure virtual functions
			virtual std::string getUserType() const = 0;
			virtual void displayInfo() const = 0;
	};
}