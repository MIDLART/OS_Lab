#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>

typedef enum {
    ok,
    invalid_arguments,
    memory_not_allocated,
    invalid_pin
} status_code;

#define LOG_SIZE 6

typedef struct _user {
    char login[LOG_SIZE];
    int pin;
    int limitation;
    struct _user* next_user;
} User;

typedef struct {
    User *head, *tail;
} List;


void push_back(List *list, char *log, int p) {
    User *new_user = (User*)malloc(sizeof(User));
    if (new_user == NULL) {
        return;
    }
    strcpy(new_user -> login, log);
    new_user -> pin = p;
    new_user -> limitation = 0;
    new_user -> next_user = NULL;
    if (list -> tail != NULL) {
        list -> tail -> next_user = new_user;
    }
    if (list -> head == NULL) {
        list -> head = new_user;
    }
    list -> tail = new_user;
}

void del_list(List *list) {
    if (list -> head == NULL) {
        return;
    }
    if (list -> head == list -> tail) {
        free(list -> tail);
        list -> head = list -> tail = NULL;
        return;
    }
    User *t_user = list -> head;
    list -> head = t_user -> next_user;
    free(t_user);
    del_list(list);
}

status_code log_function (List *list, char* log, int p, int *limit) {
    if (list -> head == NULL) {
        return invalid_arguments;
    }
    User *t_user = list -> head;
    while (t_user != NULL) {
        if (strcmp(t_user -> login, log) == 0) {
            if (t_user -> pin != p) {
                return invalid_pin;
            } else {
                *limit = t_user->limitation;
                return ok;
            }
        }
        t_user = t_user -> next_user;
    }
    return invalid_arguments;
}

void cur_time () {
    time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);
    localTime = localtime(&currentTime);
    printf("Текущее время: %02d:%02d:%02d\n", 
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec);

}

void date () {
    time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);
    localTime = localtime(&currentTime);
    printf("Текущая дата: %02d.%02d.%04d\n", 
            localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);

}

status_code howmuch (char* str_time, char* flag) {
    if (flag[0] != '-') {
        return invalid_arguments;
    }

    if (strlen(str_time) != 10) {
        return invalid_arguments;
    }
    if (str_time[2] != '.' || str_time[5] != '.') {
        return invalid_arguments;
    }
    for (int i = 0; i < 10; i++) {
        if(isdigit(str_time[i]) == 0 && i != 2 && i != 5) {
            return invalid_arguments;
        }
    }

    int day = (str_time[0] - '0') * 10 + str_time[1] - '0';
    int mon = (str_time[3] - '0') * 10 + str_time[4] - '0';
    int year = (str_time[6] - '0') * 1000 + (str_time[7] - '0') * 100 + (str_time[8] - '0') * 10 + str_time[9] - '0';

    if (year <= 1970 || year > 2023 || mon > 12) {
		return invalid_arguments;
	}

    char months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mon == 2 && (year % 4 == 0 && year % 100 != 0 && year % 400 == 0)) {
        if (day > 29) {
            return invalid_arguments;
        }
    } else {
        if (day > months[mon - 1]) {
            return invalid_arguments;
        }
    }

    time_t currentTime = time(NULL);

    struct tm specified_time;
    specified_time.tm_year = year - 1900;
    specified_time.tm_mon = mon - 1;
    specified_time.tm_mday = day;
    specified_time.tm_hour = 0;
    specified_time.tm_min = 0;
    specified_time.tm_sec = 0;
    specified_time.tm_isdst = -1;

    time_t s_time = mktime(&specified_time);
    time_t diff = difftime(currentTime, s_time);

    switch(flag[1]) {
        case 's':
            printf("Прошло %d секунд\n", diff);
            break;
        case 'm':
            printf("Прошло %d минут\n", (diff / 60));
            break;
        case 'h':
            printf("Прошло %d часов\n", (diff / 3600));
            break;
        case 'y':
            printf("Прошло лет: %d\n", (diff / 31556952));
            break;
        default:
            printf("Неверный флаг\n");
            return invalid_arguments;
    }
    return ok;
}

void sanctions (char* log, int lim, List *list) {
    if (lim < 1 || lim > INT_MAX) {
        printf("Неверный limit\n");
        return;
    }
    User *t_user = list -> head;
    while (t_user != NULL) {
        if (strcmp(t_user -> login, log) == 0) {
            t_user -> limitation = lim;
            return;
        }
        t_user = t_user -> next_user;
    }
    printf("Неверный user\n");
    return;
}

