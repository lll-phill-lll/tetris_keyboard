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


#ifdef AUDIO_ENABLE
// Define my own sounds
#define MY_OWN_SOUND HD_NOTE(_C8), HD_NOTE(_D3), HD_NOTE(_C6)
#endif

#define RICK_ROLL2      \
    Q__NOTE(_F4),      \
    Q__NOTE(_G4),      \
    Q__NOTE(_BF4),     \
    Q__NOTE(_G4),      \
    HD_NOTE(_D5),      \
    HD_NOTE(_D5),      \
    W__NOTE(_C5),      \
    S__NOTE(_REST),    \
    Q__NOTE(_F4),      \
    Q__NOTE(_G4),      \
    Q__NOTE(_BF4),     \
    Q__NOTE(_G4),      \
    HD_NOTE(_C5),      \
    HD_NOTE(_C5),      \
    W__NOTE(_BF4),     \
    S__NOTE(_REST),    \
    Q__NOTE(_F4),      \
    Q__NOTE(_G4),      \
    Q__NOTE(_BF4),     \
    Q__NOTE(_G4),      \
    W__NOTE(_BF4),     \
    H__NOTE(_C5),      \
    H__NOTE(_A4),      \
    H__NOTE(_A4),      \
    H__NOTE(_G4),      \
    H__NOTE(_F4),      \
    H__NOTE(_F4),      \
    W__NOTE(_C5),      \
    W__NOTE(_BF4),

#define LP_NUMB2	\
	H__NOTE(_CS5), H__NOTE(_E5), H__NOTE(_CS5), WD_NOTE(_FS5), \
	WD_NOTE(_A5), WD_NOTE(_GS5),   WD_NOTE(_REST),  H__NOTE(_CS5), H__NOTE(_E5), \
	H__NOTE(_CS5), WD_NOTE(_A5), WD_NOTE(_GS5), WD_NOTE(_E5),

float layer0_song[][2] = SONG(MY_OWN_SOUND);
float ode_to_joy[][2] = SONG(ODE_TO_JOY);
float rick_roll[][2] = SONG(RICK_ROLL2);
float numb[][2] = SONG(LP_NUMB2);
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
    START_MUSIC,
    END_MUSIC,
    PAUSE

};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_moonlander(
        RESTART, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, PAUSE, AU_ON  , START_MUSIC, QK_BOOT,
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
        case START_MUSIC:
#ifdef AUDIO_ENABLE
			PLAY_LOOP(tetris_song);
#endif
            return false;
        }

    }
    return true;
}


void keyboard_post_init_user(void) {
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_MULTISPLASH + 1);
    init_tetris_state();
}
