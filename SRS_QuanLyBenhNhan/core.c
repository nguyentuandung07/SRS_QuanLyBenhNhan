#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "config.h"
#include "files.h"
#include "ui.h"

bool checkDischargedPatient(char cardId[])
{
    for (int i = 0; i < RecordCount; i++)
    {
        if (strcmp(RecordList[i].cardId, cardId) == 0)
        {
            if (strcmp(RecordList[i].status, "Discharged") == 0)
            {
                printSuccess("Discharged patients cannot be updated");
                return true;
            }
        }
    }
    return false;
}

void inputNewPatient()
{
    char cardId[10], name[50], phone[15], debtStr[20];
    double debt;
    struct Patient newPatient;
    int validPhone;
    bool empty = false;
    clearScreen();
    if (PatientCount >= MAX)
    {
        printError("Patient list is full. Cannot add more patients.");
        waitForEnter();
        return;
    }
    printHeader("ADD NEW PATIENT");
    printSeparator();

    do
    {
        printf("  %sEnter Card ID%s (Enter '-1' to return): ", BRIGHT_CYAN, RESET);
        fgets(cardId, sizeof(cardId), stdin);
        cardId[strcspn(cardId, "\n")] = '\0';
        cardId[strcspn(cardId, " ")] = '\0';
        if (strlen(cardId) == 0)
        {
            printError("Card ID cannot be empty!");
        }
        else if (findPatientById(cardId) != -1)
        {
            printError("Card ID already exists!");
        }
        else if (strcmp(cardId, "-1") == 0)
        {
            clearScreen();
            return;
        }
    } while (strlen(cardId) == 0 || findPatientById(cardId) != -1);

    do
    {
        printf("  %sEnter Name%s: ", BRIGHT_CYAN, RESET);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        int start = 0, end = strlen(name) - 1;
        while (start <= end && name[start] == ' ') start++;
        while (end >= start && name[end] == ' ') end--;
        if (start > end)
        {
            printError("Name cannot be empty or contain only spaces!");
            empty = true;
        }
        else empty = false;
    } while (empty);

    do
    {
        printf("  %sEnter Phone (9 digits)%s: ", BRIGHT_CYAN, RESET);
        fgets(phone, sizeof(phone), stdin);
        phone[strcspn(phone, "\n")] = '\0';
        phone[strcspn(phone, " ")] = '\0';
        validPhone = 1;
        for (int i = 0; i < (int)strlen(phone); i++)
        {
            if (phone[i] < '0' || phone[i] > '9')
            {
                validPhone = 0;
                break;
            }
        }
        if (strlen(phone) == 0)
        {
            printError("Phone cannot be empty!");
        }
        else if (strlen(phone) != 9)
        {
            printError("Phone number must be exactly 9 digits!");
        }
        else if (!validPhone)
        {
            printError("Phone must contain only numbers!");
        }
    } while (strlen(phone) == 0 || !validPhone || strlen(phone) != 9);

    do
    {
        printf("  %sEnter Debt Amount%s: ", BRIGHT_CYAN, RESET);
        fgets(debtStr, sizeof(debtStr), stdin);
        debtStr[strcspn(debtStr, "\n")] = '\0';
        debtStr[strcspn(debtStr, " ")] = '\0';
        if (strlen(debtStr) == 0)
        {
            printError("Debt cannot be empty!");
        }
        
        else
        {
            debt = atof(debtStr);
            if (debt < 0.0)
            {
                printError("Debt cannot be negative!");
            }
        }
    } while (strlen(debtStr) == 0 || debt < 0.0);

    strcpy(newPatient.cardId, cardId);
    strcpy(newPatient.phone, phone);
    strcpy(newPatient.name, name);
    newPatient.debt = debt;
    newPatient.visitDays = 0;
    PatientList[PatientCount++] = newPatient;
    savePatientData();

    printSeparator();
    printSuccess("Patient added successfully!");
    waitForEnter();
    clearScreen();
}

