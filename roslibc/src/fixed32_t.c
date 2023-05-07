#include "fixed32_t.h"

fixed32_t fixed_32_convert_from_ulong(unsigned long input) {
    fixed32_t result = input << FIXED_32_FRACTION_SHIFT_AMOUNT;

    return result;
};

fixed32_t fixed_32_add(fixed32_t input1, fixed32_t input2) {
    fixed32_t result;

    result = input1 + input2;

    return result;
};

fixed32_t fixed_32_subtract(fixed32_t input1, fixed32_t input2) {
    fixed32_t result;

    result = input1 - input2;

    return result;
};

fixed32_t fixed_32_mult(fixed32_t input1, fixed32_t input2) {
    fixed32_t result;

    result = (input1 * input2) >> FIXED_32_FRACTION_SHIFT_AMOUNT;

    return result;
};

fixed32_t fixed_32_div(fixed32_t input1, fixed32_t input2) {
    fixed32_t result;

    unsigned long long input1_64 = input1 << FIXED_32_DIVISON_FORWARD_SHIFT;

    result = input1_64 / input2;

    return result;
};

fixed32_t fixed_32_abs(fixed32_t input) {
    fixed32_t result;

    return result;
};

fixed32_t fixed_32_get_fraction(fixed32_t input) {
    return input & FIXED_32_SHIFT_MASK;
};

fixed32_t fixed_32_get_integer(fixed32_t input) {
    return input & (~FIXED_32_SHIFT_MASK);
};