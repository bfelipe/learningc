#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
    In this demo, lets implement a safer concatenation function
    using a pointer
*/

typedef struct {
    int len;
    char buffer[64];
} container_t;

const int MAX_BUFFER = 64;

int safe_concat(container_t *dst, char *src) {
    // the same could be done as dst == NULL;
    if (!dst || !src) {
        return 1;
    }
    int src_len = (int)strlen(src);
    int remain_space = (MAX_BUFFER - dst->len) - 1;
    bool is_overflow = src_len > remain_space;
    if (is_overflow) {
        strncat(dst->buffer, src, remain_space);
        dst->len = MAX_BUFFER - 1;
    } else {
        // Use strcpy from the known end of the buffer (dst->buffer + dst->len)
        // This is more efficient than letting strcat re-find the end
        strcpy(dst->buffer + dst->len, src);
        dst->len += src_len;
    }
    if (is_overflow) return 1;
    return 0;
}

int main() {
    container_t container = {0};
    char *in = "hello";
    strcat(container.buffer, in);
    container.len += (int)strlen(in);

    printf("%d, %s\n", container.len, container.buffer);

    int out = safe_concat(&container, " world");
    printf("%d, %d, %ld, %s\n", out, container.len, strlen(container.buffer), container.buffer);

    // 55 chars - last 3 digits out
    char *long_str = "0123456789012345678901234567890123456789012345678912345";
    int out2 = safe_concat(&container, long_str);
    printf("%d, %d, %s\n", out2, container.len, container.buffer); 
    printf("Final String Length: %zu\n", strlen(container.buffer));
    return 0;
}