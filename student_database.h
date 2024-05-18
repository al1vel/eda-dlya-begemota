#ifndef __student_database_h__
#define __student_database_h__
#include "person.h"

using namespace std;

typedef struct {
    person students[20];
    int count;
} student_database;

#endif