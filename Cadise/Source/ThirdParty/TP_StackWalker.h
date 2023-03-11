#pragma once

#include "Foundation/OS.h"

#if defined(CADISE_OS_WINDOWS)
    #include "ThirdParty/StackWalker/StackWalker.h"

    #include <iostream>

    namespace cadise
    {

    // self-defined class for custom output-mechanism
    class WindowsStackWalker : public StackWalker
    {
    public:
        WindowsStackWalker() : 
            StackWalker()
        {}
    protected:
        void OnOutput(LPCSTR szText) override
        {
            std::cerr << szText; 
            StackWalker::OnOutput(szText);
        }
    };

    } // namespace cadise

#endif