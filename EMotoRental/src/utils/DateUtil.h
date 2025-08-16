#pragma once
#include <chrono>
#include <string>

namespace EMotoRental {
	class DateUtil {
		public:
			using TimePoint = std::chrono::system_clock::time_point;

			// Parse date from string formate "DD/MM/YYYY"
			static TimePoint parseDate(const std::string& dateString);

			// Format date to string "DD/MM/YYYY"
			static std::string formatDate(const TimePoint& timePoint);

			// Get current system time
			static TimePoint getCurrentTime();

			// Calculate days between 2 dates
			static int daysBetween(const TimePoint &start, const TimePoint &end);
			
			// Check if date range is valid
			static bool isValidDateRange(const TimePoint& start, const TimePoint& end);

			// Add days to a date
			static TimePoint addDays(const TimePoint& TimePoint, int days);

		private:
			// Helper function to convert time_point to tm struct
			static std::tm timePointToTm(const TimePoint& TimePoint);

			// Helper function to convert tm struct to time_point
			static TimePoint tmToTimePoint(const std::tm& tm);
	};
}