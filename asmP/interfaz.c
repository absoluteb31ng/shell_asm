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

char history[HIST_SIZE][MAX_INPUT]; // Historial de comandos
int hist_count = 0; // Número de comandos en el historial
int hist_pos = -1;  // Posición actual en el historial (para navegación)

void limpiar_comando(char *comando) {
    int i = 0, j = 0;
    while (isspace(comando[i])) i++; // Eliminar espacios al inicio
    while (comando[i]) {
        if (isprint(comando[i])) {
            comando[j++] = comando[i];
        }
        i++;
    }
    while (j > 0 && isspace(comando[j - 1])) j--; // Eliminar espacios al final
    comando[j] = '\0';
}

void ejecutar_comando(char *comando) {
    pid_t pid = fork();
    if (pid == 0) {
        execute_command(comando);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            printw("\nError al esperar por el proceso hijo\n");
            refresh();
        }
    } else {
        printw("\nError al crear proceso\n");
        refresh();
    }
}

void handle_signal(int signo) {
    if (signo == SIGINT) {
        printw("\nPresiona 'salir' para cerrar la terminal\nOh presionar Ctrl + z\nShell> ");
        refresh();
    }
}

void agregar_al_historial(const char *comando) {
    // Si el comando está vacío, no lo agregamos al historial
    if (strlen(comando) == 0) {
        return;
    }

    // Si el historial está lleno, desplazamos los comandos hacia arriba
    if (hist_count == HIST_SIZE) {
        for (int i = 0; i < HIST_SIZE - 1; i++) {
            strncpy(history[i], history[i + 1], MAX_INPUT - 1);
            history[i][MAX_INPUT - 1] = '\0';
        }
    } else {
        hist_count++; // Incrementar el contador solo si no está lleno
    }

    // Agregar el nuevo comando al final del historial
    strncpy(history[hist_count - 1], comando, MAX_INPUT - 1);
    history[hist_count - 1][MAX_INPUT - 1] = '\0';
    hist_pos = hist_count; // Reiniciar la posición de navegación
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
    printw("Shell ASM Terminal\nShell> ");
    refresh();

    while (1) {
        int ch = getch();
        if (ch == '\n') {
            comando[pos] = '\0';
            limpiar_comando(comando);
            if (strcmp(comando, "salir") == 0) break;

            if (pos > 0) {
                agregar_al_historial(comando);
            }

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
            // Navegar hacia arriba en el historial
            if (hist_pos > 0) {
                hist_pos--;
                strncpy(comando, history[hist_pos], MAX_INPUT - 1);
                comando[MAX_INPUT - 1] = '\0';
                pos = strlen(comando);
                move(getcury(stdscr), 0);
                clrtoeol();
                printw("Shell> %s", comando);
                refresh();
            }
        } else if (ch == KEY_DOWN && hist_count > 0) {
            // Navegar hacia abajo en el historial
            if (hist_pos < hist_count - 1) {
                hist_pos++;
                strncpy(comando, history[hist_pos], MAX_INPUT - 1);
                comando[MAX_INPUT - 1] = '\0';
                pos = strlen(comando);
                move(getcury(stdscr), 0);
                clrtoeol();
                printw("Shell> %s", comando);
                refresh();
            } else if (hist_pos == hist_count - 1) {
                // Si estamos en el último comando, limpiar la línea
                hist_pos++;
                pos = 0;
                move(getcury(stdscr), 0);
                clrtoeol();
                printw("Shell> ");
                refresh();
                memset(comando, 0, sizeof(comando));
            }
        } else if (pos < MAX_INPUT - 1 && isprint(ch)) {
            comando[pos++] = ch;
            addch(ch);
        }
    }

    endwin();
    return EXIT_SUCCESS;
}
