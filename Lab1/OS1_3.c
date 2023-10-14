#include <stdio.h>
#include <string.h>

typedef enum {
    ok,
    invalid_arguments,
    not_open_file
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc != 3) {
        return invalid_arguments;
    }
    return ok;
}

status_code copy_file (char input[], char output[]) {
    FILE *input_file = fopen(input, "r");
    if(input_file == NULL) {
        return not_open_file;
    }
    FILE *output_file = fopen(output, "w");
    if(input_file == NULL) {
        fclose(input_file);
        return not_open_file;
    }
    
    char character;
    while ((character = fgetc(input_file)) != EOF) {
        fputc(character, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    return ok;
}

int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    if (check_parameters(argc, argv) == invalid_arguments){
        printf("Неверный ввод аргументов!\n");
        return invalid_arguments;
    }

    if (copy_file(argv[1], argv[2]) == not_open_file) {
        printf("Не удалось открыть файл!\n");
        return not_open_file;
    } else {
        printf("Копирование успешно!\n");
    }

    return 0;
}