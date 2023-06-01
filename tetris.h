#pragma once

#include "color.h"

#define KEY_NUM 72
#define PLAY_FIELD_SIZE 70

#define I_FIGURE_SPAWN_P1 30
#define I_FIGURE_SPAWN_P2 31
#define I_FIGURE_SPAWN_P3 32
#define I_FIGURE_SPAWN_P4 33

#define T_FIGURE_SPAWN_P1 41
#define T_FIGURE_SPAWN_P2 30
#define T_FIGURE_SPAWN_P3 31
#define T_FIGURE_SPAWN_P4 21

#define RZ_FIGURE_SPAWN_P1 41
#define RZ_FIGURE_SPAWN_P2 31
#define RZ_FIGURE_SPAWN_P3 30
#define RZ_FIGURE_SPAWN_P4 20

#define I_FIGURE_COLOR RGB_BLUE
#define T_FIGURE_COLOR RGB_PURPLE

#define LEFT_CELL_1_ROW 40
#define LEFT_CELL_2_ROW 51
#define LEFT_CELL_9_ROW 58
#define LEFT_CELL_10_ROW 49
#define DOWN_CELL_1_COL 67
#define DOWN_CELL_2_COL 58

enum figure_type {
    NONE_FIGURE = 0,
    T,
    I,
    RZ,
    // O,
    // L,
    // LR,
    // Z,

    LAST_TYPE
};

enum positoin_type {
    UP_POSITION = 0,
    RIGHT_POSITION,
    DOWN_POSITION,
    LEFT_POSITION,

    LAST_POSITION
};

enum next_move {
    MOVE_NONE = 0,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_ROTATE
};

enum short_positoin_type {
    UP_SHORT_POSITION = 0,
    RIGHT_SHORT_POSITION,

    LAST_SHORT_POSITION
};


typedef struct _figure_t {
    int8_t p1;
    int8_t p2;
    int8_t p3;
    int8_t p4;

    uint8_t type;
    uint8_t position_type;
} figure_t;

typedef struct _tetris_state_t {
    char has_moving_figure;
    uint32_t anim_counter;
    uint32_t ms_per_move;
    uint32_t move_down_counter;
    uint32_t ms_per_move_down;
    figure_t saved_figures[20];
    int8_t last_saved_figure_index;
    char is_paused;
    uint8_t next_move;

    int8_t field[72];


} tetris_state_t;

void init_tetris_state(void);

void get_next_move(uint32_t delta_time, RGB bitmap[KEY_NUM]);

void tetris_move_left(void);
void tetris_move_right(void);
void tetris_move_down(void);

void tetris_rotate(void);

void tetris_pause(void);

void tetris_register_move(uint8_t move);
