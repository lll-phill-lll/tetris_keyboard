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

#define S_FIGURE_SPAWN_P1 41
#define S_FIGURE_SPAWN_P2 31
#define S_FIGURE_SPAWN_P3 30
#define S_FIGURE_SPAWN_P4 20

#define O_FIGURE_SPAWN_P1 30
#define O_FIGURE_SPAWN_P2 20
#define O_FIGURE_SPAWN_P3 21
#define O_FIGURE_SPAWN_P4 31

#define L_FIGURE_SPAWN_P1 20
#define L_FIGURE_SPAWN_P2 21
#define L_FIGURE_SPAWN_P3 31
#define L_FIGURE_SPAWN_P4 41

#define J_FIGURE_SPAWN_P1 40
#define J_FIGURE_SPAWN_P2 41
#define J_FIGURE_SPAWN_P3 31
#define J_FIGURE_SPAWN_P4 21

#define Z_FIGURE_SPAWN_P1 21
#define Z_FIGURE_SPAWN_P2 31
#define Z_FIGURE_SPAWN_P3 30
#define Z_FIGURE_SPAWN_P4 40

enum figure_type {
    NONE_FIGURE = 0,
    T,
    I,
    S,
    O,
    L,
    J,
    Z,

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
    char is_paused;
    uint8_t next_move;

    int8_t field[PLAY_FIELD_SIZE];

    char is_game_over;


} tetris_state_t;

void init_tetris_state(void);

void get_next_move(uint32_t delta_time, RGB bitmap[KEY_NUM]);

void tetris_move_left(void);
void tetris_move_right(void);
void tetris_move_down(void);

void tetris_rotate(void);

void tetris_pause(void);

void tetris_register_move(uint8_t move);

char is_game_over(void);
