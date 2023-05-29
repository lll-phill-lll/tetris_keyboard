// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(tetris)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "tetris.h"
#include "song.h"

uint8_t red_pos = 0;

#define FPS 20
uint32_t timer = 0;
uint32_t last_updated = 0;
RGB bitmap[KEY_NUM];

const uint8_t field_remap[KEY_NUM] = {
    4,  3,  2,  1,  0,
    36, 37, 38, 39, 40,
    9,  8,  7,  6,  5,
    41, 42, 43, 44, 45,
    14, 13, 12, 11, 10,
    46, 47, 48, 49, 50,
    19, 18, 17, 16, 15,
    51, 52, 53, 54, 55,
    24, 23, 22, 21, 20,
    56, 57, 58, 59, 60,
    32, 28, 27, 26, 25,
    61, 62, 63, 64, 33,
    34, 35, 31, 30, 29,
    65, 66, 67, 68, 69,
    70, 71
};

uint8_t remap_index(uint8_t index) {
    return field_remap[index];
}

void clear_bitmap(void) {
    for (uint8_t i = 0; i < KEY_NUM; ++i) {
        bitmap[i].r = 0;
        bitmap[i].g = 0;
        bitmap[i].b = 0;
    }
}

void apply_bitmap(uint8_t led_min, uint8_t led_max) {
    for (uint8_t i = led_min; i < led_max; ++i) {
        uint8_t remapped = remap_index(i);
        rgb_matrix_set_color(remapped, bitmap[i].r, bitmap[i].g, bitmap[i].b);
        // if (i == red_pos) {
        //     rgb_matrix_set_color(remapped, RGB_RED);
        // }
    }
}


// e.g: A simple effect, self-contained within a single method
static bool tetris(effect_params_t* params) {
  if (timer > g_rgb_timer) {
      return false;
  }
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  apply_bitmap(led_min, led_max);

  if (led_max == KEY_NUM) {
      // ++red_pos;
      // if (red_pos > KEY_NUM) {
      //     red_pos = 0;
      // }
      clear_bitmap();
      uint32_t delta_time = timer_elapsed32(last_updated);
      get_next_move(delta_time, bitmap);
      last_updated = timer_read32();
      timer = g_rgb_timer + 1000 / FPS;
  }
  return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
