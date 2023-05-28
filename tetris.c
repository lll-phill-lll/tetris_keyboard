#include <stdlib.h>

#include "tetris.h"


tetris_state_t tetris_state;
figure_t next_figure;

void init_tetris_state() {
    tetris_state.has_moving_figure = 0;
    tetris_state.can_move_left = 0;
    tetris_state.can_move_right = 0;
    tetris_state.anim_counter = 1200;
    tetris_state.ms_per_move = 1200;
}

char is_cell_last_row(uint8_t cell) {
    return cell % 10 == 9 || cell == DOWN_CELL_1_COL || cell == DOWN_CELL_2_COL;
}

char is_next_figure_reached_end(void) {
    return is_cell_last_row(next_figure.p1)
        || is_cell_last_row(next_figure.p2)
        || is_cell_last_row(next_figure.p3)
        || is_cell_last_row(next_figure.p4);
}

void move_figure_down(void) {
    next_figure.p1++;
    next_figure.p2++;
    next_figure.p3++;
    next_figure.p4++;
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
        tetris_state.can_move_left = 1;
        tetris_state.can_move_right = 1;
    } else {
        move_figure_down();

        if (is_next_figure_reached_end()) {
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

char is_cell_on_left_edge(uint8_t cell) {
    return cell / 10 == 6 || cell == LEFT_CELL_1_ROW || cell == LEFT_CELL_2_ROW || cell == LEFT_CELL_9_ROW || cell == LEFT_CELL_10_ROW;
}

char is_next_figure_reached_left_edge(void) {
    return is_cell_on_left_edge(next_figure.p1)
        || is_cell_on_left_edge(next_figure.p2)
        || is_cell_on_left_edge(next_figure.p3)
        || is_cell_on_left_edge(next_figure.p4);
}

void tetris_move_left() {

    next_figure.p1 += 10;
    next_figure.p2 += 10;
    next_figure.p3 += 10;
    next_figure.p4 += 10;

}

void tetris_move_right() {
    next_figure.p1 -= 10;
    next_figure.p2 -= 10;
    next_figure.p3 -= 10;
    next_figure.p4 -= 10;
}


