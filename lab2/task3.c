#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void func(char *substr, int cnt, ...) {
    va_list args;
    va_start(args, cnt);

    for (int i = 0; i < cnt; i++) {
        char *str = va_arg(args, char *);

        FILE* file = fopen(str, "r");
        if (file == NULL) {
            printf("Error opening file <%s>.\n", str);
        }
    }

    va_end(args);
}


int main() {
    func("hui", 3, "D:/aboba.txt", "D:/hui.txt", "D:/hui2.txt");
}