
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
