// Copyright (C) 2024  KA Wright

// asset.c - Asset data

#include <stdint.h>

#include "asset.h"

extern char _binary_asset_about_txt_start[];
extern char _binary_asset_about_txt_end[];
extern char _binary_asset_help_txt_start[];
extern char _binary_asset_help_txt_end[];

void ld_asset_about(char *out) {
    uint16_t i = 0;
    for (char *j = _binary_asset_about_txt_start;
            j < _binary_asset_about_txt_end; j++) {
        out[i] = *j;
        i++;
    }
    out[i] = '\0';
}

void ld_asset_help(char *out) {
    uint16_t i = 0;
    for (char *j = _binary_asset_help_txt_start;
            j < _binary_asset_help_txt_end; j++) {
        out[i] = *j;
        i++;
    }
    out[i] = '\0';
}
