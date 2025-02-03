#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 256

extern void execute_command(char *comando);

void limpiar_comando(char *comando) {
    int i, j = 0;
    for (i = 0; comando[i]; i++) {
        if (isprint(comando[i])) {
            comando[j++] = comando[i];
        }
    }
    comando[j] = '\0';
}

void ejecutar_comando(char *comando) {
    pid_t pid = fork();
    if (pid == 0) {
        execute_command(comando);
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));

    char comando[MAX_INPUT];
    int pos = 0;

    printw("Shell ASM Terminal\n> ");
    refresh();

    while (1) {
        int ch = getch();
        if (ch == '\n') {
            comando[pos] = '\0';
            limpiar_comando(comando);
            if (strcmp(comando, "salir") == 0) break;
            ejecutar_comando(comando);
            printw("\n> ");
            refresh();
            pos = 0;
        } else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (pos > 0) {
                pos--;
                mvdelch(getcury(stdscr), getcurx(stdscr) - 1);
            }
        } else if (pos < MAX_INPUT - 1) {
            comando[pos++] = ch;
            addch(ch);
        }
    }

    endwin();
    return 0;
}
