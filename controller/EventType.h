#ifndef OBJVIEWER__EVENTTYPE_H
#define OBJVIEWER__EVENTTYPE_H

enum class EventType {
    FileOpen, // Событие открытия файла
    WindowClose, // Событие закрытия окна
    CustomEvent // Пользовательское событие
};

#endif //OBJVIEWER__EVENTTYPE_H
