#include "encryption.h"

int ask_encoder(){
    int encoder;
    printf("Choose a encoder:\n");
    printf("(1) XOR\n");
    printf(">>> ");
    scanf("%d", &encoder);
    if (encoder > NUM_ENCODERS || encoder < 1){
        printf("Escolha um ID válido.");
        return -1;
    }
    return encoder;
}

void XOR_encoder_decoder(uint8_t *input, uint64_t size){
	uint8_t key[] = {'S', 'I', 'D'}; // key
	
	for(int i = 0; i < size; i++) {
		input[i] = input[i] ^ key[i % (sizeof(key)/sizeof(uint8_t))];
	}
}

