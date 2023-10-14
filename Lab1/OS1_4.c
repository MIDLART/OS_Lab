#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

typedef enum {
    ok,
    invalid_arguments,
    not_open_file,
    overflow
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc < 3) {
        return invalid_arguments;
    }
    if (argc == 4) {
        size_t len = strlen(argv[3]);
        for (int i = 0; i < len; i++) {
            if (isdigit(argv[3][i]) != 0 && isalpha(argv[3][i]) != 0) {
                return invalid_arguments;
            }
            if (argv[3][i] > 'F') {
                return invalid_arguments;
            }
        }
    }
    return ok;
}

status_code flag_xor8 (char* file_name, unsigned int *res) {
    FILE *input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        return not_open_file;
    }

    unsigned int sum = 0;
    char byte;
    while ((byte = fgetc(input_file)) != EOF) {
        sum ^= byte;
    }
    *res = sum;

    fclose(input_file);
    return ok;
}

status_code flag_xor32 (char* file_name, unsigned int *res) {
    FILE *input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        return not_open_file;
    }

    unsigned char buf[4];
    unsigned int sum = 0, value = 0;
    int len = 0;
    while ((len = fread(buf, sizeof(unsigned char), 4, input_file)) > 0) {
        for (int i = 0; i < len; i++) {
            value |= buf[i] << (24 - 8 * i);
        }
        sum ^= value;
        value = 0;
    }
    *res = sum;

    fclose(input_file);
    return ok;
}

status_code flag_mask (char* file_name, unsigned long *res, char *hex) {
    FILE *input_file = fopen(file_name, "r");
    if (input_file == NULL) {
        return not_open_file;
    }

    unsigned long mask = strtoul(hex, NULL, 16);
    if (errno == ERANGE) {
        return overflow;
    } 

    unsigned long value;
    while (fread(&value, sizeof(unsigned long), 1, input_file) == 1) {
        if (value == mask) {
            (*res)++;
        }
    }

    fclose(input_file);
    return ok;
}

int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    if (check_parameters(argc, argv) == invalid_arguments){
        printf("Неверный ввод аргументов!\n");
        return invalid_arguments;
    }

    if (strcmp(argv[2], "xor8") == 0) {
        if (argc != 3) {
            printf("Неверный ввод аргументов!\n");
            return invalid_arguments;
        }
        unsigned int sum;
        if (flag_xor8 (argv[1], &sum) == not_open_file) {
            printf("Не удалось открыть файл\n");
            return not_open_file;
        }
        printf("%X\n", sum);

    } else if (strcmp(argv[2], "xor32") == 0) {
        if (argc != 3) {
            printf("Неверный ввод аргументов!\n");
            return invalid_arguments;
        }
        unsigned int sum;
        if (flag_xor32 (argv[1], &sum) == not_open_file) {
            printf("Не удалось открыть файл\n");
            return not_open_file;
        }
        printf("%X\n", sum);

    } else if (strcmp(argv[2], "mask") == 0) {
        if (argc != 4) {
            printf("Неверный ввод аргументов!\n");
            return invalid_arguments;
        }
        unsigned long sum = 0;
        if (flag_mask (argv[1], &sum, argv[3]) != ok) {
            printf("Ошибка!\n");
            return not_open_file;
        } 
        printf("%X\n", sum);

    } else {
        printf("Неверный ввод флага!\n");
        return invalid_arguments;
    }

    return 0;
}