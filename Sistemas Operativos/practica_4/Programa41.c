#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#define COLOR_SERPIENTE 1
#define COLOR_MANZANA 2
#define COLOR_GAME_OVER 3
#define COLOR_TITULO 4

int tecla_presionada(void);
int se_estrello(int *, int, int, int);
void mover_serpiente(int *, int, int, int, int);
void poner_manzana(int *, int *, int *, int, int);
void comer_manzana(int *, int, int, int *, int *);

int main(void)
{
    size_t i, j;
    int serpiente[100][2]; // Cien pares ordenados (x, y)

    /* Se inicializan todas las coordenadas con x = 0, y = 0 */
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 2; j++) {
            serpiente[i][j] = 0;
        }
    }

    /* La serpiente comienza a avanzar en las coordenadas (3, 3) */
    serpiente[0][0] = 3;
    serpiente[0][1] = 3;

    int tecla = 0; // Para guardar la tecla que presionó el usuario

    /**
     * Para definir el eje sobre el que se mueve la serpiente
     * 1 - Avanza en esa dirección
     * 0 - No avanza en esa dirección
     */
    int dir_x = 1;
    int dir_y = 0;

    int velocidad = 10; // La velocidad con la que avanza la serpiente

    /* Las coordenadas en las que se va a colocar la manzana */
    int manzana_x = 0;
    int manzana_y = 0;

    /**
     * Para saber si la serpiente se ha comido la manzana
     * 
     * 1 - Sí
     * 0 - No
     */
    int manzana_comida = 1;
    int longitud_serpiente = 20;

    int ancho_pantalla;
    int alto_pantalla;

    initscr();
    curs_set(false);
    noecho();

    if (has_colors() == false) {
        printw("Tu terminal no soporta el texto coloreado.\n");
        refresh();
        sleep(3);
    }

    getmaxyx(stdscr, alto_pantalla, ancho_pantalla);

    while (!se_estrello(&serpiente[0][0], longitud_serpiente, alto_pantalla, ancho_pantalla)) {
        erase();

        if (has_colors()) {
            start_color();
            init_pair(COLOR_TITULO, COLOR_CYAN, COLOR_BLACK);

            attron(COLOR_PAIR(COLOR_TITULO));
            mvprintw(0, 0, "SNAKE GAME");
            attroff(COLOR_PAIR(COLOR_TITULO));
        } else {
            mvprintw(4, 0, "SNAKE GAME");
        }

        poner_manzana(&manzana_x, &manzana_y, &manzana_comida, ancho_pantalla, alto_pantalla);
        mover_serpiente(&serpiente[0][0], dir_x, dir_y, velocidad, longitud_serpiente);
        comer_manzana(&serpiente[0][0], manzana_x, manzana_y, &manzana_comida, &longitud_serpiente);

        refresh();

        if (tecla_presionada()) {
            tecla = getch();

            if (tecla == 'w' && !(dir_y == 1 && dir_x == 0)) {
                dir_y = -1;
                dir_x = 0;
            }

            if (tecla == 's' && !(dir_y == -1 && dir_x == 0)) {
                dir_y = 1;
                dir_x = 0;
            }

            if (tecla == 'a' && !(dir_y == 0 && dir_x == 1)) {
                dir_y = 0;
                dir_x = -1;
            }

            if (tecla == 'd' && !(dir_y == 0 && dir_x == -1)) {
                dir_y = 0;
                dir_x = 1;
            }
        }
    }

    erase();

    if (has_colors()) {
        start_color();
        init_pair(COLOR_GAME_OVER, COLOR_RED, COLOR_BLACK);

        attron(COLOR_PAIR(COLOR_GAME_OVER));
        mvprintw(alto_pantalla/2-2, (ancho_pantalla-strlen("GAME OVER"))/2, "GAME OVER");
        attroff(COLOR_PAIR(COLOR_GAME_OVER));
    } else {
        mvprintw(alto_pantalla/2-2, (ancho_pantalla-strlen("GAME OVER"))/2, "GAME OVER");
    }

    refresh();
    mvprintw(alto_pantalla/2+1-2, (ancho_pantalla-strlen("Tu puntuacion es: 000"))/2, "Tu puntuacion es: %d", longitud_serpiente);
    refresh();
    mvprintw(alto_pantalla/2+2-2, (ancho_pantalla-strlen("Presiona cualquier tecla para continuar"))/2, "Presiona cualquier tecla para continuar");
    refresh();
    getch();

    endwin();

    return 0;
}

