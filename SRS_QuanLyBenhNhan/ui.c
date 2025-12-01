#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

void clearScreen() {
    system("clear||cls");
}

void waitForEnter() {
    printf("\n%s[Press Enter to continue...]%s", CYAN, RESET);
    fflush(stdout);
    while (getchar() != '\n');
}

void printHeader(const char *title) {
    printf("%s%s", BG_CYAN, BLACK);
    printf("+----------------------------------------------------------------+\n");
    printf("|");
    int title_len = strlen(title);
    int padding = (64 - title_len) / 2;
    printf("%*s%s%*s|\n", padding, " ", title, padding, " ");
    printf("+----------------------------------------------------------------+\n");
    printf("%s", RESET);
}

void printFooter() {
    printf("%s%s", BG_CYAN, BLACK);
    printf("+----------------------------------------------------------------+\n");
    printf("|%*s%-*s%*s|\n", (int)(28), " ", (int)(10), "2025", (int)(24), " ");
    printf("+----------------------------------------------------------------+\n");
    printf("%s", RESET);
}

void printSeparator() {
    printf("%s", BRIGHT_CYAN);
    printf("-------------------------------------------------------------------\n");
    printf("%s", RESET);
}

void printDottedSeparator() {
    printf("%s", DIM);
    printf("-------------------------------------------------------------------\n");
    printf("%s", RESET);
}

void printSuccess(const char *message) {
    printf("%s%s\n", message, RESET);
}

void printError(const char *message) {
    printf("%s%s\n", message, RESET);
}

void printWarning(const char *message) {
    printf("%s%s\n", message, RESET);
}

void printInfo(const char *message) {
    printf("%s%s\n", message, RESET);
}

void printWelcomeScreen() {
    clearScreen();
    printf("\n\n");
    printf("%s%s", BG_BLUE, BRIGHT_WHITE);
    printf("+====================================================================+\n");
    printf("|                                                                    |\n");
    printf("|                 %sHOSPITAL PATIENT MANAGEMENT SYSTEM%s                 |\n", BG_BLUE, BG_BLUE);
    printf("|                                                                    |\n");
    printf("|                        %sWelcome to the System%s                       |\n", BRIGHT_CYAN, BG_BLUE);
    printf("|                                                                    |\n");
    printf("+====================================================================+\n");
    printf("%s\n", RESET);
}

void printTableHeader() {
    printf("\n");
    printf("%s", BRIGHT_CYAN);
    printf("+------------+-----------------------------+---------------+------------------+-------------+\n");
    printf("| Card ID    | Name                        | Phone         | Debt              | Visit Days  |\n");
    printf("+------------+-----------------------------+---------------+------------------+-------------+\n");
    printf("%s", RESET);
}

void printTableRow(const char *col1, const char *col2, const char *col3, const char *col4, const char *col5) {
    printf("%s│ %-10s │ %-27s │ %-13s │ %-16s │ %-11s │%s\n",
           BRIGHT_CYAN, col1, col2, col3, col4, col5, RESET);
}

void printTableFooter() {
    printf("%s", BRIGHT_CYAN);
    printf("+------------+-----------------------------+---------------+------------------+-------------+\n");
    printf("%s", RESET);
}

void printMenu() {
    printf("\n");
    printf("%s%s", BG_MAGENTA, BRIGHT_WHITE);
    printf("+=====================================================================+\n");
    printf("|                       PATIENT MANAGEMENT MENU                       |\n");
    printf("+=====================================================================+\n");
    printf("%s", RESET);
    
    printf("\n  %s[1]%s Patient Admission               %s[5]%s Search Patient\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_YELLOW, RESET);
    printf("  %s[2]%s Update Patient                  %s[6]%s Sort Patients by Debt\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_YELLOW, RESET);
    printf("  %s[3]%s Discharge from Hospital         %s[7]%s Medical Examination Record\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_YELLOW, RESET);
    printf("  %s[4]%s View Patient List               %s[8]%s View Medical History\n", 
           BRIGHT_YELLOW, RESET, BRIGHT_YELLOW, RESET);
    
    printf("\n  %s[0]%s Exit Program\n", BRIGHT_RED, RESET);
    
    printSeparator();
    printf("%s> Enter your choice: %s", BRIGHT_BLUE, RESET);
}
