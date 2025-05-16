#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define PATH_MAX 4096

void search_in_file(const char *file_path, const char *word, int *found_any) {
      FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int found = 0; 

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (strstr(line, word)) {
            printf("Найдено в файле: %s, строка %d: %s", file_path, line_number, line);
            found = 1; 
        }
    }

    if (found) {
        *found_any = 1; 
    }

    fclose(file);
}

void search_in_directory(const char *dir_path, const char *word, int *found_any) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Ошибка открытия директории");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            search_in_directory(path, word, found_any);
        } else if (entry->d_type == DT_REG) {
            search_in_file(path, word, found_any);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *default_directory = "/home/user/2sem/pr7/home";
    const char *directory = default_directory;
    const char *word = NULL;

    if (argc > 1) {
        directory = argv[1];
    }
    if (argc > 2) {
        word = argv[2];
    } else {
        fprintf(stderr, "Не указано слово для поиска или директория.\n");
    }

    if (access(directory, F_OK) != 0) {
        fprintf(stderr, "Указанная директория не существует: %s\n", directory);
    }

    int found_any = 0;
    search_in_directory(directory, word, &found_any);

    if (found_any) {
        printf("Слово \"%s\" найдено в одном или нескольких файлах.\n", word);
    } else {
        printf("Слово \"%s\" не найдено ни в одном файле.\n", word);
    }
}

