#pragma once

#include "common/application.h"

typedef struct chat {
    application_config app_config;

    b8 (*Initialize)(struct chat* chat_inst);

    b8 (*update)(struct chat* chat_inst, f32 delta_time);

    b8 (*render)(struct chat* chat_inst, f32 delta_time);

    void (*on_resize)(struct chat* chat_inst, u32 width, u32 height);

    void* state;
} char;