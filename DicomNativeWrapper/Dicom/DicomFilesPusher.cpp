#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/
#include "DicomFilesPusher.h"

#include "DicomPusher.h"
#include "../Utilities/Marshal.h"

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4623) // 'derived class': default constructor could not be generated because a base class default constructor is inaccessible
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "Networking/Common/FileInputBuffer.h"
#include "Networking/Message/Object.h"
#include "PvBase/FileSystemUtilities.h"
#pragma warning(pop)

using namespace DicomNativeWrapper;

//-------------------------------------------------------------------------------------------------

void DicomFilesPusher::PushData(
    String^ path_in,
    String^ hostname,
    String^ ae_title,
    UInt16 port
) {
    try {
        const std::string path = Marshal::ToStdString(path_in);
        auto files = PvBase::FileSystemUtilities::FullFilePathsMatchingPattern(path, "*.*");

        DicomPusher::OwnedObjects owned_objects(new std::deque<std::unique_ptr<Networking::Object>>(files.size()));
        for (size_t i = 0; files.size() > i; ++i) {
            Networking::FileInputBuffer buff(files[i]);
            owned_objects->at(i).reset(new Networking::Object(buff, Networking::Object::LoadAllData));
        }

        // Takes ownership of owned_objects
        DicomPusher::PushData(owned_objects, hostname, ae_title, port);

    }
    catch (const PvBase::Log& e) {
        throw gcnew System::InvalidOperationException(
            "PvBase::Log caught: " + Marshal::ToSystemString(e.ConcatenatedContents())
        );
    } catch (...) {
        throw gcnew System::InvalidOperationException("Exception caught");
    }
}

//-------------------------------------------------------------------------------------------------

