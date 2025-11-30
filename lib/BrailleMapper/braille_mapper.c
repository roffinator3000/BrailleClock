#include "braille_mapper.h"

// 4-dot Braille digit patterns (Top-L, Top-R, Bot-L, Bot-R)
// For 4-cell display: [H1][H2][M1][M2]
// Unique patterns for each digit to ensure proper display
static const uint8_t DIGIT_MAP[10][4] = {
    {1, 0, 0, 0}, // 0
    {0, 1, 0, 0}, // 1
    {1, 1, 0, 0}, // 2
    {0, 0, 1, 0}, // 3
    {1, 0, 1, 0}, // 4
    {0, 1, 1, 0}, // 5
    {1, 1, 1, 0}, // 6
    {0, 0, 0, 1}, // 7
    {1, 0, 0, 1}, // 8
    {0, 1, 0, 1}  // 9
};

void get_braille_pattern(int hours, int minutes, uint8_t* output_buffer) {
    int digits[4];
    
    // Extract individual digits
    digits[0] = hours / 10;      // Tens of hours
    digits[1] = hours % 10;      // Units of hours  
    digits[2] = minutes / 10;    // Tens of minutes
    digits[3] = minutes % 10;    // Units of minutes

    int buf_idx = 0;

    // Add each digit's pattern to the buffer
    for (int i = 0; i < 4; i++) {
        int digit = digits[i];
        for (int dot = 0; dot < 4; dot++) {
            output_buffer[buf_idx++] = DIGIT_MAP[digit][dot];
        }
    }
}