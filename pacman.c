/*
 * PAC-MAN en C (terminal, ncurses)
 * ---------------------------------
 * Compilation :
 *     gcc -O2 -Wall -o pacman pacman.c -lncurses
 * Lancement :
 *     ./pacman
 *
 * Commandes :
 *     Flèches directionnelles (ou Z/Q/S/D) pour se déplacer
 *     P pour mettre en pause
 *     Echap ou Ctrl+C pour quitter
 *
 * Règles :
 *     - Manger tous les points "." pour gagner le niveau
 *     - Les gros points "O" (super-gommes) rendent les fantômes
 *       vulnérables pendant quelques secondes : on peut alors les manger
 *     - Toucher un fantôme normal fait perdre une vie
 *     - 3 vies au départ
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* ----------------------------- Constantes ----------------------------- */

#define ROWS 21
#define COLS 28
#define NUM_GHOSTS 4
#define FRIGHT_TICKS 35     /* durée de vulnérabilité des fantômes (ticks) */
#define FRAME_DELAY_US 120000 /* ~120ms par tick de jeu */

/* Symboles de la carte */
#define WALL   '#'
#define DOT    '.'
#define POWER  'O'
#define EMPTY  ' '
#define DOOR   '-' /* porte de la maison des fantômes, infranchissable par Pac-Man */

/* Carte du niveau. '#' = mur, '.' = point, 'O' = super-gomme,
 * ' ' = couloir vide, '-' = porte de la maison des fantômes. */
static char level[ROWS][COLS + 1] = {
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#O####.#####.##.#####.####O#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.##### ## #####.######",
    "     #.##### ## #####.#     ",
    "     #.##          ##.#     ",
    "     #.## ###--### ##.#     ",
    "######.## #      # ##.######",
    "      .   #      #   .      ",
    "######.## #      # ##.######",
    "     #.## ######## ##.#     ",
    "     #.##          ##.#     ",
    "     #.## ######## ##.#     ",
    "######.## ######## ##.######",
    "#............##............#",
};
/* NB: la carte ci-dessus est réduite à 21 lignes ; on complète si besoin. */

typedef struct {
    int y, x;
} Point;

typedef struct {
    double y, x;      /* position (utilise des doubles pour lisser le mouvement) */
    int dy, dx;       /* direction courante */
    int home_y, home_x;
    int alive;        /* mangé -> retourne à la maison */
    int color;
} Ghost;

static char map[ROWS][COLS + 1];
static int dots_left = 0;
static int score = 0;
static int lives = 3;
static int fright_timer = 0;
static int level_num = 1;
static int paused = 0;

static double pac_y, pac_x;
static int pac_dy = 0, pac_dx = -1;
static int pac_next_dy = 0, pac_next_dx = -1;

static Ghost ghosts[NUM_GHOSTS];

/* ------------------------------ Utilitaires ---------------------------- */

static int is_wall(int y, int x) {
    if (y < 0 || y >= ROWS || x < 0 || x >= COLS) return 1;
    char c = map[y][x];
    return c == WALL || c == DOOR;
}

static void reset_map(void) {
    memcpy(map, level, sizeof(level));
    dots_left = 0;
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            if (map[y][x] == DOT || map[y][x] == POWER) dots_left++;
}

static void init_positions(void) {
    pac_y = 17; pac_x = 13.5;
    pac_dy = 0; pac_dx = 0;
    pac_next_dy = 0; pac_next_dx = 0;

    int gy = 11, gx = 12;
    int colors[NUM_GHOSTS] = {1, 2, 3, 4};
    for (int i = 0; i < NUM_GHOSTS; i++) {
        ghosts[i].y = gy;
        ghosts[i].x = gx + i * 2;
        ghosts[i].home_y = gy;
        ghosts[i].home_x = gx + i * 2;
        ghosts[i].dy = -1; ghosts[i].dx = 0;
        ghosts[i].alive = 1;
        ghosts[i].color = colors[i];
    }
    fright_timer = 0;
}

/* Retourne 1 si (y,x) est une case franchissable pour un fantôme
 * (les fantômes peuvent traverser la porte, contrairement à Pac-Man). */
static int ghost_can_walk(int y, int x) {
    if (y < 0 || y >= ROWS || x < 0 || x >= COLS) return 1; /* tunnel */
    return map[y][x] != WALL;
}

static int pac_can_walk(int y, int x) {
    if (y < 0 || y >= ROWS || x < 0 || x >= COLS) return 1; /* tunnel */
    return !is_wall(y, x);
}

/* ------------------------------ Déplacements --------------------------- */

