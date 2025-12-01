#ifndef PATIENT_H
#define PATIENT_H

#define PatientFile "./data/patients.dat"
#define RecordFile "./data/records.dat"

#define MAX 100

struct Patient {
    char cardId[10];
    char name[50];
    char phone[15];
    double debt;
    int visitDays;
};

struct Record {
    char recId[20];
    char cardId[10];
    char date[20];
    char status[20];
};

#endif