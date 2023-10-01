#pragma once

#include "Base.h"

namespace Core
{
    template <typename T, typename... Args>
    class CE_API Scope
    {
    private:
        T *inst;

    public:
        Scope(Args... args) { inst = new T(args...); };
        ~Scope() { delete inst; };

        T *Raw() { return inst; };
    };
}