#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum ret_type_t {
	SUCCESS,
	ERROR,
	ERROR_NOT_A_NUM,
	ERROR_NEG_NUM,
	ERROR_INVALID_NUM,
	ERROR_NUMBER_TOO_BIG,
	NO_SUCH_NUMBERS
};

typedef ret_type_t(*callback)(char*);

long long pow(int base, int exp) {
	long long res = base;
	for (int i = 1; i < exp; i++) {
		res = res * base;
	}
	return res;
}

int toInt(char* argv) {
	int res = 0;
	int i = 0;
	int neg_flag = 0;
	if (argv[0] == '-') {
		neg_flag = 1;
		i = 1;
	}
	while (argv[i] != '\0') {
		res = res * 10 + argv[i] - '0';
		i++;
	}
	if (neg_flag) {
		res *= -1;
	}
	//printf("%d\n", res);
	return res;
}

ret_type_t funcForH (char* argv) {
	int num = toInt(argv);
	if (num < 0) return ERROR_NEG_NUM;
	if (num == 0) return ERROR_INVALID_NUM;
	if (num > 100) return NO_SUCH_NUMBERS;
	for (int i = num; i <= 100; ++i) {
		if (i % num == 0) {
			printf("%d\n", i);
			i += (num - 1);
		}
	}
	return SUCCESS;
}

ret_type_t funcForP(char* argv) {
	int num = toInt(argv);
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
	else if (num == 0 or num == 1) {
		printf("%d is not composite and primary\n", num);
	}
	else {
		printf("%d is primary\n", num);
	}
	return SUCCESS;
}

ret_type_t funcForS(char* argv) {
	int num = toInt(argv);
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
	int i = 0;
	while ((pb + 1)[i] != '\0') {
		printf("%c ", (pb + 1)[i]);
		i++;
	}
	printf("\n");
	return SUCCESS;
}

ret_type_t funcForE(char* argv) {
	int num = toInt(argv);
	if (10 < num or num <= 0) return ERROR_INVALID_NUM;

	for (int i = 1; i <= num; ++i) {
		for (int j = 1; j <= 10; ++j) {
			printf("%-11lld", pow(j,i));
		}
		printf("\n");
	}
	return SUCCESS;
}

ret_type_t funcForA(char* argv) {
	int num = toInt(argv);
	if (num < 0) return ERROR_NEG_NUM;
	long long res = ((1 + num) * (num)) / 2;
	printf("%lld\n", res);
	return SUCCESS;
}

ret_type_t funcForF(char* argv) {
	int num = toInt(argv);
	if (num < 0) return ERROR_NEG_NUM;
	if (num > 20) return ERROR_NUMBER_TOO_BIG;
	long long res = 1;
	while (num > 1) {
		res *= num--;
		//printf("%lld\n", res);
	}
	printf("%lld\n", res);
	return SUCCESS;
}

int find_flag(const char* arg, const char** flags, int size) {
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
	int first = *argv - '0';
	int cnt = 0;
	while (*argv) {
		if (!isdigit(*argv)) return ERROR_NOT_A_NUM;
		argv++;
		cnt++;
	}
	//printf("%d %d", cnt, first);
	if (cnt >= 11) return ERROR_NUMBER_TOO_BIG;
	if (cnt >= 10 and first > 2) return ERROR_NUMBER_TOO_BIG;
	return SUCCESS;
}

void ValidateCode(int code) {
	switch (code) {
		case SUCCESS: printf("SUCCESS\n"); break;
		case ERROR_NOT_A_NUM: printf("ERROR_NOT_A_NUM\n"); break;
		case ERROR_NEG_NUM: printf("ERROR_NEG_NUM\n"); break;
		case ERROR_INVALID_NUM: printf("ERROR_INVALID_NUM\n"); break;
		case ERROR_NUMBER_TOO_BIG: printf("ERROR_NUMBER_TOO_BIG\n"); break;
		case NO_SUCH_NUMBERS: printf("NO_SUCH_NUMBERS\n"); break;
		default: printf("ERROR_UNKNOWN\n"); break;
	}
}


int main(int argc, char* argv[]) {
	const char* flags[] = { "-h", "-p", "-s", "-e", "-a", "-f", "/h", "/p", "/s", "/e", "/a", "/f" };
	callback cbs[] = { &funcForH, &funcForP, &funcForS, &funcForE, &funcForA, &funcForF };

	for (int i = 1; i < argc; ++i) {
		int ret = find_flag(argv[1], flags, sizeof(flags) / sizeof(char*));
		int num_ret = validateNumber(argv[i + 1]);
		if (ret == -1) {
			printf("NOT A FLAG : %s", argv[i]);
			return -1;
		}
		else if (num_ret != SUCCESS) {
			ValidateCode(num_ret);
			return -1;
		}
		else {
			int CbsInt = ret % 6;
			callback find = cbs[CbsInt];
			ret_type_t fret = find(argv[i + 1]);
			i++;

			//printf("ERROR_CODE: %d", ret);
			ValidateCode(fret);
		}
	}

	return 0;
}