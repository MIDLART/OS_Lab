#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
	ok,
	invalid_arguments,
	no,
	not_open_file
} status_code;

status_code substr_search (char* substr, FILE** file) {
	char c;
	size_t len = strlen(substr);
	int i;

	while ((c = fgetc(*file)) != EOF) {
		i = 0;
		
		while (c == substr[i] && i < len) {
		    c = fgetc(*file);
		    i++;


		}

		if (i == len) {
		    return ok;
		}
	}
	return no;
}

status_code file_search (FILE* names, char* substr) {
	FILE* file;
	char file_name[256] = {0};
	char c;
	int i, flag = 0;
	pid_t pid;
	
	while ((c = fgetc(names)) != EOF && flag == 0) {
		i = 0;
		while (c != '\n' && c != '\t' && c != ' ' && c != EOF) {
			file_name[i] = c;
			i++;
			c = fgetc(names);
		}
		file_name[i] = '\0';
		
		if (i != 0) {
			pid = fork();
			if (pid == 0) {
				if ((file = fopen(file_name, "r")) != NULL) {
					if (substr_search(substr, &file) == no) {
						printf("No occurrences\n");
					} else {
						printf("%s\n", file_name);
					}

					fclose(file);
					
				} else {
					printf("Not open file\n");
				}
				flag = 1;
				break;
			}  else if (pid == -1) {
				printf("pid error\n");
			} 
		}
	}

	return ok;
}

int main (int argc, char* argv[]) {
	if (argc != 3) {
		printf("Invalid arguments\n");
		return invalid_arguments;
	}
	
	FILE* file_of_names = fopen(argv[2], "r");
	if (file_of_names == NULL) {
		printf("Not open file of names\n");
		return not_open_file;
	}
	
	file_search(file_of_names, argv[1]);
	
	fclose(file_of_names);
	return ok;
}
