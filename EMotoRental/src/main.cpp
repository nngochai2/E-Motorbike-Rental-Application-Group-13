//
// Created by Nguyen Ngoc Hai on 8/15/2025.
//

#include <iostream>
#include "utils/DateUtil.h"

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
	
	return 0;
}