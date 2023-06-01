#include <stdlib.h>

#include "tetris.h"


tetris_state_t tetris_state;
figure_t next_figure;

void init_tetris_state() {
    tetris_state.has_moving_figure = 0;
    tetris_state.anim_counter = 50;
    tetris_state.ms_per_move = 50;
    tetris_state.move_down_counter = 800;
    tetris_state.ms_per_move_down = 800;
    tetris_state.last_saved_figure_index = -1;
    tetris_state.is_paused = 0;
    tetris_state.next_move = MOVE_NONE;

    for (uint8_t i = 0; i != 72; ++i) {
        tetris_state.field[i] = 0;
    }
    // set corner unexisting keys to taken
    tetris_state.field[50] = -1;
    tetris_state.field[60] = -1;
    tetris_state.field[61] = -1;
    tetris_state.field[59] = -1;
    tetris_state.field[68] = -1;
    tetris_state.field[69] = -1;
}

char is_transition_allowed(int8_t from, int8_t to) {
    if (tetris_state.field[to] != 0) {
        return 0;
    }

    // check that figure doesnt collide with right wall
    if (to < 0) {
        return 0;
    }

    // check that figure doesnt fall down
    if (from % 10 == 9 && to % 10 == 0) {
        return 0;
    }

    // check that figure doesnt collide with left wall
    if (from / 10 == 6 && to / 10 == 0) {
        return 0;
    }
    return 1;
}

char is_move_legal(figure_t* figure, int8_t p1_new, int8_t p2_new, int8_t p3_new, int8_t p4_new) {
    return is_transition_allowed(figure->p1, p1_new)
        && is_transition_allowed(figure->p2, p2_new)
        && is_transition_allowed(figure->p3, p3_new)
        && is_transition_allowed(figure->p4, p4_new);
}

void move_figure_down(void) {
    next_figure.p1++;
    next_figure.p2++;
    next_figure.p3++;
    next_figure.p4++;
}

void render_I_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0x87;
    bitmap[cell].g = 0xCE;
    bitmap[cell].b = 0xFA;
}

void render_T_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0x80;
    bitmap[cell].g = 0x00;
    bitmap[cell].b = 0x80;
}

void render_RZ_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0x00;
    bitmap[cell].g = 0x80;
    bitmap[cell].b = 0x00;
}

void render_figure(RGB bitmap[KEY_NUM], figure_t* figure) {
    switch((*figure).type) {
        case I:
            render_I_cell(bitmap, (*figure).p1);
            render_I_cell(bitmap, (*figure).p2);
            render_I_cell(bitmap, (*figure).p3);
            render_I_cell(bitmap, (*figure).p4);
            break;
        case T:
            render_T_cell(bitmap, (*figure).p1);
            render_T_cell(bitmap, (*figure).p2);
            render_T_cell(bitmap, (*figure).p3);
            render_T_cell(bitmap, (*figure).p4);
            break;
        case RZ:
            render_RZ_cell(bitmap, (*figure).p1);
            render_RZ_cell(bitmap, (*figure).p2);
            render_RZ_cell(bitmap, (*figure).p3);
            render_RZ_cell(bitmap, (*figure).p4);
            break;
    }
}

void render_field(RGB bitmap[KEY_NUM]) {
    render_figure(bitmap, &next_figure);

    for (uint8_t i = 0; i <= tetris_state.last_saved_figure_index; ++i) {
        render_figure(bitmap, &tetris_state.saved_figures[i]);
    }
}

