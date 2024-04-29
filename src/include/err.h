// Copyright (C) 2024  KA Wright

// err.h - Error handling

#ifndef __ERR_H__
#define __ERR_H__

#include <stdbool.h>

#define MAX_ERR_MSG_LEN 127

// Lists all supported error codes.
typedef enum __ERRCODE__ {
    ERR_OK,
    ERR_GEN,
    ERR_ARGV,
    ERR_MEM,
    ERR_INIT,
    ERR_RANGE,
    ERR_IO,
    ERR_DATA,
    ERR_SUBSYS,
    ERR_QUIT
} ErrCode;

// Stores an error state.
typedef struct __ERR__ {
    ErrCode     code;
    char        msg[MAX_ERR_MSG_LEN];
} Err;

// Initialize or reset an Err.
void init_err(Err *this);

// Test if an error condition has occurred.
bool is_err(const Err *this);

// If an error condition has occurred, print the code and message to STDERR.
void err_alert(const Err *this);

#endif

