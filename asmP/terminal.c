#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT 256

int main() {
    initscr();            // Iniciar ncurses
    cbreak();             // Entrada sin buffer
    noecho();             // No mostrar caracteres
    keypad(stdscr, TRUE); // Activar teclas especiales
    start_color();        // Iniciar colores

    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Texto verde sobre negro
    bkgd(COLOR_PAIR(1));   // Aplicar color de fondo

    char comando[MAX_INPUT];
    int pos = 0;

    printw("Shell ASM Terminal\n");
    refresh();

    while (1) {
        int ch = getch(); // Capturar tecla

        if (ch == '\n') { // Si es ENTER, ejecutar comando
            comando[pos] = '\n';

            if (strcmp(comando, "salir") == 0) break; // Salir si escribe "salir"

            // Ejecutar comando en shell ASM
            FILE *pipe = popen(comando, "r");
            if (pipe) {
                char buffer[1024];
                while (fgets(buffer, sizeof(buffer), pipe)) {
                    printw("%s", buffer);
                }
                pclose(pipe);
            } else {
                printw("Error al ejecutar comando\n");
            }

            pos = 0; // Resetear input
            printw("\n> ");
            refresh();
        } else if (ch == 127 || ch == KEY_BACKSPACE) { // Borrar caracter
            if (pos > 0) {
                pos--;
                mvdelch(getcury(stdscr), getcurx(stdscr) - 1);
            }
        } else if (pos < MAX_INPUT - 1) { // Agregar caracter a comando
            comando[pos++] = ch;
            addch(ch);
        }
    }

    endwin(); // Cerrar ncurses
    return 0;
}
