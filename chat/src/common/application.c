#include "application.h"
#include "platform/platform.h"

#include "logger.h"
#include "smemory.h"
#include "chat_type.h"
#include "event.h"
#include "input.h"

typedef struct application_state {
    chat* chat_inst;

    b8 is_running;

    b8 is_suspended;

    platform_state platform;

    i16 width;

    i16 height;

    f64 last_time;
} application_state;

static b8 initilzied = FALSE;
static application_state app_state;

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context);

b8 application_create(chat* chat_inst) {
    if (initilzied) {
        KERROR("application_create has been called more then once");
        return FALSE;
    }

    app_state.chat_inst = chat_inst;

    initialize_logging();
    input_initilize();

    KFATAL("A test message: %f", 3.14f);
    KERROR("A test message: %f", 3.14f);
    KWARN("A test message: %f", 3.14f);
    KINFO("A test message: %f", 3.14f);
    KDEBUG("A test message: %f", 3.14f);
    KTRACE("A test message: %f", 3.14f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if(!event_initilize()) {
        KERROR("Event system has failed initilizations. Application can not continue.");
        return FALSE;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    if(!platform_startup(
        &app_state.platform, 
        chat_inst->app_config.name, 
        chat_inst->app_config.start_pos_x, 
        chat_inst->app_config.start_pos_y, 
        chat_inst->app_config.start_width, 
        chat_inst->app_config.start_height
        )) {
        return FALSE;
    }

    if(!app_state.chat_inst->initialize(app_state.chat_inst)) {
        KFATAL("Chat failed to Initilize");
        return FALSE;
    }

    app_state.chat_inst->on_resize(app_state.chat_inst, app_state.width, app_state.height);

    initilzied = TRUE;
    
    return TRUE;
}

b8 application_run() {
    KINFO(get_memory_usage_str());

    while(app_state.is_running) {
        if(!platform_pump_messages(&app_state.platform)) {
            app_state.is_running = FALSE;
        }

        if(!app_state.is_suspended) {
            if(!app_state.chat_inst->update(app_state.chat_inst, (f32)0)) {
                KFATAL("chat update failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            if(!app_state.chat_inst->render(app_state.chat_inst, (f32)0)) {
                KFATAL("Chat rendering failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }

            input_update(0);
        }
    }

    app_state.is_running = FALSE;

    event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
    event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

    event_shutdown();
    input_shutdown();

    platform_shutdown(&app_state.platform);

    return TRUE;
}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            KINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.");
            app_state.is_running = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}

b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context) {
    if(code == EVENT_CODE_KEY_PRESSED) {
        u16 key_code = context.data.u16[0];
        if(key_code == KEY_ESCAPE) {
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);

            return TRUE;
        } else if (key_code == KEY_A) {
            KDEBUG("Explicitly - A key pressed!");
        } else {
            KDEBUG("'%c' key pressed in window", key_code);
        }
    } else if(code == EVENT_CODE_KEY_RELEASED) {
        u16 key_code = context.data.u16[0];
        if (key_code == KEY_B) {
            KDEBUG("Explicitly - B key released");
        } else {
            KDEBUG("'%c' key released in window", key_code);
        }
        
    }
    return FALSE;
}