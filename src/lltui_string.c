#include "lltui_string.h"

void lltui_string_init(lltui_string* str, char* ref, uint32_t max_len) {
    str->data = ref;
    str->len = 0;
    str->data[str->len] = '\0';
    str->max_len = max_len;
    str->error = false;
}

static lltui_string lltui_string_rollback(lltui_string str, uint32_t len) {
    str.len = len;
    str.data[str.len] = '\0';
    str.error = true;
    return str;
}

lltui_string lltui_string_attach_char(lltui_string str, char c) {
    if (str.len+1 < str.max_len) {
        str.data[str.len++] = c;
        str.data[str.len] = '\0';
    } else {
        str.error = true;
    }
    return str;
}

lltui_string lltui_string_attach_str(lltui_string str, const char* s) {
    uint32_t start_len = str.len;

    for (; *s != '\0'; s++) {
        str = lltui_string_attach_char(str, *s);
        if (str.error) return lltui_string_rollback(str, start_len);
    }

    return str;
}

lltui_string lltui_string_attach_number(lltui_string str, uint32_t value) {
    uint32_t start_len = str.len;

    // digits come out least significant first -> reverse them afterwards
    do{
        str = lltui_string_attach_char(str, (char)('0' + value % 10));
        if (str.error) return lltui_string_rollback(str, start_len);
        value /= 10;
    }while(value > 0);

    lltui_string_swap_last_characters(str, str.len - start_len);

    return str;
}

lltui_string lltui_string_attach_int(lltui_string str, int32_t value) {
    uint32_t start_len = str.len;
    uint32_t magnitude = (uint32_t)value;

    if (value < 0) {
        str = lltui_string_attach_char(str, '-');
        magnitude = 0u - magnitude;
    }

    str = lltui_string_attach_number(str, magnitude);
    if (str.error) return lltui_string_rollback(str, start_len);

    return str;
}

void lltui_string_swap_last_characters(lltui_string str, uint32_t len) {
    if (len > str.len) len = str.len;
    if (len < 2) return;

    uint32_t i = str.len - len;
    uint32_t k = str.len - 1;

    for (; i < k; i++, k--) {
        char tmp = str.data[i];
        str.data[i] = str.data[k];
        str.data[k] = tmp;
    }
}

static uint32_t lltui_string_pow10(uint8_t exponent) {
    uint32_t value = 1;
    for (; exponent >= 1; exponent--) {
        value *= 10;
    }
    return value;
}

lltui_string lltui_string_attach_float(lltui_string str, float value, uint8_t precision) {
    uint32_t start_len = str.len;

    if (value < 0.0f) {
        str = lltui_string_attach_char(str, '-');
        value = -value;
    }

    uint32_t scale = lltui_string_pow10(precision);
    uint32_t real_part = (uint32_t)value;
    uint32_t decimal_part = (uint32_t)((value - (float)real_part) * (float)scale + 0.5f);

    // rounding can carry into the real part (e.g. 1.999 with precision 2)
    if (decimal_part >= scale) {
        decimal_part -= scale;
        real_part++;
    }

    str = lltui_string_attach_number(str, real_part);

    if (precision > 0) {
        str = lltui_string_attach_char(str, '.');

        // leading zeros of the decimal part (e.g. 0.05 -> "05")
        for (uint32_t div = scale / 10; div > 1 && decimal_part < div; div /= 10) {
            str = lltui_string_attach_char(str, '0');
        }

        str = lltui_string_attach_number(str, decimal_part);
    }

    if (str.error) return lltui_string_rollback(str, start_len);

    return str;
}
