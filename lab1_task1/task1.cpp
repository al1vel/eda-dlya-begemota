#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum ret_type_t {
	SUCCESS,
	ERROR,
	ERROR_NOT_A_NUM,
	ERROR_NEG_NUM
};

typedef ret_type_t(*callback)(char*);

ret_type_t funcForH (char* argv) {
	int num = atoi(argv);
	if (num < 0) return ERROR_NEG_NUM;
	if (num > 100) printf("NO SUCH NUMBERS\n");
	for (int i = num; i <= 100; ++i) {
		if (i % num == 0) {
			printf("%d\n", i);
			i += (num - 1);
		}
	}
	return SUCCESS;
}

ret_type_t funcForP(char* argv) {
	int num = atoi(argv);
	if (num < 0) return ERROR_NEG_NUM;

	int found = 0;
	for (int i = 2; i * i <= num; ++i) {
		if (num % i == 0) {
			found += 1;
		}
	}
	if (found != 0) {
		printf("%d is composite\n", num);
	}
	else if (num == 0) {
		printf("0 is not composite and primary\n");
	}
	else {
		printf("%d is primary\n", num);
	}
	return SUCCESS;
}

ret_type_t funcForS(char* argv) {
	int num = atoi(argv);
	int neg_flag = 0;
	if (num < 0) {
		neg_flag = 1;
		num *= -1;
	}

	char buf[BUFSIZ], *pb = buf + BUFSIZ - 1;
	*pb-- = 0;
	int base = 16;
	int r;
	while (num) {
		*pb-- = ((r = num % base) > 9) ? r - 10 + 'A' : r + '0';
		num /= base;
	}
	if (neg_flag) printf("-");
	printf("%s\n", pb + 1);
	return SUCCESS;
}

int find_flag(char* arg, const char** flags, int size) {
	for (int i = 0; i < 12; ++i) {
		if (!strcmp(arg, flags[i])) {
			return i;
		}
	}
	return -1;
}

ret_type_t validateNumber(char* argv) {
	if (!(argv)) return ERROR_NOT_A_NUM;
	if (*argv == '-') argv++;

	while (*argv) {
		if (!isdigit(*argv)) return ERROR_NOT_A_NUM;
		argv++;
	}
	return SUCCESS;
}




int main(int argc, char* argv[]) {
	const char* flags[] = { "-h", "-p", "-s", "-e", "-a", "-f", "/h", "/p", "/s", "/e", "/a", "/f" };
	callback cbs[] = { &funcForH, &funcForP, &funcForS };

	for (int i = 1; i < argc; ++i) {
		int ret = find_flag(argv[1], flags, sizeof(flags) / sizeof(char*));
		if (ret == -1) {
			printf("NOT A FLAG : %s", argv[i]);
			return -1;
		}
		else if (validateNumber(argv[i + 1]) == ERROR_NOT_A_NUM) {
			printf("NOT A NUMBER : %s", argv[i + 1]);
			return -1;
		}
		else {
			printf("ok\n");
			int CbsInt = ret % 6;
			callback find = cbs[CbsInt];
			ret_type_t ret = find(argv[i + 1]);
			i++;

			printf("ERROR_CODE: %d", ret);
		}
	}

	return 0;
}