void updatePatient()
{
    char cardId[10], phone[15], name[50];
    int validPhone;
    bool checkDischarged = false;
    clearScreen();
    printHeader("UPDATE PATIENT");
    printSeparator();

    if (PatientCount == 0)
    {
        printError("No patients found in the system!");
        waitForEnter();
        clearScreen();
        return;
    }

    do
    {
        printf("  %sEnter Card ID%s (Enter '-1' to return): ", BRIGHT_CYAN, RESET);
        fgets(cardId, sizeof(cardId), stdin);
        cardId[strcspn(cardId, "\n")] = '\0';
        cardId[strcspn(cardId, " ")] = '\0';
        if (strcmp(cardId, "-1") == 0)
        {
            clearScreen();
            return;
        }
        else if (strlen(cardId) == 0)
        {
            printError("Card ID cannot be empty!");
        }
        else if (findPatientById(cardId) == -1)
        {
            printError("Card ID not found!");
        }
        else if (findRecordById(cardId) != -1)
        {
            if (checkDischargedPatient(cardId))
            {
                checkDischarged = true;
                printError("Cannot update discharged patient!");
                waitForEnter();
                clearScreen();
                return;
            }
        }
    } while (strlen(cardId) == 0 || findPatientById(cardId) == -1 || (checkDischarged && findRecordById(cardId) != -1));

    printf("\n  %sCurrent Information:%s\n", BRIGHT_YELLOW, RESET);
    int idx = findPatientById(cardId);
    printf("    Card ID: %s\n", PatientList[idx].cardId);
    printf("    Name: %s\n", PatientList[idx].name);
    printf("    Phone: %s\n", PatientList[idx].phone);
    printf("\n");

    do
    {
        printf("  %sEnter new Phone%s (Enter '-1' to keep current): ", BRIGHT_CYAN, RESET);
        fgets(phone, sizeof(phone), stdin);
        phone[strcspn(phone, "\n")] = '\0';
        phone[strcspn(phone, " ")] = '\0';
        if (strcmp(phone, "-1") == 0)
            break;
        validPhone = 1;
        for (int i = 0; i < (int)strlen(phone); i++)
        {
            if (phone[i] < '0' || phone[i] > '9')
            {
                validPhone = 0;
                break;
            }
        }
        if (strlen(phone) == 0)
        {
            printError("Phone cannot be empty!");
        }
        else if (strlen(phone) != 9)
        {
            printError("Phone must be 9 digits!");
        }
        else if (!validPhone)
        {
            printError("Phone must contain only numbers!");
        }
        else
        {
            strcpy(PatientList[findPatientById(cardId)].phone, phone);
        }
    } while (strlen(phone) == 0 || !validPhone || strlen(phone) != 9);

    savePatientData();
    printSeparator();
    printSuccess("Patient updated successfully!");
    waitForEnter();
    clearScreen();
}

void RemovePatient()
{
    char cardId[10], paymentChoice[10];
    double paymentAmount;
    clearScreen();
    printHeader("DISCHARGE FROM HOSPITAL");
    printSeparator();

    if (PatientCount == 0)
    {
        printError("No patients found in the system!");
        waitForEnter();
        clearScreen();
        return;
    }

    do
    {
        printf("  %sEnter Card ID to discharge%s (Enter '-1' to return): ", BRIGHT_CYAN, RESET);
        fgets(cardId, sizeof(cardId), stdin);
        cardId[strcspn(cardId, "\n")] = '\0';
        cardId[strcspn(cardId, " ")] = '\0';
        if (strcmp(cardId, "-1") == 0)
        {
            clearScreen();
            return;
        }
        else if (strlen(cardId) == 0)
        {
            printError("Card ID cannot be empty!");
        }
        else if (findPatientById(cardId) == -1)
        {
            printError("Card ID not found!");
        }
    } while (strlen(cardId) == 0 || findPatientById(cardId) == -1);

    int patientIdx = findPatientById(cardId);
    if (PatientList[patientIdx].debt > 0)
    {
        printf("\n");
        printWarning("Patient has outstanding debt!");
        printf("  Debt amount: %s%.2lf%s\n\n", BRIGHT_RED, PatientList[patientIdx].debt, RESET);

        do
        {
            printf("  %sProcess payment? (1=Yes, 0=No)%s: ", BRIGHT_CYAN, RESET);
            fgets(paymentChoice, sizeof(paymentChoice), stdin);
            paymentChoice[strcspn(paymentChoice, "\n")] = '\0';
            paymentChoice[strcspn(paymentChoice, " ")] = '\0';
            if (strcmp(paymentChoice, "1") == 0)
            {
                do
                {
                    printf("  %sEnter payment amount%s: ", BRIGHT_CYAN, RESET);
                    char amountStr[20];
                    fgets(amountStr, sizeof(amountStr), stdin);
                    amountStr[strcspn(amountStr, "\n")] = '\0';
                    amountStr[strcspn(amountStr, " ")] = '\0';
                    paymentAmount = atof(amountStr);

                    if (paymentAmount <= 0)
                    {
                        printError("Payment amount must be greater than 0!");
                    }
                    else if (paymentAmount < PatientList[patientIdx].debt)
                    {
                        printWarning("Insufficient payment amount!");
                    }
                } while (paymentAmount <= 0 || paymentAmount < PatientList[patientIdx].debt);

                PatientList[patientIdx].debt = 0;
                break;
            }
            else if (strcmp(paymentChoice, "0") == 0)
            {
                printError("Cannot discharge patient with outstanding debt!");
                waitForEnter();
                clearScreen();
                return;
            }
            else
            {
                printError("Invalid choice! Enter 1 or 0.");
            }
        } while (1);
    }
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char todayDate[20];
    strftime(todayDate, sizeof(todayDate), "%d/%m/%Y", timeinfo);

    struct Record dischargeRecord;
    strcpy(dischargeRecord.recId, itoa(RecordCount + 1, (char[20]){}, 10));
    strcpy(dischargeRecord.cardId, cardId);
    strcpy(dischargeRecord.date, todayDate);
    strcpy(dischargeRecord.status, "Discharged");
    RecordList[RecordCount++] = dischargeRecord;
    savePatientData();
    saveRecordData();
    printSeparator();
    printSuccess("Patient discharged successfully!");
    waitForEnter();
    clearScreen();
}

