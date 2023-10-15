#include <stdio.h>
#include <string.h>

typedef enum {
    ok,
    invalid_arguments,
    not_open_file,
    recording_error
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc != 3) {
        return invalid_arguments;
    }
    return ok;
}

status_code copy_file (char input[], char output[]) {
    FILE *input_file = fopen(input, "rb");
    if(input_file == NULL) {
        return not_open_file;
    }
    FILE *output_file = fopen(output, "wb");
    if(input_file == NULL) {
        fclose(input_file);
        return not_open_file;
    }
    
    unsigned char character;
    while (fread(&character, sizeof(unsigned char), 1, input_file) != 0) {
        if (fwrite(&character, sizeof(unsigned char), 1, output_file) != 1) {
            fclose(input_file);
            fclose(output_file);
            return recording_error;
        }
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

    if (copy_file(argv[1], argv[2]) != ok) {
        printf("Ошибка!\n");
        return not_open_file;
    } else {
        printf("Копирование успешно!\n");
    }

    return 0;
}
