#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

enum ret_type_t{
    SUCCESS,
    ERROR_NAN,
    ERROR_FULL,
    ERROR_ARGS,
    ERROR_MALLOC,
    ERROR_OPEN_FILE,
    ERROR_ITERATIONS_LIMIT,
    ERROR_NO_SOLVE
};

void logErrors(int code) {
    switch (code) {
        case ERROR_NAN: printf("Found not a number\n"); break;
        case ERROR_FULL: printf("Overflow detected\n"); break;
        case ERROR_ARGS: printf("Wrong arguments\n"); break;
        case ERROR_MALLOC: printf("Failed to malloc\n"); break;
        case ERROR_OPEN_FILE: printf("Failed to open file\n"); break;
        case ERROR_ITERATIONS_LIMIT: printf("Too many iterations. Time to shut up\n"); break;
        case ERROR_NO_SOLVE: printf("There is no solvement in your interval\n"); break;
        default: printf("UNKNOWN ERROR CODE\n"); break;
    }
}

void int_to_roman(int num, char *buffer) {
    struct Roman {
        int value;
        const char *symbol;
    } roman[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    buffer[0] = '\0';
    for (int i = 0; i < 13 && num > 0; i++) {
        while (num >= roman[i].value) {
            strcat(buffer, roman[i].symbol);
            num -= roman[i].value;
        }
    }
}

void int_to_fib_zec(unsigned int num, char *buffer) {
    unsigned int fib[64], i = 0;
    fib[0] = 1; fib[1] = 2;
    for (i = 2; i < 64 && fib[i - 1] <= num; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    buffer[0] = '\0';
    for (int j = i - 1; j >= 0; j--) {
        if (num >= fib[j]) {
            strcat(buffer, "1");
            num -= fib[j];
        } else {
            strcat(buffer, "0");
        }
    }
    strcat(buffer, "1");
}

void int_to_base(int num, int base, char *buffer, int uppercase) {
    char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (uppercase) {
        for (int i = 10; i < 36; i++) digits[i] = toupper(digits[i]);
    }

    char temp[64];
    int index = 0;
    int is_negative = num < 0;

    if (is_negative) num = -num;

    do {
        temp[index++] = digits[num % base];
        num /= base;
    } while (num > 0);

    if (is_negative) temp[index++] = '-';

    for (int i = 0; i < index; i++) {
        buffer[i] = temp[index - i - 1];
    }
    buffer[index] = '\0';
}

int base_to_decimal(const char *num_str, int base) {
    int result = 0;
    int is_negative = (*num_str == '-');
    if (is_negative) num_str++;

    while (*num_str) {
        char c = tolower(*num_str);
        int value = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
        result = result * base + value;
        num_str++;
    }
    return is_negative ? -result : result;
}

void memory_dump(const void *value, size_t size, char *buffer) {
    const unsigned char *byte = (const unsigned char *)value;
    char temp[16];
    buffer[0] = '\0';
    for (size_t i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            sprintf(temp, "%d", (byte[i] >> j) & 1);
            strcat(buffer, temp);
        }
        if (i < size - 1) strcat(buffer, " ");
    }
}

int overfprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    char custom_buffer[256];
    int total_written = 0;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'R':
                    format++;
                    if (*format == 'o') {
                        int flagMin = 0;
                        int num = va_arg(args, int);
                        if (num < 0) {
                            num *= -1;
                            flagMin = 1;
                        }
                        int_to_roman(num, custom_buffer);
                        if (flagMin) {
                            total_written += fprintf(stream, "-%s", custom_buffer);
                        }
                        else {
                            total_written += fprintf(stream, "%s", custom_buffer);
                        }
                    }
                    break;
                case 'Z':
                    format++;
                    if (*format == 'r') {
                        unsigned int num = va_arg(args, unsigned int);
                        int_to_fib_zec(num, custom_buffer);
                        total_written += fprintf(stream, "%s", custom_buffer);
                    }
                    break;
                case 'C':
                    format++;
                    int flagMin = 0;
                    int num = va_arg(args, int);
                    if (num < 0) {
                        num *= -1;
                        flagMin = 1;
                    }
                    int base = abs(va_arg(args, int));
                    if (base < 2 || base > 36) {
                        base = 10;
                    }
                    int_to_base(num, base, custom_buffer, *format == 'V');
                    if (flagMin) {
                        total_written += fprintf(stream, "-%s", custom_buffer);
                    }
                    else {
                        total_written += fprintf(stream, "%s", custom_buffer);
                    }
                    break;
                case 't':
                    format++;
                    if (*format == 'o') {
                        char *num_str = va_arg(args, char *);
                        int base = va_arg(args, int);
                        if (base < 2 || base > 36) base = 10;
                        int result = base_to_decimal(num_str, base);
                        total_written += fprintf(stream, "%d", result);
                    }
                    break;
                case 'T':
                    format++;
                    if (*format == 'O') {
                        char *num_str = va_arg(args, char *);
                        int base = va_arg(args, int);
                        if (base < 2 || base > 36) base = 10;
                        int result = base_to_decimal(num_str, base);
                        total_written += fprintf(stream, "%d", result);
                    }
                    break;
                case 'm':
                    format++;
                    switch (*format) {
                        case 'i': {
                            int value = va_arg(args, int);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stream, "%s", custom_buffer);
                            break;
                        }
                        case 'u': {
                            unsigned int value = va_arg(args, unsigned int);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stream, "%s", custom_buffer);
                            break;
                        }
                        case 'd': {
                            double value = va_arg(args, double);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stream, "%s", custom_buffer);
                            break;
                        }
                        case 'f': {
                            float value = (float)va_arg(args, double);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stream, "%s", custom_buffer);
                            break;
                        }
                    }
                    break;
                default:
                    fputc('%', stream);
                    fputc(*format, stream);
                    total_written += 2;
            }
        } else {
            fputc(*format, stream);
            total_written++;
        }
        format++;
    }

    va_end(args);
    return total_written;
}

int oversprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    char custom_buffer[256];
    int total_written = 0;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'R':
                    format++;
                    if (*format == 'o') {
                        int num = abs(va_arg(args, int));
                        int_to_roman(num, custom_buffer);
                        total_written += fprintf(stdout, "%s", custom_buffer);
                    }
                    break;
                case 'Z':
                    format++;
                    if (*format == 'r') {
                        unsigned int num = va_arg(args, unsigned int);
                        int_to_fib_zec(num, custom_buffer);
                        total_written += fprintf(stdout, "%s", custom_buffer);
                    }
                    break;
                case 'C':
                    format++;
                    int num = abs(va_arg(args, int));
                    int base = abs(va_arg(args, int));
                    if (base < 2 || base > 36) base = 10;
                    int_to_base(num, base, custom_buffer, *format == 'V');
                    total_written += fprintf(stdout, "%s", custom_buffer);
                    break;
                case 't':
                    format++;
                    if (*format == 'o') {
                        char *num_str = va_arg(args, char *);
                        int base = va_arg(args, int);
                        if (base < 2 || base > 36) base = 10;
                        int result = base_to_decimal(num_str, base);
                        total_written += fprintf(stdout, "%d", result);
                    }
                    break;
                case 'T':
                    format++;
                    if (*format == 'O') {
                        char *num_str = va_arg(args, char *);
                        int base = va_arg(args, int);
                        if (base < 2 || base > 36) base = 10;
                        int result = base_to_decimal(num_str, base);
                        total_written += fprintf(stdout, "%d", result);
                    }
                    break;
                case 'm':
                    format++;
                    switch (*format) {
                        case 'i': {
                            int value = va_arg(args, int);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stdout, "%s", custom_buffer);
                            break;
                        }
                        case 'u': {
                            unsigned int value = va_arg(args, unsigned int);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stdout, "%s", custom_buffer);
                            break;
                        }
                        case 'd': {
                            double value = va_arg(args, double);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stdout, "%s", custom_buffer);
                            break;
                        }
                        case 'f': {
                            float value = (float)va_arg(args, double);
                            memory_dump(&value, sizeof(value), custom_buffer);
                            total_written += fprintf(stdout, "%s", custom_buffer);
                            break;
                        }
                    }
                    break;
                default:
                    fputc('%', stdout);
                    fputc(*format, stdout);
                    total_written += 2;
            }
        } else {
            fputc(*format, stdout);
            total_written++;
        }
        format++;
    }

    va_end(args);
    return total_written;
}

int main() {
    overfprintf(stdout, "Roman: %Ro\n", -31);
    overfprintf(stdout, "Fibonacci-Zec: %Zr\n", 61);
    overfprintf(stdout, "Custom Base Low: %Cv\n", -255, 16);
    overfprintf(stdout, "Custom Base High: %CV\n", -255, 16);
    overfprintf(stdout, "Change base Low: %to\n", "-fff", 16);
    overfprintf(stdout, "Change base High: %TO\n", "-FF", 16);
    overfprintf(stdout, "Mem dump int: %mi\n", -5);
    overfprintf(stdout, "Mem dump unsigned int: %mu\n", -5);
    overfprintf(stdout, "Mem dump double: %md\n", -9.0);
    overfprintf(stdout, "Mem dump float: %mf\n", -7.0);

    return SUCCESS;
}