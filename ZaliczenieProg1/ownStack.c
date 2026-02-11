#include "includes.h"
#include "ownStack.h"


static void ow_push(OwnStack* stack, void* el);
static void* ow_popCpy(OwnStack* stack, void* destination);
static void ow_pop(OwnStack* stack);
static void* ow_search(OwnStack* stack, void* desination, void* searchTerm);

static void ow_printAsCharacters(OwnStack* stack);
static void ow_printAsIntegers(OwnStack* stack);

static void ow_saveToFile(OwnStack* stack, const char* outputPath);
static OwnStack* ow_loadFromFile(OwnStack* destination, const char* inputPath);
int ow_getWriteSizeBeforeMemory(OwnStack* stack);

static void ow_clean(OwnStack* stack);
static void ow_free(OwnStack* stack);
static void ow_allocateNewMemory(OwnStack* stack, size_t amountOfEls);



OwnStack* OwnStack_init(int amountOfElements, size_t sizeOfElement) {
	if (amountOfElements < 0 || sizeOfElement < 0 || sizeOfElement > ((size_t)-1)*0.9) { 
		perror("OwnStack_init: Wrong values for initializing parameters!");
		return NULL;
	}

	// memory inits
	OwnStack* ret = (OwnStack*)malloc(sizeof(OwnStack));
	ALLOC_CHECK(ret);

	ret->memory = calloc(amountOfElements, sizeOfElement);
	ALLOC_CHECK(ret->memory);

	ret->maxLength = amountOfElements;

	// member inits
	ret->size = sizeOfElement * amountOfElements;
	ret->elSize = sizeOfElement;
	ret->top = ret->memory;
	ret->length = 0;

	// method inits
	ret->operation.push = &ow_push;
	ret->operation.popCpy = &ow_popCpy;
	ret->operation.pop = ow_pop;
	ret->operation.search = &ow_search;

	ret->operation.printAsCharacters = &ow_printAsCharacters;
	ret->operation.printAsIntegers = &ow_printAsIntegers;
	ret->operation.saveToFile = &ow_saveToFile;
	ret->operation.loadFromFile = &ow_loadFromFile;
	ret->operation.getWriteSize = &ow_getWriteSizeBeforeMemory;

	ret->operation.clean = &ow_clean;
	ret->operation.free = &ow_free;
	ret->operation.allocateNewMemory = &ow_allocateNewMemory;

	return ret;
}

// methods
static void ow_push(OwnStack* stack, void* el) {
	if (!stack) return;
	if (stack->length >= stack->maxLength) {
		stack->operation.allocateNewMemory(stack, stack->maxLength + 5);
	}

	if (stack->length)
		stack->top = (char*)stack->top + stack->elSize;

	memcpy(stack->top, el, stack->elSize);
	stack->length++;
}

static void ow_pop(OwnStack* stack) {
	if (!stack) return;
	if (stack->length == 0) {
		perror("Stack underflow on operation pop!");
		return;
	}

	stack->length--;

	memset(stack->top, 0, stack->elSize);

	if (stack->length)
		stack->top = (char*)stack->top - stack->elSize;
}

static void* ow_popCpy(OwnStack* stack, void* destination) {
	if (!stack) return NULL;
	if (stack->length == 0) {
		perror("Stack underflow on operation pop!");
		return NULL;
	}

	stack->length--;


	memcpy(destination, stack->top, stack->elSize);
	memset(stack->top, 0, stack->elSize);

	if (stack->length)
		stack->top = (char*)stack->top - stack->elSize;

	return destination;
}

static void* ow_search(OwnStack* stack, void* destination, void* searchTerm) {
	for (void* curEl = stack->top; curEl > stack->memory; curEl = (char*)curEl - stack->elSize) {
		if (memcmp(searchTerm, curEl, stack->elSize) == 0) {
			memcpy(destination, curEl, stack->elSize);
			return destination;
		}
	}

	destination = NULL;
	return NULL;
}


static void ow_printAsIntegers(OwnStack* stack) {
	char buff[256] = { '\0' };

	printf("\n===============================================================\n");
	for (void* curEl = stack->top; curEl > stack->memory; curEl = (char*)curEl - stack->elSize) {
		strncpy(buff, curEl, stack->elSize);
		printf("%s\n", buff);
	}
	printf("===============================================================\n");
}

static void ow_printAsCharacters(OwnStack* stack) {
	printf("\n===============================================================\n");
	for (void* curEl = stack->top; curEl > stack->memory; curEl = (char*)curEl - stack->elSize) {
		printf("%d\n", *(int*)curEl);
	}
	printf("===============================================================\n");
}


typedef struct SaveData
{
	unsigned int elSize;
	unsigned int length;
} SaveData;

static void ow_saveToFile(OwnStack* stack, const char* outPath) {
	FILE* out = fopen(outPath, "wb");
	FILE_CHECK(out);

	SaveData header = { stack->elSize, stack->length };

	if (fwrite(&header, sizeof(header), 1, out) != 1) {
		perror("saveToFile: Failed to write to file!");
		fclose(out);
		return;
	}

	if (fwrite(stack->memory, stack->elSize, stack->length, out) != stack->length) {
		perror("saveToFile: Failed to write to file!");
		fclose(out);
		return;
	}

	fclose(out);
}

static OwnStack* ow_loadFromFile(OwnStack* destination, const char* inputPath) {
	FILE* in = fopen(inputPath, "rb");
	FILE_CHECK(in);

	// load header
	SaveData header;

	if (fread(&header, sizeof(header), 1, in) != 1) {
		perror("loadFromFile: Failed to read from file!");
		fclose(in);
		return NULL;
	}

	if (header.elSize != (destination)->elSize) {
		perror("loadFromFile: element sizes don't match! Likely wrong file.");
		fclose(in);
		return NULL;
	}

	destination->operation.clean(destination);
	destination->operation.allocateNewMemory(destination, header.length);
	destination->length = (int)header.length;

	if (destination->length) {
		destination->top = (char*)destination->memory + destination->elSize * (destination->length - 1);
	} else {
		destination->top = destination->length;
	}

	if (fread((destination)->memory, (destination)->elSize, (destination)->length, in) != header.length) {
		perror("loadFromFile: Failed to read from file!");
		fclose(in);
		return NULL;
	}


	fclose(in);
	return (destination);
}


int ow_getWriteSizeBeforeMemory(OwnStack* stack) {
	return sizeof(SaveData) + stack->size;
}

static void ow_clean(OwnStack* stack) {
	if (!(stack)) return;

	(stack)->top = (stack)->memory;
	(stack)->length = 0;

	if ((stack)->memory) {
		memset((stack)->memory, 0, (stack)->maxLength * (stack)->elSize);
	}
}

static void ow_free(OwnStack* stack) {
	if (!(stack)) return;

	if ((stack)->memory) {
		memset((stack)->memory, 0, (stack)->maxLength * (stack)->elSize);
		free((stack)->memory);
		(stack)->memory = NULL;
	}

	free((stack));
	(stack) = NULL;
}

static void ow_allocateNewMemory(OwnStack* stack, size_t amountOfEls) {
	if (amountOfEls < stack->length) { 
		perror("allocateNewMemory: cutting memory is not allowed!"); 
		exit(1);
	}

	void *newMemory = realloc(stack->memory, stack->elSize * amountOfEls);
	ALLOC_CHECK(newMemory);

	stack->memory = newMemory;
	stack->maxLength = amountOfEls;
	stack->size = stack->elSize * amountOfEls;

	if (stack->length)
		stack->top = (char*)stack->memory + (stack->elSize * (stack->length - 1));
	else
		stack->top = stack->memory;
}
