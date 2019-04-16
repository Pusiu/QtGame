#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <QString>

class Debug
{
public:
    Debug();

    static void Log(QString s);
};

#endif // DEBUG_H
