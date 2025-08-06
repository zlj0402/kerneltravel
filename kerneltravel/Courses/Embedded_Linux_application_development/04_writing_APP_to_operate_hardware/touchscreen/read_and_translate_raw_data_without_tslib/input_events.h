#ifndef __INPUT_EVENTS_H__
#define __INPUT_EVENTS_H__

#define INPUT_PROP_DIRECT 0x01	/* direct input devices */

extern const char *ev_names[];
extern const char *ev_properties[];
extern const char *syn_values[];
extern const char *rel_values[];
extern const char *switch_values[];
extern const char *misc_values[];
extern const char *led_values[];
extern const char *autorepeat_values[];
extern const char *sound_values[];
extern const char *abs_values[];
extern const char *key_button_values[];

extern const char** ev_type_values[];

#endif
