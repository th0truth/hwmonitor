#include "file.h"
#include "base.h"
#include "cJSON.h"
#include <stdint.h>
#include <dirent.h>

#define FILE_READ_BUFFER 4096

char *
file_read_stripped(const char *filename, const char *exclude, bool verbose)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        if (verbose) {
            fprintf(stderr, "error: failed to open '%s': %s\n", filename, strerror(errno));
        }
        return NULL;
    }

    char *buffer = malloc(FILE_READ_BUFFER);
    if (buffer == NULL) {
        if (verbose) {
            fprintf(stderr, "error: failed to allocate memory: %s", strerror(errno));
        }
        fclose(fp);
        return NULL;
    }

    int c;
    size_t n = 0;
    size_t capacity = FILE_READ_BUFFER;

    /* Read the file character by character and strip excluded ones */
    while ((c = fgetc(fp)) != EOF) {
        if (n + 1 >= capacity) {
            capacity *= 2;
            char *line_buffer = realloc(buffer, capacity);
            if (line_buffer == NULL) {
                free(buffer);
                fclose(fp);
                return NULL;
            }
            buffer = line_buffer;
        }

        if (exclude == NULL || strchr(exclude, c) == NULL) {
            buffer[n++] = c;
        }
    }

    buffer[n] = '\0';
    fclose(fp);
    return buffer;
}

bool
file_write_string(const char *filename, const char *data)
{
    if (filename == NULL || data == NULL) {
        return false;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "error: failed to open '%s' for writing: %s\n", filename, strerror(errno));
        return false;
    }

    if (fputs(data, fp) == EOF) {
        fprintf(stderr, "error: failed to write to '%s': %s\n", filename, strerror(errno));
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

void **
sysfs_enumerate(const char *dir_path, sysfs_parse_fn parse_fn, int max_items, int *out_count)
{
    if (out_count == NULL) {
        return NULL;
    } 
    *out_count = 0;

    if (dir_path == NULL || parse_fn == NULL || max_items <= 0) {
        return NULL;
    }

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        return NULL; 
    }

    void **list = calloc(max_items, sizeof(void *));
    if (list == NULL) {
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && *out_count < max_items) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        void *item = parse_fn(entry->d_name);
        if (item != NULL) {
            list[(*out_count)++] = item;
        }
    }   
    
    closedir(dir);
        
    return list;
}

