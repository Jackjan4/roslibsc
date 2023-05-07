#include "stringutils.h"

short string_last_occurence(const char* str, char cha, size_t len) {
    short result = -1;
    
    size_t str_len = strlen(str);
    if (len < str_len) {
        str_len = len;
    }

    for(unsigned char i = 0; i < len; i++) {
        if (str[i] == cha) {
            result = i;
        }
    }
    return result;
}