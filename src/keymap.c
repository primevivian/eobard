// en_US Layout
// Copied from kmk/ANSI en_US Layout/code.py

#include "keyboard.h"

/// Home Row Layer Taps

#define S_L2    LT(2, HID_KEY_S)
#define D_L1    LT(1, HID_KEY_D)
#define F_L4    LT(4, HID_KEY_F)
#define J_L3    LT(3, HID_KEY_J)
#define K_L5    LT(5, HID_KEY_K)
#define SPC_L6  LT(6, HID_KEY_SPACE)

/// Keymap

keycode_t const keymap[NUM_LAYERS][NUM_KEYS] =
{
  [0] = { // QWERTY LAYER
    HID_KEY_Q,                  HID_KEY_W,                   HID_KEY_E,               HID_KEY_R,             HID_KEY_T,         HID_KEY_Y,          HID_KEY_U,           HID_KEY_I,               HID_KEY_O,                HID_KEY_P,
    HID_KEY_A,                  S_L2,                        D_L1,                    F_L4,                  HID_KEY_G,         HID_KEY_H,          J_L3,                K_L5,                    HID_KEY_L,                HID_KEY_SEMICOLON,
    HID_KEY_Z,                  HID_KEY_X,                   HID_KEY_C,               HID_KEY_V,             HID_KEY_B,         HID_KEY_N,          HID_KEY_M,           HID_KEY_COMMA,           HID_KEY_PERIOD,           HID_KEY_SLASH,
    HID_KEY_SHIFT_LEFT,         SPC_L6,                      XXXXXXX,                 XXXXXXX,               XXXXXXX,           XXXXXXX,            XXXXXXX,             XXXXXXX,                 HID_KEY_ENTER,            HID_KEY_BACKSPACE,
  },
  [1] = { // NAVIGATION LAYER
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             HID_KEY_PAGE_UP,         _______,                  _______,
    HID_KEY_PRINT_SCREEN,       HID_KEY_GUI_LEFT,            _______,                 KEY_HYPER,             _______,           HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN,  HID_KEY_ARROW_UP,        HID_KEY_ARROW_RIGHT,      _______,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            HID_KEY_HOME,        HID_KEY_PAGE_DOWN,       HID_KEY_END,              _______,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             _______,                 _______,                  _______,
  },
  [2] = { // NUMPAD
    _______,                    _______,                     _______,                 _______,               _______,           HID_KEY_SLASH,      HID_KEY_7,           HID_KEY_8,               HID_KEY_9,                HID_KEY_MINUS,
    _______,                    _______,                     _______,                 _______,               _______,           LSFT(HID_KEY_8),    HID_KEY_4,           HID_KEY_5,               HID_KEY_6,                LSFT(HID_KEY_EQUAL),
    _______,                    _______,                     _______,                 _______,               _______,           HID_KEY_0,          HID_KEY_1,           HID_KEY_2,               HID_KEY_3,                HID_KEY_EQUAL,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             _______,                 _______,                  _______,
  },
  [3] = { // LEFT SYMBOLS
    _______,                    LSFT(HID_KEY_SEMICOLON),     LSFT(HID_KEY_COMMA),     LSFT(HID_KEY_PERIOD),  HID_KEY_SEMICOLON, _______,            _______,             _______,                 _______,                  _______,
    LSFT(HID_KEY_BRACKET_LEFT), LSFT(HID_KEY_BRACKET_RIGHT), LSFT(HID_KEY_9),         LSFT(HID_KEY_0),       LSFT(HID_KEY_2),   _______,            _______,             HID_KEY_EQUAL,           LSFT(HID_KEY_EQUAL),      LSFT(HID_KEY_5),
    _______,                    LSFT(HID_KEY_1),             HID_KEY_BRACKET_LEFT,    HID_KEY_BRACKET_RIGHT, _______,           _______,            _______,             _______,                 _______,                  _______,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             _______,                 _______,                  _______,
  },
  [4] = { // RIGHT SYMBOLS
    _______,                    _______,                     _______,                 _______,               _______,           _______,            LSFT(HID_KEY_MINUS), LSFT(HID_KEY_BACKSLASH), HID_KEY_APOSTROPHE,       _______,
    LSFT(HID_KEY_6),            LSFT(HID_KEY_8),             LSFT(HID_KEY_7),         _______,               _______,           LSFT(HID_KEY_3),    LSFT(HID_KEY_GRAVE), HID_KEY_SLASH,           LSFT(HID_KEY_APOSTROPHE), LSFT(HID_KEY_4),
    _______,                    _______,                     _______,                 _______,               _______,           _______,            HID_KEY_MINUS,       HID_KEY_BACKSLASH,       HID_KEY_GRAVE,            _______,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             _______,                 _______,                  _______,
  },
  [5] = { // FUNCTION KEYS
    HID_KEY_F9,                 HID_KEY_F10,                 HID_KEY_F11,             HID_KEY_F12,           _______,           _______,            _______,             _______,                 _______,                  _______,
    HID_KEY_F5,                 HID_KEY_F6,                  HID_KEY_F7,              HID_KEY_F8,            _______,           _______,            _______,             _______,                 _______,                  _______,
    HID_KEY_F1,                 HID_KEY_F2,                  HID_KEY_F3,              HID_KEY_F4,            _______,           _______,            _______,             _______,                 _______,                  _______,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             _______,                 _______,                  _______,
  },
  [6] = { // ALWAYS AVAILABLE [THUMB QUICK ACCESS LAYER]
    _______,                    _______,                     LSFT(HID_KEY_SEMICOLON), HID_KEY_ESCAPE,        _______,           _______,            _______,             _______,                 _______,                  HID_KEY_DELETE,
    HID_KEY_TAB,                LSFT(HID_KEY_5),             HID_KEY_SLASH,           HID_KEY_ENTER,         _______,           _______,            HID_KEY_GUI_LEFT,    _______,                 _______,                  _______,
    HID_KEY_CAPS_LOCK,          _______,                     _______,                 LSFT(HID_KEY_5),       _______,           _______,            HID_KEY_ALT_RIGHT,   HID_KEY_CONTROL_RIGHT,   _______,                  KEY_BOOTSEL,
    _______,                    _______,                     _______,                 _______,               _______,           _______,            _______,             _______,                 _______,                  _______,
  },
};
