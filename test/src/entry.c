#include <entry.h>


b8 create_chat(chat* out_chat) {

    out_chat->config.start_pos_x = 100;
    out_chat->config.start_pos_y = 100;
    out_chat->config.start_width = 1280;
    out_chat->config.start_height = 720 ;
    out_chat->config.name = "Sea Chat Testbed";   
}