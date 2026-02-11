#pragma once

typedef enum {
	studia_kierunki_informatyka,
	studia_kierunki_matematyka,
	studia_kierunki_biologia,
	studia_kierunki_filologia,
	studia_kierunki_filozofia,
	studia_kierunki_medycyna,
	studia_kierunki_LAST
} studia_kierunki;

typedef struct MY_STUDENT {
	char* nazwisko;
	int rokUrodzenia;
	char kierunek[32];
} MY_STUDENT;

// kierunek = studia_kierunki, strKierunek = NULL | kierunek = -1, strKierunek = "str"
MY_STUDENT* createMyStudent(const char* nazwisko, int rokUrodzenia, studia_kierunki kierunek, const char* strKierunek);

const char* lista_kierunki[];