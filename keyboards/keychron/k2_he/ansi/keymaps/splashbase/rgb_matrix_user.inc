// Splash on base: every key rests at the configured color (so legends stay
// readable) and each keypress sends out the stock Splash rainbow ripple,
// blended over that base instead of over black.

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

RGB_MATRIX_EFFECT(SPLASH_ON_BASE)
RGB_MATRIX_EFFECT(SPLASH_ON_WHITE)

#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// Resting brightness as a fraction of the global brightness (255 = same).
// Below 255 so the ripple stands out against the base.
#        ifndef SPLASH_ON_BASE_REST_LEVEL
#            define SPLASH_ON_BASE_REST_LEVEL 180
#        endif

// Hue distance between the base color and the ripple's wavefront. The ripple
// then walks the rest of the color wheel and fades out on the base hue, so
// the trailing edge melts back into the background.
#        ifndef SPLASH_ON_BASE_HUE_OFFSET
#            define SPLASH_ON_BASE_HUE_OFFSET 85
#        endif

// rest_level: resting brightness as a fraction of the global brightness.
// sharpness: ripple coverage multiplier; above 1 a key reaches the ripple's
// full color sooner, leaving less of the wave half-blended with the base.
// black_front: width of a dark band leading the wave (0 = none), followed by
// an equally wide ramp up to the rainbow; one key is roughly 15 units.
static bool splash_on(effect_params_t* params, uint8_t base_h, uint8_t base_s, uint8_t rest_level, uint8_t sharpness, uint8_t black_front) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    rgb_t base = rgb_matrix_hsv_to_rgb((hsv_t){base_h, base_s, scale8(rgb_matrix_config.hsv.v, rest_level)});

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // Same wave geometry as the stock Splash; when several ripples
        // overlap, the strongest one wins.
        uint8_t  amount = 0;
        uint8_t  hue    = 0;
        uint16_t front  = 255;
        for (uint8_t j = 0; j < count; j++) {
            int16_t  dx     = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy     = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist   = sqrt16(dx * dx + dy * dy);
            uint16_t tick   = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
            uint16_t effect = tick - dist;
            if (effect > 255) effect = 255;
            if (255 - effect > amount) {
                amount = 255 - effect;
                front  = effect;
                hue    = base_h + SPLASH_ON_BASE_HUE_OFFSET + scale8(effect, 255 - SPLASH_ON_BASE_HUE_OFFSET);
            }
        }
        amount = MIN(255, amount * sharpness);

        // front counts up from 0 at the wavefront toward the tail.
        uint8_t wave_v = rgb_matrix_config.hsv.v;
        if (front < black_front) {
            wave_v = 0;
        } else if (front < 2 * black_front) {
            wave_v = scale8(wave_v, (front - black_front) * 255 / black_front);
        }

        // Full saturation so the ripple shows even on a white base.
        rgb_t wave = rgb_matrix_hsv_to_rgb((hsv_t){hue, 255, wave_v});
        rgb_matrix_region_set_color(params->region, i, lerp8by8(base.r, wave.r, amount), lerp8by8(base.g, wave.g, amount), lerp8by8(base.b, wave.b, amount));
    }
    return rgb_matrix_check_finished_leds(led_max);
}

// Base color from the launcher color picker / Fn hue+saturation keys.
static bool SPLASH_ON_BASE(effect_params_t* params) {
    return splash_on(params, rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, SPLASH_ON_BASE_REST_LEVEL, 1, 0);
}

// Fixed white base, independent of the configured color. White mixed with
// the ripple turns pastel, so it rests dimmer and the ripple saturates
// faster, led by a dark band for contrast.
static bool SPLASH_ON_WHITE(effect_params_t* params) {
    return splash_on(params, 0, 0, 115, 3, 16);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // RGB_MATRIX_KEYREACTIVE_ENABLED
