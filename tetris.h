#pragma once

#include "color.h"

#define KEY_NUM 72

#define I_FIGURE_SPAWN_P1 30
#define I_FIGURE_SPAWN_P2 31
#define I_FIGURE_SPAWN_P3 32
#define I_FIGURE_SPAWN_P4 33

#define T_FIGURE_SPAWN_P1 41
#define T_FIGURE_SPAWN_P2 30
#define T_FIGURE_SPAWN_P3 31
#define T_FIGURE_SPAWN_P4 21

#define I_FIGURE_COLOR RGB_BLUE
#define T_FIGURE_COLOR RGB_PURPLE

#define LEFT_CELL_1_ROW 40
#define LEFT_CELL_2_ROW 51
#define LEFT_CELL_9_ROW 58
#define LEFT_CELL_10_ROW 49
#define DOWN_CELL_1_COL 67
#define DOWN_CELL_2_COL 58

static const uint8_t initial_last_row_cells[] = {9, 19, 29, 39, 49, 58, 67};



enum figure_type {
    T = 0,
    I,
    // O,
    // L,
    // LR,
    // Z,
    // RZ,

    LAST_TYPE
};


typedef struct _figure_t {
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    uint8_t p4;

    uint8_t type;
} figure_t;

typedef struct _tetris_state_t {
    char has_moving_figure;
    char can_move_left;
    char can_move_right;
    uint32_t anim_counter;
    uint32_t ms_per_move;
    figure_t saved_figures[20];
    uint8_t last_saved_figure_index;
    uint8_t last_free_cell_in_col[7];

} tetris_state_t;

void init_tetris_state(void);

void get_next_move(uint32_t delta_time, RGB bitmap[KEY_NUM]);

void tetris_move_left(void);
void tetris_move_right(void);
