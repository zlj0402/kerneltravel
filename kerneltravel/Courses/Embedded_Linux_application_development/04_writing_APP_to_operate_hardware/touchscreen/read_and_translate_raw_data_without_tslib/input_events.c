#include "input_events.h"
#include <stdio.h>

const char *ev_names[] = {
	"EV_SYN", // 0
	"EV_KEY", // 1
	"EV_REL", // 2
	"EV_ABS", // 3
	"EV_MSC", // 4
	"EV_SW", // 5
	"NULL", // 6
	"NULL", // 7
	"NULL", // 8
	"NULL", // 9
	"NULL", // 10
	"NULL", // 11
	"NULL", // 12
	"NULL", // 13
	"NULL", // 14
	"NULL", // 15 -> 0x0f
	"NULL", // 16 -> 0x10
	"EV_LED", // 17 -> 0x11
	"EV_SND", // 18 -> 0x12
	"NULL", // 19 -> 0x13
	"EV_REP", // 20 -> 0x14
	"EV_FF", // 21 -> 0x15
	"EV_PWR", // 22 -> 0x16
};

const char *ev_properties[] = {
	"INPUT_PROP_POINTER", // 1 needs a pointer
	"INPUT_PROP_DIRECT", // 2 direct input devices
	"INPUT_PROP_BUTTONPAD", // 3 has button(s) under pad
	"INPUT_PROP_SEMI_MT", // 4 touch rectangle only
	"INPUT_PROP_TOPBUTTONPAD", // 5 softbuttons at top of pad
	"INPUT_PROP_POINTING_STICK", // 6 is a pointing stick
	"INPUT_PROP_ACCELEROMETER" // 7 has accelerometer
};

const char *syn_values[] = {
	"SYN_REPORT",	// 0
	"SYN_CONFIG",	// 1
	"SYN_MT_REPORT",	// 2
	"SYN_DROPPED",	// 3
	"NULL", // 4
	"NULL", // 5
	"NULL", // 6
	"NULL", // 7
	"NULL", // 8
	"NULL", // 9
	"NULL", // 10
	"NULL", // 11
	"NULL", // 12
	"NULL", // 13
	"NULL", // 14
	"SYN_MAX",	// 0xf -> 15
};

const char *rel_values[] = {
	"REL_X",	// 0x00 -> 0
	"REL_Y",	// 0x01 -> 1
	"REL_Z",	// 0x02 -> 2
	"REL_RX",	// 0x03 -> 3
	"REL_RY",	// 0x04 -> 4
	"REL_RZ",	// 0x05 -> 5
	"REL_HWHEEL",	// 0x06 -> 6
	"REL_DIAL",	// 0x07 -> 7
	"REL_WHEEL",	// 0x08 -> 8
	"REL_MISC",	// 0x09 -> 9
	"NULL",	// 0xa -> 10
	"NULL",	// 0xb -> 11
	"NULL",	// 0xc -> 12
	"NULL",	// 0xd -> 13
	"NULL",	// 0xe -> 14
	"REL_MAX",	// 0x0f -> 15
};

const char *switch_values[] = {
	"SW_LID",	// 0x00 -> 0
	"SW_TABLET_MODE",	// 0x01 -> 1
	"SW_HEADPHONE_INSERT",	// 0x02 -> 2
	"SW_RFKILL_ALL",	// 0x03 -> 3
	"SW_MICROPHONE_INSERT",	// 0x04 -> 4
	"SW_DOCK",	// 0x05 -> 5
	"SW_LINEOUT_INSERT",	// 0x06 -> 6
	"SW_JACK_PHYSICAL_INSERT",	// 0x07 -> 7
	"SW_VIDEOOUT_INSERT",	// 0x08 -> 8
	"SW_CAMERA_LENS_COVER",	// 0x09 -> 9
	"SW_KEYPAD_SLIDE",	// 0x0a -> 10
	"SW_FRONT_PROXIMITY",	// 0x0b -> 11
	"SW_ROTATE_LOCK",	// 0x0c -> 12
	"SW_LINEIN_INSERT",	// 0x0d -> 13
	"SW_MUTE_DEVICE",	// 0x0e -> 14
	"SW_PEN_INSERTED",	// 0x0f -> 15
};

const char *misc_values[] = {
	"MSC_SERIAL",	// 0x00 -> 0
	"MSC_PULSELED", // 0x01 -> 1
	"MSC_GESTURE",	// 0x02 -> 2
	"MSC_RAW",	// 0x03 -> 3
	"MSC_SCAN", // 0x04 -> 4
	"MSC_TIMESTAMP",	// 0x05 -> 5
	"NULL", // 0x6 -> 6
	"MSC_MAX",	// 0x07 -> 7
};

const char *led_values[] = {
	"LED_NUML", // 0x00 -> 0
	"LED_CAPSL",	// 0x01 -> 1
	"LED_SCROLLL",	// 0x02 -> 2
	"LED_COMPOSE",	// 0x03 -> 3
	"LED_KANA", // 0x04 -> 4
	"LED_SLEEP",	// 0x05 -> 5
	"LED_SUSPEND",	// 0x06 -> 6
	"LED_MUTE", // 0x07 -> 7
	"LED_MISC", // 0x08 -> 8
	"LED_MAIL", // 0x09 -> 9
	"LED_CHARGING", // 0x0a -> 10
	"NULL", // 0xb -> 11
	"NULL", // 0xc -> 12
	"NULL", // 0xd -> 13
	"NULL", // 0xe -> 14
	"LED_MAX",	// 0x0f -> 15
};

const char *autorepeat_values[] = {
	"REP_DELAY",	// 0x00 -> 0
	"REP_PERIOD",	// 0x01 -> 1
};

const char *sound_values[] = {
	"SND_CLICK",	// 0x00 -> 0
	"SND_BELL", // 0x01 -> 1
	"SND_TONE", // 0x02 -> 2
	"NULL", // 0x3 -> 3
	"NULL", // 0x4 -> 4
	"NULL", // 0x5 -> 5
	"NULL", // 0x6 -> 6
	"SND_MAX",	// 0x07 -> 7
};