void viewPatient()
{
    clearScreen();
    printHeader("VIEW PATIENT LIST");

    if (PatientCount == 0)
    {
        printSeparator();
        printInfo("No patients found in the system.");
        printSeparator();
        waitForEnter();
        clearScreen();
        return;
    }

    int itemsPerPage = 10;
    int currentPage = 1;
    int totalPages = (PatientCount + itemsPerPage - 1) / itemsPerPage;

    while (1)
    {
        clearScreen();
        printHeader("VIEW PATIENT LIST");
        printf("\n");
        printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);
        printf("%s| %-10s | %-28s | %-9s | %-16s | %-11s |%s\n", BRIGHT_CYAN, "Card ID", "Name", "Phone", "Debt", "Visit Days", RESET);
        printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);

        int startIdx = (currentPage - 1) * itemsPerPage;
        int endIdx = (currentPage * itemsPerPage < PatientCount) ? currentPage * itemsPerPage : PatientCount;

        for (int i = startIdx; i < endIdx; i++)
        {
            char debtStr[30];
            snprintf(debtStr, sizeof(debtStr), "%.0lf", PatientList[i].debt);

            printf("%s| %-10s | %-28s | %-9s | %s%-16s%s | %-11d |%s\n",
                   BRIGHT_CYAN,
                   PatientList[i].cardId,
                   PatientList[i].name,
                   PatientList[i].phone,
                   PatientList[i].debt > 0 ? BRIGHT_RED : BRIGHT_GREEN,
                   debtStr,
                   BRIGHT_CYAN,
                   PatientList[i].visitDays,
                   RESET);
        }
        printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);
        printf("\n  %sTotal Patients: %d | Page %d/%d%s\n", BRIGHT_YELLOW, PatientCount, currentPage, totalPages, RESET);
        printf("  %s[1] Previous | [2] Next | [0] Exit%s\n\n", BRIGHT_BLUE, RESET);

        printf("%s> Enter your choice: %s", BRIGHT_BLUE, RESET);
        char input[10];
        fgets(input, sizeof(input), stdin);
        int choice = atoi(input);

        if (choice == 2 && currentPage < totalPages)
        {
            currentPage++;
        }
        else if (choice == 1 && currentPage > 1)
        {
            currentPage--;
        }
        else if (choice == 0)
        {
            clearScreen();
            return;
        }
    }
}

