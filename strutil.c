#include <stdint.h>
#include <stdbool.h>

int str_startswith(char const* str, char const* prefix) {
    if (!str || !prefix) {
        return 0;
    }
    while (*str != '\0' && *prefix != '\0') {
        if (*str != *prefix) {
            return 0;
        }
        ++str;
        ++prefix;
    }
    if (*str == '\0' && *prefix != '\0') {
        return 0;
    }
    return 1;
}

uint32_t str_len(char const* str) {
    if (!str) {
        return 0;
    }
    char const* p = str;
    while (*p != 0) {
        ++p;
    }
    return p - str;
}

// TODO: handle hex
int str_parse_int(char const* str) {
    int result = 0;
    bool negative = false;
    while (*str && *str == ' ') {
        ++str;
    }
    if (*str == '-') {
        ++str;
        negative = true;
    }
    if (*str == '+') {
        ++str;
    }
    char const* start = str;
    while (*str && *str >= '0' && *str <= '9') {
        ++str;
    }
    --str;
    --start;
    int power = 1;
    while (str != start) {
        int digit = *str - '0';
        result += digit * power;
        power *= 10;
        --str;
    }
    if (negative) {
        return -result;
    }
    return result;
}

char* str_int_to_str(int value, char* buff, int len) {
    bool negative = false;
    if (value < 0) {
        negative = true;
        value *= -1;
    }
    buff += len - 1;
    *buff = '\0';
    --buff;
    if (value == 0) {
        *buff = '0';
        return buff;
    }
    while (value > 0) {
        *buff = '0' + (value % 10);
        --buff;
        value /= 10;
    }
    if (negative) {
        *buff = '-';
        return buff;
    }
    return buff + 1;
}
