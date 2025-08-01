#include "astro_helpers.h"
#include "algorithms.h" // Include the implementation file for algorithms

#include "file_reading.h"

#include <iostream>
#include <cstdio>

#include <filesystem>
int main() {
	std::string filename = "../Data/simbad_all_CV_and_DN.xml";

	VOTable votable = VOTable();
	if (votable.load(filename) == 1) {
		return 1;
	};

	votable.printTable();

	return 0;
}