/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "keymap_spanish_latin_america.h"

#define KEEP_AWAKE_INTERVAL 60000

enum {
    SWTPC_CK = SAFE_RANGE,
    LYRLCK_CK,
    LYR1_CK,
    LYR2_CK,
    KPAWK_CK
};

static uint8_t current_computer = 1;
static bool is_layer_one_pressed = false;
static bool is_layer_two_pressed = false;
static uint8_t current_locked_layer = 0;
static uint8_t last_locked_layer = 0;
static bool is_keep_awake_task_enabled = false;
static uint32_t keep_awake_timer;

void switch_computer(void);
void try_switch_layer(void);
void lock_layer(void);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         KC_TAB,  ES_DOT, ES_COMM, ES_NTIL,    KC_P,    KC_Y,                         KC_F,    KC_G,    KC_C,    KC_H,    KC_L, KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                         KC_D,    KC_R,    KC_T,    KC_N,    KC_S, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  CTL_T(KC_ESC), ES_MINS,    KC_Q,    KC_J,    KC_K,    KC_X,                         KC_B,    KC_M,    KC_W,    KC_V,    KC_Z, RCTL_T(KC_DEL),
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                    KC_LGUI,  LALT_T(KC_SPC), LYR1_CK,     LYR2_CK,  RALT_T(KC_ENT), XXXXXXX
                                        //`--------------------------'  `--------------------------'
  ),

    [1] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      LYRLCK_CK,  KC_F12,   KC_F5,   KC_F4,   KC_F2, KC_PSCR,                      XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY,                      XXXXXXX, KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,     _______, _______, _______
                                        //`--------------------------'  `--------------------------'
  ),

    [2] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      LYRLCK_CK, ES_BSLS, ES_SLSH, ES_ASTR, ES_LPRN, ES_RPRN,                      ES_IEXL, ES_EXLM, ES_DQUO, ES_QUOT, ES_GRV,  _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, ES_PERC, ES_PLUS,  ES_EQL, ES_LCBR, ES_RCBR,                      ES_IQUE, ES_QUES, ES_PIPE, ES_AMPR, ES_ACUT, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, ES_NUMB, ES_LABK, ES_RABK, ES_LBRC, ES_RBRC,                      ES_CIRC,  ES_DLR, ES_TILD, XXXXXXX, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,     _______, _______, _______
                                        //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      LYRLCK_CK, QK_BOOT, XXXXXXX, XXXXXXX,KPAWK_CK,SWTPC_CK,                      XXXXXXX,    KC_1,    KC_2,    KC_3, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, RM_PREV, RM_HUED, RM_HUEU, RM_NEXT, RM_TOGG,                      XXXXXXX,    KC_4,    KC_5,    KC_6, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    KC_7,    KC_8,    KC_9,    KC_0, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,     _______, _______, _______    
                                        //`--------------------------'  `--------------------------'
  )
};

void switch_computer(void) {
    SEND_STRING(SS_DOWN(X_LCTL) SS_DOWN(X_LSFT) SS_DOWN(X_LALT));
    SEND_STRING(SS_UP(X_LALT) SS_UP(X_LSFT) SS_UP(X_LCTL));
    SEND_STRING(SS_TAP(X_LCTL) SS_TAP(X_LCTL));
    if (current_computer == 1) {
        current_computer = 2;
        SEND_STRING(SS_TAP(X_2));
    } else {
        current_computer = 1;
        SEND_STRING(SS_TAP(X_1));
    }
}

bool is_layer_three_pressed(void) {
    return (is_layer_one_pressed && is_layer_two_pressed) || 
        (is_layer_one_pressed && current_locked_layer == 2) || 
        (is_layer_two_pressed && current_locked_layer == 1);
}

void try_switch_layer(void) {
    if (is_layer_three_pressed()) {
        layer_on(3);
    } else {
        if (current_locked_layer != 3) {
            layer_off(3);
        }
        if (is_layer_one_pressed) {
            layer_on(1);
        } else if (current_locked_layer != 1) {
            layer_off(1);
        }
        if (is_layer_two_pressed) {
            layer_on(2);
        } else if (current_locked_layer != 2) {
            layer_off(2);
        }
    }
}

void lock_layer(void) {
    uint8_t current_layer = get_highest_layer(layer_state);
    if(current_layer != current_locked_layer) {
        if(current_locked_layer != 0) {
            layer_off(current_locked_layer);
            last_locked_layer = current_locked_layer;
        }
        layer_on(current_layer);
        current_locked_layer = current_layer;
    } else {
        layer_off(current_locked_layer);
        current_locked_layer = last_locked_layer;
        layer_on(last_locked_layer);
        last_locked_layer = 0;
    }
}

const uint16_t PROGMEM caps_lock_combo[] = {KC_LSFT, KC_RSFT, COMBO_END};
const uint16_t PROGMEM quit_window_combo[] = {RCTL_T(KC_DEL), KC_Q, COMBO_END};

combo_t key_combos[] = {
    COMBO(caps_lock_combo, KC_CAPS),
    COMBO(quit_window_combo, LALT(KC_F4))
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SWTPC_CK:
            if (record->event.pressed) {
                switch_computer();
            }
            return false;
        case LYRLCK_CK:
            if (record->event.pressed) {
                lock_layer();
            }
            return false;
        case LYR1_CK:
            is_layer_one_pressed = record->event.pressed;
            try_switch_layer();
            return false;
        case LYR2_CK:
            is_layer_two_pressed = record->event.pressed;
            try_switch_layer();
            return false;
        case KPAWK_CK:
            if (record->event.pressed) {
                is_keep_awake_task_enabled = !is_keep_awake_task_enabled;
            }
            return false;
    }
    return true;
}

void matrix_scan_user(void) {
    if (is_keep_awake_task_enabled && timer_elapsed32(keep_awake_timer) > KEEP_AWAKE_INTERVAL) {
        tap_code(KC_F24);
        keep_awake_timer = timer_read32();
    }
}