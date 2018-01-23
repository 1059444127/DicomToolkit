#pragma once
/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

using namespace System;

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {

//-------------------------------------------------------------------------------------------------

public ref class DicomFilesPusher abstract sealed
{

public: // Methods

    static void PushData(
        String^ path,
        String^ hostname,
        String^ ae_title,
        UInt16 port
    );

};

}

//-------------------------------------------------------------------------------------------------

