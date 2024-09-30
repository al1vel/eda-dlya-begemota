#include <cstdio>

typedef struct {
    long int key_int;
    char key_ch;
    char text[100];
} Record;

void shakerSort(Record mass[], int count) {
    int left = 0, right = count - 1;
    int flag = 1;
    while ((left < right) && flag > 0) {
        flag = 0;
        for (int i = left; i < right; i++) {
            if (mass[i].key_int > mass[i+1].key_int) {
                Record t = mass[i];
                mass[i] = mass[i+1];
                mass[i+1] = t;
                flag = 1;
            }
            else if (mass[i].key_int == mass[i+1].key_int) {
                if (mass[i].key_ch > mass[i+1].key_ch) {
                    Record t = mass[i];
                    mass[i] = mass[i+1];
                    mass[i+1] = t;
                    flag = 1;
                }
            }
        }
        right--;
        for (int i = right; i > left; i--) {
            if (mass[i - 1].key_int > mass[i].key_int) {
                Record t = mass[i];
                mass[i] = mass[i-1];
                mass[i-1] = t;
                flag = 1;
            }
            else if (mass[i].key_int == mass[i+1].key_int) {
                if (mass[i].key_ch > mass[i+1].key_ch) {
                    Record t = mass[i];
                    mass[i] = mass[i+1];
                    mass[i+1] = t;
                    flag = 1;
                }
            }
        }
        left++;
        if(flag == 0) break;
    }
}

int binarySearch(Record arr[], int l, int r, long int key1, char key2) {
    while (l <= r) {
        int m = l + (r - l) / 2;
        if ((arr[m].key_int == key1) and (arr[m].key_ch == key2)) {
            return m;
        }
        if (arr[m].key_int < key1) {
            l = m + 1;
        }
        else if (arr[m].key_int == key1) {
            if (arr[m].key_ch < key2) {
                l = m + 1;
            }
            else {
                r = m - 1;
            }
        }
        else {
            r = m - 1;
        }
    }
    return -1;
}

void printArray(Record arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%ld: %c %s\n", arr[i].key_int, arr[i].key_ch, arr[i].text);
    printf("\n");
}

int main() {
    Record records[20];
    int n = 20;
    FILE* file = fopen("data.txt", "r");
    if (file == nullptr) {
        printf("Error opening file\n");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        fscanf(file, "%ld%c,%[^\n]", &records[i].key_int, &records[i].key_ch, records[i].text);
    }
    fclose(file);
    printf("Original array:\n");
    printArray(records, n);
    shakerSort(records, n);
    printf("Sorted array:\n");
    printArray(records, n);

    long int key1;
    char key2;

    printf("Enter key to search: ");
    scanf("%ld%c", &key1, &key2);
    int result = binarySearch(records, 0, n - 1, key1, key2);
    if (result != -1) {
        printf("Found %ld%c : %s\n", records[result].key_int, records[result].key_ch, records[result].text);
    }
    else {
        printf("Key not found\n");
    }
    return 0;
}
