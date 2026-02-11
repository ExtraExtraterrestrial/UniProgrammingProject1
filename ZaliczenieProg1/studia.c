#include "includes.h"
#include "studia.h"

const char* lista_kierunki[] = {
	"informatyka",
	"matematyka",
	"biologia",
	"filologia",
	"filozofia",
	"medycyna"
};

MY_STUDENT* createMyStudent(const char* nazwisko, int rokUrodzenia, studia_kierunki kierunek, const char* strKierunek) {
	MY_STUDENT* student = (MY_STUDENT*)malloc(sizeof(MY_STUDENT));

	student->nazwisko = calloc(strlen(nazwisko) + 1, sizeof(char));
	ALLOC_CHECK(student->nazwisko);

	strcpy(student->nazwisko, nazwisko);
	student->rokUrodzenia = rokUrodzenia;
	if (kierunek != -1)
		strcpy(student->kierunek, lista_kierunki[kierunek]);
	else if (strKierunek != NULL)
		strcpy(student->kierunek, strKierunek);

	return student;
}
