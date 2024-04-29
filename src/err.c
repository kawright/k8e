// Copyright (C) 2024  KA Wright

// err.c - Error handling

#include "err.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void init_err(Err *this) {
    this->code = ERR_OK;
    strcpy(this->msg, "");
}

bool is_err(const Err *this) {
    return this->code != ERR_OK;
}

void err_alert(const Err *this) {
    if (is_err(this)) {
        fprintf(stderr, "ERROR 0x%02x: %s\n", this->code, this->msg);
    }
}

