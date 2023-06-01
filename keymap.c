/* Copyright 2020 ZSA Technology Labs, Inc <@zsa>
 * Copyright 2020 Jack Humbert <jack.humb@gmail.com>
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "version.h"

#include "tetris.h"
#include "song.h"

float tetris_song[][2] = SONG(TETRIS_SONG);

enum layers {
    BASE,  // default layer
};

enum custom_keycodes {
    LEFT = SAFE_RANGE,
    RIGHT,
    DOWN,
    ROTATE,
    RESTART,
    START_TETRIS,
    END_MUSIC,
    PAUSE

};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_moonlander(
        RESTART, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, PAUSE,   AU_ON,   START_TETRIS, QK_BOOT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,           ROTATE,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,           LEFT,    DOWN,    RIGHT
    ),
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
        case RESTART:
            rgb_matrix_mode(RGB_MATRIX_CUSTOM_tetris);
            init_tetris_state();
            tetris_start();
			return false;
        case LEFT:
            tetris_register_move(MOVE_LEFT);
            return false;
        case ROTATE:
            tetris_register_move(MOVE_ROTATE);
            return false;
        case RIGHT:
            tetris_register_move(MOVE_RIGHT);
            return false;
        case PAUSE:
            tetris_pause();
            return false;
        case START_TETRIS:
            tetris_start();
#ifdef AUDIO_ENABLE
            PLAY_LOOP(tetris_song);
            return false;
#endif
        }

    }
    return true;
}


void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_MULTISPLASH + 1);
    init_tetris_state();
}
