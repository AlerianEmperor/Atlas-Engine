#ifndef AE_KEYCODES_H
#define AE_KEYCODES_H

#include "../System.h"

typedef int32_t Keycode;

#define SCANCODE_MASK (1 << 30)
#define SCANCODE_TO_KEYCODE(X)  (X | SCANCODE_MASK)
#define KEYCODE_TO_SCANCODE(X) (X & ~(SCANCODE_MASK))

enum {
	AE_KEY_UNKNOWN = 0,

	AE_KEY_ENTER = '\r',
	AE_KEY_ESCAPE = '\033',
	AE_KEY_BACKSPACE = '\b',
	AE_KEY_TAB = '\t',
	AE_KEY_SPACE = ' ',
	AE_KEY_EXCLAIM = '!',
	AE_KEY_QUOTEDBL = '"',
	AE_KEY_HASH = '#',
	AE_KEY_PERCENT = '%',
	AE_KEY_DOLLAR = '$',
	AE_KEY_AMPERSAND = '&',
	AE_KEY_QUOTE = '\'',
	AE_KEY_LEFTPAREN = '(',
	AE_KEY_RIGHTPAREN = ')',
	AE_KEY_ASTERISK = '*',
	AE_KEY_PLUS = '+',
	AE_KEY_COMMA = ',',
	AE_KEY_MINUS = '-',
	AE_KEY_PERIOD = '.',
	AE_KEY_SLASH = '/',
	AE_KEY_0 = '0',
	AE_KEY_1 = '1',
	AE_KEY_2 = '2',
	AE_KEY_3 = '3',
	AE_KEY_4 = '4',
	AE_KEY_5 = '5',
	AE_KEY_6 = '6',
	AE_KEY_7 = '7',
	AE_KEY_8 = '8',
	AE_KEY_9 = '9',
	AE_KEY_COLON = ':',
	AE_KEY_SEMICOLON = ';',
	AE_KEY_LESS = '<',
	AE_KEY_EQUALS = '=',
	AE_KEY_GREATER = '>',
	AE_KEY_QUESTION = '?',
	AE_KEY_AT = '@',
	/*
	Skip uppercase letters
	*/
	AE_KEY_LEFTBRACKET = '[',
	AE_KEY_BACKSLASH = '\\',
	AE_KEY_RIGHTBRACKET = ']',
	AE_KEY_CARET = '^',
	AE_KEY_UNDERSCORE = '_',
	AE_KEY_BACKQUOTE = '`',
	AE_KEY_A = 'a',
	AE_KEY_B = 'b',
	AE_KEY_C = 'c',
	AE_KEY_D = 'd',
	AE_KEY_E = 'e',
	AE_KEY_F = 'f',
	AE_KEY_G = 'g',
	AE_KEY_H = 'h',
	AE_KEY_I = 'i',
	AE_KEY_J = 'j',
	AE_KEY_K = 'k',
	AE_KEY_L = 'l',
	AE_KEY_M = 'm',
	AE_KEY_N = 'n',
	AE_KEY_O = 'o',
	AE_KEY_P = 'p',
	AE_KEY_Q = 'q',
	AE_KEY_R = 'r',
	AE_KEY_S = 's',
	AE_KEY_T = 't',
	AE_KEY_U = 'u',
	AE_KEY_V = 'v',
	AE_KEY_W = 'w',
	AE_KEY_X = 'x',
	AE_KEY_Y = 'y',
	AE_KEY_Z = 'z',

	AE_KEY_CAPSLOCK = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

