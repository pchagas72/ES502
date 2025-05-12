#include "encryption.h"

void encryptFile(uint8_t *input, uint64_t size){
	uint8_t key[] = {'A', 'B', 'C'}; //Can be any chars, and any size array
	
	for(int i = 0; i < size; i++) {
		input[i] = input[i] ^ key[i % (sizeof(key)/sizeof(uint8_t))];
	}

	printf("File is encrypted.\n");
}

void decryptFile(){

}

