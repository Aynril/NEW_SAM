#pragma once

// Make sure macros aren't borked
#define TEST1
#define TEST2 1
#define TEST3 0
#define TEST4 true
#if ENABLED(TEST0)
#error "ENABLED is borked!"
#endif
#if DISABLED(TEST1)
#error "DISABLED is borked!"
#endif
#if !ENABLED(TEST2)
#error "ENABLED is borked!"
#endif
#if ENABLED(TEST3)
#error "ENABLED is borked!"
#endif
#if DISABLED(TEST4)
#error "DISABLED is borked!"
#endif
#if !ANY(TEST1, TEST2, TEST3, TEST4) || ANY(TEST0, TEST3)
#error "ANY is borked!"
#endif
#if DISABLED(TEST0, TEST1, TEST2, TEST4)
#error "DISABLED is borked!"
#endif
#undef TEST1
#undef TEST2
#undef TEST3
#undef TEST4

#if !ANY(SD_SUPPORT, RADIO_SUPPORT, SERIAL_OUTPUT_SUPPORT)
#error "Neither SD, Radio nor Serial is enabled. Code is useless"
#endif
