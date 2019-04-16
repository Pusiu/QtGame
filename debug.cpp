#include "debug.h"

Debug::Debug()
{

}

void Debug::Log(QString s)
{
    std::cout << s.constData();
}
