#include "DateUtil.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace EMotoRental {

	DateUtil::TimePoint DateUtil::parseDate(const std::string& dateString) {
		std::istringstream iss(dateString);
		std::string day, month, year;

		// Parse DD/MM/YYYY format
		if (std::getline(iss, day, '/') &&
			std::getline(iss, month, '/') &&
			std::getline(iss, year)) {
				
				std::tm tm = {};
				tm.tm_mday = std::stoi(day);
				tm.tm_mon = std::stoi(month) - 1; // tm_mon is 0-based
				tm.tm_year = std::stoi(year) - 1900; // tm_year is years since 1900

				return tmToTimePoint(tm);
		}

		throw std::invalid_argument("Invalid date format. Please use DD/MM/YYYY");
	}

	std::string DateUtil::formatDate(const TimePoint& timePoint) {
		std::tm tm = timePointToTm(timePoint);
		std::ostringstream oss;
		oss << std::setfill('0') << std::setw(2) << tm.tm_mday << "/" 
				<< std::setfill('0') << std::setw(2) << (tm.tm_mon + 1) << "/"
				<< (tm.tm_year + 1900);

		return oss.str();
	}

	DateUtil::TimePoint DateUtil::getCurrentTime() {
		return std::chrono::system_clock::now();
	}

	int DateUtil::daysBetween(const TimePoint& start, const TimePoint& end) {
		auto duration = end - start;
		auto hours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
		return static_cast<int>(hours / 24);
	}

	bool DateUtil::isValidDateRange(const TimePoint& start, const TimePoint& end) {
		return start <= end;
	}

	DateUtil::TimePoint DateUtil::addDays(const TimePoint& timePoint, int days) {
		return timePoint + std::chrono::hours(days * 24);
	}


	std::tm DateUtil::timePointToTm(const TimePoint &timePoint) {
		const std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
		return *std::localtime(&time);
	}

	DateUtil::TimePoint DateUtil::tmToTimePoint(const std::tm &tm) {
		std::tm tm_copy = tm;
		const std::time_t time = std::mktime(&tm_copy);
		return std::chrono::system_clock::from_time_t(time);
	}
}