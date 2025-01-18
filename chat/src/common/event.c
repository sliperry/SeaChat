#include "event.h"

#include "smemory.h"
#include "containers\darray.h"

typedef struct register_event{
    void* listner;
    PFN_on_event callback;
} register_event;

typedef struct event_code_entry {
    register_event* events;
} event_code_entry;

#define MAX_MESSAGE_CODES 16384

typedef struct event_system_state {
    event_code_entry registered[MAX_MESSAGE_CODES];
} event_system_state;

static b8 is_inititlized = FALSE;
static event_system_state state;

b8 event_initilize() {
    if (is_inititlized == TRUE) {
        return FALSE;
    }

    is_inititlized = FALSE;
    szero_memory(&state, sizeof(state));

    is_inititlized = TRUE;
    return TRUE;
}

void event_shutdown() {
    for(u16 i = 0; i < MAX_MESSAGE_CODES; i++) {
        if(state.registered[i].events != 0) {
            darray_destroy(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

b8 event_register(u16 code, void* listener, PFN_on_event on_event) {
    if (is_inititlized == FALSE) {
        return FALSE;
    }

    if(state.registered[code].events == 0) {
        state.registered[code].events = darray_create(register_event);
    }

    u64 register_count = darray_length(state.registered[code].events);
    for(u64 i = 0; i < register_count; i++) {
        if(state.registered[code].events[i].listner == listener) {
            KWARN("Listner already exists in state regiseter events %i!", i);
            return FALSE;
        }
    }

    register_event event;
    event.listner = listener;
    event.callback = on_event;
    darray_push(state.registered[code].events, event);

    return TRUE;
}

b8 event_unregister(u16 code, void* listener, PFN_on_event on_event) {
    if (is_inititlized == FALSE) {
        return FALSE;
    }

    if (state.registered[code].events == 0) {
        KWARN("Listner in state register events has not even been registerd yet");
    }

        u64 registered_count = darray_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; ++i) {
        register_event e = state.registered[code].events[i];
        if(e.listner == listener && e.callback == on_event) {
            // Found one, remove it
            register_event popped_event;
            darray_pop_at(state.registered[code].events, i, &popped_event);
            return TRUE;
        }
    }

    KINFO("There are no listner's as provided in parameter in state register events");
    return FALSE;
}

b8 event_fire(u16 code, void* sender, event_context context) {
    
    if(is_inititlized == FALSE) {
        return FALSE;
    }

    if(state.registered[code].events == 0) {
        return FALSE;
    }
    
    u64 registered_count = darray_length(state.registered[code].events);
    
    for(u64 i = 0; i < registered_count; ++i) {
        
        register_event e = state.registered[code].events[i];
        if(e.callback(code, sender, e.listner, context)) {
            // Message has been handled, do not send to other listeners.
            return TRUE;
        }
    }
    return FALSE;
}