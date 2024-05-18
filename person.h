#ifndef __person_h__
#define __person_h__
#include "grades.h"
using namespace std;

typedef struct {
    char last_name[15];
    char initials[5];
    const char *gender;
    char group_number[3];
    grades student_grades;
} person;

#endif