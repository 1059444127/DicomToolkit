#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/
#include "../Utilities/Marshal.h"

#pragma warning(push)
// Disable warnings caused by MSCLR code
#pragma warning(disable: 4365) // 'action': conversion from 'type_1' to 'type_2', signed/unsigned mismatch
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include <msclr/marshal_cppstd.h>
#pragma warning(pop)

using namespace DicomNativeWrapper;

//-------------------------------------------------------------------------------------------------

std::string Marshal::ToStdString(
    String^ value
) {
    return ToUnmanaged<std::string>(value);
}

//-------------------------------------------------------------------------------------------------

std::wstring Marshal::ToStdWstring(
    String^ value
) {
    return ToUnmanaged<std::wstring>(value);
}

//-------------------------------------------------------------------------------------------------

String^ Marshal::ToSystemString(
    const std::string& value
) {
    return ToManaged(value);
}

//-------------------------------------------------------------------------------------------------

String^ Marshal::ToSystemString(
    const std::wstring& value
) {
    return ToManaged(value);
}

//-------------------------------------------------------------------------------------------------

template<class T>
inline T Marshal::ToUnmanaged(String^ value) {
    return msclr::interop::marshal_as<T>(value);
}

//-------------------------------------------------------------------------------------------------

template<class T>
inline String^ Marshal::ToManaged(T value) {
    return gcnew String(value.c_str());
}

//-------------------------------------------------------------------------------------------------

