//
// Created by Nguyen Ngoc Hai on 8/25/2025.
//

#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace EMotoRental
{
    // ========================================= BASIC FILE OPERATIONS =================================================

    bool FileHandler::writeToFile(const std::string& content, const std::string& fileName) {
        try {
            // Ensure data directory exists
            ensureDataDirectory();

            // Create backup if a file already exists
            if (fileExists(fileName)) {
                createBackup(fileName);
            }

            // Open file for writing
            std::ofstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "FileHandler: Failed to open file for writing " << fileName << std::endl;
                return false;
            }

            // Write content
            file << content;

            // Check if write was successful
            if (file.bad()) {
                std::cerr << "FileHandler: Failed to write to file " << fileName << std::endl;
                file.close();
                return false;
            }

            file.close();

            // Verify file was written successfully
            if (!fileExists(fileName)) {
                std::cerr << "FileHandler: File was not created successfully " << fileName << std::endl;
                return false;
            }

            std::cout << "FileHandler: Successfully wrote to file " << fileName << std::endl;
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception while writing file: " << fileName << ": " << e.what() << std::endl;
            return false;
        }
    }

    std::string FileHandler::readFromFile(const std::string& fileName) {
        try {
            // Check if a file exists
            if (!fileExists(fileName)) {
                std::cout << "FileHandler: File does not exist " << fileName << std::endl;
                return "";
            }

            // Open file for reading
            std::ifstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "FileHandler: Failed to open file for writing " << fileName << std::endl;
                return "";
            }

            // Read entire file content
            std::ostringstream buffer;
            buffer << file.rdbuf();

            file.close();

            std::string content = buffer.str();
            std::cout << "FileHandler: Successfully read " << content.length() << " characters from file: "
                << fileName << std::endl;

            return content;
        }
        catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception while reading file " << fileName
                << ": " << e.what() << std::endl;
            return "";
        }
    }

    bool FileHandler::fileExists(const std::string& fileName) {
        try {
            std::ifstream file(fileName);
            bool exists = file.good();
            file.close();
            return exists;
        }
        catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception while checking file existence " << fileName
                << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool FileHandler::createFile(const std::string& fileName) {
        try {
            // Ensure directory exists
            ensureDataDirectory();

            // Create an empty file
            std::ofstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "FileHandler: Failed to open file for writing " << fileName << std::endl;
                return false;
            }

            file.close();
            std::cout << "FileHandler: Created empty file: " << fileName << std::endl;
            return true;
        }
        catch (std::exception& e) {
            std::cerr << "FileHandler: Exception while creating file " << fileName
                << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool FileHandler::createBackup(const std::string& fileName) {
        try {
            if (!fileExists(fileName)) {
                return true; // Nothing backup
            }

            // Create backup filename with timestamp
            std::string backupName = fileName + ".backup";

            // Read original content
            std::string content = readFromFile(backupName);
            if (content.empty()) {
                return true; // Empty file, no need to back up
            }

            // Write to a backup file
            std::ofstream backup(backupName);
            if (!backup.is_open()) {
                std::cerr << "FileHandler: Failed to create backup file: " << backupName << std::endl;
                return false;
            }

            backup << content;
            backup.close();

            std::cout << "FileHandler: Successfully created backup file: " << backupName << std::endl;
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception while creating backup for " << fileName
                << ": " << e.what() << std::endl;
            return false;
        }
    }

    // ============================================ CSV PARSING UTILS ==================================================

    /**
     * Parses a single line from the CSV file and returns the individual fields as a vector of string.
     * @param line the CSV line to parse
     * @return std::vector<std::string> containing the parsed tokens
     */
    std::vector<std::string> FileHandler::parseCSVLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::string token;
        bool inQuotes = false;

        for (size_t i = 0; i < line.length(); i++) {
            const char c = line[i];

            if (c == '"') { // Commas inside quotes should be treated as literal text, not field seperator
                inQuotes = !inQuotes;
            }
            else if (c == ',' && !inQuotes) { // Marks the end of a field
                // End of token
                tokens.push_back(token);
                token.clear();
            }
            else {
                token += c;
            }
        }

        // Add the last token
        tokens.push_back(token);

        return tokens;
    }

    /**
     * Takes a vector of strings and formats them to a proper CSV line
     * @param data fields to format
     * @return std::string containing the properly formatted CSV line
     */
    std::string FileHandler::formatCSVLine(const std::vector<std::string>& data) {
        // Returns empty is no data is found
        if (data.empty()) {
            return "";
        }

        // Build the output string
        std::ostringstream oss;

        for (size_t i = 0; i < data.size(); ++i) {
            std::string field = data[i];

            // Escape quotes and wrap in quotes if contains comma or quote
            if (field.find(',') != std::string::npos ||
                field.find('"') != std::string::npos ||
                field.find('\n') != std::string::npos) {

                // Replace quotes with double quotes
                size_t pos = 0;
                while ((pos = field.find('"')) != std::string::npos) {
                    field.replace(pos, 1, "\"\"");
                    pos += 2;
                }

                // Wrap in quotes
                field = "\"" + field + "\"";
            }

            oss << field;

            if (i < data.size() - 1) {
                oss << ", ";
            }
        }

        return oss.str();
    }

    // =========================================== DIRECTORY OPERATIONS ================================================

    bool FileHandler::createDirectory(const std::string& dirPath) {
        try {
            // Use C++17 file system
            std::filesystem::path path(dirPath);

            if (std::filesystem::exists(path)) {
                return true; // Directory already exists
            }

            bool success = std::filesystem::create_directories(path);

            if (success) {
                std::cout << "FileHandler: Created directory: " << dirPath << std::endl;
            } else {
                std::cerr << "FileHandler: Failed to create directory " << dirPath << std::endl;
            }

            return success;
        } catch (const std::exception& e) {
            std::cerr << "FileHandler: Exception while creating directory " << dirPath
                        << ": " << e.what() << std::endl;
            return false;
        }
    }

    bool FileHandler::ensureDataDirectory() {
        return createDirectory("data");
    }
}
