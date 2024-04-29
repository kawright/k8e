// Copyright (C) 2024  KA Wright

// key.h - Keyboard state

#ifndef __KEY_H__
#define __KEY_H__

#include <stdint.h>

// Stores info about the keyboard state.
typedef struct __KEYST__ {
    int32_t         st_len;
    const uint8_t   *st;
} KeySt;

// Initialize a KeySt.
void init_key_st(KeySt *this);

// Block until a supported key is pressed and return it.
uint8_t poll_key();

// Read the state of a supported key. Unsupported keys always return false.
bool read_key(KeySt *this, uint8_t key);

// Update the keyboard state.
void update_key_st();

#endif

