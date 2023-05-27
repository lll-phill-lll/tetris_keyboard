// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(tetris)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "tetris.h"

#define FPS 12
uint32_t timer = 0;


// e.g: A simple effect, self-contained within a single method
static bool tetris(effect_params_t* params) {
  if (timer > g_rgb_timer) {
      return false;
  }
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
      if (i == red_pos) {
        rgb_matrix_set_color(i, RGB_RED);
      } else {
        rgb_matrix_set_color(i, RGB_YELLOW);
      }
  }
  if (led_max == 72) {
      timer = g_rgb_timer + 1000 / FPS;
      ++red_pos;
      if (red_pos >= 72) {
          red_pos = 0;
      }
  }
  return rgb_matrix_check_finished_leds(led_max);
}

// e.g: A more complex effect, relying on external methods and state, with
// dedicated init and run methods
// static uint8_t some_global_state;
// static void my_cool_effect2_complex_init(effect_params_t* params) {
//   some_global_state = 1;
// }
// static bool my_cool_effect2_complex_run(effect_params_t* params) {
//   RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   for (uint8_t i = led_min; i < led_max; i++) {
//     rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
//   }
//   return rgb_matrix_check_finished_leds(led_max);
// }
// static bool my_cool_effect2(effect_params_t* params) {
//   if (params->init) my_cool_effect2_complex_init(params);
//   return my_cool_effect2_complex_run(params);
// }

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
