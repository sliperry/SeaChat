#include "chat.h"

#include <common/logger.h>

b8 chat_initialize(chat* chat_inst) {
    KDEBUG("chat initilization is called");
    return TRUE;
}

b8 chat_update(chat* chat_inst, f32 delta) {
    return TRUE;
}

b8 chat_render(chat* chat_inst, f32 delta) {
    return TRUE;
}

void chat_on_resize(chat* chat_inst, u32 width, u32 height) {
}