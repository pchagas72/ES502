#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "./AES/AES.h"

#define NUM_ENCODERS 5

int ask_encoder();
void XOR_encoder_decoder(uint8_t *input, uint64_t size);
void caesar_encoder(uint8_t *input, uint64_t size, int key);
void caesar_decoder(uint8_t *input, uint64_t size, int key);
int caesar_ask_key();
int caesar_ask_choice();
void AES_encoder();
void AES_decoder();
void RSA_encoder();
void RSA_decoder();
