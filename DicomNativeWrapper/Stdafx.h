#pragma once
/**************************************************************************************************

Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

// Disable selected warnings
#pragma warning(disable: 4339) // 'type' : use of undefined type detected in CLR meta-data - use of this type may lead to a runtime exception
#pragma warning(disable: 4514) // 'function' : unreferenced inline function has been removed
#pragma warning(disable: 4710) // 'function' : function not inlined
#pragma warning(disable: 4820) // 'bytes' bytes padding added after construct 'member_name'

// Needed to exclude VCF or other external files from code analysis.
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )

// STL includes
#pragma warning(push)
// Disable warnings caused by STL code
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning(disable: 4711) // function 'function' selected for inline expansion
#pragma warning(disable: 4986) // 'symbol': exception specification does not match previous declaration
#include <deque>
#include <memory>
#pragma warning(pop)

// Base includes
// VoxarDicom disables a pile of warnings which we want to keep enabled
#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4619) // #pragma warning: there is no warning number 'number'
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning(disable: 4917) // 'declarator': a GUID can only be associated with a class, interface, or namespace
#include "Networking/Common/PreCompiledDeclarations.h"
#pragma warning(pop)

#pragma warning(pop)