	AE_KEY_F1 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
	AE_KEY_F2 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
	AE_KEY_F3 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
	AE_KEY_F4 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
	AE_KEY_F5 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
	AE_KEY_F6 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
	AE_KEY_F7 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
	AE_KEY_F8 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
	AE_KEY_F9 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
	AE_KEY_F10 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
	AE_KEY_F11 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
	AE_KEY_F12 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),

	AE_KEY_PRINTSCREEN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
	AE_KEY_SCROLLLOCK = SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
	AE_KEY_PAUSE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
	AE_KEY_INSERT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
	AE_KEY_HOME = SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
	AE_KEY_PAGEUP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
	AE_KEY_DELETE = '\177',
	AE_KEY_END = SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
	AE_KEY_PAGEDOWN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
	AE_KEY_RIGHT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
	AE_KEY_LEFT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
	AE_KEY_DOWN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
	AE_KEY_UP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),

	AE_KEY_NUMLOCKCLEAR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
	AE_KEY_KEYPAD_DIVIDE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
	AE_KEY_KEYPAD_MULTIPLY = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
	AE_KEY_KEYPAD_MINUS = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
	AE_KEY_KEYPAD_PLUS = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
	AE_KEY_KEYPAD_ENTER = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
	AE_KEY_KEYPAD_1 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
	AE_KEY_KEYPAD_2 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
	AE_KEY_KEYPAD_3 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
	AE_KEY_KEYPAD_4 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
	AE_KEY_KEYPAD_5 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
	AE_KEY_KEYPAD_6 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
	AE_KEY_KEYPAD_7 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
	AE_KEY_KEYPAD_8 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
	AE_KEY_KEYPAD_9 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
	AE_KEY_KEYPAD_0 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),
	AE_KEY_KEYPAD_PERIOD = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

	AE_KEY_APPLICATION = SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
	AE_KEY_POWER = SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
	AE_KEY_KEYPAD_EQUALS = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
	AE_KEY_F13 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
	AE_KEY_F14 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
	AE_KEY_F15 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
	AE_KEY_F16 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
	AE_KEY_F17 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
	AE_KEY_F18 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
	AE_KEY_F19 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
	AE_KEY_F20 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
	AE_KEY_F21 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
	AE_KEY_F22 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
	AE_KEY_F23 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
	AE_KEY_F24 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
	AE_KEY_EXECUTE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
	AE_KEY_HELP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
	AE_KEY_MENU = SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
	AE_KEY_SELECT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
	AE_KEY_STOP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
	AE_KEY_AGAIN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
	AE_KEY_UNDO = SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
	AE_KEY_CUT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
	AE_KEY_COPY = SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
	AE_KEY_PASTE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
	AE_KEY_FIND = SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
	AE_KEY_MUTE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
	AE_KEY_VOLUMEUP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
	AE_KEY_VOLUMEDOWN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
	AE_KEY_KEYPAD_COMMA = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
	AE_KEY_KEYPAD_EQUALSAS400 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

	AE_KEY_ALTERASE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
	AE_KEY_SYSREQ = SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
	AE_KEY_CANCEL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
	AE_KEY_CLEAR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
	AE_KEY_PRIOR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
	AE_KEY_RETURN2 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
	AE_KEY_SEPARATOR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
	AE_KEY_OUT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
	AE_KEY_OPER = SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
	AE_KEY_CLEARAGAIN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
	AE_KEY_CRSEL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
	AE_KEY_EXSEL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

	AE_KEY_KEYPAD_00 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
	AE_KEY_KEYPAD_000 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
	AE_KEY_THOUSANDSSEPARATOR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
	AE_KEY_DECIMALSEPARATOR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
	AE_KEY_CURRENCYUNIT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
	AE_KEY_CURRENCYSUBUNIT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
	AE_KEY_KEYPAD_LEFTPAREN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
	AE_KEY_KEYPAD_RIGHTPAREN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
	AE_KEY_KEYPAD_LEFTBRACE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
	AE_KEY_KEYPAD_RIGHTBRACE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
	AE_KEY_KEYPAD_TAB = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
	AE_KEY_KEYPAD_BACKSPACE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
	AE_KEY_KEYPAD_A = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
	AE_KEY_KEYPAD_B = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
	AE_KEY_KEYPAD_C = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
	AE_KEY_KEYPAD_D = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
	AE_KEY_KEYPAD_E = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
	AE_KEY_KEYPAD_F = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
	AE_KEY_KEYPAD_XOR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
	AE_KEY_KEYPAD_POWER = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
	AE_KEY_KEYPAD_PERCENT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
	AE_KEY_KEYPAD_LESS = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
	AE_KEY_KEYPAD_GREATER = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
	AE_KEY_KEYPAD_AMPERSAND = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
	AE_KEY_KEYPAD_DBLAMPERSAND = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
	AE_KEY_KEYPAD_VERTICALBAR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
	AE_KEY_KEYPAD_DBLVERTICALBAR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
	AE_KEY_KEYPAD_COLON = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
	AE_KEY_KEYPAD_HASH = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
	AE_KEY_KEYPAD_SPACE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
	AE_KEY_KEYPAD_AT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
	AE_KEY_KEYPAD_EXCLAM = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
	AE_KEY_KEYPAD_MEMSTORE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
	AE_KEY_KEYPAD_MEMRECALL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
	AE_KEY_KEYPAD_MEMCLEAR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
	AE_KEY_KEYPAD_MEMADD = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
	AE_KEY_KEYPAD_MEMSUBTRACT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
	AE_KEY_KEYPAD_MEMMULTIPLY = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
	AE_KEY_KEYPAD_MEMDIVIDE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
	AE_KEY_KEYPAD_PLUSMINUS = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
	AE_KEY_KEYPAD_CLEAR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
	AE_KEY_KEYPAD_CLEARENTRY = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
	AE_KEY_KEYPAD_BINARY = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
	AE_KEY_KEYPAD_OCTAL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
	AE_KEY_KEYPAD_DECIMAL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
	AE_KEY_KEYPAD_HEXADECIMAL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

	AE_KEY_LCTRL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
	AE_KEY_LSHIFT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
	AE_KEY_LALT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
	AE_KEY_LGUI = SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
	AE_KEY_RCTRL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
	AE_KEY_RSHIFT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
	AE_KEY_RALT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
	AE_KEY_RGUI = SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),

	AE_KEY_MODE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

	AE_KEY_AUDIONEXT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
	AE_KEY_AUDIOPREV = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
	AE_KEY_AUDIOSTOP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
	AE_KEY_AUDIOPLAY = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
	AE_KEY_AUDIOMUTE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
	AE_KEY_MEDIASELECT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
	AE_KEY_WWW = SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
	AE_KEY_MAIL = SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
	AE_KEY_CALCULATOR = SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
	AE_KEY_COMPUTER = SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
	AE_KEY_AC_SEARCH = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
	AE_KEY_AC_HOME = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
	AE_KEY_AC_BACK = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
	AE_KEY_AC_FORWARD = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
	AE_KEY_AC_STOP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
	AE_KEY_AC_REFRESH = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
	AE_KEY_AC_BOOKMARKS = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

	AE_KEY_BRIGHTNESSDOWN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
	AE_KEY_BRIGHTNESSUP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
	AE_KEY_DISPLAYSWITCH = SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
	AE_KEY_KBDILLUMTOGGLE = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
	AE_KEY_KBDILLUMDOWN = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
	AE_KEY_KBDILLUMUP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
	AE_KEY_EJECT = SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
	AE_KEY_SLEEP = SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP),
	AE_KEY_APP1 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP1),
	AE_KEY_APP2 = SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP2),

	AE_KEY_AUDIOREWIND = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOREWIND),
	AE_KEY_AUDIOFASTFORWARD = SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOFASTFORWARD)
};


#endif