void app (List *list, int limit) {
    printf("Введите команду\n");
    char character;
    int i = 0, j = 0, k = 0, lim;
    char buf[10];
    char buf2[11];
    char buf3[3];
    int no_lim = 1;
    if (limit != 0) {
        no_lim = 0;
    }
    while (strcmp(buf, "Logout") != 0 && (no_lim == 1 || limit > 0)) {
        buf[9] = '\0';
        buf2[10] = '\0';
        buf3[2] = '\0';
        scanf("%s", &buf);
        if (buf[9] != '\0') {
            printf("Ошибка!\n");
        }
        if (strcmp(buf, "Time") == 0) {
            cur_time();
        } else if (strcmp(buf, "Date") == 0) {
            date();
        } else if (strcmp(buf, "Howmuch") == 0) {
            scanf("%s %s", buf2, buf3);
            if (buf2[10] != '\0' || buf3[2] != '\0') {
                printf("Ошибка!\n");
            } else {
                if (howmuch(buf2, buf3) != ok) {
                    printf("Неверный ввод аргументов Howmuch\n");
                }
            }
        } else if (strcmp(buf, "Sanctions") == 0) {
            buf2[6] = '\0';
            scanf("%s", buf2);
            getchar();
            if (scanf("%d", &lim) != 1) {
                printf("Ошибка!!\n");
            }
            if (buf2[6] != '\0') {
                printf("Ошибка!!!\n");
            }
            sanctions(buf2, lim, list);

            printf("Лимит установлен\n");
        } else {
            printf("Нет такой команды\n");
        }

        limit--;
    }

    if (no_lim == 0 && limit == 0) {
        printf("Лимит исчерпан\n");
    }

    return;
}

int main () {
    printf("Введите l для авторизации\nВведите r для регистрации\nВведите e для выхода\n");
    char flag;
    int exit = 0, pin;
    char login[7];
    int flag_inv = 1;

    List *list = (List*)malloc(sizeof(List));
    if (list){
        list -> head = NULL;
        list -> tail = NULL;
    } else {
        printf("Ошибка!\n");
        return memory_not_allocated;
    }

    while (exit == 0) {
        login[6] = '\0';
        scanf("%c", &flag);
        switch (flag) {
        case 'r': {
            flag_inv = 1;

            printf("Введите login длинной до 6 символов\n");
            scanf("%s", &login);
            if (login[6] != '\0') {
                printf("Неверный ввод!\n");
                getchar();
                break;
            }

            printf("Введите pin от 0 до 100000\n");
            if (scanf("%d", &pin) != 1) {
                printf("Неверный ввод!\n");
                getchar();
                break;
            } else if (pin < 0 || pin > 1000000) {
                printf("Неверный ввод!\n");
                getchar();
                break;
            }

            push_back (list, login, pin);
            printf("Пользователь зарегистрирован!\n");
            printf("Введите l для авторизации\nВведите r для регистрации\nВведите e для выхода\n");
            getchar();
            break;
        }

        case 'l':
            flag_inv = 1;

            printf("Введите login\n");
            scanf("%s", &login);
            if (login[6] != '\0') {
                printf("Неверный ввод!\n");
                getchar();
                break;
            }

            printf("Введите pin\n");
            if (scanf("%d", &pin) != 1) {
                printf("Неверный ввод!\n");
                getchar();
                break;
            } else if (pin < 0 || pin > 1000000) {
                printf("Неверный ввод!\n");
                getchar();
                break;
            }
            int limit;
            int res_of_log = log_function (list, login, pin, &limit);
            if (res_of_log == ok) {
                printf("Пользователь авторизован!\n");
                app(list, limit);
                printf("Введите l для авторизации\nВведите r для регистрации\nВведите e для выхода\n");
            } else if (res_of_log == invalid_pin) {
                printf("Неверный pin\n");
            } else {
                printf("Неверный login\n");
            }
            getchar();
            break;

        case 'e':
            exit = 1;
            break;
        
        default:
            if (flag_inv) {
                printf("Неверный ввод!!!!\n");
            }
            flag_inv = 0;
            break;
        }
    }
    
    del_list(list);
    free(list);
    return 0;
} 
