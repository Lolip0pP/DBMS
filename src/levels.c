#include "levels.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int levels(int n) {
    int r = 0, m = 0;
    char *filename = "../materials/master_levels.db";
    FILE *fp = fopen(filename, "rb");

    if (fp != NULL) {
        fclose(fp);
        switch (n) {
            case 0:;
                read_master_levels(filename);
                break;
            case 1:;
                printf("Вращайте барабан и выбирайте число! (номер уровня)\n");
                r = get_int(&m);
                if (r == 0) {
                    if (select_level(filename, m) == 0) printf("Нет уровня с номером %d\n", m);
                } else
                    printf("Число вводи, кому сказал?!\n");
                break;
            case 2:;
                printf("Введите номер уровня, номер ячейки и флаг защищённости:\n");
                if (insert_level(filename) != 0) printf("Да ну тебя, ничего вставлять не буду!\n");
                break;
            case 3:;
                printf("На каком уровне поменять флаг?\n");
                r = get_int(&m);
                if (select_level(filename, m) == 1) {
                    printf("На какой флаг поменять?\n");
                    int new_flag = 0;
                    r = get_int(&new_flag);
                    if (r == 0)
                        update_level(filename, m, new_flag);
                    else
                        printf("Такой флаг установить нельзя.\n");
                } else
                    printf("Нет уровня с номером %d\n", m);
                break;
            case 4:;
                printf("Какой уровень удалим? (число!!)\n");
                r = get_int(&m);
                if (r == 0)
                    delete_level(filename, m);
                else
                    printf("Да ну тебя, ничего удалять не буду!\n");
                break;
            case 5:;
                printf("На каком уровне установить защищённый режим?\n");
                r = get_int(&m);
                if (select_level(filename, m) == 1) {
                    update_level(filename, m, 1);
                    select_level(filename, m);
                    printf("Защищённый режим на уровне %d успешно установлен.\n", m);
                } else
                    printf("Нет уровня с номером %d\n", m);
                break;
            default:
                printf("n/a\n");
        }
    } else
        printf("n/a\n");

    return 0;
}

int get_int(int *num) {
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

void read_master_levels(char *filename) {
    FILE *fp = fopen(filename, "rb");
    int n = 0;
    struct level buf;

    while (fread(&buf, LEVEL_SIZE, 1, fp) == 1) {
        printf("%d %d %d\n", buf.lev_num, buf.n_cells, buf.flag);
        n++;
    }
    if (n == 0) printf("n/a");
    fclose(fp);
}

int select_level(char *filename, int id) {
    FILE *file = fopen(filename, "r+b");
    struct level tmp;
    int found = 0;

    while (fread(&tmp, LEVEL_SIZE, 1, file) != 0) {
        if (id == tmp.lev_num) {
            printf("%d %d %d\n", tmp.lev_num, tmp.n_cells, tmp.flag);
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

int insert_level(char *filename) {
    int d = 0;
    FILE *file = fopen(filename, "ab");
    struct level buf;

    int r = scanf("%d %d %d", &buf.lev_num, &buf.n_cells, &buf.flag);
    if (r == 3 && select_level(filename, buf.lev_num) == 0)
        fwrite(&buf, LEVEL_SIZE, 1, file);
    else {
        d = 1;
    }
    fclose(file);

    return d;
}

void update_level(char *filename, int id, int new_flag) {
    FILE *file = fopen(filename, "r+b");
    struct level tmp;
    long offset = 0;

    while (fread(&tmp, LEVEL_SIZE, 1, file) != 0) {
        if (id == tmp.lev_num) {
            fseek(file, offset, SEEK_SET);
            tmp.flag = new_flag;
            fwrite(&tmp, LEVEL_SIZE, 1, file);
            break;
        }
        offset += LEVEL_SIZE;
    }
    fclose(file);
}

void delete_level(char *filename, int id) {
    struct level myrecord;
    FILE *fp, *fp_tmp;
    int found = 0;

    fp = fopen(filename, "rb");
    fp_tmp = fopen("tmp.db", "wb");
    if (fp_tmp == NULL) {
        perror("Error opening temporary file\n");
        fclose(fp);
        return;
    }

    while (fread(&myrecord, LEVEL_SIZE, 1, fp) != 0) {
        if (id == myrecord.lev_num) {
            printf("A record with the requested name found and delete_leveld.\n");
            found = 1;
        } else {
            fwrite(&myrecord, LEVEL_SIZE, 1, fp_tmp);
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
