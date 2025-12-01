#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

struct Patient PatientList[MAX];
int PatientCount = 0;
struct Record RecordList[MAX];
int RecordCount = 0;

// Patient

void loadPatientData() {
    FILE *file = fopen(PatientFile, "rb");
    if (file == NULL) {
        perror("Error: can't read file");
        return;
    }
    fread(&PatientCount, sizeof(int), 1, file);
    fread(PatientList, sizeof(struct Patient), (size_t)PatientCount, file);
    fclose(file);
}

void savePatientData() {
    FILE *file = fopen(PatientFile, "wb");
    if (file == NULL) {
        perror("Error: can't write file");
        return;
    }
    fwrite(&PatientCount, sizeof(int), 1, file);
    fwrite(PatientList, sizeof(struct Patient), (size_t)PatientCount, file);
    fclose(file);
}

int findPatientById(const char *cardId) {
    for (int i = 0; i < PatientCount; i++) {
        if (strcmp(PatientList[i].cardId, cardId) == 0) {
            return i;
        }
    }
    return -1;
}

// Record

void loadRecordData() {
    FILE *file = fopen(RecordFile, "rb");
    if (file == NULL) {
        perror("Error: can't read file");
        return;
    }
    fread(&RecordCount, sizeof(int), 1, file);
    fread(RecordList, sizeof(struct Record), (size_t)RecordCount, file);
    fclose(file);
}

void saveRecordData() {
    FILE *file = fopen(RecordFile, "wb");
    if (file == NULL) {
        perror("Error: can't write file");
        return;
    }
    fwrite(&RecordCount, sizeof(int), 1, file);
    fwrite(RecordList, sizeof(struct Record), (size_t)RecordCount, file);
    fclose(file);
}

int findRecordById(const char *recId) {
    for (int i = 0; i < RecordCount; i++) {
        if (strcmp(RecordList[i].recId, recId) == 0) {
            return i;
        }
    }
    return -1;
}