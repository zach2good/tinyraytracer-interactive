// Many thanks to fmela:
// https://gist.github.com/fmela/591333/c64f4eb86037bb237862a8283df70cdfc25f01d3

#include <execinfo.h> // for backtrace
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <chrono>

#include <stack>

#define NO_INSTRUMENT __attribute__((no_instrument_function))
using time_point = std::chrono::high_resolution_clock::time_point;

NO_INSTRUMENT void getSymbolFromAddress(void *addr, char *symbol) {
    Dl_info info;
    if (dladdr(addr, &info) && info.dli_sname)
    {
        char* demangled;
        int status;
        demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
        if(demangled)
        {
            strcpy(symbol, demangled);
        }
        else
        {
            strcpy(symbol, info.dli_sname);
        }
        free(demangled);
    }
}

struct FunctionCall
{
    uint64_t id;
    time_point tp;
    const char* name;
    size_t depth;
};
std::stack<FunctionCall> stk;

extern "C"
{
    void __cyg_profile_func_enter(void* this_fn, void* call_site)
    {
        char functionName[1024] = { 0 };
        getSymbolFromAddress(this_fn, functionName);
        auto id = (uint64_t)this_fn;
        char* pch = strtok(functionName, "(");
        stk.push(FunctionCall{ id, std::chrono::high_resolution_clock::now(), pch, stk.size()});
    }

    void __cyg_profile_func_exit(void* this_fn, void* call_site)
    {
        char functionName[1024] = { 0 };
        getSymbolFromAddress(this_fn, functionName);
        auto id = (uint64_t)this_fn;
        char* pch = strtok(functionName, "(");

        auto functionStart = stk.top();
        stk.pop();

        auto now = std::chrono::high_resolution_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(now - functionStart.tp).count();

        std::string str;
        for (int i = 0; i <= functionStart.depth; ++i) {
            str += '>';
        }

        printf("[%5lluus]%s %s\n", us, str.c_str(), functionStart.name);
    }
}
