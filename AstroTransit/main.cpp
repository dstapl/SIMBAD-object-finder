#include <iostream>
#include <cstdio>
#include "algorithms.h"

int main() {
	using namespace algorithms;

	int year;
	int month;
	int day;

	std::cout << "Enter birth year: " << std::endl;
	std::cin >> year;
	std::cout << "month: " << std::endl;
	std::cin >> month;
	std::cout << "day: " << std::endl;
	std::cin >> day;

	double birthday = JD(year, month, day);

	std::cout << "Your *JULIAN* birthday is: ";
	std::printf("%.2f", birthday);
	std::cout << std::endl;
	return EXIT_SUCCESS;
}