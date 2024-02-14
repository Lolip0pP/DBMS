#include "status_events.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int events(int n) {
    int r = 0, m = 0;
    char *filename = "../materials/master_status_events.db";
    FILE *fp = fopen(filename, "rb");

    if (fp != NULL) {
        fclose(fp);
        switch (n) {
            case 0:;
                read_events(filename);
                break;
            case 1:;
                printf("Вращайте барабан и выбирайте число! (ID события)\n");
                r = get8int(&m);
                if (r == 0) {
                    if (select_event(filename, m) == 0) printf("Нет события с ID = %d\n", m);
                } else
                    printf("Число вводи, кому сказал?!\n");
                break;
            case 2:;
                printf("Введите ID события, ID модуля, новый статус модуля, дату и время:\n");
                if (insert(filename) != 0) printf("Да ну тебя, ничего вставлять не буду!\n");
                break;
            case 3:;
                printf("В каком событии поменять статус?\n");
                r = get8int(&m);
                if (select_event(filename, m) == 1) {
                    printf("На какой статус поменять?\n");
                    int new_flag = 0;
                    r = get8int(&new_flag);
                    if (r == 0)
                        update(filename, m, new_flag);
                    else
                        printf("Такой статус установить нельзя.\n");
                } else
                    printf("Нет события с ID = %d\n", m);
                break;
            case 4:;
                printf("По какому ID удалять будем? (число!!)\n");
                r = get8int(&m);
                if (r == 0) {
                    delete (filename, m);
                } else
                    printf("Да ну тебя, ничего удалять не буду!\n");
                break;
            default:
                printf("n/a\n");
        }
    } else
        printf("n/a\n");

    return 0;
}

int get8int(int *num) {
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

void read_events(char *filename) {
    FILE *fp = fopen(filename, "rb");
    int n = 0;
    struct events buf;
    while (fread(&buf, STATUS_SIZE, 1, fp) == 1) {
        printf("%d %d %d %s %s\n", buf.id, buf.module_id, buf.new_module_status, buf.date, buf.time);
        n++;
    }
    if (n == 0) printf("n/a\n");
    fclose(fp);
}

int select_event(char *filename, int id) {
    FILE *file = fopen(filename, "r+b");
    struct events buf;
    int found = 0;
    printf("Вращайте барабан и выбирайте число! (ID события)\n");
    while (fread(&buf, STATUS_SIZE, 1, file) != 0) {
        if (id == buf.id) {
            printf("%d %d %d %s %s\n", buf.id, buf.module_id, buf.new_module_status, buf.date, buf.time);
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

int insert(char *filename) {
    int d = 0;
    FILE *file = fopen(filename, "ab");
    struct events buf;

    int r = scanf("%d %d %d %10s %8s", &buf.id, &buf.module_id, &buf.new_module_status, buf.date, buf.time);
    if (r == 5 && select_event(filename, buf.id) == 0)
        fwrite(&buf, STATUS_SIZE, 1, file);
    else {
        d = 1;
    }
    fclose(file);

    return d;
}

void update(char *filename, int id, int new_flag) {
    FILE *file = fopen(filename, "r+b");
    struct events tmp;
    long offset = 0;

    while (fread(&tmp, STATUS_SIZE, 1, file) != 0) {
        if (id == tmp.id) {
            fseek(file, offset, SEEK_SET);
            tmp.new_module_status = new_flag;
            fwrite(&tmp, STATUS_SIZE, 1, file);
            break;
        }
        offset += STATUS_SIZE;
    }
    fclose(file);
}

void delete (char *filename, int id) {
    struct events myrecord;
    FILE *fp, *fp_tmp;
    int found = 0;

    fp = fopen(filename, "rb");
    fp_tmp = fopen("tmp.db", "wb");
    if (fp_tmp == NULL) {
        perror("Error opening temporary file\n");
        fclose(fp);
        return;
    }

    while (fread(&myrecord, STATUS_SIZE, 1, fp) != 0) {
        if (id == myrecord.id) {
            printf("A record with the requested name found and deleted.\n");
            found = 1;
        } else {
            fwrite(&myrecord, STATUS_SIZE, 1, fp_tmp);
        }
    }

    fclose(fp);
    fclose(fp_tmp);
    remove(filename);
    rename("tmp.db", filename);

    if (!found) {
        printf("No record found with this ID: %d\n", id);
    }
}