void SearchPatientByName()
{
    clearScreen();
    printHeader("SEARCH PATIENT");
    printSeparator();

    if (PatientCount == 0)
    {
        printError("No patients found in the system!");
        waitForEnter();
        clearScreen();
        return;
    }

    char name[50];
    bool empty = false;
    do
    {
        printf("  %sEnter name to search%s (Enter '-1' to return): ", BRIGHT_CYAN, RESET);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        if (strcmp(name, "-1") == 0)
        {
            clearScreen();
            return;
        }
        int start = 0, end = strlen(name) - 1;
        while (start <= end && name[start] == ' ') start++;
        while (end >= start && name[end] == ' ') end--;
        if (start > end)
        {
            printError("Name cannot be empty or contain only spaces!");
            empty = true;
        }
        else empty = false;
    } while (empty);

    clearScreen();
    printHeader("SEARCH RESULTS");
    printf("\n");

    int found = 0;
    printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);
    printf("%s| %-10s | %-28s | %-9s | %-16s | %-11s |%s\n", BRIGHT_CYAN, "Card ID", "Name", "Phone", "Debt", "Visit Days", RESET);
    printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);
    for (int i = 0; i < PatientCount; i++)
    {
        if (strstr(strlwr(PatientList[i].name), strlwr(name)) != NULL)
        {
            char debtStr[30];
            snprintf(debtStr, sizeof(debtStr), "%.0lf", PatientList[i].debt);

            printf("%s| %-10s | %-28s | %-9s | %s%-16s%s | %-11d |%s\n",
                   BRIGHT_CYAN,
                   PatientList[i].cardId,
                   PatientList[i].name,
                   PatientList[i].phone,
                   PatientList[i].debt > 0 ? BRIGHT_RED : BRIGHT_GREEN,
                   debtStr,
                   BRIGHT_CYAN,
                   PatientList[i].visitDays,
                   RESET);
            found = 1;
        }
    }
    printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);

    if (!found)
    {
        printf("\n");
        printWarning("No patients found matching the search criteria.");
    }
    printf("\n");
    waitForEnter();
    clearScreen();
}

void SortAsendingByDebt()
{
    for (int i = 0; i < PatientCount - 1; i++)
    {
        for (int j = i + 1; j < PatientCount; j++)
        {
            if (PatientList[i].debt > PatientList[j].debt)
            {
                struct Patient temp = PatientList[i];
                PatientList[i] = PatientList[j];
                PatientList[j] = temp;
            }
        }
    }
    printSuccess("Patients sorted by debt in ascending order!");
}

void SortDescendingByDebt()
{
    for (int i = 0; i < PatientCount - 1; i++)
    {
        for (int j = i + 1; j < PatientCount; j++)
        {
            if (PatientList[i].debt < PatientList[j].debt)
            {
                struct Patient temp = PatientList[i];
                PatientList[i] = PatientList[j];
                PatientList[j] = temp;
            }
        }
    }
    printSuccess("Patients sorted by debt in descending order!");
}

void SortPatientByDebt()
{
    clearScreen();
    printHeader("SORT PATIENTS BY DEBT");
    printSeparator();

    if (PatientCount == 0)
    {
        printError("No patients found in the system!");
        waitForEnter();
        clearScreen();
        return;
    }

    do
    {
        printf("\n  %s[1]%s Ascending Order (Low to High)\n", BRIGHT_YELLOW, RESET);
        printf("  %s[2]%s Descending Order (High to Low)\n", BRIGHT_YELLOW, RESET);
        printf("  %s[0]%s Back to Main Menu\n\n", BRIGHT_RED, RESET);

        printf("%s> Enter your choice: %s", BRIGHT_BLUE, RESET);
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL || input[0] == '\n' || strcspn(input, " ") == 0)
        {
            clearScreen();
            printError("Please enter a valid choice!");
            waitForEnter();
            continue;
        }
        int choice = atoi(input);
        clearScreen();
        printHeader("SORT PATIENTS BY DEBT");

        switch (choice)
        {
        case 1:
            SortAsendingByDebt();
            break;
        case 2:
            SortDescendingByDebt();
            break;
        case 0:
            return;
        default:
            printError("Invalid choice! Please enter 0-2.");
            continue;
        }

        printf("\n");
        printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);
        printf("%s| %-10s | %-28s | %-9s | %-16s | %-11s |%s\n", BRIGHT_CYAN, "Card ID", "Name", "Phone", "Debt", "Visit Days", RESET);
        printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);

        for (int i = 0; i < PatientCount; i++)
        {
            char debtStr[30];
            snprintf(debtStr, sizeof(debtStr), "%.0lf", PatientList[i].debt);

            printf("%s| %-10s | %-28s | %-9s | %s%-16s%s | %-11d |%s\n",
                   BRIGHT_CYAN,
                   PatientList[i].cardId,
                   PatientList[i].name,
                   PatientList[i].phone,
                   PatientList[i].debt > 0 ? BRIGHT_RED : BRIGHT_GREEN,
                   debtStr,
                   BRIGHT_CYAN,
                   PatientList[i].visitDays,
                   RESET);
        }
        printf("%s+------------+------------------------------+-----------+------------------+-------------+%s\n", BRIGHT_CYAN, RESET);

        waitForEnter();
        clearScreen();
        return;

    } while (1);
}

