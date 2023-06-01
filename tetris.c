#include <stdlib.h>

#include "tetris.h"

tetris_state_t tetris_state;
figure_t next_figure;

void shuffle_array(int8_t arr[], int8_t size);

void init_tetris_state() {
    tetris_state.has_moving_figure = 0;
    tetris_state.anim_counter = 50;
    tetris_state.ms_per_move = 50;
    tetris_state.move_down_counter = 800;
    tetris_state.ms_per_move_down = 800;
    tetris_state.is_paused = 0;
    tetris_state.next_move = MOVE_NONE;
    tetris_state.is_game_over = 0;
    tetris_state.random_figure_index = 0;

    for (uint8_t i = 0; i != PLAY_FIELD_SIZE; ++i) {
        tetris_state.field[i] = 0;
    }
    // set corner unexisting keys to taken
    tetris_state.field[50] = -1;
    tetris_state.field[60] = -1;
    tetris_state.field[61] = -1;
    tetris_state.field[59] = -1;
    tetris_state.field[68] = -1;
    tetris_state.field[69] = -1;

    for (uint8_t i = T; i < LAST_TYPE; ++i) {
        tetris_state.random_figures_1[i - 1] = i;
        tetris_state.random_figures_2[i - 1] = i;
    }
    // shuffle only first array, second will be shuffled while generating random
    // figure
    shuffle_array(tetris_state.random_figures_1, FIGURES_NUMBER);
}

// use Fisher–Yates to shuffle
void shuffle_array(int8_t arr[], int8_t size) {
    for (int8_t i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int8_t tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
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

void render_S_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0x00;
    bitmap[cell].g = 0x80;
    bitmap[cell].b = 0x00;
}

void render_O_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0xFF;
    bitmap[cell].g = 0xFF;
    bitmap[cell].b = 0x00;
}

void render_L_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0xFF;
    bitmap[cell].g = 0xA5;
    bitmap[cell].b = 0x00;
}

void render_J_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0x00;
    bitmap[cell].g = 0x00;
    bitmap[cell].b = 0xFF;
}

void render_Z_cell(RGB bitmap[KEY_NUM], int8_t cell) {
    if (cell < 0) {
        return;
    }
    bitmap[cell].r = 0xFF;
    bitmap[cell].g = 0x00;
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
        case S:
            render_S_cell(bitmap, (*figure).p1);
            render_S_cell(bitmap, (*figure).p2);
            render_S_cell(bitmap, (*figure).p3);
            render_S_cell(bitmap, (*figure).p4);
            break;
        case O:
            render_O_cell(bitmap, (*figure).p1);
            render_O_cell(bitmap, (*figure).p2);
            render_O_cell(bitmap, (*figure).p3);
            render_O_cell(bitmap, (*figure).p4);
            break;
        case L:
            render_L_cell(bitmap, (*figure).p1);
            render_L_cell(bitmap, (*figure).p2);
            render_L_cell(bitmap, (*figure).p3);
            render_L_cell(bitmap, (*figure).p4);
            break;
        case J:
            render_J_cell(bitmap, (*figure).p1);
            render_J_cell(bitmap, (*figure).p2);
            render_J_cell(bitmap, (*figure).p3);
            render_J_cell(bitmap, (*figure).p4);
            break;
        case Z:
            render_Z_cell(bitmap, (*figure).p1);
            render_Z_cell(bitmap, (*figure).p2);
            render_Z_cell(bitmap, (*figure).p3);
            render_Z_cell(bitmap, (*figure).p4);
            break;
        default:
            break;
    }
}

void render_field(RGB bitmap[KEY_NUM]) {
    for (int8_t i = 0; i < PLAY_FIELD_SIZE; ++i) {
        switch (tetris_state.field[i]) {
            case I:
                render_I_cell(bitmap, i);
                break;
            case T:
                render_T_cell(bitmap, i);
                break;
            case S:
                render_S_cell(bitmap, i);
                break;
            case O:
                render_O_cell(bitmap, i);
                break;
            case L:
                render_L_cell(bitmap, i);
                break;
            case J:
                render_J_cell(bitmap, i);
                break;
            case Z:
                render_Z_cell(bitmap, i);
                break;
            default:
                break;
        }
    }
    render_figure(bitmap, &next_figure);
}

void process_game_over(void) {
    tetris_state.is_game_over = 1;
}


void game_over_if_cant_spawn_figure(int8_t p1, int8_t p2, int8_t p3, int8_t p4) {
    if (tetris_state.field[p1] || tetris_state.field[p2] ||tetris_state.field[p3] ||tetris_state.field[p4]) {
        process_game_over();
    }
}

