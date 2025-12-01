#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

bool checkDischargedPatient(char cardId[]);
void inputNewPatient(); //Case1
void updatePatient(); //Case2
void RemovePatient(); //Case3
void viewPatient(); //Case4
void SearchPatientByName(); //Case5
void SortAsendingByDebt(); //Case6
void SortDescendingByDebt(); //Case6
void SortPatientByDebt(); //Case6
bool checkRealDate(char date[]); //Case7
bool checkDateUsed(char date[]); //Case7
void inputNewRecord(); //Case7
void viewRecord(); //Case8

void clearScreen();
void waitForEnter();

// UI Functions
void printHeader(const char *title);
void printFooter();
void printSuccess(const char *message);
void printError(const char *message);
void printWarning(const char *message);
void printInfo(const char *message);
void printSeparator();
void printDottedSeparator();
void printWelcomeScreen();
void printTableHeader();
void printTableRow(const char *col1, const char *col2, const char *col3, const char *col4, const char *col5);
void printTableFooter();
void printMenu();

#endif