bool checkRealDate(char date[])
{
    char dayStr[3], monthStr[3], yearStr[5];
    int day, month, year;
    if (strlen(date) != 10 || date[2] != '/' || date[5] != '/') {
        return false;
    }
    sscanf(date, "%2s/%2s/%4s", dayStr, monthStr, yearStr);
    day = atoi(dayStr);
    month = atoi(monthStr);
    year = atoi(yearStr);
    if (month < 1 || month > 12)
    {
        return false;
    }
    int maxDays;
    switch (month)
    {
    case 1:case 3:case 5:case 7:case 8:case 10:case 12:
        maxDays = 31;
        break;
    case 4:case 6:case 9:case 11:
        maxDays = 30;
        break;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            maxDays = 29;
        }
        else
        {
            maxDays = 28;
        }
        break;
    default:
        return false;
    }
    if (day < 1 || day > maxDays)
    {
        return false;
    }
    return true;
}

bool checkDateUsed(char date[])
{
    for (int i = 0; i < RecordCount; i++)
    {
        if (strcmp(RecordList[i].date, date) == 0)
        {
            return true;
        }
    }
    return false;
}

bool checkFutureDate(char date[])
{
    char dayStr[3], monthStr[3], yearStr[5];
    int day, month, year;
    sscanf(date, "%2s/%2s/%4s", dayStr, monthStr, yearStr);
    day = atoi(dayStr);
    month = atoi(monthStr);
    year = atoi(yearStr);

    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    int currentDay = timeinfo->tm_mday;
    int currentMonth = timeinfo->tm_mon + 1;
    int currentYear = timeinfo->tm_year + 1900;

    if (year > currentYear ||
        (year == currentYear && month > currentMonth) ||
        (year == currentYear && month == currentMonth && day > currentDay))
    {
        return true;
    }
    return false;
}

void inputNewRecord()
{
    char cardId[10], date[20], status[100];
    struct Record newRecord;
    bool checkDischarged = false;
    clearScreen();
    printHeader("MEDICAL EXAMINATION RECORD");
    printSeparator();

    if (PatientCount == 0)
    {
        printError("No patients found! Please add patients first.");
        waitForEnter();
        clearScreen();
        return;
    }

    do
    {
        printf("  %sEnter Card ID of patient%s (Enter '-1' to return): ", BRIGHT_CYAN, RESET);
        fgets(cardId, sizeof(cardId), stdin);
        cardId[strcspn(cardId, "\n")] = '\0';
        cardId[strcspn(cardId, " ")] = '\0';
        if (strcmp(cardId, "-1") == 0)
        {
            clearScreen();
            return;
        }
        else if (strlen(cardId) == 0)
        {
            printError("Card ID cannot be empty!");
        }
        else if (findPatientById(cardId) == -1)
        {
            printError("Card ID not found!");
        }
        else if (findRecordById(cardId) != -1)
        {
            if (checkDischargedPatient(cardId))
            {
                checkDischarged = true;
                printError("Cannot add record for discharged patient!");
                waitForEnter();
                clearScreen();
                return;
            }
        }
    } while (strlen(cardId) == 0 || findPatientById(cardId) == -1 || (checkDischarged && findRecordById(cardId) != -1));

    do
    {
        printf("  %sEnter Record Date (DD/MM/YYYY)%s: ", BRIGHT_CYAN, RESET);
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = '\0';
        date[strcspn(date, " ")] = '\0';
        if (strlen(date) == 0)
        {
            printError("Date cannot be empty!");
        }
        else if (checkDateUsed(date))
        {
            printWarning("Date already used for another record!");
        }
        else if (!checkRealDate(date))
        {
            printError("Invalid date format! Use DD/MM/YYYY");
        }
        else if (checkFutureDate(date))
        {
            printError("Date cannot be in the future!");
        }
    } while (strlen(date) == 0 || !checkRealDate(date) || checkDateUsed(date) || checkFutureDate(date));

    do
    {
        printf("  %sEnter Status (1=Follow-Up/2=Re-Examination)%s: ", BRIGHT_CYAN, RESET);
        fgets(status, sizeof(status), stdin);
        status[strcspn(status, "\n")] = '\0';
        status[strcspn(status, " ")] = '\0';
        if (strlen(status) == 0)
        {
            printError("Status cannot be empty!");
        }
        if (strcmp(status, "1") == 0)
        {
            strcpy(status, "Follow-Up");
        }
        else if (strcmp(status, "2") == 0)
        {
            strcpy(status, "Re-Examination");
        }
        else
        {
            printError("Invalid status!");
        }
    } while (strlen(status) == 0 || (strcmp(status, "Follow-Up") != 0 && strcmp(status, "Re-Examination") != 0));
    
    strcpy(newRecord.recId, itoa(RecordCount + 1, (char[20]){}, 10));
    strcpy(newRecord.cardId, cardId);
    strcpy(newRecord.date, date);
    strcpy(newRecord.status, status);
    PatientList[findPatientById(cardId)].visitDays += 1;
    RecordList[RecordCount++] = newRecord;
    savePatientData();
    saveRecordData();
    printSeparator();
    printSuccess("Medical record added successfully!");
    waitForEnter();
    clearScreen();
}

