#pragma once

#if defined(PV_WINDOWING_API_GLFW)
#define PV_KEYBOARD_KEY_SPACE              32
#define PV_KEYBOARD_KEY_APOSTROPHE         39  /* ' */
#define PV_KEYBOARD_KEY_COMMA              44  /* , */
#define PV_KEYBOARD_KEY_MINUS              45  /* - */
#define PV_KEYBOARD_KEY_PERIOD             46  /* . */
#define PV_KEYBOARD_KEY_SLASH              47  /* / */
#define PV_KEYBOARD_KEY_0                  48
#define PV_KEYBOARD_KEY_1                  49
#define PV_KEYBOARD_KEY_2                  50
#define PV_KEYBOARD_KEY_3                  51
#define PV_KEYBOARD_KEY_4                  52
#define PV_KEYBOARD_KEY_5                  53
#define PV_KEYBOARD_KEY_6                  54
#define PV_KEYBOARD_KEY_7                  55
#define PV_KEYBOARD_KEY_8                  56
#define PV_KEYBOARD_KEY_9                  57
#define PV_KEYBOARD_KEY_SEMICOLON          59  /* ; */
#define PV_KEYBOARD_KEY_EQUAL              61  /* = */
#define PV_KEYBOARD_KEY_A                  65
#define PV_KEYBOARD_KEY_B                  66
#define PV_KEYBOARD_KEY_C                  67
#define PV_KEYBOARD_KEY_D                  68
#define PV_KEYBOARD_KEY_E                  69
#define PV_KEYBOARD_KEY_F                  70
#define PV_KEYBOARD_KEY_G                  71
#define PV_KEYBOARD_KEY_H                  72
#define PV_KEYBOARD_KEY_I                  73
#define PV_KEYBOARD_KEY_J                  74
#define PV_KEYBOARD_KEY_K                  75
#define PV_KEYBOARD_KEY_L                  76
#define PV_KEYBOARD_KEY_M                  77
#define PV_KEYBOARD_KEY_N                  78
#define PV_KEYBOARD_KEY_O                  79
#define PV_KEYBOARD_KEY_P                  80
#define PV_KEYBOARD_KEY_Q                  81
#define PV_KEYBOARD_KEY_R                  82
#define PV_KEYBOARD_KEY_S                  83
#define PV_KEYBOARD_KEY_T                  84
#define PV_KEYBOARD_KEY_U                  85
#define PV_KEYBOARD_KEY_V                  86
#define PV_KEYBOARD_KEY_W                  87
#define PV_KEYBOARD_KEY_X                  88
#define PV_KEYBOARD_KEY_Y                  89
#define PV_KEYBOARD_KEY_Z                  90
#define PV_KEYBOARD_KEY_LEFT_BRACKET       91  /* [ */
#define PV_KEYBOARD_KEY_BACKSLASH          92  /* \ */
#define PV_KEYBOARD_KEY_RIGHT_BRACKET      93  /* ] */
#define PV_KEYBOARD_KEY_GRAVE_ACCENT       96  /* ` */
#define PV_KEYBOARD_KEY_WORLD_1            161 /* non-US #1 */
#define PV_KEYBOARD_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define PV_KEYBOARD_KEY_ESCAPE             256
#define PV_KEYBOARD_KEY_ENTER              257
#define PV_KEYBOARD_KEY_TAB                258
#define PV_KEYBOARD_KEY_BACKSPACE          259
#define PV_KEYBOARD_KEY_INSERT             260
#define PV_KEYBOARD_KEY_DELETE             261
#define PV_KEYBOARD_KEY_RIGHT              262
#define PV_KEYBOARD_KEY_LEFT               263
#define PV_KEYBOARD_KEY_DOWN               264
#define PV_KEYBOARD_KEY_UP                 265
#define PV_KEYBOARD_KEY_PAGEUP             266
#define PV_KEYBOARD_KEY_PAGEDOWN           267
#define PV_KEYBOARD_KEY_HOME               268
#define PV_KEYBOARD_KEY_END                269
#define PV_KEYBOARD_KEY_CAPS_LOCK          280
#define PV_KEYBOARD_KEY_SCROLL_LOCK        281
#define PV_KEYBOARD_KEY_NUM_LOCK           282
#define PV_KEYBOARD_KEY_PRINT_SCREEN       283
#define PV_KEYBOARD_KEY_PAUSE              284
#define PV_KEYBOARD_KEY_F1                 290
#define PV_KEYBOARD_KEY_F2                 291
#define PV_KEYBOARD_KEY_F3                 292
#define PV_KEYBOARD_KEY_F4                 293
#define PV_KEYBOARD_KEY_F5                 294
#define PV_KEYBOARD_KEY_F6                 295
#define PV_KEYBOARD_KEY_F7                 296
#define PV_KEYBOARD_KEY_F8                 297
#define PV_KEYBOARD_KEY_F9                 298
#define PV_KEYBOARD_KEY_F10                299
#define PV_KEYBOARD_KEY_F11                300
#define PV_KEYBOARD_KEY_F12                301
#define PV_KEYBOARD_KEY_F13                302
#define PV_KEYBOARD_KEY_F14                303
#define PV_KEYBOARD_KEY_F15                304
#define PV_KEYBOARD_KEY_F16                305
#define PV_KEYBOARD_KEY_F17                306
#define PV_KEYBOARD_KEY_F18                307
#define PV_KEYBOARD_KEY_F19                308
#define PV_KEYBOARD_KEY_F20                309
#define PV_KEYBOARD_KEY_F21                310
#define PV_KEYBOARD_KEY_F22                311
#define PV_KEYBOARD_KEY_F23                312
#define PV_KEYBOARD_KEY_F24                313
#define PV_KEYBOARD_KEY_F25                314
#define PV_KEYBOARD_KEY_KP_0               320
#define PV_KEYBOARD_KEY_KP_1               321
#define PV_KEYBOARD_KEY_KP_2               322
#define PV_KEYBOARD_KEY_KP_3               323
#define PV_KEYBOARD_KEY_KP_4               324
#define PV_KEYBOARD_KEY_KP_5               325
#define PV_KEYBOARD_KEY_KP_6               326
#define PV_KEYBOARD_KEY_KP_7               327
#define PV_KEYBOARD_KEY_KP_8               328
#define PV_KEYBOARD_KEY_KP_9               329
#define PV_KEYBOARD_KEY_KP_DECIMAL         330
#define PV_KEYBOARD_KEY_KP_DIVIDE          331
#define PV_KEYBOARD_KEY_KP_MULTIPLY        332
#define PV_KEYBOARD_KEY_KP_SUBTRACT        333
#define PV_KEYBOARD_KEY_KP_ADD             334
#define PV_KEYBOARD_KEY_KP_ENTER           335
#define PV_KEYBOARD_KEY_KP_EQUAL           336
#define PV_KEYBOARD_KEY_LEFT_SHIFT         340
#define PV_KEYBOARD_KEY_LEFT_CONTROL       341
#define PV_KEYBOARD_KEY_LEFT_ALT           342
#define PV_KEYBOARD_KEY_LEFT_SUPER         343
#define PV_KEYBOARD_KEY_RIGHT_SHIFT        344
#define PV_KEYBOARD_KEY_RIGHT_CONTROL      345
#define PV_KEYBOARD_KEY_RIGHT_ALT          346
#define PV_KEYBOARD_KEY_RIGHT_SUPER        347
#define PV_KEYBOARD_KEY_MENU               348
#elif defined(PV_WINDOWING_API_WIN32)
#define PV_KEYBOARD_KEY_A	0x41
#define PV_KEYBOARD_KEY_B	0x42
#define PV_KEYBOARD_KEY_C	0x43
#define PV_KEYBOARD_KEY_D	0x44
#define PV_KEYBOARD_KEY_E	0x45
#define PV_KEYBOARD_KEY_F	0x46
#define PV_KEYBOARD_KEY_G	0x47
#define PV_KEYBOARD_KEY_H	0x48
#define PV_KEYBOARD_KEY_I	0x49
#define PV_KEYBOARD_KEY_J	0x4A
#define PV_KEYBOARD_KEY_K	0x4B
#define PV_KEYBOARD_KEY_L	0x4C
#define PV_KEYBOARD_KEY_M	0x4D
#define PV_KEYBOARD_KEY_N	0x4E
#define PV_KEYBOARD_KEY_O	0x4F
#define PV_KEYBOARD_KEY_P	0x50
#define PV_KEYBOARD_KEY_Q	0x51
#define PV_KEYBOARD_KEY_R	0x52
#define PV_KEYBOARD_KEY_S	0x53
#define PV_KEYBOARD_KEY_T	0x54
#define PV_KEYBOARD_KEY_U	0x55
#define PV_KEYBOARD_KEY_V	0x56
#define PV_KEYBOARD_KEY_W	0x57
#define PV_KEYBOARD_KEY_X	0x58
#define PV_KEYBOARD_KEY_Y	0x59
#define PV_KEYBOARD_KEY_Z	0x5A

#define PV_KEYBOARD_KEY_LEFT	0x25
#define PV_KEYBOARD_KEY_UP		0x26
#define PV_KEYBOARD_KEY_RIGHT	0x27
#define PV_KEYBOARD_KEY_DOWN	0x28

#define PV_KEYBOARD_KEY_BACKSPACE	0x08
#define PV_KEYBOARD_KEY_TAB			0x09
#define PV_KEYBOARD_KEY_ENTER		0x0D
#define PV_KEYBOARD_KEY_ESCAPE		0x1B
#define PV_KEYBOARD_KEY_SPACE		0x20
#define PV_KEYBOARD_KEY_PAGEUP		0x21
#define PV_KEYBOARD_KEY_PAGEDOWN	0x22
#define PV_KEYBOARD_KEY_END			0x23
#define PV_KEYBOARD_KEY_HOME		0x24
#define PV_KEYBOARD_KEY_INSERT		0x2D
#define PV_KEYBOARD_KEY_DELETE		0x2E

#endif