void spawn_figure(void) {
    next_figure.type = T + rand() % (LAST_TYPE - T);
    next_figure.position_type = UP_POSITION;
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
        case RZ:
            next_figure.p1 = RZ_FIGURE_SPAWN_P1;
            next_figure.p2 = RZ_FIGURE_SPAWN_P2;
            next_figure.p3 = RZ_FIGURE_SPAWN_P3;
            next_figure.p4 = RZ_FIGURE_SPAWN_P4;
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

int8_t get_cells_col(int8_t cell) {
    return cell / 10;
}


void freeze_next_figure(void) {
    figure_t* saved_figure = &tetris_state.saved_figures[++tetris_state.last_saved_figure_index];
    (*saved_figure).p1 = next_figure.p1;
    (*saved_figure).p2 = next_figure.p2;
    (*saved_figure).p3 = next_figure.p3;
    (*saved_figure).p4 = next_figure.p4;
    (*saved_figure).type = next_figure.type;

    tetris_state.field[next_figure.p1] = 1;
    tetris_state.field[next_figure.p2] = 1;
    tetris_state.field[next_figure.p3] = 1;
    tetris_state.field[next_figure.p4] = 1;
}

char do_move_if_possible(figure_t* figure, int8_t p1_new, int8_t p2_new, int8_t p3_new, int8_t p4_new){
    if (!is_move_legal(figure, p1_new, p2_new, p3_new, p4_new)) {
        return 0;
    }
    figure->p1 = p1_new;
    figure->p2 = p2_new;
    figure->p3 = p3_new;
    figure->p4 = p4_new;
    return 1;

}

char move_down_if_possible(figure_t* figure) {
    return do_move_if_possible(figure, figure->p1+1, figure->p2+1, figure->p3+1, figure->p4+1);
}

void do_move(void) {
    if (tetris_state.is_paused) {
        return;
    }
    if (tetris_state.has_moving_figure == 0) {
        spawn_figure();
        tetris_state.has_moving_figure = 1;
        return;
    }

    switch(tetris_state.next_move) {
        case MOVE_RIGHT:
            tetris_move_right();
            break;
        case MOVE_LEFT:
            tetris_move_left();
            break;
        case MOVE_ROTATE:
            tetris_rotate();
            break;
        default:
            break;
    }

    tetris_state.next_move = MOVE_NONE;
}

void get_next_move(uint32_t delta_time, RGB bitmap[KEY_NUM]) {
    if (tetris_state.anim_counter > delta_time) {
        tetris_state.anim_counter -= delta_time;
    } else {
        uint32_t remaining = delta_time - tetris_state.anim_counter;
        tetris_state.anim_counter = tetris_state.ms_per_move + remaining;
        do_move();
    }

    if (tetris_state.move_down_counter > delta_time) {
        tetris_state.move_down_counter -= delta_time;
    } else {
        uint32_t move_down_remaining = delta_time - tetris_state.move_down_counter;
        tetris_state.move_down_counter = tetris_state.ms_per_move_down + move_down_remaining;
        tetris_move_down();
    }

    render_field(bitmap);

}

void tetris_move_left() {
    do_move_if_possible(&next_figure,
            next_figure.p1+10,
            next_figure.p2+10,
            next_figure.p3+10,
            next_figure.p4+10);
}

void tetris_move_right() {
    do_move_if_possible(&next_figure,
            next_figure.p1-10,
            next_figure.p2-10,
            next_figure.p3-10,
            next_figure.p4-10);
}

void rotate_next_figure_as_T(void) {
    next_figure.position_type = (next_figure.position_type+1) % LAST_POSITION;
    next_figure.p1 = next_figure.p2;
    next_figure.p2 = next_figure.p4;
    switch (next_figure.position_type) {
        case RIGHT_POSITION:
            next_figure.p4 = next_figure.p3 + 1;
            break;
        case DOWN_POSITION:
            next_figure.p4 = next_figure.p3 + 10;
            break;
        case LEFT_POSITION:
            next_figure.p4 = next_figure.p3 - 1;
            break;
        case UP_POSITION:
            next_figure.p4 = next_figure.p3 - 10;
            break;
    }
}

void rotate_next_figure_as_RZ(void) {
    next_figure.position_type = (next_figure.position_type+1) % LAST_SHORT_POSITION;
    switch (next_figure.position_type) {
        case RIGHT_SHORT_POSITION:
            next_figure.p4 = next_figure.p3 + 1;
            next_figure.p2 = next_figure.p3 + 10;
            next_figure.p1 = next_figure.p3 + 9;
            break;
        case UP_SHORT_POSITION:
            next_figure.p4 = next_figure.p3 - 10;
            next_figure.p2 = next_figure.p3 + 1;
            next_figure.p1 = next_figure.p3 + 11;
            break;
    }
}

void rotate_next_figure_as_I(void) {
    next_figure.position_type = (next_figure.position_type+1) % LAST_SHORT_POSITION;
    switch (next_figure.position_type) {
        case RIGHT_SHORT_POSITION:
            next_figure.p1 = next_figure.p3 + 20;
            next_figure.p2 = next_figure.p3 + 10;
            next_figure.p4 = next_figure.p3 - 10;
            break;
        case UP_SHORT_POSITION:
            next_figure.p1 = next_figure.p3 - 2;
            next_figure.p2 = next_figure.p3 - 1;
            next_figure.p4 = next_figure.p3 + 1;
            break;
    }
}

void tetris_rotate() {
    switch(next_figure.type) {
        case T:
            rotate_next_figure_as_T();
            break;
        case RZ:
            rotate_next_figure_as_RZ();
            break;
        case I:
            rotate_next_figure_as_I();
            break;
    }
}

void tetris_pause() {
    if (tetris_state.is_paused) {
        tetris_state.is_paused = 0;
    } else {
        tetris_state.is_paused = 1;
    }
}

void tetris_move_down() {
    if (tetris_state.is_paused) {
        return;
    }
    if (!move_down_if_possible(&next_figure)) {
        freeze_next_figure();
        tetris_state.has_moving_figure = 0;
    }
}

void tetris_register_move(uint8_t move) {
    tetris_state.next_move = move;
}
