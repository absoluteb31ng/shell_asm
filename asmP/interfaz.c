#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT 256
#define HIST_SIZE 10

extern void execute_command(char *comando);

char history[HIST_SIZE][MAX_INPUT];
int hist_index = 0, hist_count = 0, hist_pos = -1;

void limpiar_comando(char *comando) {
    int i = 0, j = 0;
    while (isspace(comando[i])) i++;
    while (comando[i]) {
        if (isprint(comando[i])) {
            comando[j++] = comando[i];
        }
        i++;
    }
    while (j > 0 && isspace(comando[j - 1])) j--;
    comando[j] = '\0';
}

void ejecutar_comando(char *comando) {
    pid_t pid = fork();
    if (pid == 0) {
        execute_command(comando);
        exit(0);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        printw("\nError al crear proceso\n");
        refresh();
    }
}

void handle_signal(int signo) {
    if (signo == SIGINT) {
        printw("\nPresiona 'salir' para cerrar la terminal\nShell> ");
        refresh();
    }
}

int main() {
    signal(SIGINT, handle_signal);
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));

    char comando[MAX_INPUT];
    int pos = 0;
    printw("[Shell ASM Terminal]\n ");
    refresh();

    while (1) {
        int ch = getch();
        if (ch == '\n') {
            comando[pos] = '\0';
            limpiar_comando(comando);
            if (strcmp(comando, "salir") == 0) break;

            if (pos > 0) {
                strcpy(history[hist_index], comando);
                hist_index = (hist_index + 1) % HIST_SIZE;
                if (hist_count < HIST_SIZE) hist_count++;
            }
            hist_pos = hist_index;

            ejecutar_comando(comando);
            printw("\nShell> ");
            refresh();
            pos = 0;
            memset(comando, 0, sizeof(comando));
        } else if (ch == 127 || ch == KEY_BACKSPACE) {
            if (pos > 0) {
                pos--;
                mvdelch(getcury(stdscr), getcurx(stdscr) - 1);
            }
        } else if (ch == KEY_UP && hist_count > 0) {
            hist_pos = (hist_pos - 1 + HIST_SIZE) % HIST_SIZE;
            strcpy(comando, history[hist_pos]);
            pos = strlen(comando);
            move(getcury(stdscr), 0);
            clrtoeol();
            printw("Shell> %s", comando);
            refresh();
        } else if (ch == KEY_DOWN && hist_count > 0) {
            hist_pos = (hist_pos + 1) % HIST_SIZE;
            strcpy(comando, history[hist_pos]);
            pos = strlen(comando);
            move(getcury(stdscr), 0);
            clrtoeol();
            printw("Shell> %s", comando);
            refresh();
        } else if (pos < MAX_INPUT - 1 && isprint(ch)) {
            comando[pos++] = ch;
            addch(ch);
        }
    }

    endwin();
    return 0;
}
