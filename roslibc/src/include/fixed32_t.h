#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define FIXED_32_DIVISON_FORWARD_SHIFT 10
#define FIXED_32_FRACTION_SHIFT_AMOUNT 10
#define FIXED_32_SHIFT_MASK ((1 << FIXED_32_FRACTION_SHIFT_AMOUNT) - 1) // (all LSB set, all MSB clear)

typedef unsigned long fixed32_t;

fixed32_t fixed_32_convert_from_int(unsigned long input);

fixed32_t fixed_32_add(fixed32_t input1, fixed32_t input2);

fixed32_t fixed_32_subtract(fixed32_t input1, fixed32_t input2);

fixed32_t fixed_32_mult(fixed32_t input1, fixed32_t input2);

fixed32_t fixed_32_div(fixed32_t input1, fixed32_t input2);

fixed32_t fixed_32_abs(fixed32_t input);

fixed32_t fixed_32_get_fraction(fixed32_t input);

fixed32_t fixed_32_get_integer(fixed32_t input);

#ifdef __cplusplus
}
#endif