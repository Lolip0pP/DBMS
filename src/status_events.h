#ifndef STATUS_EVENTS_H
#define STATUS_EVENTS_H

#define STATUS_SIZE sizeof(struct events)

struct events {
    int id;
    int module_id;
    int new_module_status;
    char date[11];
    char time[9];
};

int get8int(int *num);
void read_events(char *filename);
int select_event(char *filename, int id);
int insert(char *filename);
void update(char *filename, int id, int new_flag);
void delete (char *filename, int id);

int events(int n);

#endif
