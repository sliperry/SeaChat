#include "common/application.h"
#include "common/logger.h"
#include "chat_type.h"

extern b8 create_chat(chat* out_chat);

int main(void) {

    chat chat_inst;

    if(!create_chat(&chat_inst)) {
        KFATAL("Could not create chat");
        return -1;
    }

    if(!chat_inst.initialize || !chat_inst.on_resize || !chat_inst.render || !chat_inst.update) {
        KFATAL("The chat's function pointers must be assigned");
        return -2;
    }

    if(!application_create(&chat_inst)) {
        KINFO("failed to application create");
        return 1;
    }

    if(!application_run()) {
        KINFO("Failed to succesfull shutdown application");
        return 2;
    } 

    return 0;
}