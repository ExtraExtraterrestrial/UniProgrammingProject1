#include "includes.h"
#include "menu.h"

static void menu_clearScreen();
static void menu_wait();

void menu_loop() {
	int choice = -1;

	OwnStack* students = OwnStack_init(2, sizeof(MY_STUDENT));

	if (!students) {
		perror("Error!");
		return;
	}

	for (;;) {

		printf(
			"Wybierz opcje:\n"
			"\t1. Pokarz liste\n"
			"\t2. Dodaj nowego studenta\n"
			"\t3. Usun gornego studenta\n"
			"\t4. Szukaj studenta\n"
			"\t5. Resetuj studentow\n"
			"\t6. Zapisz do pliku\n"
			"\t7. Wczytaj z pliku\n"
			"\n\t0. Exit\n"
			"\n> "
		);

		scanf("%i", &choice);
		menu_clearScreen();

		switch (choice) {
			case 1:
				menu_printStudents(students);
				menu_wait();
				menu_clearScreen();
				break;
			case 2:
				menu_pushStudent(students);
				menu_clearScreen();
				break;
			case 3:
				menu_popStudent(students);
				menu_clearScreen();
				break;
			case 4:
				menu_searchStudents(students);
				menu_wait();
				menu_clearScreen();
			case 5:
				menu_resetStudents(students);
				menu_clearScreen();
				break;
			case 6:
				menu_saveToFile(students);
				menu_clearScreen();
				break;
			case 7:
				menu_loadFromFile(students);
				menu_clearScreen();
				menu_printStudents(students);
				menu_wait();
				menu_clearScreen();
				break;
			default:
				if (choice != 0)
					printf("Prosze wybrac poprawna opcje.\n");
				break;
		}


		if (choice == 0)
			break;
	}

	printf("Dziekuje za skorzystanie z programu!\n\n");
	menu_resetStudents(students);
	menu_wait();
}


void menu_printStudents(OwnStack* students) {
	for (MY_STUDENT* student = students->top; student >= (MY_STUDENT*)students->memory; student--) {
		if (student->nazwisko) {
			printf(
				"--------------------------------\n"
				"Nazwisko: %s\nRok urodzenia: %d\nKierunek: %s\n",
				student->nazwisko, student->rokUrodzenia, student->kierunek
			);
		} else {
			printf("No students!\n");
		}
	}
}


void menu_pushStudent(OwnStack* students) {
	char nazwisko[256];
	int rok;
	studia_kierunki kierunek;
	
	for (int i = 0; i < (int)studia_kierunki_LAST; i++) {
		printf("%2d. %s\n", (studia_kierunki)i, lista_kierunki[i]);
	}
	printf("\nKierunek:\n> ");
	scanf("%d", &kierunek);

	if (kierunek >= studia_kierunki_LAST) {
		printf("Zly kierunek!\n");
		menu_wait();
		return;
	}

	printf("Nazwisko:\n> ");
	scanf("%255s", nazwisko);

	printf("Rok urodzenia:\n> ");
	scanf("%d", &rok);
	printf("\n");

	MY_STUDENT* newStudent = createMyStudent(nazwisko, rok, kierunek, NULL);

	students->operation.push(students, newStudent);

}


void menu_popStudent(OwnStack* students) {
	free(((MY_STUDENT*)students->top)->nazwisko);
	students->operation.pop(students);
}


void menu_searchStudents(OwnStack* students) {
	char nazwisko[256];
	int rok;
	studia_kierunki kierunek;

	for (int i = 0; i < (int)studia_kierunki_LAST; i++) {
		printf("%2d. %s\n", (studia_kierunki)i, lista_kierunki[i]);
	}
	printf("\nKierunek:\n> ");
	scanf("%d", &kierunek);

	if (kierunek >= studia_kierunki_LAST) {
		printf("Zly kierunek!\n");
		menu_wait();
		return;
	}

	printf("\nNazwisko:\n> ");
	scanf("%s", nazwisko);

	printf("\nRok urodzenia:\n> ");
	scanf("%d", &rok);
	printf("\n");

	MY_STUDENT* newStudent = createMyStudent(nazwisko, rok, kierunek, NULL);

	students->operation.search(students, newStudent, newStudent); // original, destination, search term

	if (newStudent) {
		printf("Student znaleziony.\nNazwisko: %s\n", newStudent->nazwisko);
	} else {
		printf("Student nie znaleziony.\n");
	}

	free(newStudent);
}

void menu_resetStudents(OwnStack* students) {
	for (MY_STUDENT* student = (MY_STUDENT*)students->top; student >= (MY_STUDENT*)students->memory; student--) {
		free(student->nazwisko);
		student->nazwisko = NULL;
	}
	students->operation.clean(students);
	students = NULL;
}

void menu_saveToFile(OwnStack* students) {
	if (!students || students->length < 1) {
		perror("Empty pointer to list!");
	}

	char path[128];
	printf("\nPodaj nazwe pliku:\n> ");
	scanf("%s", path);
	students->operation.saveToFile(students, path);

	FILE* out = fopen(path, "ab");
	FILE_CHECK(out);

	for (MY_STUDENT* student = (MY_STUDENT*)students->top; student >= (MY_STUDENT*)students->memory; student--) {
		unsigned int nazwLen = strlen(student->nazwisko) + 1;
		fwrite(&nazwLen, sizeof(nazwLen), 1, out);
		fwrite(student->nazwisko, sizeof(char), nazwLen, out);
	}

	fclose(out);
}

void menu_loadFromFile(OwnStack* students) {
	char path[128];
	printf("\nPodaj nazwe pliku:\n> ");
	scanf("%s", path);

	menu_resetStudents(students);

	students->operation.loadFromFile(students, path);

	if (students->length <= 0)
		return;


	FILE* in = fopen(path, "rb");
	FILE_CHECK(in);

	fseek(in, students->operation.getWriteSize(students), SEEK_SET);



	for (MY_STUDENT* student = (MY_STUDENT*)students->top; student >= (MY_STUDENT*)students->memory; student--) {
		char* nazwisko;
		unsigned int nazwLen = 0;
		

		if (fread(&nazwLen, sizeof(nazwLen), 1, in) != 1) {
			OUTPUT_ERROR("Couldn't read last name's size!", 1, 1, 1, in, ((MY_STUDENT*)students->memory)->nazwisko, students);
		}

		nazwisko = (char*)malloc(nazwLen);
		ALLOC_CHECK(nazwisko);

		if (fread(nazwisko, sizeof(char), nazwLen, in) != nazwLen) {
			OUTPUT_ERROR("Couldn't read the last name!", 1, 1, 1, in, ((MY_STUDENT*)students->memory)->nazwisko, students);
		}

		student->nazwisko = nazwisko;

		//MY_STUDENT* newStudent = createMyStudent(
		//	nazwiskoBuf, 
		//	((MY_STUDENT*)students->memory)->rokUrodzenia,
		//	-1,
		//	((MY_STUDENT*)students->memory)->kierunek);
		//students->operation.push(students, newStudent);
	}

	fclose(in);
}


static void menu_clearScreen() {
	//printf("\033[2J\033[1; 1H");
	system(CLEAR_CMD);
}


static void menu_wait() {
	printf("\nPress enter to continue\n> ");
	getchar(); // newline from previous input
	getchar();
}