#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define NUM_ENCODERS 2

int ask_encoder();
void XOR_encoder_decoder(uint8_t *input, uint64_t size);
void caesar_encoder(uint8_t *input, uint64_t size, int key);
void caesar_decoder(uint8_t *input, uint64_t size, int key);
int caesar_ask_key();
int caesar_ask_choice();
void RSA_encoder();
void RSA_decoder();
