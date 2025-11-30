#ifndef BRAILLE_MAPPER_H
#define BRAILLE_MAPPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void get_braille_pattern(int hours, int minutes, uint8_t* output_buffer);

#ifdef __cplusplus
}
#endif

#endif