const char *abs_values[] = {
	"ABS_X",	// 0x00 -> 0
	"ABS_Y",	// 0x01 -> 1
	"ABS_Z",	// 0x02 -> 2
	"ABS_RX",	// 0x03 -> 3
	"ABS_RY",	// 0x04 -> 4
	"ABS_RZ",	// 0x05 -> 5
	"ABS_THROTTLE",	// 0x06 -> 6
	"ABS_RUDDER",	// 0x07 -> 7
	"ABS_WHEEL",	// 0x08 -> 8
	"ABS_GAS",	// 0x09 -> 9
	"ABS_BRAKE",	// 0x0a -> 10
	"NULL",	// 0xb -> 11
	"NULL",	// 0xc -> 12
	"NULL",	// 0xd -> 13
	"NULL",	// 0xe -> 14
	"NULL",	// 0xf -> 15
	"ABS_HAT0X",	// 0x10 -> 16
	"ABS_HAT0Y",	// 0x11 -> 17
	"ABS_HAT1X",	// 0x12 -> 18
	"ABS_HAT1Y",	// 0x13 -> 19
	"ABS_HAT2X",	// 0x14 -> 20
	"ABS_HAT2Y",	// 0x15 -> 21
	"ABS_HAT3X",	// 0x16 -> 22
	"ABS_HAT3Y",	// 0x17 -> 23
	"ABS_PRESSURE",	// 0x18 -> 24
	"ABS_DISTANCE",	// 0x19 -> 25
	"ABS_TILT_X",	// 0x1a -> 26
	"ABS_TILT_Y",	// 0x1b -> 27
	"ABS_TOOL_WIDTH",	// 0x1c -> 28
	"NULL",	// 0x1d -> 29
	"NULL",	// 0x1e -> 30
	"NULL",	// 0x1f -> 31
	"ABS_VOLUME",	// 0x20 -> 32
	"NULL",	// 0x21 -> 33
	"NULL",	// 0x22 -> 34
	"NULL",	// 0x23 -> 35
	"NULL",	// 0x24 -> 36
	"NULL",	// 0x25 -> 37
	"NULL",	// 0x26 -> 38
	"NULL",	// 0x27 -> 39
	"ABS_MISC",	// 0x28 -> 40
	"NULL",	// 0x29 -> 41
	"NULL",	// 0x2a -> 42
	"NULL",	// 0x2b -> 43
	"NULL",	// 0x2c -> 44
	"NULL",	// 0x2d -> 45
	"NULL",	// 0x2e -> 46
	"ABS_MT_SLOT",	// 0x2f -> 47
	"ABS_MT_TOUCH_MAJOR",	// 0x30 -> 48	/* Major axis of touching ellipse */
	"ABS_MT_TOUCH_MINOR",	// 0x31 -> 49
	"ABS_MT_WIDTH_MAJOR",	// 0x32 -> 50
	"ABS_MT_WIDTH_MINOR",	// 0x33 -> 51
	"ABS_MT_ORIENTATION",	// 0x34 -> 52
	"ABS_MT_POSITION_X",	// 0x35 -> 53	/* Center X touch position */
	"ABS_MT_POSITION_Y",	// 0x36 -> 54	/* Center Y touch position */
	"ABS_MT_TOOL_TYPE",	// 0x37 -> 55
	"ABS_MT_BLOB_ID",	// 0x38 -> 56
	"ABS_MT_TRACKING_ID",	// 0x39 -> 57	/* Unique ID of initiated contact */
	"ABS_MT_PRESSURE",	// 0x3a -> 58		/* Pressure on contact area */
	"ABS_MT_DISTANCE",	// 0x3b -> 59
	"ABS_MT_TOOL_X",	// 0x3c -> 60
	"ABS_MT_TOOL_Y",	// 0x3d -> 61
};

