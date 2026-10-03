# ███████  ██████  ██████   █████  ██████  ██████      ███████ ██     ██ 
# ██      ██    ██ ██   ██ ██   ██ ██   ██ ██   ██     ██      ██     ██ 
# █████   ██    ██ ██████  ███████ ██████  ██   ██     █████   ██  █  ██ 
# ██      ██    ██ ██   ██ ██   ██ ██   ██ ██   ██     ██      ██ ███ ██ 
# ███████  ██████  ██████  ██   ██ ██   ██ ██████      ██       ███ ███  
#                         
## Written by primevivian, 2024
## en_US Layout

print("Starting")
import board

from kmk.kmk_keyboard import KMKKeyboard
from kmk.keys import KC
from kmk.scanners import DiodeOrientation
from kmk.modules.split import Split, SplitType
from kmk.modules.layers import Layers

keyboard = KMKKeyboard()
keyboard.debug_enabled = True

### Keyboard Modules
split = Split(split_type=SplitType.UART, split_side=None, data_pin=board.GP9, data_pin2=board.GP8, uart_flip=True, use_pio=True)
keyboard.modules.append(split)
layers = Layers()
keyboard.modules.append(layers)

### Key Matrix and Diodes
keyboard.col_pins = (board.GP10,board.GP11,board.GP12,board.GP13,board.GP14) 
keyboard.row_pins = (board.GP2,board.GP3,board.GP4,board.GP5)
keyboard.diode_orientation = DiodeOrientation.ROW2COL

### Cleanup naming scheme
_______ = KC.TRNS
XXXXXXX = KC.NO

### Home Row Layer Taps
S_L2 = KC.LT(2, KC.S)
D_L1 = KC.LT(1, KC.D)
F_L4 = KC.LT(4, KC.F)
J_L3 = KC.LT(3, KC.J)
K_L5 = KC.LT(5, KC.K)
SPC_L6 = KC.LT(6, KC.SPC)

### Keymap
keyboard.keymap = [
    [  # 0 QWERTY LAYER
        KC.Q,    KC.W,    KC.E,    KC.R,    KC.T,    KC.Y,    KC.U,    KC.I,    KC.O,    KC.P,
        KC.A,    S_L2,    D_L1,    F_L4,    KC.G,    KC.H,    J_L3,    K_L5,    KC.L, KC.SCLN,
        KC.Z,    KC.X,    KC.C,    KC.V,    KC.B,    KC.N,    KC.M, KC.COMM,  KC.DOT, KC.SLSH,
     KC.LSFT,  SPC_L6, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC.ENT, KC.BSPC,
    ],
    [  # 1 NAVIGATION LAYER
    _______, _______, _______, _______, _______, _______, _______, KC.PGUP, _______, _______,
    KC.PSCR, KC.LGUI, _______, KC.HYPR, _______, KC.LEFT, KC.DOWN,   KC.UP, KC.RGHT, _______,
    _______, _______, _______, _______, _______, _______, KC.HOME, KC.PGDN,  KC.END, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    ],
    [  # 2 NUMPAD
    _______, _______, _______, _______, _______, KC.SLSH,   KC.N7,   KC.N8,   KC.N9, KC.MINS,
    _______, _______, _______, _______, _______, KC.ASTR,   KC.N4,   KC.N5,   KC.N6, KC.PLUS,
    _______, _______, _______, _______, _______,   KC.N0,   KC.N1,   KC.N2,   KC.N3,  KC.EQL,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    ],
    [  # 3 LEFT SYMBOLS
    _______, KC.COLN, KC.LABK, KC.RABK, KC.SCLN, _______, _______, _______, _______, _______,
    KC.LCBR, KC.RCBR, KC.LPRN, KC.RPRN,   KC.AT, _______, _______,  KC.EQL, KC.PLUS, KC.PERC,
    _______, KC.EXLM, KC.LBRC, KC.RBRC, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    ],
    [  # 4 RIGHT SYMBOLS
    _______, _______, _______, _______, _______, _______, KC.UNDS, KC.PIPE, KC.QUOT, _______,
    KC.CIRC, KC.ASTR, KC.AMPR, _______, _______, KC.HASH, KC.TILD, KC.SLSH, KC.DQUO,  KC.DLR,
    _______, _______, _______, _______, _______, _______, KC.MINS, KC.BSLS,  KC.GRV, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    ],
    [  # 5 FUNCTION KEYS
      KC.F9,  KC.F10,  KC.F11,  KC.F12, _______, _______, _______, _______, _______, _______,
      KC.F5,   KC.F6,   KC.F7,   KC.F8, _______, _______, _______, _______, _______, _______,
      KC.F1,   KC.F2,   KC.F3,   KC.F4, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    ],
    [  # 6 ALWAYS AVAILABLE [THUMB QUICK ACCESS LAYER]
    _______, _______, KC.COLN,  KC.ESC, _______, _______, _______, _______, _______,  KC.DEL,
     KC.TAB, KC.PERC, KC.SLSH,  KC.ENT, _______, _______, KC.LGUI, _______, _______, _______,
    KC.CAPS, _______, _______, KC.PERC, _______, _______, KC.RALT, KC.RCTL, _______, KC.RESET,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    ],
]

if __name__ == '__main__':
    keyboard.go()
