/* base64 decoder */
#ifndef B64_H
#define B64_H

#define ARR_LEN(a) (sizeof(a) / sizeof(a[0]))

int b64_decode(char const* b64, char *buff, int len);

#endif
