#include "common.h"

_Static_assert(sizeof(char16_t) == 2, "sizeof(char16_t) != 2");
_Static_assert(sizeof(char16_t) == sizeof(UChar), "sizeof(char16_t) != sizeof(UChar)");

char *wc2c(const char16_t *s)
{
    UErrorCode errorCode = U_ZERO_ERROR;
    int32_t len = 0;
    u_strToUTF8(NULL, 0, &len, s, -1, &errorCode);
    assert(errorCode == U_BUFFER_OVERFLOW_ERROR);

    char *dst = malloc(len + 1);
    errorCode = U_ZERO_ERROR;
    u_strToUTF8(dst, len + 1, NULL, s, -1, &errorCode);
    assert(!U_FAILURE(errorCode));

    return dst;
}

char16_t *c2wc(const char *s)
{
    UErrorCode errorCode = U_ZERO_ERROR;
    int32_t len = 0;
    u_strFromUTF8(NULL, 0, &len, s, -1, &errorCode);
    assert(errorCode == U_BUFFER_OVERFLOW_ERROR);

    UChar *dst = malloc((len + 1) * sizeof(UChar));
    errorCode = U_ZERO_ERROR;
    u_strFromUTF8(dst, len + 1, NULL, s, -1, &errorCode);
    assert(!U_FAILURE(errorCode));

    return dst;
}
