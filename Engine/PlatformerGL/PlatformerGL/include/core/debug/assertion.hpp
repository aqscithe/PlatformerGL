#include <string>

#include "core/core.hpp"

class Core::Debug::Assertion
{
    public:
        Assertion() = delete;
        Assertion(const Assertion& assertion) = delete;

        void            operator=(const Assertion& assertion) = delete;

        static void     assertTest(bool expression);

        static bool     enabled;
    private:
};
