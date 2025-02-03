# shell_asm
Un proyecto en el que se crea una shell usando asm y lenguaje C
# Shell ASM Terminal

Este proyecto es una interfaz de shell en C que ejecuta comandos a través de una función en ensamblador utilizando `execve`. Usa `ncurses` para la interfaz de usuario en la terminal.

## Requisitos

### **Linux**
- `gcc` (para compilar el código en C)
- `nasm` (para ensamblar el código en ASM)
- `ncurses` (para la interfaz de terminal)

Instalar dependencias en Debian/Ubuntu:
```bash
sudo apt update
sudo apt install gcc nasm libncurses5-dev libncursesw5-dev
```
En Arch/Manjaro:
```bash
sudo pacman -S gcc nasm ncurses
```

### **Windows**
- `MinGW-w64` (para `gcc` y ensamblador NASM)
- `PDCurses` (alternativa a ncurses para Windows)

Instalar dependencias con `MSYS2`:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-nasm mingw-w64-x86_64-pdcurses
```

## Compilación y Ejecución

### **Linux**
Compilar el código ensamblador:
```bash
nasm -f elf64 shell.asm -o shell.o
```
Compilar y enlazar con C:
```bash
gcc interfaz.c shell.o -o shell -no-pie -lncurses
```
Ejecutar:
```bash
./shell
```

### **Windows**
Compilar el código ensamblador:
```bash
nasm -f win64 shell.asm -o shell.o
```
Compilar y enlazar con C:
```bash
gcc interfaz.c shell.o -o shell.exe -lpdcurses
```
Ejecutar:
```bash
./shell.exe
```

## Uso
- Escribe un comando en la terminal y presiona `Enter` para ejecutarlo.
- Escribe `salir` para cerrar la aplicación.

## Notas
- En Windows, `ncurses` no está disponible, por lo que se usa `PDCurses`.
- El código ensamblador puede requerir ajustes para ejecutarse correctamente en Windows.

## Licencia
Este proyecto está bajo la licencia MIT.

