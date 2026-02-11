#pragma once

void menu_loop();

void menu_printStudents(struct OwnStack* students);

void menu_pushStudent(struct OwnStack* students);

void menu_popStudent(struct OwnStack* students);

void menu_searchStudents(struct OwnStack* students);

void menu_resetStudents(struct OwnStack* students);

void menu_saveToFile(struct OwnStack* students);

void menu_loadFromFile(struct OwnStack* students);