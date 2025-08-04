#include "astro_helpers.h"
#include "algorithms.h" // Include the implementation file for algorithms

#include "file_reading.h"

#include <iostream>
#include <cstdio>

#include <filesystem>


int main() {
	std::filesystem::path filename = "../Data/simbad_all_CV_and_DN.xml";

	VOTable votable = VOTable();
	if (votable.load(filename) == 1) {
		return 1;
	};

	//votable.printTable();
	auto& tableData = votable.getTableData();

	auto testkey = "MAIN_ID";
    
	auto& row = tableData[0];
	auto value = row.getFieldValue<std::string>(testkey);
	
	if (value) {
		std::cout << "hello from " << *value << std::endl;
	}
	else {
		auto rawVal = row.getRawValue(testkey);
		std::cerr << "raw value is " << (rawVal ? *rawVal : "null") << std::endl;
		std::cerr << "Key or type not found in row: " << std::endl;
		for (const auto& i : row.fields) {
			std::cerr << i.first << ", " << i.second << std::endl;
		}
		std::cerr << std::endl;
		return 1;
	};

	return 0;
}
