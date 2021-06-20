#pragma once

#include <string>

namespace Core
{
    struct rdrVertex
    {
        float x, y, z;    // Pos
        float r, g, b, a; // Color
        float nx, ny, nz; // Normal
        float u, v;       // Texture coordinates
    };

    namespace Debug
    {
        enum class LogType
        {
            DEBUG = 0,
            INFO = 1,
            WARNING = 2,
            ERROR = 3,
            FATAL = 4
        };

        enum class LogFile
        {
            MESSAGE = 0,
            ERROR = 1
        };

        enum class LogOptions
        {
            MESSAGE,
            ERROR,
            HYBRID
        };

        class Assertion;
        class Log;
    }
}
