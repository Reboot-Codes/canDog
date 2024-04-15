#pragma once

typedef enum {
    CandogCustomEventStartscreenUp,
    CandogCustomEventStartscreenDown,
    CandogCustomEventStartscreenLeft,
    CandogCustomEventStartscreenRight,
    CandogCustomEventStartscreenOk,
    CandogCustomEventStartscreenBack,
    CandogCustomEventScene1Up,
    CandogCustomEventScene1Down,
    CandogCustomEventScene1Left,
    CandogCustomEventScene1Right,
    CandogCustomEventScene1Ok,
    CandogCustomEventScene1Back,
    CandogCustomEventScene2Up,
    CandogCustomEventScene2Down,
    CandogCustomEventScene2Left,
    CandogCustomEventScene2Right,
    CandogCustomEventScene2Ok,
    CandogCustomEventScene2Back,
} CandogCustomEvent;

enum CandogCustomEventType {
    // Reserve first 100 events for button types and indexes, starting from 0
    CandogCustomEventMenuVoid,
    CandogCustomEventMenuSelected,
};

#pragma pack(push, 1)
typedef union {
    uint32_t packed_value;
    struct {
        uint16_t type;
        int16_t value;
    } content;
} CandogCustomEventMenu;
#pragma pack(pop)

static inline uint32_t candog_custom_menu_event_pack(uint16_t type, int16_t value) {
    CandogCustomEventMenu event = {.content = {.type = type, .value = value}};
    return event.packed_value;
}
static inline void
    candog_custom_menu_event_unpack(uint32_t packed_value, uint16_t* type, int16_t* value) {
    CandogCustomEventMenu event = {.packed_value = packed_value};
    if(type) *type = event.content.type;
    if(value) *value = event.content.value;
}

static inline uint16_t candog_custom_menu_event_get_type(uint32_t packed_value) {
    uint16_t type;
    candog_custom_menu_event_unpack(packed_value, &type, NULL);
    return type;
}

static inline int16_t candog_custom_menu_event_get_value(uint32_t packed_value) {
    int16_t value;
    candog_custom_menu_event_unpack(packed_value, NULL, &value);
    return value;
}