void viewRecord()
{
    char cardId[10];
    clearScreen();
    printHeader("VIEW MEDICAL RECORDS");
    printSeparator();

    if (RecordCount == 0)
    {
        printError("No medical records found in the system!");
        waitForEnter();
        clearScreen();
        return;
    }

    do
    {
        printf("  %sEnter Card ID to view records%s (Enter '-1' to return): ", BRIGHT_CYAN, RESET);
        fgets(cardId, sizeof(cardId), stdin);
        cardId[strcspn(cardId, "\n")] = '\0';
        cardId[strcspn(cardId, " ")] = '\0';
        if (strcmp(cardId, "-1") == 0)
        {
            clearScreen();
            return;
        }
        else if (strlen(cardId) == 0)
        {
            printError("Card ID cannot be empty!");
        }
        else if (findPatientById(cardId) == -1)
        {
            printError("Card ID not found!");
        }
    } while (strlen(cardId) == 0 || findPatientById(cardId) == -1);

    // Count records for this patient
    int patientRecordCount = 0;
    for (int i = 0; i < RecordCount; i++)
    {
        if (strcmp(RecordList[i].cardId, cardId) == 0)
        {
            patientRecordCount++;
        }
    }

    if (patientRecordCount == 0)
    {
        clearScreen();
        printHeader("MEDICAL RECORDS");
        printf("\n");
        printWarning("No medical records found for this patient.");
        printf("\n");
        waitForEnter();
        clearScreen();
        return;
    }

    int itemsPerPage = 10;
    int currentPage = 1;
    int totalPages = (patientRecordCount + itemsPerPage - 1) / itemsPerPage;

    while (1)
    {
        clearScreen();
        printHeader("MEDICAL RECORDS");
        printf("\n");
        printf("%s+----------+-----------+------------+----------------------------------+%s\n", BRIGHT_CYAN, RESET);
        printf("%s| %-8s | %-9s | %-10s | %-32s |%s\n", BRIGHT_CYAN, "Rec ID", "Card ID", "Date", "Status", RESET);
        printf("%s+----------+-----------+------------+----------------------------------+%s\n", BRIGHT_CYAN, RESET);

        int startIdx = (currentPage - 1) * itemsPerPage;
        int endIdx = (currentPage * itemsPerPage < patientRecordCount) ? currentPage * itemsPerPage : patientRecordCount;
        int count = 0;

        for (int i = 0; i < RecordCount; i++)
        {
            if (strcmp(RecordList[i].cardId, cardId) == 0)
            {
                if (count >= startIdx && count < endIdx)
                {
                    printf("%s| %-8s | %-9s | %-10s | %-32s |%s\n",
                           BRIGHT_CYAN,
                           RecordList[i].recId,
                           RecordList[i].cardId,
                           RecordList[i].date,
                           RecordList[i].status,
                           RESET);
                }
                count++;
            }
        }
        printf("%s+----------+-----------+------------+----------------------------------+%s\n", BRIGHT_CYAN, RESET);
        printf("\n  %sTotal Records: %d | Page %d/%d%s\n", BRIGHT_YELLOW, patientRecordCount, currentPage, totalPages, RESET);
        printf("  %s[1] Previous | [2] Next | [0] Exit%s\n\n", BRIGHT_BLUE, RESET);

        printf("%s> Enter your choice: %s", BRIGHT_BLUE, RESET);
        char input[10];
        fgets(input, sizeof(input), stdin);
        int choice = atoi(input);

        if (choice == 2 && currentPage < totalPages)
        {
            currentPage++;
        }
        else if (choice == 1 && currentPage > 1)
        {
            currentPage--;
        }
        else if (choice == 0)
        {
            clearScreen();
            return;
        }
    }
}




