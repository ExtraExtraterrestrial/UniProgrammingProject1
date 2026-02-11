#pragma once

#if defined(_WIN32) 
#define				DIR_DELIMITER	 	"\\"
#define				CLEAR_CMD			"cls"
#else
#define				DIR_DELIMITER	 	"/"
#define				CLEAR_CMD			"clear"
#endif // defined(_WIN32) 


#define OUTPUT_LOG(text) \
	printf("\x1B[36m[[INFO]\033[0m %s:%s \n%s", __FILE__, __func__, text);\

#define OUTPUT_ERROR(text, FILE_AMNT, MEM_AMNT,	STACKS_AMNT, ...) \
	printf("\x1B[31m[ERROR]\033[0m %s:%s \n%s", __FILE__, __func__, text);\
	closeBeforeEvacuation(FILE_AMNT, MEM_AMNT, STACKS_AMNT, __VA_ARGS__);\
	exit(1);

#define FILE_CHECK(f) \
		if(!f) {\
			closeBeforeEvacuation(1, 0, 0, f);\
			printf("\x1B[31m[ERROR]\033[0m %s%s%s%s", __FILE__, ":", __func__, "\nCouldn't open file!");\
			exit(1);\
		}

#define ALLOC_CHECK(m) \
		if(!m) {\
			closeBeforeEvacuation(0, 1, 0, m);\
			printf("\x1B[31m[ERROR]\033[0m %s%s%s%s", __FILE__, ":", __func__, "\nCouldn't allocate memory!");\
			exit(1);\
		}

void closeBeforeEvacuation(int fileAmnt, int memAmnt, int stacksAmnt, ...);
