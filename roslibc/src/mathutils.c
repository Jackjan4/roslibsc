#include "mathutils.h"

#include "math.h"

float calculate_dino_dini_sigmoid(float x, float k)
{
    float dividend = x - k * x;

    float divisor = k - 2 * k * fabs(x) + 1;

    float result = dividend / divisor;

    return result;
}

void ull_to_base32hex_str(unsigned long long int num, size_t buf_size, char *buffer)
{

    unsigned char window_pos = 0;
    for (unsigned char i = buf_size; i > 0; i--)
    {
        unsigned char filter_window = (num >> window_pos) & 0b11111;

        if (filter_window < 10)
        {
            buffer[i - 1] = '0' + filter_window;
        }
        else
        {
            buffer[i - 1] = 'A' + filter_window - 10;
        }

        window_pos += 5;
    }
}