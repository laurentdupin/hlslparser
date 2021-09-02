#ifndef ENGINE_H
#define ENGINE_H

#include <stdarg.h> // va_list, vsnprintf
#include <vector>
#include <stdexcept>

#ifndef NULL
#define NULL    0
#endif

#ifndef va_copy
#define va_copy(a, b) (a) = (b)
#endif

// Engine/Assert.h

#define ASSERT(...) while(!(__VA_ARGS__)) throw std::runtime_error(#__VA_ARGS__ " condition failed");
#define assert(...) while(!(__VA_ARGS__)) throw std::runtime_error(#__VA_ARGS__ " condition failed");

namespace M4 
{

// Engine/String.h

int String_Printf(char * buffer, int size, const char * format, ...);
int String_PrintfArgList(char * buffer, int size, const char * format, va_list args);
int String_FormatFloat(char * buffer, int size, float value);
bool String_Equal(const char * a, const char * b);
bool String_EqualNoCase(const char * a, const char * b);
double String_ToDouble(const char * str, char ** end);
int String_ToInteger(const char * str, char ** end);


// Engine/Log.h

void Log_Error(const char * format, ...);
void Log_ErrorArgList(const char * format, va_list args);


// Engine/StringPool.h

// @@ Implement this with a hash table!
struct StringPool {
    StringPool();
    ~StringPool();

    const char * AddString(const char * string);
    const char * AddStringFormat(const char * fmt, ...);
    const char * AddStringFormatList(const char * fmt, va_list args);
    bool GetContainsString(const char * string) const;

    std::vector<const char *> stringArray;
};


} // M4 namespace

#endif // ENGINE_H
