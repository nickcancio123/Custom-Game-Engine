#include "pch.h"
#include "ScopeWrapper.h"

#include "FloatParseHandler.h"
#include "IntParseHandler.h"
#include "Vec4ParseHandler.h"
#include "MatrixParseHandler.h"
#include "StringParseHandler.h"
#include "ScopeParseHandler.h"


using namespace Fiea::GameEngine;


RTTI_DEFINITIONS(ScopeWrapper);


void ScopeWrapper::Initialize()
{
	_CurrentScope = &_RootScope;
}

void ScopeWrapper::Cleanup()
{
}


Scope& ScopeWrapper::GetRoot()
{
	return _RootScope;
}

Scope* ScopeWrapper::GetCurrent()
{
	return _CurrentScope;
}

void ScopeWrapper::SetCurrent(Scope& NewCurrentScope)
{
	_CurrentScope = &NewCurrentScope;
}


void ScopeWrapper::AddScopeParseHandlers(ParseCoordinator& PC)
{
	PC.AddParseHandler(std::make_shared<IntParseHandler>());
	PC.AddParseHandler(std::make_shared<FloatParseHandler>());
	PC.AddParseHandler(std::make_shared<Vec4ParseHandler>());
	PC.AddParseHandler(std::make_shared<MatrixParseHandler>());
	PC.AddParseHandler(std::make_shared<StringParseHandler>());
	PC.AddParseHandler(std::make_shared<ScopeParseHandler>());
}
