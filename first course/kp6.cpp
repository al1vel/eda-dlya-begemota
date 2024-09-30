#include "functions.h"
using namespace std;

int main() {
    student_database db;
    generateDatabase(&db, 10);
    printDatabase(&db);
    find_min_avg(db);
}