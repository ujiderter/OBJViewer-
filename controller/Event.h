#ifndef OBJVIEWER__EVENT_H
#define OBJVIEWER__EVENT_H

#include <variant>
#include <string>
#include "EventType.h"

struct Event {
    EventType type; // Тип события
    std::variant<std::string, int> payload;

    explicit Event(EventType type, const std::variant<std::string, int>& payload = {})
            : type(type), payload(payload) {}
};

#endif //OBJVIEWER__EVENT_H
