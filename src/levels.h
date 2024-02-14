#ifndef LEVELS_H
#define LEVELS_H

#define LEVEL_SIZE sizeof(struct level)

struct level {
    int lev_num;
    int n_cells;
    int flag;
};

int get_int(int *num);
void read_master_levels(char *filename);
int select_level(char *filename, int id);
int insert_level(char *filename);
void update_level(char *filename, int id, int new_flag);
void delete_level(char *filename, int id);

int levels(int n);

#endif
