#include <stdio.h>

#define TODO(message) do { fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message); abort(); } while(0)
#define UNIMPLEMENTED(message) do { fprintf(stderr, "%s:%d: Not implemented: %s\n", __FILE__, __LINE__, message); abort(); } while(0)

#define UNUSED_VAR(expr) do { (void)(expr); } while (0)


#define FILE_OPEN(FILEPOINT, FILEPATH, MODE)\
    do {\
        errno_t file_err = fopen_s(&(FILEPOINT), FILEPATH, MODE);\
        if (file_err != 0) {\
            perror("Failed to open file. File name: ");\
            perror(FILEPATH);\
            FILE_CLOSE(FILEPOINT, FILEPATH);\
            return 1;\
		}\
    } while(0)

#define FILE_CLOSE(FILEPOINT, FILENAME)\
    do {\
        if (FILEPOINT) {\
            if (fclose(FILEPOINT) == 0) {\
                printf("The file '%s' was closed\n", FILENAME);\
            } else {\
                printf("The file '%s' was not closed\n", FILENAME);\
			}\
		}\
    } while(0)

