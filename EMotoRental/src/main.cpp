//
// Created by Nguyen Ngoc Hai on 8/15/2025.
//

#include <iostream>
#include "utils/DateUtil.h"
#include "utils/FileHandler.h"

using namespace EMotoRental;

int main() {
	std::cout << "E-Motorbike Rental System" << std::endl;
	std::cout << "=========================" << std::endl;
	
	// Test DateUtil
	try {
		auto now = DateUtil::getCurrentTime();
		std::cout << "Current date: " << DateUtil::formatDate(now) << std::endl;
		
		auto future = DateUtil::addDays(now, 7);
		std::cout << "One week later: " << DateUtil::formatDate(future) << std::endl;
		
		int days = DateUtil::daysBetween(now, future);
		std::cout << "Days between: " << days << std::endl;
		
	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "=== Testing FileHandler Implementation ===" << std::endl;

    // Test directory creation
    std::cout << "\n--- Testing Directory Creation ---" << std::endl;
    bool dirCreated = FileHandler::ensureDataDirectory();
    std::cout << "Data directory creation: " << (dirCreated ? "Success" : "Failed") << std::endl;

    // Test file writing
    std::cout << "\n--- Testing File Writing ---" << std::endl;
    std::string testContent = "username,password,fullName,email\njohn123,pass123,John Doe,john@email.com\njane456,pass456,Jane Smith,jane@email.com";
    bool writeSuccess = FileHandler::writeToFile(testContent, "data/test.csv");
    std::cout << "File writing: " << (writeSuccess ? "Success" : "Failed") << std::endl;

    // Test file existence
    std::cout << "\n--- Testing File Existence ---" << std::endl;
    bool exists = FileHandler::fileExists("data/test.csv");
    std::cout << "File exists: " << (exists ? "Yes" : "No") << std::endl;

    // Test file reading
    std::cout << "\n--- Testing File Reading ---" << std::endl;
    std::string readContent = FileHandler::readFromFile("data/test.csv");
    std::cout << "File reading: " << (readContent.empty() ? "Failed" : "Success") << std::endl;
    std::cout << "Content length: " << readContent.length() << " characters" << std::endl;

    // Test CSV parsing
    std::cout << "\n--- Testing CSV Parsing ---" << std::endl;
    std::string sampleLine = "john123,Strong@Pass1,John Doe,john@email.com,0123456789";
    auto tokens = FileHandler::parseCSVLine(sampleLine);
    std::cout << "Parsed " << tokens.size() << " tokens from CSV line:" << std::endl;
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cout << "  [" << i << "]: " << tokens[i] << std::endl;
    }

    // Test CSV formatting
    std::cout << "\n--- Testing CSV Formatting ---" << std::endl;
    std::vector<std::string> data = {"user123", "pass,word", "John \"Doe\"", "email@test.com"};
    std::string csvLine = FileHandler::formatCSVLine(data);
    std::cout << "Formatted CSV line: " << csvLine << std::endl;

    // Test backup creation
    std::cout << "\n--- Testing Backup Creation ---" << std::endl;
    bool backupSuccess = FileHandler::createBackup("data/test.csv");
    std::cout << "Backup creation: " << (backupSuccess ? "Success" : "Failed") << std::endl;

    // Check if backup file exists
    bool backupExists = FileHandler::fileExists("data/test.csv.backup");
    std::cout << "Backup file exists: " << (backupExists ? "Yes" : "No") << std::endl;

    // Test non-existent file
    std::cout << "\n--- Testing Non-existent File ---" << std::endl;
    std::string emptyContent = FileHandler::readFromFile("data/nonexistent.csv");
    std::cout << "Reading non-existent file: " << (emptyContent.empty() ? "Correctly returned empty" : "Unexpected content") << std::endl;

    std::cout << "\n=== FileHandler Testing Complete ===" << std::endl;
	
	return 0;
}