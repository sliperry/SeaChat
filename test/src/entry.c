#include "chat.h"

#include <entry.h>
#include <platform/platform.h>

b8 create_chat(chat* out_chat) {

    out_chat->app_config.start_pos_x = 100;
    out_chat->app_config.start_pos_y = 100;
    out_chat->app_config.start_width = 1280;
    out_chat->app_config.start_height = 720 ;
    out_chat->app_config.name = "Sea Chat Testbed";   
    out_chat->update = chat_update;
    out_chat->render = chat_render;
    out_chat->initialize = chat_initialize;
    out_chat->on_resize =  chat_on_resize;

    out_chat->state = platform_allocate(sizeof(chat_state), FALSE);

    return TRUE;
}