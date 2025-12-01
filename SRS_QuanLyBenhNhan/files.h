#ifndef FILES_H
#define FILES_H

#include "config.h"

struct Patient PatientList[MAX];
struct Record RecordList[MAX];

int PatientCount;
int RecordCount;

void loadPatientData();
void savePatientData();
int findPatientById(const char *cardId);

void loadRecordData();
void saveRecordData();
int findRecordById(const char *recId);

#endif