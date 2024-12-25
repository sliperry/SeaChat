#pragma once

#include "defines.h"
#include "chat_type.h"

typedef struct application_config {
    i16 start_pos_x;

    i16 start_pos_y;

    i16 start_width;

    i16 start_height;

    char* name;
} application_config;

KAPI b8 application_create(chat* chat_inst);

KAPI b8 application_run();