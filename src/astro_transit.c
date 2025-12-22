#include <stdio.h>

// #include "VOTable.h"
// #include "Vector.h"
#include "algorithms.h"

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

	// vector(int) foo = {};
	// vector_push(foo, 2);
	// vector_extend(foo, 3,4,5,6);
	//
	//
	// printf("%lld", vector_count(foo));
	

	double src[NUMBER_OBJECTS] = {0};

	const double curr_jd = 2461031;
	generate_anomaly_vector(src, curr_jd);

    for (int i = 0; i < NUMBER_OBJECTS; i++) {
        printf("%lf ", src[i]);
    }
    printf("\n");

	return 0;
}
