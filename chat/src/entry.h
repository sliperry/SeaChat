#include "common/application.h"
#include "common/logger.h"
#include "chat_type.h"
#include "common/smemory.h"

extern b8 create_chat(chat* out_chat);

int main(void) {

    initilize_memory();

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

    shutdown_memory();

    return 0;
}