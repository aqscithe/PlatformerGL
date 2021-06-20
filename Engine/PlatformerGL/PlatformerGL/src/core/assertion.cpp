#include <iostream>
#include <assert.h>

#include "core/debug/assertion.hpp"

using namespace Core::Debug;

bool Assertion::enabled = true;

void Assertion::assertTest(bool expression)
{
	if (enabled)
		assert(expression);
}