int8_t get_random_figure_type(void) {
    int8_t type;
    if (tetris_state.random_figure_index < FIGURES_NUMBER) {
        type = tetris_state.random_figures_1[tetris_state.random_figure_index];
    } else {
        type = tetris_state.random_figures_2[tetris_state.random_figure_index - FIGURES_NUMBER];
    }

    // shuffle other array in case of change
    if (tetris_state.random_figure_index == 0) {
        shuffle_array(tetris_state.random_figures_2, FIGURES_NUMBER);
    } else if (tetris_state.random_figure_index == FIGURES_NUMBER) {
        shuffle_array(tetris_state.random_figures_1, FIGURES_NUMBER);
    }

    ++tetris_state.random_figure_index;

    return type;
}

void spawn_figure(void) {
    next_figure.type = get_random_figure_type();
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
        case S:
            next_figure.p1 = S_FIGURE_SPAWN_P1;
            next_figure.p2 = S_FIGURE_SPAWN_P2;
            next_figure.p3 = S_FIGURE_SPAWN_P3;
            next_figure.p4 = S_FIGURE_SPAWN_P4;
            break;
        case O:
            next_figure.p1 = O_FIGURE_SPAWN_P1;
            next_figure.p2 = O_FIGURE_SPAWN_P2;
            next_figure.p3 = O_FIGURE_SPAWN_P3;
            next_figure.p4 = O_FIGURE_SPAWN_P4;
            break;
        case L:
            next_figure.p1 = L_FIGURE_SPAWN_P1;
            next_figure.p2 = L_FIGURE_SPAWN_P2;
            next_figure.p3 = L_FIGURE_SPAWN_P3;
            next_figure.p4 = L_FIGURE_SPAWN_P4;
            break;
        case J:
            next_figure.p1 = J_FIGURE_SPAWN_P1;
            next_figure.p2 = J_FIGURE_SPAWN_P2;
            next_figure.p3 = J_FIGURE_SPAWN_P3;
            next_figure.p4 = J_FIGURE_SPAWN_P4;
            break;
        case Z:
            next_figure.p1 = Z_FIGURE_SPAWN_P1;
            next_figure.p2 = Z_FIGURE_SPAWN_P2;
            next_figure.p3 = Z_FIGURE_SPAWN_P3;
            next_figure.p4 = Z_FIGURE_SPAWN_P4;
            break;
        default:
            next_figure.p1 = 0;
            next_figure.p2 = 0;
            next_figure.p3 = 0;
            next_figure.p4 = 0;
            break;
    }
    game_over_if_cant_spawn_figure(next_figure.p1,
            next_figure.p2, next_figure.p3, next_figure.p4);
    return;
}

int8_t get_cells_col(int8_t cell) {
    return cell / 10;
}

void remove_row(int8_t remove_row) {
    for (int8_t row = remove_row; row >= 0; --row) {
        for (int8_t i = 0; i <= 6; ++i) {
            int8_t field_index = row + i * 10;
            // skip missing keys on the keyboard
            if (tetris_state.field[field_index] < 0) {
                continue;
            }

            // if it's not the first row and if cell is not located under missing key
            if (field_index % 10 && tetris_state.field[field_index - 1] >= 0) {
                tetris_state.field[field_index] = tetris_state.field[field_index - 1];
            } else {
                tetris_state.field[field_index] = 0;
            }
        }
    }
}

void remove_full_lines(void) {
    for (int8_t row = 9; row >= 0; --row) {
        char is_row_full = 1;
        for (int8_t i = 0; i <= 6; ++i) {
            // if at least one cell is empty multiplication result will be 0
            is_row_full *= tetris_state.field[row + i * 10];

        }

        if (is_row_full) {
            remove_row(row);
            // process current row once again
            ++row;
        }
    }
}

char is_next_move_ends_game(void) {
    // if figure doesn't fit into screen
    // one of it's coodinates will be on the last row
    // and another on the first
    char has_first_row_cell = 0;
    char has_last_row_cell = 0;
    if (next_figure.p1 % 10 == 0 || next_figure.p2 % 10 == 0 || next_figure.p3 % 10 == 0 || next_figure.p4 % 10 == 0) {
        has_first_row_cell = 1;
    }

    if (next_figure.p1 % 10 == 9 || next_figure.p2 % 10 == 9 || next_figure.p3 % 10 == 9 || next_figure.p4 % 10 == 9) {
        has_last_row_cell = 1;
    }

    return has_last_row_cell && has_first_row_cell;
}



