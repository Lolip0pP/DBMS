#ifndef MODULES_H
#define MODULES_H

#define MODULE_SIZE sizeof(struct modules)

struct modules {
    int id;
    char name[30];
    int n_level;
    int n_cell;
    int flag;
};

int getint(int *num);
void read_modules(char *filename);
int select_module(char *filename, int id);
int insert_module(char *filename);
void update_module(char *filename, int id, int new_flag);
void delete_module(char *filename, int id);
void read_active_modules(char *filename);
void change_place(char *filename, int id, int new_level, int new_cell);

int modules(int n);

#endif
