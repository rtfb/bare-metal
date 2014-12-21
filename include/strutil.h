#ifndef STRUTIL_H
#define STRUTIL_H

int str_startswith(char const* str, char const* prefix);
uint32_t str_len(char const* str);
int str_parse_int(char const* str);
char* str_int_to_str(int value, char* buffer, int len);

#endif
