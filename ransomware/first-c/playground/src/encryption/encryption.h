#include <stdint.h>
#include <stdio.h>

#define NUM_ENCODERS 1
#define XOR 0

int ask_encoder();
void XOR_encoder_decoder(uint8_t *input, uint64_t size);
void one_time_pad_encoder(uint8_t *input, uint64_t size);
