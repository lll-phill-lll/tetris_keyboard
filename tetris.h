#pragma once

#include "color.h"

#define KEY_NUM 72

#define I_FIGURE_SPAWN_P1 19
#define I_FIGURE_SPAWN_P2 18
#define I_FIGURE_SPAWN_P3 17
#define I_FIGURE_SPAWN_P4 16

#define T_FIGURE_SPAWN_P1 23
#define T_FIGURE_SPAWN_P2 19
#define T_FIGURE_SPAWN_P3 18
#define T_FIGURE_SPAWN_P4 13

#define I_FIGURE_COLOR RGB_BLUE
#define T_FIGURE_COLOR RGB_PURPLE


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
    uint32_t anim_counter;
    uint32_t ms_per_move;

} tetris_state_t;

void init_tetris_state(void);

void get_next_move(uint32_t delta_time, RGB bitmap[KEY_NUM]);