int tecla_presionada(void)
{
    struct timeval tv;
    fd_set leer_fd;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&leer_fd);
    FD_SET(0, &leer_fd);

    if (select(1, &leer_fd, NULL, NULL, &tv) == -1) {
        return 0;
    }

    if (FD_ISSET(0, &leer_fd)) {
        return 1;
    }

    return 0;
}

int se_estrello(int *serpiente, int longitud_serpiente, int alto_pantalla, int ancho_pantalla)
{
    size_t i;
    int x = *serpiente;
    int y = *(serpiente + 1);

    if (x < 0 || x > ancho_pantalla) {
        return 1;
    } else if (y < 0 || y > alto_pantalla) {
        return 1;
    }

    for (i = 2; i < longitud_serpiente * 2 + 2; i += 2) {
        if (x == *(serpiente + i) && y == *(serpiente + i + 1)) {
            return 1;
        }
    }

    return 0;
}

void mover_serpiente(int *serpiente, int dir_x, int dir_y, int velocidad, int longitud_serpiente)
{
    size_t i;
    int x_anterior = 0;
    int y_anterior = 0;
    int x_actual = *serpiente;
    int y_actual = *(serpiente + 1);
    int x_nueva = x_actual + dir_x;
    int y_nueva = y_actual + dir_y;

    *(serpiente) = x_nueva;
    *(serpiente + 1) = y_nueva;

    if (has_colors()) {
        start_color();
        init_pair(COLOR_SERPIENTE, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_MANZANA, COLOR_RED, COLOR_BLACK);

        attron(COLOR_PAIR(COLOR_SERPIENTE));
        mvprintw(y_nueva, x_nueva, "0");
        attroff(COLOR_PAIR(COLOR_SERPIENTE));
    } else {
        mvprintw(y_nueva, x_nueva, "*");
    }

    for (i = 2; i < longitud_serpiente * 2; i += 2) {
        x_anterior = x_actual;
        y_anterior = y_actual;
        x_actual = *(serpiente + i);
        y_actual = *(serpiente + i + 1);
        x_nueva = x_anterior;
        y_nueva = y_anterior;
        *(serpiente + i) = x_nueva;
        *(serpiente + i + 1) = y_nueva;

        if (has_colors()) {
            start_color();
            init_pair(COLOR_SERPIENTE, COLOR_GREEN, COLOR_BLACK);

            attron(COLOR_PAIR(COLOR_SERPIENTE));
            mvprintw(y_nueva, x_nueva, "*");
            attroff(COLOR_PAIR(COLOR_SERPIENTE));
        } else {
            mvprintw(y_nueva, x_nueva, "*");
        }
    }

    if (dir_y != 0)
        usleep(500000/velocidad);

    usleep(1000000/velocidad);
}

void poner_manzana(int *manzana_x, int *manzana_y, int *manzana_comida, int ancho_pantalla, int alto_pantalla)
{
    if (*manzana_comida) {
        srand(time(0));
        *manzana_x = (rand() % ancho_pantalla) + 1;
        *manzana_y = (rand() % alto_pantalla) + 1;
        *manzana_comida = 0;
    }

    if (has_colors()) {
        start_color();
        init_pair(COLOR_MANZANA, COLOR_RED, COLOR_BLACK);

        attron(COLOR_PAIR(COLOR_MANZANA));
        mvprintw(*manzana_y, *manzana_x, "0");
        attroff(COLOR_PAIR(COLOR_MANZANA));
    } else {
        mvprintw(*manzana_y, *manzana_x, "0");
    }
}

void comer_manzana(int *serpiente, int manzana_x, int manzana_y, int *manzana_comida, int *longitud_serpiente)
{
    int x = *serpiente;
    int y = *(serpiente + 1);

    if (x == manzana_x && y == manzana_y) {
        *manzana_comida = 1;
        *longitud_serpiente += 1;
        int long_serpiente = *longitud_serpiente;
        int lastX = *(serpiente + long_serpiente * 2 - 2);
        int lastY = *(serpiente + long_serpiente * 2 - 2 + 1);
        *(serpiente + long_serpiente * 2) = lastX;
        *(serpiente + long_serpiente * 2 + 1) = lastY;
    }

    mvprintw(1, 0, "Puntaje: %d", *longitud_serpiente);
}
