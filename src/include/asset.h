// Copyright (C) 2024  KA Wright

// asset.h - Asset data.

#ifndef __ASSET_H__
#define __ASSET_H__

#define MAX_ASSET_ABOUT_SZ      255
#define MAX_ASSET_HELP_SZ       1027

// Load the contents of the about file into the out param.
void ld_asset_about(char *out);

// Load the contents of the help file into the out param.
void ld_asset_help(char *out);

#endif
