#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "ui.c"
#include "core.c"
#include "files.c"

void menu_main() {
    do
    {
        clearScreen();
        printMenu(); 
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL || input[0] == '\n' || strcspn(input, " ") == 0) {
            clearScreen();
            printError("Please enter a valid choice!");
            waitForEnter();
            continue;
        }
        int choice = atoi(input);
        clearScreen();
        switch (choice)
        {
        case 1:
            inputNewPatient();
            break;
        case 2:
            updatePatient();
            break;
        case 3:
            RemovePatient();
            break;
        case 4:
            viewPatient();
            break;
        case 5:
            SearchPatientByName();
            break;
        case 6:
            SortPatientByDebt();
            break;
        case 7:
            inputNewRecord();
            break;
        case 8:
            viewRecord();
            break;
        case 0:
            clearScreen();
            printHeader("EXIT PROGRAM");
            printf("\n");
            printInfo("Thank you for using Hospital Patient Management System!");
            printf("\n");
            printFooter();
            printf("\n");
            return;
        default:
            printError("Please enter a valid choice (0-8)!");
            waitForEnter();
            break;
        }
    }while (1);
}

int main() {
    printWelcomeScreen();
    printf("\n  Loading data...\n\n");
    loadPatientData();
    loadRecordData();
    waitForEnter();
    menu_main();
    return 0;
}
