CC = gcc -std=c11
FLAGS = -Wall -Werror -Wextra
SOURCES = modules_db.c shared.c levels.c modules.c status_events.c

all: build_db

build_db:
	$(CC) $(FLAGS) $(SOURCES) -o ../build/build_db

rebuild: 
	clean 
	all