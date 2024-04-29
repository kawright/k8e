// Copyright (C) 2024  KA Wright

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argv.h"
#include "err.h"

void init_argv(Argv *this) {
    this->about             = false;
    for (int i=0; i<16; i++) {
        this->brkpts[i] = 0;
    }
    this->brkpts_len        = 0;
    this->bg                = 0x000000;
    this->fg                = 0xffffff;
    this->clk_freq          = 500;
    this->debug             = false;
    this->help              = false;
    this->svst              = NULL;
    this->mute              = false;
    this->paused            = false;
    this->pitch             = 880;
    this->px_sz             = 8;
    this->fname             = NULL;
}

void parse_argv(Argv *this, uint16_t argc, char *argv[], Err *err) {
    int curr_opt;
    while ((curr_opt = getopt(argc, argv, ARGV_OPTSTR)) != -1) {
        switch (curr_opt) {
            
            case 'a':
            this->about = true;
            break;

            case 'b':
            if (this->brkpts_len >= ARGV_MAX_BRKPTS - 1) {
                err->code = ERR_ARGV;
                strcpy(err->msg, "Cannot set more than 16 breakpoints");
                return;
            }
            this->brkpts[this->brkpts_len] = 
                (uint16_t) strtol(optarg, NULL, 16);
            this->brkpts_len++;
            break;

            case 'B':
            this->bg = (uint32_t) strtol(optarg, NULL, 16);
            break;

            case 'c':
            this->clk_freq = (uint32_t) strtol(optarg, NULL, 16);
            break;

            case 'F':
            this->fg = (uint32_t) strtol(optarg, NULL, 16);
            break;

            case 'h':
            this->help = true;
            break;

            case 'l':
            this->svst = optarg;
            break;

            case 'm':
            this->mute = true;
            break;

            case 'd':
            this->debug = true;
            break;

            case 'p':
            this->paused = true;
            break;

            case 'P':
            this->px_sz = (uint8_t) strtol(optarg, NULL, 10);
            break;

            case 't':
            this->pitch = (uint16_t) strtol(optarg, NULL, 10);
            break;

            case '?':
            err->code = ERR_ARGV;
            if (optopt == 'b' || optopt == 'B' || optopt == 'c' || 
                    optopt == 'F' || optopt == 'l' || optopt == 'P' ||
                    optopt == 't') {
                snprintf(err->msg, MAX_ERR_MSG_LEN, "Option -%c requires "
                    "argument", optopt);
                return;
            }
            snprintf(err->msg, MAX_ERR_MSG_LEN, "Unknown option -%c", optopt);
            return;

            default:
            return;
        }
    }
    if (optind != argc - 1 && !(this->about || this->help)) {
        err->code = ERR_ARGV;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "1 positional argument expected, "
            "but %d given", argc - optind);
        return;
    } 
    this->fname = argv[optind];
}
