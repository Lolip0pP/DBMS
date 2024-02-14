#include "modules.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int modules(int n) {
    int r = 0, m = 0;
    char *filename = "../materials/master_modules.db";
    FILE *fp = fopen(filename, "rb");

    if (fp != NULL) {
        fclose(fp);
        switch (n) {
            case 0:;
                read_modules(filename);
                break;
            case 1:;
                printf("Вращайте барабан и выбирайте число! (ID модуля)\n");
                r = getint(&m);
                if (r == 0) {
                    if (select_module(filename, m) == 0) printf("Нет модуля с ID = %d\n", m);
                } else
                    printf("Число вводи, кому сказал?!\n");
                break;
            case 2:;
                printf("Введите ID модуля, его название, номер уровня, номер ячейки и флаг удаления:\n");
                if (insert_module(filename) != 0) printf("Да ну тебя, ничего вставлять не буду!\n");
                break;
            case 3:;
                printf("В каком модуле поменять флаг?\n");
                r = getint(&m);
                if (select_module(filename, m) == 1) {
                    printf("На какой флаг поменять?\n");
                    int new_flag = 0;
                    r = getint(&new_flag);
                    if (r == 0)
                        update_module(filename, m, new_flag);
                    else
                        printf("Такой флаг установить нельзя.\n");
                } else
                    printf("Нет модуля с ID = %d\n", m);
                break;
            case 4:;
                printf("По какому ID удалять будем? (число!!)\n");
                r = getint(&m);
                if (r == 0) {
                    delete_module(filename, m);
                } else
                    printf("Да ну тебя, ничего удалять не буду!\n");
                break;
            case 5:;
                read_active_modules(filename);
                break;
            case 6:;
                printf("По какому ID удалять будем?\n");
                r = getint(&m);
                if (r == 0) {
                    delete_module(filename, m);
                } else
                    printf("Да ну тебя, ничего удалять не буду!\n");
                break;
            case 7:;
                printf("В каком модуле установить protected mode?\n");
                r = getint(&m);
                if (select_module(filename, m) == 1) {
                    update_module(filename, m, 0);
                    update_module(filename, m, 1);
                    select_module(filename, m);
                    update_module(filename, m, -20);
                    select_module(filename, m);
                    printf("Модуль №%d переведён в защитный режим.\n", m);
                } else
                    printf("Нет уровня с ID == %d, ха-ха-ха!\n", m);
                break;
            case 8:;
                printf("Над каким модулем поиздеваемся сегодня? (ID натуральным числом, пожалуйста)\n");
                r = getint(&m);
                printf("Куда эту красоту закинем? (номер уровня и номер ячейки)\n");
                int new_level = 0, new_cell = 0;
                int two = getint(&new_level) + getint(&new_cell);
                if (r + two == 0) {
                    change_place(filename, m, new_level, new_cell);
                    printf("Модуль с ID %d теперь можно найти по адресу: уровень %d, ячейка %d.\n", m, new_level, new_cell);
                } else
                    printf("Ты сам себя наказываешь этими ужасными выходками!\n");
                break;
            default:
                printf("n/a\n");
        }
    } else
        printf("n/a\n");

    return 0;
}

int getint(int *num) {
    int rc = 0;
    float tmp = 0;

    if (scanf("%f", &tmp) == 1) {
        if (tmp - (int)tmp > 0.0001)
            rc = 1;
        else
            *num = tmp;
    } else
        rc = 1;

    return rc;
}

void read_modules(char *filename) {
    FILE *fp = fopen(filename, "rb");
    int n = 0;
    struct modules buf;
    while (fread(&buf, MODULE_SIZE, 1, fp) == 1) {
        printf("%d %s %d %d %d\n", buf.id, buf.name, buf.n_level, buf.n_cell, buf.flag);
        n++;
    }
    if (n == 0) printf("n/a\n");
    fclose(fp);
}

int select_module(char *filename, int id) {
    FILE *file = fopen(filename, "r+b");
    struct modules buf;
    int found = 0;
    while (fread(&buf, MODULE_SIZE, 1, file) != 0) {
        if (id == buf.id) {
            printf("%d %s %d %d %d\n", buf.id, buf.name, buf.n_level, buf.n_cell, buf.flag);
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

int insert_module(char *filename) {
    int d = 0;
    FILE *file = fopen(filename, "ab");
    struct modules buf;

    int r = scanf("%d %30s %d %d %d", &buf.id, buf.name, &buf.n_level, &buf.n_cell, &buf.flag);
    if (r == 5 && select_module(filename, buf.id) == 0)
        fwrite(&buf, MODULE_SIZE, 1, file);
    else {
        d = 1;
    }
    fclose(file);

    return d;
}

void update_module(char *filename, int id, int new_flag) {
    FILE *file = fopen(filename, "r+b");
    struct modules tmp;
    long offset = 0;

    while (fread(&tmp, MODULE_SIZE, 1, file) != 0) {
        if (id == tmp.id) {
            fseek(file, offset, SEEK_SET);
            tmp.flag = new_flag;
            fwrite(&tmp, MODULE_SIZE, 1, file);
            break;
        }
        offset += MODULE_SIZE;
    }
    fclose(file);
}

void delete_module(char *filename, int id) {
    struct modules myrecord;
    FILE *fp, *fp_tmp;
    int found = 0;

    fp = fopen(filename, "rb");
    fp_tmp = fopen("tmp.db", "wb");
    if (fp_tmp == NULL) {
        perror("Error opening temporary file\n");
        fclose(fp);
        return;
    }

    while (fread(&myrecord, MODULE_SIZE, 1, fp) != 0) {
        if (id == myrecord.id) {
            printf("Модуль с ID = %d успешно удалён.\n", id);
            found = 1;
        } else {
            fwrite(&myrecord, MODULE_SIZE, 1, fp_tmp);
        }
    }

    fclose(fp);
    fclose(fp_tmp);
    remove(filename);
    rename("tmp.db", filename);

    if (!found) {
        printf("Нет модуля с таким ID: %d\n", id);
    }
}

void read_active_modules(char *filename) {
    FILE *fp = fopen(filename, "rb");
    int n = 0;
    struct modules buf;
    printf("Сейчас выведутся все активные модули, если они есть...\n");
    while (fread(&buf, MODULE_SIZE, 1, fp) == 1) {
        if (buf.flag == 1) {
            printf("%d %s %d %d %d\n", buf.id, buf.name, buf.n_level, buf.n_cell, buf.flag);
            n++;
        }
    }
    if (n == 0) printf("А вот и нет, из активного сегодня только я!\n");
    fclose(fp);
}

void change_place(char *filename, int id, int new_level, int new_cell) {
    FILE *file = fopen(filename, "r+b");
    struct modules tmp;
    long offset = 0;

    while (fread(&tmp, MODULE_SIZE, 1, file) != 0) {
        if (id == tmp.id) {
            fseek(file, offset, SEEK_SET);
            tmp.n_level = new_level;
            tmp.n_cell = new_cell;
            fwrite(&tmp, MODULE_SIZE, 1, file);
            break;
        }
        offset += MODULE_SIZE;
    }
    fclose(file);
}
