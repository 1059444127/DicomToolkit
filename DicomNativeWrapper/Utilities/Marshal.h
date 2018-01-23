#pragma once
/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

using namespace System;

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {

//-------------------------------------------------------------------------------------

private  ref class Marshal abstract sealed {

public: // Methods

    static std::string ToStdString(String^ value);
    static std::wstring ToStdWstring(String^ value);
    static String^ ToSystemString(const std::string& value);
    static String^ ToSystemString(const std::wstring& value);

private: // Methods

    template<class T>
    inline static T ToUnmanaged(String^ value);

    template<class T>
    inline static String^ ToManaged(T value);

};

}

//-------------------------------------------------------------------------------------------------
