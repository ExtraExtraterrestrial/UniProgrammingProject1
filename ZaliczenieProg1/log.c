#include "includes.h"
#include "log.h"


void closeBeforeEvacuation(int fileAmnt, int memAmnt, int stacksAmnt, ...) {
	va_list args;
	va_start(args, stacksAmnt);

	for (int i = 0; i < fileAmnt; i++) {
		FILE* f = va_arg(args, FILE*);
		if (f) fclose(f);
	}

	for (int i = 0; i < memAmnt; i++) {
		void* p = va_arg(args, void*);
		if (p) free(p);
	}

	for (int i = 0; i < stacksAmnt; i++) {
		OwnStack* stack = va_arg(args, OwnStack*);
		stack->operation.free(stack); // if(stack) already inside
	}

	va_end(args);
}