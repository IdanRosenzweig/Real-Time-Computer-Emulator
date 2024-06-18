#ifndef BIOS_COMMON_H
#define BIOS_COMMON_H

#include "sizes.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

// write word to the address
void write_word_to_addr(uint16_t seg, uint32_t off, uint16_t val);

// write word to the address
uint16_t read_word_from_addr(uint16_t seg, uint32_t off);


// set the value of es
EXTERNC
void set_es(uint16_t value);

// write byte at es:addr
EXTERNC
void write_es_byte(uint8_t *addr, uint8_t byte);

// write word at es:addr
EXTERNC
void write_es_word(uint16_t *addr, uint16_t word);

// write dword at es:addr
EXTERNC
void write_es_dword(uint32_t *addr, uint32_t dword);

EXTERNC
void copy_es_word(uint16_t *daddr, uint16_t *saddr);


// get input from io port
EXTERNC
uint8_t in_port(uint16_t port);

// send value to io port
EXTERNC
void out_port(uint16_t port, uint8_t value);


// disable interrupts
EXTERNC
void disable_intr(void);

// enable interrupts
EXTERNC
void enable_intr(void);


EXTERNC
void bsv_test(void);

EXTERNC
void bsv_video(void);

EXTERNC
void bsv_disk(void);

EXTERNC
void bsv_keyboard(void);

EXTERNC
void bsv_irq_disk(void);


EXTERNC
void write_char(uint8_t character);

EXTERNC
int write_string_cpp(const char *s);

EXTERNC
void scroll_lines(int n);


#endif