void freeze_next_figure(void) {
    tetris_state.field[next_figure.p1] = next_figure.type;
    tetris_state.field[next_figure.p2] = next_figure.type;
    tetris_state.field[next_figure.p3] = next_figure.type;
    tetris_state.field[next_figure.p4] = next_figure.type;

    remove_full_lines();
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

void do_move(int8_t next_move) {
    if (tetris_state.is_game_over) {
        return;
    }
    if (tetris_state.is_paused) {
        return;
    }
    if (tetris_state.has_moving_figure == 0) {
        spawn_figure();
        tetris_state.has_moving_figure = 1;
        return;
    }

    switch(next_move) {
        case MOVE_RIGHT:
            tetris_move_right();
            break;
        case MOVE_LEFT:
            tetris_move_left();
            break;
        case MOVE_ROTATE:
            tetris_rotate();
            break;
        case MOVE_DOWN:
            tetris_move_down();
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
        do_move(tetris_state.next_move);
    }

    if (tetris_state.move_down_counter > delta_time) {
        tetris_state.move_down_counter -= delta_time;
    } else {
        uint32_t move_down_remaining = delta_time - tetris_state.move_down_counter;
        tetris_state.move_down_counter = tetris_state.ms_per_move_down + move_down_remaining;
        do_move(MOVE_DOWN);
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

void rotate_next_figure_as_S(void) {
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

void rotate_next_figure_as_O(void) {
    return;
}

void rotate_next_figure_as_L(void) {
    next_figure.position_type = (next_figure.position_type+1) % LAST_POSITION;
    switch (next_figure.position_type) {
        case RIGHT_POSITION:
            next_figure.p1 = next_figure.p3 - 9;
            next_figure.p2 = next_figure.p3 + 1;
            next_figure.p4 = next_figure.p3 - 1;
            break;
        case DOWN_POSITION:
            next_figure.p1 = next_figure.p3 + 11;
            next_figure.p2 = next_figure.p3 + 10;
            next_figure.p4 = next_figure.p3 - 10;
            break;
        case LEFT_POSITION:
            next_figure.p1 = next_figure.p3 + 9;
            next_figure.p2 = next_figure.p3 - 1;
            next_figure.p4 = next_figure.p3 + 1;
            break;
        case UP_POSITION:
            next_figure.p1 = next_figure.p3 - 11;
            next_figure.p2 = next_figure.p3 - 10;
            next_figure.p4 = next_figure.p3 + 10;
            break;
    }
}

void rotate_next_figure_as_J(void) {
    next_figure.position_type = (next_figure.position_type+1) % LAST_POSITION;
    switch (next_figure.position_type) {
        case RIGHT_POSITION:
            next_figure.p1 = next_figure.p3 - 11;
            next_figure.p2 = next_figure.p3 - 1;
            next_figure.p4 = next_figure.p3 + 1;
            break;
        case DOWN_POSITION:
            next_figure.p1 = next_figure.p3 - 9;
            next_figure.p2 = next_figure.p3 - 10;
            next_figure.p4 = next_figure.p3 + 10;
            break;
        case LEFT_POSITION:
            next_figure.p1 = next_figure.p3 + 11;
            next_figure.p2 = next_figure.p3 + 1;
            next_figure.p4 = next_figure.p3 - 1;
            break;
        case UP_POSITION:
            next_figure.p1 = next_figure.p3 + 9;
            next_figure.p2 = next_figure.p3 + 10;
            next_figure.p4 = next_figure.p3 - 10;
            break;
    }
}

void rotate_next_figure_as_Z(void) {
    next_figure.position_type = (next_figure.position_type+1) % LAST_SHORT_POSITION;
    switch (next_figure.position_type) {
        case RIGHT_SHORT_POSITION:
            next_figure.p4 = next_figure.p3 - 1;
            next_figure.p2 = next_figure.p3 + 10;
            next_figure.p1 = next_figure.p3 + 11;
            break;
        case UP_SHORT_POSITION:
            next_figure.p4 = next_figure.p3 + 10;
            next_figure.p2 = next_figure.p3 + 1;
            next_figure.p1 = next_figure.p3 - 9;
            break;
    }
}

void tetris_rotate() {
    switch(next_figure.type) {
        case T:
            rotate_next_figure_as_T();
            break;
        case S:
            rotate_next_figure_as_S();
            break;
        case I:
            rotate_next_figure_as_I();
            break;
        case O:
            rotate_next_figure_as_O();
            break;
        case L:
            rotate_next_figure_as_L();
            break;
        case J:
            rotate_next_figure_as_J();
            break;
        case Z:
            rotate_next_figure_as_Z();
            break;
        default:
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
    // if we can't move figure down freeze it, check for game over
    // and spawn new figure
    if (!move_down_if_possible(&next_figure)) {
        if (is_next_move_ends_game()) {
            process_game_over();
            return;
        }
        freeze_next_figure();
        tetris_state.has_moving_figure = 0;
        tetris_state.next_move = MOVE_NONE;
    }
}

void tetris_register_move(uint8_t move) {
    tetris_state.next_move = move;
}

char is_game_over(void) {
    return tetris_state.is_game_over;
}
