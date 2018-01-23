#pragma once
/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

//-------------------------------------------------------------------------------------------------

using namespace System;

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {

//-------------------------------------------------------------------------------------------------

private ref class DicomPusher abstract sealed
{
public: // Types

    typedef std::unique_ptr<std::deque<std::unique_ptr<Networking::Object>>> OwnedObjects;

public: // Methods

    static void PushData(
        // Pass a reference to work around Visual Studio defect 
        // 777768 "Linker error passing unique_ptr to function with C++/CLI" 
        // https://connect.microsoft.com/VisualStudio/feedback/details/777768/linker-error-passing-unique-ptr-to-function-with-c-cli
        OwnedObjects& objects,
        String^ hostname,
        String^ ae_title,
        UInt16 port
    );
    
    static void SaveData(
        // Pass a reference to work around Visual Studio defect 
        // 777768 "Linker error passing unique_ptr to function with C++/CLI" 
        // https://connect.microsoft.com/VisualStudio/feedback/details/777768/linker-error-passing-unique-ptr-to-function-with-c-cli
        OwnedObjects& objects,
        String^ folder_path
    );

    static void SaveFluoroData(
        Networking::Object& object,
        String^ folder_path_in,
        int frame_index
    );
};

}

//-------------------------------------------------------------------------------------------------

