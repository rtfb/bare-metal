// Build with:
// $ gcc -ggdb -std=c99 b64.c tests/b64_test.c -Iinclude
#include <stdio.h>
#include <string.h>

#include "b64.h"

int main() {
    // Generated with:
    //
    // import base64
    // data = ['f', 'fo', 'foo', 'foobar', 'Guten Abend']
    // for d in data:
    //     print('"%s"  -->  "%s"' % (base64.b64encode(d), d))
    char* arr[] = {
        "Zg==",
        "Zm8=",
        "Zm9v",
        "Zm9vYmFy",
        "R3V0ZW4gQWJlbmQ=",
    };
    char* expected[] = {
        "f",
        "fo",
        "foo",
        "foobar",
        "Guten Abend",
    };
    char buff[256];
    int len = ARR_LEN(buff);
    int passed = 0;
    for (int i = 0; i < ARR_LEN(arr); ++i) {
        int bytes_decoded = b64_decode(arr[i], buff, len);
        if (bytes_decoded != strlen(expected[i])) {
            printf("b64_decode('%s'): bytes_decoded = %d (expected '%ld').\n",
                arr[i], bytes_decoded, strlen(expected[i]));
        } else {
            buff[bytes_decoded] = '\0';
            if (0 != strcmp(buff, expected[i])) {
                printf("b64_decode('%s') = '%s', expected '%s'.\n",
                    arr[i], buff, expected[i]);
            } else {
                ++passed;
            }
        }
    }
    if (passed == ARR_LEN(arr)) {
        printf("OK\n");
    } else {
        printf("FAILED\n");
    }
}
