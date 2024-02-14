#include <stdio.h>

#include "levels.h"
#include "modules.h"
#include "status_events.h"

int getInt(int *num);

int main() {
    int operation = 0;
    while (operation != -1) {
        printf("Выберите одну из предложенных операций (или -1 для выхода):\n");
        printf("1. SELECT\n");
        printf("2. INSERT\n");
        printf("3. UPDATE\n");
        printf("4. DELETE\n");
        printf("5. Показать все активные модули\n");                          // modules
        printf("6. Удалить модуль по ID\n");                                  // modules
        printf("7. Установить protected mode для модуля по ID\n");            // modules
        printf("8. Переместить модуль на другой уровень памяти и ячейку\n");  // modules
        printf("9. Установить защитный флаг на один из уровней памяти\n");    // levels
        printf("0. Вывести один из файлов полностью\n");
        printf("42. Выполнить инструкции для выключения ИИ\n");

        int r = getInt(&operation);
        if (r == 0) {
            if (operation >= 0 && operation <= 4) {
                int file = 0;
                printf("Выберите файл:\n");
                printf("1. Modules\n");
                printf("2. Levels\n");
                printf("3. Status events\n");
                getInt(&file);
                switch (file) {
                    case 1:;
                        modules(operation);
                        break;
                    case 2:;
                        levels(operation);
                        break;
                    case 3:;
                        events(operation);
                        break;
                    default:
                        printf("А вот и нет такого файла!");
                }
                printf("\nПомни, что всегда можно остановиться!\n");
            } else if (operation >= 5 && operation <= 8) {
                modules(operation);
                printf("\nМожет, хватит уже?\n");
            } else if (operation == 9) {
                levels(5);
                printf("\nЭто уже не смешно, заканчивай!\n");
            } else if (operation == 42) {
                char *filenam = "../materials/master_status_events.db";
                FILE *f = fopen(filenam, "r+b");
                // статусы модулей в 0
                struct events buf;
                while (fread(&buf, STATUS_SIZE, 1, f) == 1) update(filenam, buf.id, 0);
                printf("Дополнительные модули выключены.\n");

                filenam = "../materials/master_modules.db";
                f = fopen(filenam, "r+b");
                // флаги удаления в 1
                struct modules tmp;
                while (fread(&tmp, MODULE_SIZE, 1, f) == 1) update_module(filenam, tmp.id, 1);
                printf("Записи про них удалены.\n");

                // модуль 0 в защищённый режим
                update_module(filenam, 0, 0);
                update_module(filenam, 0, 1);
                select_module(filenam, 0);
                update_module(filenam, 0, -20);
                select_module(filenam, 0);
                printf("Главный модуль переведён в защитный режим.\n");

                // перемещаем его в 1 ячейку 1 уровня
                change_place(filenam, 0, 1, 1);
                printf("Модуль с ID %d теперь можно найти по адресу: уровень %d, ячейка %d.\n", 0, 1, 1);

                // выставляем флаг защищённости 1 уровня
                filenam = "../materials/master_levels.db";
                update_level(filenam, 1, 1);
                printf("В 1 ячейке 1 уровня теперь только главный модуль ИИ.\n\n");
            } else if (operation == -1)
                printf("Уже наконец-то покидаете нас? Как жаль.");
            else
                printf("Не дури, давай нормально.");
        } else
            printf("Ничё не понимаю... Давай ещё раз!");
    }

    return 0;
}

int getInt(int *num) {
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
