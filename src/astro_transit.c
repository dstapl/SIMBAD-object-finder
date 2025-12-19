#include <stdio.h>
#include "VOTable.h"

const char *filename = "/Data/simbad_all_CV_and_DN.xml";
int main() {

	// FILE *file = fopen(filename, "r");
	// if (file == NULL) {
	// 	perror("Error opening XML file");	
	// 	return 1;
	// }
	//
	//
	// if (fclose(file) != 0) {
	// 	perror("Error closing file");
	// 	return 1;
	// }

	vector(int) foo = {0};
	printf("%lld", vector_count(foo));

	return 0;
}
