#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void Walk(const char *basePath) {
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
                printf("Directory: %s\n", path);
                Walk(path); // Recursive call
            } else {
                printf("File: %s\n", path);
            }
        }
    }

    closedir(dir);
}

int main() {
    Walk(".");
    return 0;
}