const char *key_button_values[] = {
	"KEY_RESERVED", // 0
	"KEY_ESC",	// 1
	"KEY_1",	// 2
	"KEY_2",	// 3
	"KEY_3",	// 4
	"KEY_4",	// 5
	"KEY_5",	// 6
	"KEY_6",	// 7
	"KEY_7",	// 8
	"KEY_8",	// 9
	"KEY_9",	// 10
	"KEY_0",	// 11
	"KEY_MINUS",	// 12
	"KEY_EQUAL",	// 13
	"KEY_BACKSPACE",	// 14
	"KEY_TAB",	// 15
	"KEY_Q",	// 16
	"KEY_W",	// 17
	"KEY_E",	// 18
	"KEY_R",	// 19
	"KEY_T",	// 20
	"KEY_Y",	// 21
	"KEY_U",	// 22
	"KEY_I",	// 23
	"KEY_O",	// 24
	"KEY_P",	// 25
	"KEY_LEFTBRACE",	// 26
	"KEY_RIGHTBRACE",	// 27
	"KEY_ENTER",	// 28
	"KEY_LEFTCTRL", // 29
	"KEY_A",	// 30
	"KEY_S",	// 31
	"KEY_D",	// 32
	"KEY_F",	// 33
	"KEY_G",	// 34
	"KEY_H",	// 35
	"KEY_J",	// 36
	"KEY_K",	// 37
	"KEY_L",	// 38
	"KEY_SEMICOLON",	// 39
	"KEY_APOSTROPHE",	// 40
	"KEY_GRAVE",	// 41
	"KEY_LEFTSHIFT",	// 42
	"KEY_BACKSLASH",	// 43
	"KEY_Z",	// 44
	"KEY_X",	// 45
	"KEY_C",	// 46
	"KEY_V",	// 47
	"KEY_B",	// 48
	"KEY_N",	// 49
	"KEY_M",	// 50
	"KEY_COMMA",	// 51
	"KEY_DOT",	// 52
	"KEY_SLASH",	// 53
	"KEY_RIGHTSHIFT",	// 54
	"KEY_KPASTERISK",	// 55
	"KEY_LEFTALT",	// 56
	"KEY_SPACE",	// 57
	"KEY_CAPSLOCK", // 58
	"KEY_F1",	// 59
	"KEY_F2",	// 60
#include "input_events.h"
#include <stdio.h>

const char *ev_names[] = {
	"EV_SYN", // 0
	"EV_KEY", // 1
	"EV_REL", // 2
	"EV_ABS", // 3
	"EV_MSC", // 4
	"EV_SW", // 5
	"NULL", // 6
	"NULL", // 7
	"NULL", // 8
	"NULL", // 9
	"NULL", // 10
	"NULL", // 11
	"NULL", // 12
	"NULL", // 13
	"NULL", // 14
	"NULL", // 15 -> 0x0f
	"NULL", // 16 -> 0x10
	"EV_LED", // 17 -> 0x11
	"EV_SND", // 18 -> 0x12
	"NULL", // 19 -> 0x13
	"EV_REP", // 20 -> 0x14
	"EV_FF", // 21 -> 0x15
	"EV_PWR", // 22 -> 0x16
};

const char *ev_properties[] = {
	"INPUT_PROP_POINTER", // 1 needs a pointer
	"INPUT_PROP_DIRECT", // 2 direct input devices
	"INPUT_PROP_BUTTONPAD", // 3 has button(s) under pad
	"INPUT_PROP_SEMI_MT", // 4 touch rectangle only
	"INPUT_PROP_TOPBUTTONPAD", // 5 softbuttons at top of pad
	"INPUT_PROP_POINTING_STICK", // 6 is a pointing stick
	"INPUT_PROP_ACCELEROMETER" // 7 has accelerometer
};

const char *syn_values[] = {
	"SYN_REPORT",	// 0
	"SYN_CONFIG",	// 1
	"SYN_MT_REPORT",	// 2
	"SYN_DROPPED",	// 3
	"NULL", // 4
	"NULL", // 5
	"NULL", // 6
	"NULL", // 7
	"NULL", // 8
	"NULL", // 9
	"NULL", // 10
	"NULL", // 11
	"NULL", // 12
	"NULL", // 13
	"NULL", // 14
	"SYN_MAX",	// 0xf -> 15
};

const char *rel_values[] = {
	"REL_X",	// 0x00 -> 0
	"REL_Y",	// 0x01 -> 1
	"REL_Z",	// 0x02 -> 2
	"REL_RX",	// 0x03 -> 3
	"REL_RY",	// 0x04 -> 4
	"REL_RZ",	// 0x05 -> 5
	"REL_HWHEEL",	// 0x06 -> 6
	"REL_DIAL",	// 0x07 -> 7
	"REL_WHEEL",	// 0x08 -> 8
	"REL_MISC",	// 0x09 -> 9
	"NULL",	// 0xa -> 10
	"NULL",	// 0xb -> 11
	"NULL",	// 0xc -> 12
	"NULL",	// 0xd -> 13
	"NULL",	// 0xe -> 14
	"REL_MAX",	// 0x0f -> 15
};

const char *switch_values[] = {
	"SW_LID",	// 0x00 -> 0
	"SW_TABLET_MODE",	// 0x01 -> 1
	"SW_HEADPHONE_INSERT",	// 0x02 -> 2
	"SW_RFKILL_ALL",	// 0x03 -> 3
	"SW_MICROPHONE_INSERT",	// 0x04 -> 4
	"SW_DOCK",	// 0x05 -> 5
	"SW_LINEOUT_INSERT",	// 0x06 -> 6
	"SW_JACK_PHYSICAL_INSERT",	// 0x07 -> 7
	"SW_VIDEOOUT_INSERT",	// 0x08 -> 8
	"SW_CAMERA_LENS_COVER",	// 0x09 -> 9
	"SW_KEYPAD_SLIDE",	// 0x0a -> 10
	"SW_FRONT_PROXIMITY",	// 0x0b -> 11
	"SW_ROTATE_LOCK",	// 0x0c -> 12
	"SW_LINEIN_INSERT",	// 0x0d -> 13
	"SW_MUTE_DEVICE",	// 0x0e -> 14
	"SW_PEN_INSERTED",	// 0x0f -> 15
};

const char *misc_values[] = {
	"MSC_SERIAL",	// 0x00 -> 0
	"MSC_PULSELED", // 0x01 -> 1
	"MSC_GESTURE",	// 0x02 -> 2
	"MSC_RAW",	// 0x03 -> 3
	"MSC_SCAN", // 0x04 -> 4
	"MSC_TIMESTAMP",	// 0x05 -> 5
	"NULL", // 0x6 -> 6
	"MSC_MAX",	// 0x07 -> 7
};

const char *led_values[] = {
	"LED_NUML", // 0x00 -> 0
	"LED_CAPSL",	// 0x01 -> 1
	"LED_SCROLLL",	// 0x02 -> 2
	"LED_COMPOSE",	// 0x03 -> 3
	"LED_KANA", // 0x04 -> 4
	"LED_SLEEP",	// 0x05 -> 5
	"LED_SUSPEND",	// 0x06 -> 6
	"LED_MUTE", // 0x07 -> 7
	"LED_MISC", // 0x08 -> 8
	"LED_MAIL", // 0x09 -> 9
	"LED_CHARGING", // 0x0a -> 10
	"NULL", // 0xb -> 11
	"NULL", // 0xc -> 12
	"NULL", // 0xd -> 13
	"NULL", // 0xe -> 14
	"LED_MAX",	// 0x0f -> 15
};

const char *autorepeat_values[] = {
	"REP_DELAY",	// 0x00 -> 0
	"REP_PERIOD",	// 0x01 -> 1
};

const char *sound_values[] = {
	"SND_CLICK",	// 0x00 -> 0
	"SND_BELL", // 0x01 -> 1
	"SND_TONE", // 0x02 -> 2
	"NULL", // 0x3 -> 3
	"NULL", // 0x4 -> 4
	"NULL", // 0x5 -> 5
	"NULL", // 0x6 -> 6
	"SND_MAX",	// 0x07 -> 7
};

const char *abs_values[] = {
	"ABS_X",	// 0x00 -> 0
	"ABS_Y",	// 0x01 -> 1
	"ABS_Z",	// 0x02 -> 2
	"ABS_RX",	// 0x03 -> 3
	"ABS_RY",	// 0x04 -> 4
	"ABS_RZ",	// 0x05 -> 5
	"ABS_THROTTLE",	// 0x06 -> 6
	"ABS_RUDDER",	// 0x07 -> 7
	"ABS_WHEEL",	// 0x08 -> 8
	"ABS_GAS",	// 0x09 -> 9
	"ABS_BRAKE",	// 0x0a -> 10
	"NULL",	// 0xb -> 11
	"NULL",	// 0xc -> 12
	"NULL",	// 0xd -> 13
	"NULL",	// 0xe -> 14
	"NULL",	// 0xf -> 15
	"ABS_HAT0X",	// 0x10 -> 16
	"ABS_HAT0Y",	// 0x11 -> 17
	"ABS_HAT1X",	// 0x12 -> 18
	"ABS_HAT1Y",	// 0x13 -> 19
	"ABS_HAT2X",	// 0x14 -> 20
	"ABS_HAT2Y",	// 0x15 -> 21
	"ABS_HAT3X",	// 0x16 -> 22
	"ABS_HAT3Y",	// 0x17 -> 23
	"ABS_PRESSURE",	// 0x18 -> 24
	"ABS_DISTANCE",	// 0x19 -> 25
	"ABS_TILT_X",	// 0x1a -> 26
	"ABS_TILT_Y",	// 0x1b -> 27
	"ABS_TOOL_WIDTH",	// 0x1c -> 28
	"NULL",	// 0x1d -> 29
	"NULL",	// 0x1e -> 30
	"NULL",	// 0x1f -> 31
	"ABS_VOLUME",	// 0x20 -> 32
	"NULL",	// 0x21 -> 33
	"NULL",	// 0x22 -> 34
	"NULL",	// 0x23 -> 35
	"NULL",	// 0x24 -> 36
	"NULL",	// 0x25 -> 37
	"NULL",	// 0x26 -> 38
	"NULL",	// 0x27 -> 39
	"ABS_MISC",	// 0x28 -> 40
	"NULL",	// 0x29 -> 41
	"NULL",	// 0x2a -> 42
	"NULL",	// 0x2b -> 43
	"NULL",	// 0x2c -> 44
	"NULL",	// 0x2d -> 45
	"NULL",	// 0x2e -> 46
	"ABS_MT_SLOT",	// 0x2f -> 47
	"ABS_MT_TOUCH_MAJOR",	// 0x30 -> 48	/* Major axis of touching ellipse */
	"ABS_MT_TOUCH_MINOR",	// 0x31 -> 49
	"ABS_MT_WIDTH_MAJOR",	// 0x32 -> 50
	"ABS_MT_WIDTH_MINOR",	// 0x33 -> 51
	"ABS_MT_ORIENTATION",	// 0x34 -> 52
	"ABS_MT_POSITION_X",	// 0x35 -> 53	/* Center X touch position */
	"ABS_MT_POSITION_Y",	// 0x36 -> 54	/* Center Y touch position */
	"ABS_MT_TOOL_TYPE",	// 0x37 -> 55
	"ABS_MT_BLOB_ID",	// 0x38 -> 56
	"ABS_MT_TRACKING_ID",	// 0x39 -> 57	/* Unique ID of initiated contact */
	"ABS_MT_PRESSURE",	// 0x3a -> 58		/* Pressure on contact area */
	"ABS_MT_DISTANCE",	// 0x3b -> 59
	"ABS_MT_TOOL_X",	// 0x3c -> 60
	"ABS_MT_TOOL_Y",	// 0x3d -> 61
};

const char *key_button_values[] = {
	"KEY_RESERVED", // 0
	"KEY_ESC",	// 1
	"KEY_1",	// 2
	"KEY_2",	// 3
	"KEY_3",	// 4
	"KEY_4",	// 5
	"KEY_5",	// 6
	"KEY_6",	// 7
	"KEY_7",	// 8
	"KEY_8",	// 9
	"KEY_9",	// 10
	"KEY_0",	// 11
	"KEY_MINUS",	// 12
	"KEY_EQUAL",	// 13
	"KEY_BACKSPACE",	// 14
	"KEY_TAB",	// 15
	"KEY_Q",	// 16
	"KEY_W",	// 17
	"KEY_E",	// 18
	"KEY_R",	// 19
	"KEY_T",	// 20
	"KEY_Y",	// 21
	"KEY_U",	// 22
	"KEY_I",	// 23
	"KEY_O",	// 24
	"KEY_P",	// 25
	"KEY_LEFTBRACE",	// 26
	"KEY_RIGHTBRACE",	// 27
	"KEY_ENTER",	// 28
	"KEY_LEFTCTRL", // 29
	"KEY_A",	// 30
	"KEY_S",	// 31
	"KEY_D",	// 32
	"KEY_F",	// 33
	"KEY_G",	// 34
	"KEY_H",	// 35
	"KEY_J",	// 36
	"KEY_K",	// 37
	"KEY_L",	// 38
	"KEY_SEMICOLON",	// 39
	"KEY_APOSTROPHE",	// 40
	"KEY_GRAVE",	// 41
	"KEY_LEFTSHIFT",	// 42
	"KEY_BACKSLASH",	// 43
	"KEY_Z",	// 44
	"KEY_X",	// 45
	"KEY_C",	// 46
	"KEY_V",	// 47
	"KEY_B",	// 48
	"KEY_N",	// 49
	"KEY_M",	// 50
	"KEY_COMMA",	// 51
	"KEY_DOT",	// 52
	"KEY_SLASH",	// 53
	"KEY_RIGHTSHIFT",	// 54
	"KEY_KPASTERISK",	// 55
	"KEY_LEFTALT",	// 56
	"KEY_SPACE",	// 57
	"KEY_CAPSLOCK", // 58
	"KEY_F1",	// 59
	"KEY_F2",	// 60
	"KEY_F3",	// 61
	"KEY_F4",	// 62
	"KEY_F5",	// 63
	"KEY_F6",	// 64
	"KEY_F7",	// 65
	"KEY_F8",	// 66
	"KEY_F9",	// 67
	"KEY_F10",	// 68
	"KEY_NUMLOCK",	// 69
	"KEY_SCROLLLOCK",	// 70
	"KEY_KP7",	// 71
	"KEY_KP8",	// 72
	"KEY_KP9",	// 73
	"KEY_KPMINUS",	// 74
	"KEY_KP4",	// 75
	"KEY_KP5",	// 76
	"KEY_KP6",	// 77
	"KEY_KPPLUS",	// 78
	"KEY_KP1",	// 79
	"KEY_KP2",	// 80
	"KEY_KP3",	// 81
	"KEY_KP0",	// 82
	"KEY_KPDOT",	// 83
	"NULL", // 84
	"KEY_ZENKAKUHANKAKU",	// 85
	"KEY_102ND",	// 86
	"KEY_F11",	// 87
	"KEY_F12",	// 88
	"KEY_RO",	// 89
	"KEY_KATAKANA", // 90
	"KEY_HIRAGANA", // 91
	"KEY_HENKAN",	// 92
	"KEY_KATAKANAHIRAGANA", // 93
	"KEY_MUHENKAN", // 94
	"KEY_KPJPCOMMA",	// 95
	"KEY_KPENTER",	// 96
	"KEY_RIGHTCTRL",	// 97
	"KEY_KPSLASH",	// 98
	"KEY_SYSRQ",	// 99
	"KEY_RIGHTALT", // 100
	"KEY_LINEFEED", // 101
	"KEY_HOME", // 102
	"KEY_UP",	// 103
	"KEY_PAGEUP",	// 104
	"KEY_LEFT", // 105
	"KEY_RIGHT",	// 106
	"KEY_END",	// 107
	"KEY_DOWN", // 108
	"KEY_PAGEDOWN", // 109
	"KEY_INSERT",	// 110
	"KEY_DELETE",	// 111
	"KEY_MACRO",	// 112
	"KEY_MUTE", // 113
	"KEY_VOLUMEDOWN",	// 114
	"KEY_VOLUMEUP", // 115
	"KEY_POWER",	// 116
	"KEY_KPEQUAL",	// 117
	"KEY_KPPLUSMINUS",	// 118
	"KEY_PAUSE",	// 119
	"KEY_SCALE",	// 120
	"KEY_KPCOMMA",	// 121
	"KEY_HANGEUL",	// 122
	"KEY_HANJA",	// 123
	"KEY_YEN",	// 124
	"KEY_LEFTMETA", // 125
	"KEY_RIGHTMETA",	// 126
	"KEY_COMPOSE",	// 127
	"KEY_STOP", // 128
	"KEY_AGAIN",	// 129
	"KEY_PROPS",	// 130
	"KEY_UNDO", // 131
	"KEY_FRONT",	// 132
	"KEY_COPY", // 133
	"KEY_OPEN", // 134
	"KEY_PASTE",	// 135
	"KEY_FIND", // 136
	"KEY_CUT",	// 137
	"KEY_HELP", // 138
	"KEY_MENU", // 139
	"KEY_CALC", // 140
	"KEY_SETUP",	// 141
	"KEY_SLEEP",	// 142
	"KEY_WAKEUP",	// 143
	"KEY_FILE", // 144
	"KEY_SENDFILE", // 145
	"KEY_DELETEFILE",	// 146
	"KEY_XFER", // 147
	"KEY_PROG1",	// 148
	"KEY_PROG2",	// 149
	"KEY_WWW",	// 150
	"KEY_MSDOS",	// 151
	"KEY_COFFEE",	// 152
	"KEY_ROTATE_DISPLAY",	// 153
	"KEY_CYCLEWINDOWS", // 154
	"KEY_MAIL", // 155
	"KEY_BOOKMARKS",	// 156
	"KEY_COMPUTER", // 157
	"KEY_BACK", // 158
	"KEY_FORWARD",	// 159
	"KEY_CLOSECD",	// 160
	"KEY_EJECTCD",	// 161
	"KEY_EJECTCLOSECD", // 162
	"KEY_NEXTSONG", // 163
	"KEY_PLAYPAUSE",	// 164
	"KEY_PREVIOUSSONG", // 165
	"KEY_STOPCD",	// 166
	"KEY_RECORD",	// 167
	"KEY_REWIND",	// 168
	"KEY_PHONE",	// 169
	"KEY_ISO",	// 170
	"KEY_CONFIG",	// 171
	"KEY_HOMEPAGE", // 172
	"KEY_REFRESH",	// 173
	"KEY_EXIT", // 174
	"KEY_MOVE", // 175
	"KEY_EDIT", // 176
	"KEY_SCROLLUP", // 177
	"KEY_SCROLLDOWN",	// 178
	"KEY_KPLEFTPAREN",	// 179
	"KEY_KPRIGHTPAREN", // 180
	"KEY_NEW",	// 181
	"KEY_REDO", // 182
	"KEY_F13",	// 183
	"KEY_F14",	// 184
	"KEY_F15",	// 185
	"KEY_F16",	// 186
	"KEY_F17",	// 187
	"KEY_F18",	// 188
	"KEY_F19",	// 189
	"KEY_F20",	// 190
	"KEY_F21",	// 191
	"KEY_F22",	// 192
	"KEY_F23",	// 193
	"KEY_F24",	// 194
	"NULL", // 195
	"NULL", // 196
	"NULL", // 197
	"NULL", // 198
	"NULL", // 199
	"KEY_PLAYCD",	// 200
	"KEY_PAUSECD",	// 201
	"KEY_PROG3",	// 202
	"KEY_PROG4",	// 203
	"KEY_DASHBOARD",	// 204
	"KEY_SUSPEND",	// 205
	"KEY_CLOSE",	// 206
	"KEY_PLAY", // 207
	"KEY_FASTFORWARD",	// 208
	"KEY_BASSBOOST",	// 209
	"KEY_PRINT",	// 210
	"KEY_HP",	// 211
	"KEY_CAMERA",	// 212
	"KEY_SOUND",	// 213
	"KEY_QUESTION", // 214
	"KEY_EMAIL",	// 215
	"KEY_CHAT", // 216
	"KEY_SEARCH",	// 217
	"KEY_CONNECT",	// 218
	"KEY_FINANCE",	// 219
	"KEY_SPORT",	// 220
	"KEY_SHOP", // 221
	"KEY_ALTERASE", // 222
	"KEY_CANCEL",	// 223
	"KEY_BRIGHTNESSDOWN",	// 224
	"KEY_BRIGHTNESSUP", // 225
	"KEY_MEDIA",	// 226
	"KEY_SWITCHVIDEOMODE",	// 227
	"KEY_KBDILLUMTOGGLE",	// 228
	"KEY_KBDILLUMDOWN", // 229
	"KEY_KBDILLUMUP",	// 230
	"KEY_SEND", // 231
	"KEY_REPLY",	// 232
	"KEY_FORWARDMAIL",	// 233
	"KEY_SAVE", // 234
	"KEY_DOCUMENTS",	// 235
	"KEY_BATTERY",	// 236
	"KEY_BLUETOOTH",	// 237
	"KEY_WLAN", // 238
	"KEY_UWB",	// 239
	"KEY_UNKNOWN",	// 240
	"KEY_VIDEO_NEXT",	// 241
	"KEY_VIDEO_PREV",	// 242
	"KEY_BRIGHTNESS_CYCLE", // 243
	"KEY_BRIGHTNESS_AUTO",	// 244
	"KEY_DISPLAY_OFF",	// 245
	"KEY_WWAN", // 246
	"KEY_RFKILL",	// 247
	"KEY_MICMUTE",	// 248
	"NULL", // 249
	"NULL", // 250
	"NULL", // 251
	"NULL", // 252
	"NULL", // 253
	"NULL", // 254
	"NULL", // 255
	"BTN_MISC|BTN_0", // 0x100 -> 256
	"BTN_1",	// 0x101 -> 257
	"BTN_2",	// 0x102 -> 258
	"BTN_3",	// 0x103 -> 259
	"BTN_4",	// 0x104 -> 260
	"BTN_5",	// 0x105 -> 261
	"BTN_6",	// 0x106 -> 262
	"BTN_7",	// 0x107 -> 263
	"BTN_8",	// 0x108 -> 264
	"BTN_9",	// 0x109 -> 265
	"NULL", // 0x10a -> 266
	"NULL", // 0x10b -> 267
	"NULL", // 0x10c -> 268
	"NULL", // 0x10d -> 269
	"NULL", // 0x10e -> 270
	"NULL", // 0x10f -> 271
	"BTN_MOUSE|BTN_LEFT",	// 0x110 -> 272
	"BTN_RIGHT",	// 0x111 -> 273
	"BTN_MIDDLE",	// 0x112 -> 274
	"BTN_SIDE", // 0x113 -> 275
	"BTN_EXTRA",	// 0x114 -> 276
	"BTN_FORWARD",	// 0x115 -> 277
	"BTN_BACK", // 0x116 -> 278
	"BTN_TASK", // 0x117 -> 279
	"NULL", // 0x118 -> 280
	"NULL", // 0x119 -> 281
	"NULL", // 0x11a -> 282
	"NULL", // 0x11b -> 283
	"NULL", // 0x11c -> 284
	"NULL", // 0x11d -> 285
	"NULL", // 0x11e -> 286
	"NULL", // 0x11f -> 287
	"BTN_JOYSTICK|BTN_TRIGGER", // 0x120 -> 288
	"BTN_THUMB",	// 0x121 -> 289
	"BTN_THUMB2",	// 0x122 -> 290
	"BTN_TOP",	// 0x123 -> 291
	"BTN_TOP2", // 0x124 -> 292
	"BTN_PINKIE",	// 0x125 -> 293
	"BTN_BASE", // 0x126 -> 294
	"BTN_BASE2",	// 0x127 -> 295
	"BTN_BASE3",	// 0x128 -> 296
	"BTN_BASE4",	// 0x129 -> 297
	"BTN_BASE5",	// 0x12a -> 298
	"BTN_BASE6",	// 0x12b -> 299
	"NULL", // 0x12c -> 300
	"NULL", // 0x12d -> 301
	"NULL", // 0x12e -> 302
	"BTN_DEAD", // 0x12f -> 303
	"BTN_GAMEPAD|BTN_SOUTH",	// 0x130 -> 304
	"BTN_EAST", // 0x131 -> 305
	"BTN_C",	// 0x132 -> 306
	"BTN_NORTH",	// 0x133 -> 307
	"BTN_WEST", // 0x134 -> 308
	"BTN_Z",	// 0x135 -> 309
	"BTN_TL",	// 0x136 -> 310
	"BTN_TR",	// 0x137 -> 311
	"BTN_TL2",	// 0x138 -> 312
	"BTN_TR2",	// 0x139 -> 313
	"BTN_SELECT",	// 0x13a -> 314
	"BTN_START",	// 0x13b -> 315
	"BTN_MODE", // 0x13c -> 316
	"BTN_THUMBL",	// 0x13d -> 317
	"BTN_THUMBR",	// 0x13e -> 318
	"NULL", // 0x13f -> 319
	"BTN_DIGI|BTN_TOOL_PEN", // 0x140 -> 320
	"BTN_TOOL_RUBBER",	// 0x141 -> 321
	"BTN_TOOL_BRUSH",	// 0x142 -> 322
	"BTN_TOOL_PENCIL",	// 0x143 -> 323
	"BTN_TOOL_AIRBRUSH",	// 0x144 -> 324
	"BTN_TOOL_FINGER",	// 0x145 -> 325
	"BTN_TOOL_MOUSE",	// 0x146 -> 326
	"BTN_TOOL_LENS",	// 0x147 -> 327
	"BTN_TOOL_QUINTTAP",	// 0x148 -> 328
	"NULL", // 0x149 -> 329
	"BTN_TOUCH",	// 0x14a -> 330
	"BTN_STYLUS",	// 0x14b -> 331
	"BTN_STYLUS2",	// 0x14c -> 332
	"BTN_TOOL_DOUBLETAP",	// 0x14d -> 333
	"BTN_TOOL_TRIPLETAP",	// 0x14e -> 334
	"BTN_TOOL_QUADTAP", // 0x14f -> 335
	"BTN_WHEEL|BTN_GEAR_DOWN",	// 0x150 -> 336
	"BTN_GEAR_UP",	// 0x151 -> 337
	"NULL", // 0x152 -> 338
	"NULL", // 0x153 -> 339
	"NULL", // 0x154 -> 340
	"NULL", // 0x155 -> 341
	"NULL", // 0x156 -> 342
	"NULL", // 0x157 -> 343
	"NULL", // 0x158 -> 344
	"NULL", // 0x159 -> 345
	"NULL", // 0x15a -> 346
	"NULL", // 0x15b -> 347
	"NULL", // 0x15c -> 348
	"NULL", // 0x15d -> 349
	"NULL", // 0x15e -> 350
	"NULL", // 0x15f -> 351
	"KEY_OK",	// 0x160 -> 352
	"KEY_SELECT",	// 0x161 -> 353
	"KEY_GOTO", // 0x162 -> 354
	"KEY_CLEAR",	// 0x163 -> 355
	"KEY_POWER2",	// 0x164 -> 356
	"KEY_OPTION",	// 0x165 -> 357
	"KEY_INFO", // 0x166 -> 358
	"KEY_TIME", // 0x167 -> 359
	"KEY_VENDOR",	// 0x168 -> 360
	"KEY_ARCHIVE",	// 0x169 -> 361
	"KEY_PROGRAM",	// 0x16a -> 362
	"KEY_CHANNEL",	// 0x16b -> 363
	"KEY_FAVORITES",	// 0x16c -> 364
	"KEY_EPG",	// 0x16d -> 365
	"KEY_PVR",	// 0x16e -> 366
	"KEY_MHP",	// 0x16f -> 367
	"KEY_LANGUAGE", // 0x170 -> 368
	"KEY_TITLE",	// 0x171 -> 369
	"KEY_SUBTITLE", // 0x172 -> 370
	"KEY_ANGLE",	// 0x173 -> 371
	"KEY_ZOOM", // 0x174 -> 372
	"KEY_MODE", // 0x175 -> 373
	"KEY_KEYBOARD", // 0x176 -> 374
	"KEY_SCREEN",	// 0x177 -> 375
	"KEY_PC",	// 0x178 -> 376
	"KEY_TV",	// 0x179 -> 377
	"KEY_TV2",	// 0x17a -> 378
	"KEY_VCR",	// 0x17b -> 379
	"KEY_VCR2", // 0x17c -> 380
	"KEY_SAT",	// 0x17d -> 381
	"KEY_SAT2", // 0x17e -> 382
	"KEY_CD",	// 0x17f -> 383
	"KEY_TAPE", // 0x180 -> 384
	"KEY_RADIO",	// 0x181 -> 385
	"KEY_TUNER",	// 0x182 -> 386
	"KEY_PLAYER",	// 0x183 -> 387
	"KEY_TEXT", // 0x184 -> 388
	"KEY_DVD",	// 0x185 -> 389
	"KEY_AUX",	// 0x186 -> 390
	"KEY_MP3",	// 0x187 -> 391
	"KEY_AUDIO",	// 0x188 -> 392
	"KEY_VIDEO",	// 0x189 -> 393
	"KEY_DIRECTORY",	// 0x18a -> 394
	"KEY_LIST", // 0x18b -> 395
	"KEY_MEMO", // 0x18c -> 396
	"KEY_CALENDAR", // 0x18d -> 397
	"KEY_RED",	// 0x18e -> 398
	"KEY_GREEN",	// 0x18f -> 399
	"KEY_YELLOW",	// 0x190 -> 400
	"KEY_BLUE", // 0x191 -> 401
	"KEY_CHANNELUP",	// 0x192 -> 402
	"KEY_CHANNELDOWN",	// 0x193 -> 403
	"KEY_FIRST",	// 0x194 -> 404
	"KEY_LAST", // 0x195 -> 405
	"KEY_AB",	// 0x196 -> 406
	"KEY_NEXT", // 0x197 -> 407
	"KEY_RESTART",	// 0x198 -> 408
	"KEY_SLOW", // 0x199 -> 409
	"KEY_SHUFFLE",	// 0x19a -> 410
	"KEY_BREAK",	// 0x19b -> 411
	"KEY_PREVIOUS", // 0x19c -> 412
	"KEY_DIGITS",	// 0x19d -> 413
	"KEY_TEEN", // 0x19e -> 414
	"KEY_TWEN", // 0x19f -> 415
	"KEY_VIDEOPHONE",	// 0x1a0 -> 416
	"KEY_GAMES",	// 0x1a1 -> 417
	"KEY_ZOOMIN",	// 0x1a2 -> 418
	"KEY_ZOOMOUT",	// 0x1a3 -> 419
	"KEY_ZOOMRESET",	// 0x1a4 -> 420
	"KEY_WORDPROCESSOR",	// 0x1a5 -> 421
	"KEY_EDITOR",	// 0x1a6 -> 422
	"KEY_SPREADSHEET",	// 0x1a7 -> 423
	"KEY_GRAPHICSEDITOR",	// 0x1a8 -> 424
	"KEY_PRESENTATION", // 0x1a9 -> 425
	"KEY_DATABASE", // 0x1aa -> 426
	"KEY_NEWS", // 0x1ab -> 427
	"KEY_VOICEMAIL",	// 0x1ac -> 428
	"KEY_ADDRESSBOOK",	// 0x1ad -> 429
	"KEY_MESSENGER",	// 0x1ae -> 430
	"KEY_DISPLAYTOGGLE",	// 0x1af -> 431
	"KEY_SPELLCHECK",	// 0x1b0 -> 432
	"KEY_LOGOFF",	// 0x1b1 -> 433
	"KEY_DOLLAR",	// 0x1b2 -> 434
	"KEY_EURO", // 0x1b3 -> 435
	"KEY_FRAMEBACK",	// 0x1b4 -> 436
	"KEY_FRAMEFORWARD", // 0x1b5 -> 437
	"KEY_CONTEXT_MENU", // 0x1b6 -> 438
	"KEY_MEDIA_REPEAT", // 0x1b7 -> 439
	"KEY_10CHANNELSUP", // 0x1b8 -> 440
	"KEY_10CHANNELSDOWN",	// 0x1b9 -> 441
	"KEY_IMAGES",	// 0x1ba -> 442
	"NULL", // 0x1bb -> 443
	"NULL", // 0x1bc -> 444
	"NULL", // 0x1bd -> 445
	"NULL", // 0x1be -> 446
	"NULL", // 0x1bf -> 447
	"KEY_DEL_EOL",	// 0x1c0 -> 448
	"KEY_DEL_EOS",	// 0x1c1 -> 449
	"KEY_INS_LINE", // 0x1c2 -> 450
	"KEY_DEL_LINE", // 0x1c3 -> 451
	"NULL", // 0x1c4 -> 452
	"NULL", // 0x1c5 -> 453
	"NULL", // 0x1c6 -> 454
	"NULL", // 0x1c7 -> 455
	"NULL", // 0x1c8 -> 456
	"NULL", // 0x1c9 -> 457
	"NULL", // 0x1ca -> 458
	"NULL", // 0x1cb -> 459
	"NULL", // 0x1cc -> 460
	"NULL", // 0x1cd -> 461
	"NULL", // 0x1ce -> 462
	"NULL", // 0x1cf -> 463
	"KEY_FN",	// 0x1d0 -> 464
	"KEY_FN_ESC",	// 0x1d1 -> 465
	"KEY_FN_F1",	// 0x1d2 -> 466
	"KEY_FN_F2",	// 0x1d3 -> 467
	"KEY_FN_F3",	// 0x1d4 -> 468
	"KEY_FN_F4",	// 0x1d5 -> 469
	"KEY_FN_F5",	// 0x1d6 -> 470
	"KEY_FN_F6",	// 0x1d7 -> 471
	"KEY_FN_F7",	// 0x1d8 -> 472
	"KEY_FN_F8",	// 0x1d9 -> 473
	"KEY_FN_F9",	// 0x1da -> 474
	"KEY_FN_F10",	// 0x1db -> 475
	"KEY_FN_F11",	// 0x1dc -> 476
	"KEY_FN_F12",	// 0x1dd -> 477
	"KEY_FN_1", // 0x1de -> 478
	"KEY_FN_2", // 0x1df -> 479
	"KEY_FN_D", // 0x1e0 -> 480
	"KEY_FN_E", // 0x1e1 -> 481
	"KEY_FN_F", // 0x1e2 -> 482
	"KEY_FN_S", // 0x1e3 -> 483
	"KEY_FN_B", // 0x1e4 -> 484
	"NULL", // 0x1e5 -> 485
	"NULL", // 0x1e6 -> 486
	"NULL", // 0x1e7 -> 487
	"NULL", // 0x1e8 -> 488
	"NULL", // 0x1e9 -> 489
	"NULL", // 0x1ea -> 490
	"NULL", // 0x1eb -> 491
	"NULL", // 0x1ec -> 492
	"NULL", // 0x1ed -> 493
	"NULL", // 0x1ee -> 494
	"NULL", // 0x1ef -> 495
	"NULL", // 0x1f0 -> 496
	"KEY_BRL_DOT1", // 0x1f1 -> 497
	"KEY_BRL_DOT2", // 0x1f2 -> 498
	"KEY_BRL_DOT3", // 0x1f3 -> 499
	"KEY_BRL_DOT4", // 0x1f4 -> 500
	"KEY_BRL_DOT5", // 0x1f5 -> 501
	"KEY_BRL_DOT6", // 0x1f6 -> 502
	"KEY_BRL_DOT7", // 0x1f7 -> 503
	"KEY_BRL_DOT8", // 0x1f8 -> 504
	"KEY_BRL_DOT9", // 0x1f9 -> 505
	"KEY_BRL_DOT10",	// 0x1fa -> 506
	"NULL", // 0x1fb -> 507
	"NULL", // 0x1fc -> 508
	"NULL", // 0x1fd -> 509
	"NULL", // 0x1fe -> 510
	"NULL", // 0x1ff -> 511
	"KEY_NUMERIC_0",	// 0x200 -> 512
	"KEY_NUMERIC_1",	// 0x201 -> 513
	"KEY_NUMERIC_2",	// 0x202 -> 514
	"KEY_NUMERIC_3",	// 0x203 -> 515
	"KEY_NUMERIC_4",	// 0x204 -> 516
	"KEY_NUMERIC_5",	// 0x205 -> 517
	"KEY_NUMERIC_6",	// 0x206 -> 518
	"KEY_NUMERIC_7",	// 0x207 -> 519
	"KEY_NUMERIC_8",	// 0x208 -> 520
	"KEY_NUMERIC_9",	// 0x209 -> 521
	"KEY_NUMERIC_STAR", // 0x20a -> 522
	"KEY_NUMERIC_POUND",	// 0x20b -> 523
	"KEY_NUMERIC_A",	// 0x20c -> 524
	"KEY_NUMERIC_B",	// 0x20d -> 525
	"KEY_NUMERIC_C",	// 0x20e -> 526
	"KEY_NUMERIC_D",	// 0x20f -> 527
	"KEY_CAMERA_FOCUS", // 0x210 -> 528
	"KEY_WPS_BUTTON",	// 0x211 -> 529
	"KEY_TOUCHPAD_TOGGLE",	// 0x212 -> 530
	"KEY_TOUCHPAD_ON",	// 0x213 -> 531
	"KEY_TOUCHPAD_OFF", // 0x214 -> 532
	"KEY_CAMERA_ZOOMIN",	// 0x215 -> 533
	"KEY_CAMERA_ZOOMOUT",	// 0x216 -> 534
	"KEY_CAMERA_UP",	// 0x217 -> 535
	"KEY_CAMERA_DOWN",	// 0x218 -> 536
	"KEY_CAMERA_LEFT",	// 0x219 -> 537
	"KEY_CAMERA_RIGHT", // 0x21a -> 538
	"KEY_ATTENDANT_ON", // 0x21b -> 539
	"KEY_ATTENDANT_OFF",	// 0x21c -> 540
	"KEY_ATTENDANT_TOGGLE", // 0x21d -> 541
	"KEY_LIGHTS_TOGGLE",	// 0x21e -> 542
	"NULL", // 0x21f -> 543
	"BTN_DPAD_UP",	// 0x220 -> 544
	"BTN_DPAD_DOWN",	// 0x221 -> 545
	"BTN_DPAD_LEFT",	// 0x222 -> 546
	"BTN_DPAD_RIGHT",	// 0x223 -> 547
	"NULL", // 0x224 -> 548
	"NULL", // 0x225 -> 549
	"NULL", // 0x226 -> 550
	"NULL", // 0x227 -> 551
	"NULL", // 0x228 -> 552
	"NULL", // 0x229 -> 553
	"NULL", // 0x22a -> 554
	"NULL", // 0x22b -> 555
	"NULL", // 0x22c -> 556
	"NULL", // 0x22d -> 557
	"NULL", // 0x22e -> 558
	"NULL", // 0x22f -> 559
	"KEY_ALS_TOGGLE",	// 0x230 -> 560
	"NULL", // 0x231 -> 561
	"NULL", // 0x232 -> 562
	"NULL", // 0x233 -> 563
	"NULL", // 0x234 -> 564
	"NULL", // 0x235 -> 565
	"NULL", // 0x236 -> 566
	"NULL", // 0x237 -> 567
	"NULL", // 0x238 -> 568
	"NULL", // 0x239 -> 569
	"NULL", // 0x23a -> 570
	"NULL", // 0x23b -> 571
	"NULL", // 0x23c -> 572
	"NULL", // 0x23d -> 573
	"NULL", // 0x23e -> 574
	"NULL", // 0x23f -> 575
	"KEY_BUTTONCONFIG", // 0x240 -> 576
	"KEY_TASKMANAGER",	// 0x241 -> 577
	"KEY_JOURNAL",	// 0x242 -> 578
	"KEY_CONTROLPANEL", // 0x243 -> 579
	"KEY_APPSELECT",	// 0x244 -> 580
	"KEY_SCREENSAVER",	// 0x245 -> 581
	"KEY_VOICECOMMAND", // 0x246 -> 582
	"NULL", // 0x247 -> 583
	"NULL", // 0x248 -> 584
	"NULL", // 0x249 -> 585
	"NULL", // 0x24a -> 586
	"NULL", // 0x24b -> 587
	"NULL", // 0x24c -> 588
	"NULL", // 0x24d -> 589
	"NULL", // 0x24e -> 590
	"NULL", // 0x24f -> 591
	"KEY_BRIGHTNESS_MIN",	// 0x250 -> 592
	"KEY_BRIGHTNESS_MAX",	// 0x251 -> 593
	"NULL", // 0x252 -> 594
	"NULL", // 0x253 -> 595
	"NULL", // 0x254 -> 596
	"NULL", // 0x255 -> 597
	"NULL", // 0x256 -> 598
	"NULL", // 0x257 -> 599
	"NULL", // 0x258 -> 600
	"NULL", // 0x259 -> 601
	"NULL", // 0x25a -> 602
	"NULL", // 0x25b -> 603
	"NULL", // 0x25c -> 604
	"NULL", // 0x25d -> 605
	"NULL", // 0x25e -> 606
	"NULL", // 0x25f -> 607
	"KEY_KBDINPUTASSIST_PREV",	// 0x260 -> 608
	"KEY_KBDINPUTASSIST_NEXT",	// 0x261 -> 609
	"KEY_KBDINPUTASSIST_PREVGROUP", // 0x262 -> 610
	"KEY_KBDINPUTASSIST_NEXTGROUP", // 0x263 -> 611
	"KEY_KBDINPUTASSIST_ACCEPT",	// 0x264 -> 612
	"KEY_KBDINPUTASSIST_CANCEL",	// 0x265 -> 613
	"KEY_RIGHT_UP", // 0x266 -> 614
	"KEY_RIGHT_DOWN",	// 0x267 -> 615
	"KEY_LEFT_UP",	// 0x268 -> 616
	"KEY_LEFT_DOWN",	// 0x269 -> 617
	"KEY_ROOT_MENU",	// 0x26a -> 618
	"KEY_MEDIA_TOP_MENU",	// 0x26b -> 619
	"KEY_NUMERIC_11",	// 0x26c -> 620
	"KEY_NUMERIC_12",	// 0x26d -> 621
	"KEY_AUDIO_DESC",	// 0x26e -> 622
	"KEY_3D_MODE",	// 0x26f -> 623
	"KEY_NEXT_FAVORITE",	// 0x270 -> 624
	"KEY_STOP_RECORD",	// 0x271 -> 625
	"KEY_PAUSE_RECORD", // 0x272 -> 626
	"KEY_VOD",	// 0x273 -> 627
	"KEY_UNMUTE",	// 0x274 -> 628
	"KEY_FASTREVERSE",	// 0x275 -> 629
	"KEY_SLOWREVERSE",	// 0x276 -> 630
	"KEY_DATA", // 0x277 -> 631
	"NULL", // 0x278 -> 632
	"NULL", // 0x279 -> 633
	"NULL", // 0x27a -> 634
	"NULL", // 0x27b -> 635
	"NULL", // 0x27c -> 636
	"NULL", // 0x27d -> 637
	"NULL", // 0x27e -> 638
	"NULL", // 0x27f -> 639
	"NULL", // 0x280 -> 640
	"NULL", // 0x281 -> 641
	"NULL", // 0x282 -> 642
	"NULL", // 0x283 -> 643
	"NULL", // 0x284 -> 644
	"NULL", // 0x285 -> 645
	"NULL", // 0x286 -> 646
	"NULL", // 0x287 -> 647
	"NULL", // 0x288 -> 648
	"NULL", // 0x289 -> 649
	"NULL", // 0x28a -> 650
	"NULL", // 0x28b -> 651
	"NULL", // 0x28c -> 652
	"NULL", // 0x28d -> 653
	"NULL", // 0x28e -> 654
	"NULL", // 0x28f -> 655
	"NULL", // 0x290 -> 656
	"NULL", // 0x291 -> 657
	"NULL", // 0x292 -> 658
	"NULL", // 0x293 -> 659
	"NULL", // 0x294 -> 660
	"NULL", // 0x295 -> 661
	"NULL", // 0x296 -> 662
	"NULL", // 0x297 -> 663
	"NULL", // 0x298 -> 664
	"NULL", // 0x299 -> 665
	"NULL", // 0x29a -> 666
	"NULL", // 0x29b -> 667
	"NULL", // 0x29c -> 668
	"NULL", // 0x29d -> 669
	"NULL", // 0x29e -> 670
	"NULL", // 0x29f -> 671
	"NULL", // 0x2a0 -> 672
	"NULL", // 0x2a1 -> 673
	"NULL", // 0x2a2 -> 674
	"NULL", // 0x2a3 -> 675
	"NULL", // 0x2a4 -> 676
	"NULL", // 0x2a5 -> 677
	"NULL", // 0x2a6 -> 678
	"NULL", // 0x2a7 -> 679
	"NULL", // 0x2a8 -> 680
	"NULL", // 0x2a9 -> 681
	"NULL", // 0x2aa -> 682
	"NULL", // 0x2ab -> 683
	"NULL", // 0x2ac -> 684
	"NULL", // 0x2ad -> 685
	"NULL", // 0x2ae -> 686
	"NULL", // 0x2af -> 687
	"NULL", // 0x2b0 -> 688
	"NULL", // 0x2b1 -> 689
	"NULL", // 0x2b2 -> 690
	"NULL", // 0x2b3 -> 691
	"NULL", // 0x2b4 -> 692
	"NULL", // 0x2b5 -> 693
	"NULL", // 0x2b6 -> 694
	"NULL", // 0x2b7 -> 695
	"NULL", // 0x2b8 -> 696
	"NULL", // 0x2b9 -> 697
	"NULL", // 0x2ba -> 698
	"NULL", // 0x2bb -> 699
	"NULL", // 0x2bc -> 700
	"NULL", // 0x2bd -> 701
	"NULL", // 0x2be -> 702
	"NULL", // 0x2bf -> 703
	"BTN_TRIGGER_HAPPY|BTN_TRIGGER_HAPPY1",	// 0x2c0 -> 704
	"BTN_TRIGGER_HAPPY2",	// 0x2c1 -> 705
	"BTN_TRIGGER_HAPPY3",	// 0x2c2 -> 706
	"BTN_TRIGGER_HAPPY4",	// 0x2c3 -> 707
	"BTN_TRIGGER_HAPPY5",	// 0x2c4 -> 708
	"BTN_TRIGGER_HAPPY6",	// 0x2c5 -> 709
	"BTN_TRIGGER_HAPPY7",	// 0x2c6 -> 710
	"BTN_TRIGGER_HAPPY8",	// 0x2c7 -> 711
	"BTN_TRIGGER_HAPPY9",	// 0x2c8 -> 712
	"BTN_TRIGGER_HAPPY10",	// 0x2c9 -> 713
	"BTN_TRIGGER_HAPPY11",	// 0x2ca -> 714
	"BTN_TRIGGER_HAPPY12",	// 0x2cb -> 715
	"BTN_TRIGGER_HAPPY13",	// 0x2cc -> 716
	"BTN_TRIGGER_HAPPY14",	// 0x2cd -> 717
	"BTN_TRIGGER_HAPPY15",	// 0x2ce -> 718
	"BTN_TRIGGER_HAPPY16",	// 0x2cf -> 719
	"BTN_TRIGGER_HAPPY17",	// 0x2d0 -> 720
	"BTN_TRIGGER_HAPPY18",	// 0x2d1 -> 721
	"BTN_TRIGGER_HAPPY19",	// 0x2d2 -> 722
	"BTN_TRIGGER_HAPPY20",	// 0x2d3 -> 723
	"BTN_TRIGGER_HAPPY21",	// 0x2d4 -> 724
	"BTN_TRIGGER_HAPPY22",	// 0x2d5 -> 725
	"BTN_TRIGGER_HAPPY23",	// 0x2d6 -> 726
	"BTN_TRIGGER_HAPPY24",	// 0x2d7 -> 727
	"BTN_TRIGGER_HAPPY25",	// 0x2d8 -> 728
	"BTN_TRIGGER_HAPPY26",	// 0x2d9 -> 729
	"BTN_TRIGGER_HAPPY27",	// 0x2da -> 730
	"BTN_TRIGGER_HAPPY28",	// 0x2db -> 731
	"BTN_TRIGGER_HAPPY29",	// 0x2dc -> 732
	"BTN_TRIGGER_HAPPY30",	// 0x2dd -> 733
	"BTN_TRIGGER_HAPPY31",	// 0x2de -> 734
	"BTN_TRIGGER_HAPPY32",	// 0x2df -> 735
	"BTN_TRIGGER_HAPPY33",	// 0x2e0 -> 736
	"BTN_TRIGGER_HAPPY34",	// 0x2e1 -> 737
	"BTN_TRIGGER_HAPPY35",	// 0x2e2 -> 738
	"BTN_TRIGGER_HAPPY36",	// 0x2e3 -> 739
	"BTN_TRIGGER_HAPPY37",	// 0x2e4 -> 740
	"BTN_TRIGGER_HAPPY38",	// 0x2e5 -> 741
	"BTN_TRIGGER_HAPPY39",	// 0x2e6 -> 742
	"BTN_TRIGGER_HAPPY40",	// 0x2e7 -> 743
	"NULL", // 0x2e8 -> 744
	"NULL", // 0x2e9 -> 745
	"NULL", // 0x2ea -> 746
	"NULL", // 0x2eb -> 747
	"NULL", // 0x2ec -> 748
	"NULL", // 0x2ed -> 749
	"NULL", // 0x2ee -> 750
	"NULL", // 0x2ef -> 751
	"NULL", // 0x2f0 -> 752
	"NULL", // 0x2f1 -> 753
	"NULL", // 0x2f2 -> 754
	"NULL", // 0x2f3 -> 755
	"NULL", // 0x2f4 -> 756
	"NULL", // 0x2f5 -> 757
	"NULL", // 0x2f6 -> 758
	"NULL", // 0x2f7 -> 759
	"NULL", // 0x2f8 -> 760
	"NULL", // 0x2f9 -> 761
	"NULL", // 0x2fa -> 762
	"NULL", // 0x2fb -> 763
	"NULL", // 0x2fc -> 764
	"NULL", // 0x2fd -> 765
	"NULL", // 0x2fe -> 766
	"KEY_MAX",	// 0x2ff -> 767
};

const char** ev_type_values[] = {
	syn_values,	// 0x00
	key_button_values,	// 0x01
	rel_values,	// 0x02
	abs_values,	// 0x03
	misc_values,	// 0x04
	switch_values,	// 0x05
	NULL,	// 0x06
	NULL,	// 0x07
	NULL,	// 0x08
	NULL,	// 0x09
	NULL,	// 0x10
	NULL,	// 0xa
	NULL,	// 0xb
	NULL,	// 0xc
	NULL,	// 0xd
	NULL,	// 0xe
	NULL,	// 0xf
	NULL,	// 0x10
	led_values,	// 0x11
	sound_values,	// 0x12
	NULL,	// 0x13
	autorepeat_values,	// 0x14
	NULL,	// 0x15 -> EV_FF
	NULL,	// 0x16 -> EV_PWR
};
