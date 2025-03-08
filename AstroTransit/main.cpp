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

	double birthday = date::JD(year, month, day);

	std::cout << "Your *JULIAN* birthday is: ";
	std::printf("%.2f", birthday);
	std::cout << std::endl;

	std::tuple<int, int> y = { 1,2 };
	std::cout << algorithms::astro_helper::between2((int)2, y, true);
	
	return EXIT_SUCCESS;
}