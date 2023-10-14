#include <stdio.h>
#include <string.h>

typedef enum {
    ok,
    invalid_arguments,
    file_error
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc != 2) {
        return invalid_arguments;
    }
    return ok;
}

status_code file_recording (char* file_name, char* str) {
    FILE *input_file = fopen(file_name, "wb");
    if (input_file == NULL) {
        return file_error;
    }
    size_t len = strlen(str);
    if (fwrite(str, sizeof(char), len, input_file) != len) {
        fclose(input_file);
        return file_error;
    }
 
    fclose(input_file);
    return ok;
}

int main (int argc, char *argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    if (check_parameters(argc, argv) == invalid_arguments) {
        printf("Введите путь к файлу\n");
        return invalid_arguments;
    }

    unsigned char str[11] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    if (file_recording(argv[1], str) != ok) {
        printf("Не удалось открыть файл\n");
        return file_error;
    }
 
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("Не удалось открыть файл\n");
        return file_error;
    }

    char character;
    while (fread(&character, sizeof(char), 1, input_file)) {
        printf("%x\t", character);
        printf("%c %d %d %d %d %d %c %c\n", input_file->_base, input_file->_bufsiz,
                input_file->_charbuf, input_file->_cnt, input_file->_file, 
                input_file->_flag, input_file->_ptr, input_file->_tmpfname);

    }
    fclose(input_file);

    input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("Не удалось открыть файл\n");
        return file_error;
    }

    fseek(input_file, 3, SEEK_SET);

    char buf[4];
    if (fread(buf, sizeof(char), 4, input_file) != 4) {
        fclose(input_file);
        return file_error;
    }

    for (int i = 0; i < 4; i++) {
        printf("%u ", buf[i]);
    }
    
    fclose(input_file);

    return 0;
}
