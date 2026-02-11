#pragma once

typedef struct OwnStack OwnStack;

typedef struct OwnStackOperations {
	void (*push)(OwnStack* stack, void* el);
	void* (*popCpy)(OwnStack* stack, void* destination);
	void (*pop)(OwnStack* stack);
	void* (*search)(OwnStack* stack, void* desination, void* searchTerm);

	void (*saveToFile)(OwnStack* stack, const char* outputPath);
	OwnStack* (*loadFromFile)(OwnStack* destination, const char* inputPath);

	void (*clean)(OwnStack* stack);
	void (*free)(OwnStack* stack);
	void (*allocateNewMemory)(OwnStack* stack, size_t amountOfEls);
	void (*printAsCharacters)(OwnStack* stack);
	void (*printAsIntegers)(OwnStack* stack);
	int (*getWriteSize)(OwnStack* stack);
} OwnStackOperations;

typedef struct OwnStack {
	void* memory;
	void* top;
	size_t size;
	size_t elSize;
	int maxLength;
	int length;

	OwnStackOperations operation;
} OwnStack;



OwnStack* OwnStack_init(int amountOfElements, size_t sizeOfElement);