static void move_pacman(void) {
    /* Essaie d'appliquer la direction demandée si possible, sinon garde l'actuelle */
    int ty = (int)(pac_y + 0.5) + pac_next_dy;
    int tx = (int)(pac_x + 0.5) + pac_next_dx;
    if (pac_can_walk(ty, tx)) {
        pac_dy = pac_next_dy;
        pac_dx = pac_next_dx;
    }

    int ny = (int)(pac_y + 0.5) + pac_dy;
    int nx = (int)(pac_x + 0.5) + pac_dx;

    if (pac_can_walk(ny, nx)) {
        pac_y += pac_dy;
        pac_x += pac_dx;
    }

    /* Tunnel gauche/droite */
    if (pac_x < 0) pac_x = COLS - 1;
    if (pac_x >= COLS) pac_x = 0;

    int cy = (int)(pac_y + 0.5);
    int cx = (int)(pac_x + 0.5);
    if (cy >= 0 && cy < ROWS && cx >= 0 && cx < COLS) {
        char c = map[cy][cx];
        if (c == DOT) {
            map[cy][cx] = EMPTY;
            score += 10;
            dots_left--;
        } else if (c == POWER) {
            map[cy][cx] = EMPTY;
            score += 50;
            dots_left--;
            fright_timer = FRIGHT_TICKS;
        }
    }
}

static int dist2(int y1, int x1, int y2, int x2) {
    int dy = y1 - y2, dx = x1 - x2;
    return dy * dy + dx * dx;
}

static void move_ghost(Ghost *g) {
    int gy = (int)(g->y + 0.5);
    int gx = (int)(g->x + 0.5);

    /* Choisit une direction à chaque intersection (case centrée) */
    if (g->y == gy && g->x == gx) {
        int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        int best = -1;
        int best_score = -1;
        int target_y, target_x;

        if (!g->alive) {
            target_y = g->home_y;
            target_x = g->home_x;
        } else if (fright_timer > 0) {
            target_y = (int)(pac_y + 0.5);
            target_x = (int)(pac_x + 0.5);
        } else {
            target_y = (int)(pac_y + 0.5);
            target_x = (int)(pac_x + 0.5);
        }

        for (int i = 0; i < 4; i++) {
            int ndy = dirs[i][0], ndx = dirs[i][1];
            /* évite le demi-tour sauf si aucune autre option */
            if (ndy == -g->dy && ndx == -g->dx) continue;
            int ny = gy + ndy, nx = gx + ndx;
            if (!ghost_can_walk(ny, nx)) continue;

            int d;
            if (!g->alive || fright_timer == 0) {
                d = -dist2(ny, nx, target_y, target_x); /* se rapprocher */
            } else {
                d = dist2(ny, nx, target_y, target_x);  /* fuir */
            }
            if (d > best_score) {
                best_score = d;
                best = i;
            }
        }
        if (best == -1) {
            /* Aucune option sauf demi-tour */
            for (int i = 0; i < 4; i++) {
                int ny = gy + dirs[i][0], nx = gx + dirs[i][1];
                if (ghost_can_walk(ny, nx)) { best = i; break; }
            }
        }
        if (best != -1) {
            g->dy = dirs[best][0];
            g->dx = dirs[best][1];
        }
    }

    double speed = 1.0;
    g->y += g->dy * speed;
    g->x += g->dx * speed;

    if (g->x < 0) g->x = COLS - 1;
    if (g->x >= COLS) g->x = 0;

    /* Fantôme mangé de retour à la maison */
    if (!g->alive) {
        int cy = (int)(g->y + 0.5), cx = (int)(g->x + 0.5);
        if (cy == g->home_y && cx == g->home_x) {
            g->alive = 1;
        }
    }
}

/* -------------------------------- Rendu --------------------------------- */

