//
// Created by Nguyen Ngoc Hai on 8/25/2025.
//

#pragma once
#include <string>
#include <vector>

namespace EMotoRental
{
    class FileHandler
    {
    public:
        // Basic file operations
        static bool writeToFile(const std::string& content, const std::string& fileName);
        static std::string readFromFile(const std::string& fileName);
        static bool fileExists(const std::string& fileName);
        static bool createFile(const std::string& fileName);
        static bool createBackup(const std::string& fileName);

        // CSV parsing utils
        static std::vector<std::string> parseCSVLine(const std::string& line);
        static std::string formatCSVLine(const std::vector<std::string>& data);

        // Directory operations
        static bool createDirectory(const std::string& dirPath);
        static bool ensureDataDirectory();
    };
}
