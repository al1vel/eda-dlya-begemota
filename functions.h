#ifndef __functions_h__
#define __functions_h__
#include "string"
#include <cstring>
#include "student_database.h"
#include <map>
#include <iostream>
using namespace std;


void generateDatabase(student_database * db, int cnt) {
    db -> count = 0;

    // Массивы для генерации случайных значений
    const char* last_names[] = { "Ivanov", "Petrov", "Sidorov", "Gusev", "Mironov" };
    const char* all_initials[] = { "A.B", "H.E", "A.P", "T.A", "C.B" };
    const char* genders[] = { "M", "F" };
    const char* all_group_numbers[] = { "114", "115", "103", "107", "109" };

    // Генерация 20 записей
    for (int i = 0; i < cnt; i++) {
        person st;

        // Фамилия
        strcpy(st.last_name, last_names[rand() % 5]);

        // Инициалы
        strcpy(st.initials, all_initials[rand() % 5]);

        // Пол
        st.gender = genders[rand() % 2];

        // Группа
        strcpy(st.group_number, all_group_numbers[rand() % 5]);

        // Оценки
        st.student_grades.mathematical_analysis = rand() % 4 + 2;
        st.student_grades.computer_science = rand() % 4 + 2;
        st.student_grades.mathematical_logic = rand() % 4 + 2;
        st.student_grades.linear_algebra = rand() % 4 + 2;
        st.student_grades.python_test = rand() % 4 + 2;
        st.student_grades.english = rand() % 4 + 2;

        //Avg
        float avg = ((float)st.student_grades.mathematical_analysis + (float)st.student_grades.computer_science + (float)st.student_grades.mathematical_logic +
                (float)st.student_grades.linear_algebra + (float)st.student_grades.python_test + (float)st.student_grades.english) / 6;
        st.student_grades.avg = avg;

        if (db -> count < 20) {
            db -> students[db -> count++] = st;
        }
    }
}

void printDatabase(const student_database * db) {
    printf("===========================================================================\n");
    printf("GRADES TABLE:\n");
    printf("%-9s %-8s %-3s %-5s %-6s %-6s %-6s %-6s %-6s %-6s %-4s\n", "Last Name", "Initials", "Gen", "Group", "MathAn", "ComSci", "MatLog", "LinAl", "Python", "Eng", "AVG");
    for (int i = 0; i < db -> count; i++) {
        const person* per = &db -> students[i];
        printf("%-9s %-8s %-3s %-5s %-6d %-6d %-6d %-6d %-6d %-6d %.2f\n",
               per -> last_name, per -> initials, per -> gender, per -> group_number, per -> student_grades.mathematical_analysis,
               per -> student_grades.computer_science, per -> student_grades.mathematical_logic, per -> student_grades.linear_algebra,
               per -> student_grades.python_test, per -> student_grades.english, per -> student_grades.avg);
    }
    printf("===========================================================================\n\n");
}

void find_min_avg(const student_database &db) {
    float min_avg = 10;
    for (int i = 0; i < db.count; ++i) {
        const person per = db.students[i];
        if (per.student_grades.avg < min_avg) {
            min_avg = per.student_grades.avg;
        }
    }
    printf("Min average mark: %.2f.\n", min_avg);
    map<string, int> group_data;
    for (int i = 0; i < db.count; ++i) {
        const person per = db.students[i];
        if (per.student_grades.avg == min_avg) {
            group_data[per.group_number] += 1;
        }
    }
    int max_losers = 0;
    string loser_gr;
    for (auto el : group_data) {
        if (el.second > max_losers) {
            max_losers = el.second;
        }
    }
    for (auto el : group_data) {
        if (el.second == max_losers) {
            loser_gr = el.first;
        }
    }
    printf("Loser group: %s.\n", loser_gr.c_str());
}

#endif