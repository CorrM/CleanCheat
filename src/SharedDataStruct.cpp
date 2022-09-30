#include "SharedDataStruct.h"

void SharedDataStruct::Tick(void* data)
{
    const int dataValue = *static_cast<int*>(data);
}
