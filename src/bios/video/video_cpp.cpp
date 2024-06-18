#include "../common/common.h"

uint16_t cursor_x, cursor_y;

EXTERNC
void set_video_mode_cpp(void) {
    out_port(0x3c4, 2);
    out_port(0x3c5, 4);

    out_port(0x3c4, 4);
    out_port(0x3c5, 6);

    out_port(0x3ce, 5);
    out_port(0x3cf, 0);

    out_port(0x3ce, 6);
    out_port(0x3cf, 5);
}

//EXTERNC
//void write_char(uint8_t character) {
//    __asm__("push es\n");
//    set_es(0xb800);
//
//    write_es_word((uint16_t *) ((cursor_y * 0x28 + cursor_x) * 2), 0x0700 + (uint16_t) character);
//    cursor_x++;
//    if (cursor_x >= 0x28 || !(character ^ 0x0a)) {
//        cursor_x = 0;
//        cursor_y++;
//    }
//
//    if (cursor_y >= 0x19) {
//        uint32_t j;
//        for (j = 0; j < 0x18; j++)
//            copy_es_word((uint16_t *) (j * 2), (uint16_t *) ((0x28 + j) * 2));
//        for (; j < 0x19 * 0x28; j++)
//            write_es_word((uint16_t *) (j * 2), 0x0700);
//        cursor_x = 0;
//        cursor_y--;
//    }
//
//    __asm__("pop es");
//}

EXTERNC
void write_char(uint8_t character) {
    write_word_to_addr(0x0, 0xb8000 + (cursor_y * 0x28 + cursor_x) * sizeof(uint16_t), 0x0700 + (uint16_t) character);

    cursor_x++;
    if (cursor_x >= 0x28 || !(character ^ 0x0a)) {
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= 0x19) {
        scroll_lines(1);
//        cursor_x = 0;
//        cursor_y--;
    }
}

//EXTERNC
//uint32_t write_string(const char *s) {
//    __asm__("push es\n");
//    set_es(0xb800);
//
//    uint32_t i = 0;
//    while (s[i]) {
//        write_es_word((uint16_t *) ((cursor_y * 0x28 + cursor_x) * 2), 0x0700 + s[i]);
//        cursor_x++;
//        if (cursor_x >= 0x28 || !(s[i] ^ 0x0a)) {
//            cursor_x = 0;
//            cursor_y++;
//        }
//
//        if (cursor_y >= 0x19) {
//            uint32_t j;
//            for (j = 0; j < 0x18 * 0x28; j++)
//                copy_es_word((uint16_t *) (j * 2), (uint16_t *) ((0x28 + j) * 2));
//            for (; j < 0x19 * 0x28; j++)
//                write_es_word((uint16_t *) (j * 2), 0x0700);
//            cursor_x = 0;
//            cursor_y--;
//        }
//
//        i++;
//    }
//
//    __asm__("pop es");
//
//    return i;
//
//
//}

EXTERNC
int write_string_cpp(const char* s) {
    int i = 0;
    while (s[i]) {
        write_char(s[i]);
        i++;
    }

    return i;
}

EXTERNC
void scroll_lines(int n) {
    uint32_t vram = 0xb8000;

    uint16_t i;
    for (i = 0; i < (0x19 - n) * 0x28; i++)
        write_word_to_addr(0x0, vram + i * sizeof(uint16_t),
                           read_word_from_addr(0x0, vram + (n * 0x28 + i) * sizeof(uint16_t))
        );
    for (; i < 0x19 * 0x28; i++)
        write_word_to_addr(0x0, vram + i * sizeof(uint16_t), 0x700);

    cursor_x = 0;
    cursor_y -= n;
}