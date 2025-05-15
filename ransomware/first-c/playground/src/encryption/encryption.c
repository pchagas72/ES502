#include "encryption.h"

int ask_encoder(){
    int encoder;
    printf("Choose a encoder:\n");
    printf("(1) XOR\n");
    printf("(2) Caesar\n");
    printf("(3) AES\n");
    printf(">>> ");
    scanf("%d", &encoder);
    if (encoder > NUM_ENCODERS || encoder < 1){
        printf("Escolha um ID válido.");
        return -1;
    }
    return encoder;
}

void XOR_encoder_decoder(uint8_t *input, uint64_t size, unsigned char *key){
    key = (uint8_t*)key;
	for(int i = 0; i < size; i++) {
		input[i] = input[i] ^ key[i % (sizeof(key)/sizeof(uint8_t))];
	}
}

void caesar_encoder(uint8_t *input, uint64_t size, int key){
	for(int i = 0; i < size; i++) {
		input[i] = input[i] + key;
	}
}

void caesar_decoder(uint8_t *input, uint64_t size, int key){
	for(int i = 0; i < size; i++) {
		input[i] = input[i] - key;
	}
}

int caesar_ask_choice(){
    int choice;
    printf("(1) Encode \n (2) Decode \n");
    printf(">>> ");
    scanf("%d", &choice);
    return choice;
}

int caesar_ask_key(){
    int key;
    printf("Type the key\n");
    printf(">>> ");
    scanf("%d", &key);
    return key;
}
