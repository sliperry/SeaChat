#include "input.h"
#include "event.h"
#include "smemory.h"
#include "logger.h"

typedef struct keyboard_state {
    b8 keys[256];
} keyboard_state;

typedef struct mouse_state {
    i16 x;
    i16 y;
    u8 buttons[BUTTON_MAX_BUTTONS];
} mouse_state;

typedef struct input_state {
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state mouse_current;
    mouse_state mouse_prevous;
} input_state;

static b8 is_initilized = FALSE;
static input_state state = {};

void input_initilize() {
    szero_memory(&state, sizeof(input_state));
    is_initilized = TRUE;
    KINFO("Input state has been Initilized");
}

void input_shutdown() {
    is_initilized = FALSE;
}

void input_update(f64 delta_time) {
    if(!is_initilized) {
        return;
    }

    scopy_memory(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    scopy_memory(&state.mouse_prevous, &state.mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, b8 pressed) {
    // Track modifier keys
    switch (key) {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            state.keyboard_current.keys[KEY_SHIFT] = pressed;
            break;
        case KEY_LCONTROL:
        case KEY_RCONTROL:
            state.keyboard_current.keys[KEY_CONTROL] = pressed;
            break;
        case KEY_LALT:
        case KEY_AL:
            state.keyboard_current.keys[KEY_ALT] = pressed;
            break;
        default:
            break;
    }


    if(state.keyboard_current.keys[key] != pressed) {
        state.keyboard_current.keys[key] = pressed;

        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}

void input_process_button(buttons button, b8 pressed) {
    if(state.mouse_current.buttons[button] != pressed) {
        state.mouse_current.buttons[button] = pressed;

        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}

void input_process_mouse_move(i16 x, i16 y) {
    if(state.mouse_current.x != x || state.mouse_current.y != y) {
        KDEBUG("Mouse pos: %d, %d", x, y);
        
        state.mouse_current.x = x;
        state.mouse_current.y = y;

        event_context context;
        context.data.i64[0] = x;
        context.data.i64[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(i8 z_delta) {
    event_context context;
    context.data.u8[0] = z_delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}

b8 input_is_key_down(keys key) {
    if(!is_initilized) {
        return FALSE;
    }
    return state.keyboard_current.keys[key] == TRUE;
}

b8 input_is_key_up(keys key) {
    if(!is_initilized) {
        return TRUE;
    }
    return state.keyboard_current.keys[key] == FALSE;
}

b8 input_was_key_down(keys key) {
    if(!is_initilized) {
        return FALSE;
    }
    return state.keyboard_previous.keys[key] == TRUE;
}

b8 input_was_key_up(keys key) {
    if(!is_initilized) {
        return TRUE;
    }
    return state.keyboard_previous.keys[key] == FALSE;
}

b8 input_is_button_down(buttons button) {
    if (!is_initilized) {
        return FALSE;
    }
    return state.mouse_current.buttons[button] == TRUE;
}

b8 input_is_button_up(buttons button) {
    if(!is_initilized) {
        return TRUE;
    }
    return state.mouse_current.buttons[button] == FALSE;
}

b8 input_was_button_down(buttons button) {
    if(!is_initilized) {
        return FALSE;
    }
    return state.mouse_prevous.buttons[button] == TRUE;
}

b8 input_was_button_up(buttons button) {
    if(!is_initilized) {
        return TRUE;
    }
    return state.mouse_prevous.buttons[button] == FALSE;
}

void input_get_mouse_position(i32* x, i32* y) {
    if(!is_initilized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}

void input_get_previous_mouse_position(i32* x, i32* y) {
    if(!is_initilized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_prevous.x;
    *y = state.mouse_prevous.y;
}