static void draw(void) {
    erase();
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            char c = map[y][x];
            switch (c) {
                case WALL:
                    attron(COLOR_PAIR(5));
                    mvaddch(y, x, ' ' | A_REVERSE);
                    attroff(COLOR_PAIR(5));
                    break;
                case DOOR:
                    attron(COLOR_PAIR(6));
                    mvaddch(y, x, '-');
                    attroff(COLOR_PAIR(6));
                    break;
                case DOT:
                    attron(COLOR_PAIR(7));
                    mvaddch(y, x, '.');
                    attroff(COLOR_PAIR(7));
                    break;
                case POWER:
                    attron(COLOR_PAIR(7) | A_BOLD);
                    mvaddch(y, x, 'o');
                    attroff(COLOR_PAIR(7) | A_BOLD);
                    break;
                default:
                    mvaddch(y, x, ' ');
            }
        }
    }

    for (int i = 0; i < NUM_GHOSTS; i++) {
        Ghost *g = &ghosts[i];
        int color = (fright_timer > 0 && g->alive) ? 8 : g->color;
        attron(COLOR_PAIR(color) | A_BOLD);
        mvaddch((int)(g->y + 0.5), (int)(g->x + 0.5),
                (fright_timer > 0 && g->alive) ? 'm' : 'M');
        attroff(COLOR_PAIR(color) | A_BOLD);
    }

    attron(COLOR_PAIR(3) | A_BOLD);
    char pac_char = 'C';
    if (pac_dx > 0) pac_char = '>';
    else if (pac_dx < 0) pac_char = '<';
    else if (pac_dy > 0) pac_char = 'v';
    else if (pac_dy < 0) pac_char = '^';
    mvaddch((int)(pac_y + 0.5), (int)(pac_x + 0.5), pac_char);
    attroff(COLOR_PAIR(3) | A_BOLD);

    mvprintw(ROWS, 0, "Score: %-6d  Vies: %d  Niveau: %d   [Fleches: bouger | P: pause | Echap: quitter]",
              score, lives, level_num);
    if (paused) {
        mvprintw(ROWS + 1, 0, ">>> PAUSE <<<");
    }
    refresh();
}

/* ------------------------------- Logique --------------------------------- */

static void reset_after_death(void) {
    init_positions();
    napms(500);
}

static int check_collisions(void) {
    int py = (int)(pac_y + 0.5);
    int px = (int)(pac_x + 0.5);
    for (int i = 0; i < NUM_GHOSTS; i++) {
        Ghost *g = &ghosts[i];
        int gy = (int)(g->y + 0.5);
        int gx = (int)(g->x + 0.5);
        if (gy == py && gx == px) {
            if (fright_timer > 0 && g->alive) {
                /* Pac-Man mange le fantôme */
                g->alive = 0;
                g->y = g->home_y; g->x = g->home_x;
                score += 200;
            } else if (g->alive) {
                lives--;
                if (lives <= 0) return 1; /* game over */
                reset_after_death();
                return 0;
            }
        }
    }
    return 0;
}

static int input_to_dir(int ch, int *dy, int *dx) {
    switch (ch) {
        case KEY_UP: case 'z': case 'Z': case 'w': case 'W':
            *dy = -1; *dx = 0; return 1;
        case KEY_DOWN: case 's': case 'S':
            *dy = 1; *dx = 0; return 1;
        case KEY_LEFT: case 'q': case 'Q': case 'a': case 'A':
            *dy = 0; *dx = -1; return 1;
        case KEY_RIGHT: case 'd': case 'D':
            *dy = 0; *dx = 1; return 1;
        default:
            return 0;
    }
}

static void init_colors(void) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLUE); /* fantôme vulnérable */
}

static void show_message(const char *msg) {
    int h, w;
    getmaxyx(stdscr, h, w);
    (void)h;
    mvprintw(ROWS / 2, (w - (int)strlen(msg)) / 2 > 0 ? (w - (int)strlen(msg)) / 2 : 0, "%s", msg);
    refresh();
    napms(2000);
}

int main(void) {
    srand((unsigned)time(NULL));

    initscr();
    if (has_colors()) init_colors();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    reset_map();
    init_positions();

    int running = 1;
    while (running) {
        int ch;
        while ((ch = getch()) != ERR) {
            int dy, dx;
            if (ch == 27) { running = 0; break; }        /* Echap */
            if (ch == 'p' || ch == 'P') { paused = !paused; continue; }
            if (input_to_dir(ch, &dy, &dx)) {
                pac_next_dy = dy;
                pac_next_dx = dx;
            }
        }
        if (!running) break;

        if (!paused) {
            move_pacman();
            for (int i = 0; i < NUM_GHOSTS; i++) move_ghost(&ghosts[i]);
            if (fright_timer > 0) fright_timer--;

            if (check_collisions()) {
                draw();
                show_message("GAME OVER - Appuyez sur une touche pour quitter");
                nodelay(stdscr, FALSE);
                getch();
                running = 0;
                break;
            }

            if (dots_left <= 0) {
                draw();
                char buf[64];
                snprintf(buf, sizeof(buf), "NIVEAU %d TERMINE !", level_num);
                show_message(buf);
                level_num++;
                reset_map();
                init_positions();
            }
        }

        draw();
        usleep(FRAME_DELAY_US);
    }

    endwin();
    printf("Merci d'avoir joue ! Score final : %d\n", score);
    return 0;
}
