#pragma once

#include "Factory.h"
#include "Scope.h"
#include "AttributedBar.h"
#include "AttributedChild.h"

namespace Fiea::GameEngine::Test
{
	MAKE_FACTORY(Scope, AttributedChild)

	MAKE_FACTORY(Scope, AttributedBar)
}