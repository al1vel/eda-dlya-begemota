#include "functions.h"
using namespace std;

int main() {
    student_database db;
    generateDatabase(&db, 20);
    printDatabase(&db);
    find_min_avg(db);
}
