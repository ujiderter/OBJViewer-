#ifndef OBJVIEWER__IEVENTHANDLER_H
#define OBJVIEWER__IEVENTHANDLER_H

#include "Event.h"

class IEventHandler {
public:
    virtual ~IEventHandler() = default;

    // Метод для обработки событий
    virtual void handleEvent(const Event& event) = 0;
};

#endif //OBJVIEWER__IEVENTHANDLER_H
