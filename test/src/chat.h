#pragma once

#include <common/defines.h>
#include <chat_type.h>

typedef struct chat_state {
    f32 delta_time;

}chat_state;

b8 chat_initialize(chat* chat_inst);

b8 chat_update(chat* chat_inst, f32 delta);

b8 chat_render(chat* chat_inst, f32 delta);

void chat_on_resize(chat* chat_inst, u32 width, u32 height);