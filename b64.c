#include "b64.h"
#include "common.h"

int decode_one(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    return 0;
}

int b64_decode(char const* b64, char *buff, int len) {
    int shifts[] = {18, 12, 6, 0};
    int shift = 0;
    long int decoder = 0;
    char const *p = b64;
    char *pb = buff;
    while (*p != '\0' && *p != '=') {
        decoder |= (decode_one(*p) << (shifts[shift]));
        ++shift;
        ++p;
        if (shift == ARR_LEN(shifts)) {
            shift = 0;
            if ((pb - buff + 4) >= len) {
                return -1;
            }
            *pb++ = ((decoder & 0x00ff0000) >> 16);
            *pb++ = ((decoder & 0x0000ff00) >> 8);
            *pb++ = (decoder & 0x000000ff);
            decoder = 0;
        }
    }
    switch ((p - b64) % 4) {
        case 0:
        case 1:
            break;
        case 2:
            *pb++ = ((decoder & 0x00ff0000) >> 16);
            break;
        case 3:
            *pb++ = ((decoder & 0x00ff0000) >> 16);
            *pb++ = ((decoder & 0x0000ff00) >> 8);
            break;
    }
    return pb - buff;
}
