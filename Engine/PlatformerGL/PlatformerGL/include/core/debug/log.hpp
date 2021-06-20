#pragma once

#include <fstream>

#include "core/core.hpp"

namespace Core
{
    namespace Debug
    {
        class Log
        {
        public:
            Log() = delete;
            Log(const Log& log) = delete;
            void operator=(const Log& log) = delete;

            static void             print(const std::string& statement, const LogType& logType);
            static void             configureLogFiles();
            static void             redirectLogs(const LogOptions& outputType);
            static void             endRedirect(const LogOptions& outputType);
            
            static bool             enabled;
            static int              logDir;

        private:
            static void             printTitle(const LogFile& logfile);
            static void             logHeader(const LogFile& logFile);
            static void             logDivider(const LogFile& logFile);
            static void             printDateTime(const LogFile& logFile);

            static std::ofstream    outMessage;
            static std::ofstream    outError;
            static std::streambuf*  coutbuf;
            static std::streambuf*  cerrbuf; 
        };
    }
}
