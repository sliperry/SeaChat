#pragma once

#include "defines.h"

struct chat;

typedef struct application_config {
    i16 start_pos_x;

    i16 start_pos_y;

    i16 start_width;

    i16 start_height;

    char* name;
} application_config;

KAPI b8 application_create(struct chat* chat_inst);

KAPI b8 application_run();