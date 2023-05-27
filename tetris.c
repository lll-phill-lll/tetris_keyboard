#include <stdlib.h>

#include "tetris.h"


tetris_state_t tetris_state;
figure_t next_figure;

void init_tetris_state() {
    tetris_state.has_moving_figure = 0;
    tetris_state.anim_counter = 1200;
    tetris_state.ms_per_move = 1200;
}

uint8_t move_figures_cell_down(uint8_t cell) {
    if (cell == 0 || cell == 5 || cell == 10 || cell == 15 || cell == 20 || cell == 25) {
        return 36 + cell;
    } else if (cell == 29) {
        return 65;
    }

    if (cell < 36) {
        return cell - 1;
    }
    return cell + 1;
}

char is_cell_last_row(uint8_t cell) {
    return cell == 40 || cell == 45 || cell == 50 || cell == 55 || cell == 60 || cell == 64 || cell == 67;
}

char is_figure_reached_end(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
    return is_cell_last_row(p1)
        || is_cell_last_row(p2)
        || is_cell_last_row(p3)
        || is_cell_last_row(p4);
}

void move_figure_down(void) {
    next_figure.p1 = move_figures_cell_down(next_figure.p1);
    next_figure.p2 = move_figures_cell_down(next_figure.p2);
    next_figure.p3 = move_figures_cell_down(next_figure.p3);
    next_figure.p4 = move_figures_cell_down(next_figure.p4);
}

void render_I_cell(RGB bitmap[KEY_NUM], uint8_t cell) {
    bitmap[cell].r = 0x87;
    bitmap[cell].g = 0xCE;
    bitmap[cell].b = 0xFA;
}

void render_T_cell(RGB bitmap[KEY_NUM], uint8_t cell) {
    bitmap[cell].r = 0xFF;
    bitmap[cell].g = 0x00;
    bitmap[cell].b = 0x00;
}

void render_field(RGB bitmap[KEY_NUM]) {
    switch(next_figure.type) {
        case I:
            render_I_cell(bitmap, next_figure.p1);
            render_I_cell(bitmap, next_figure.p2);
            render_I_cell(bitmap, next_figure.p3);
            render_I_cell(bitmap, next_figure.p4);
            break;
        case T:
            render_T_cell(bitmap, next_figure.p1);
            render_T_cell(bitmap, next_figure.p2);
            render_T_cell(bitmap, next_figure.p3);
            render_T_cell(bitmap, next_figure.p4);
            break;
    }
}

void spawn_figure(void) {
    next_figure.type = rand() % LAST_TYPE;
    switch (next_figure.type) {
        case I:
            next_figure.p1 = I_FIGURE_SPAWN_P1;
            next_figure.p2 = I_FIGURE_SPAWN_P2;
            next_figure.p3 = I_FIGURE_SPAWN_P3;
            next_figure.p4 = I_FIGURE_SPAWN_P4;
            break;
        case T:
            next_figure.p1 = T_FIGURE_SPAWN_P1;
            next_figure.p2 = T_FIGURE_SPAWN_P2;
            next_figure.p3 = T_FIGURE_SPAWN_P3;
            next_figure.p4 = T_FIGURE_SPAWN_P4;
            break;
        default:
            next_figure.p1 = 0;
            next_figure.p2 = 0;
            next_figure.p3 = 0;
            next_figure.p4 = 0;
            break;
    }
    return;
}

void do_move(void) {
    if (tetris_state.has_moving_figure == 0) {
        spawn_figure();
        tetris_state.has_moving_figure = 1;
    } else {
        move_figure_down();

        if (is_figure_reached_end(next_figure.p1, next_figure.p2, next_figure.p3, next_figure.p4)) {
            tetris_state.has_moving_figure = 0;
        }
    }

}

void get_next_move(uint32_t delta_time, RGB bitmap[KEY_NUM]) {
    if (tetris_state.anim_counter > delta_time) {
        tetris_state.anim_counter -= delta_time;
    } else {
        uint32_t remaining = delta_time - tetris_state.anim_counter;
        tetris_state.anim_counter = tetris_state.ms_per_move + remaining;
        do_move();
    }

    render_field(bitmap);

}


