#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/
#include "DicomPusher.h"

#include "MemoryImageSource.h"
#include "../Utilities/Marshal.h"

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "Networking/Common/SocketConnectionFactory.h"
#include "Networking/Common/Thread.h"
#include "Networking/Common/FileOutputBuffer.h"
#include "Networking/Message/AE.h"
#include "Networking/Message/UI.h"
#include "Networking/Message/Object.h"
#include "Networking/Services/Service.h"
#pragma warning(pop)

using namespace DicomNativeWrapper;

//-------------------------------------------------------------------------------------------------

void DicomPusher::PushData(
    OwnedObjects& objects,
    String^ hostname_in,
    String^ ae_title_in,
    UInt16 port
) {
    const std::string hostname = Marshal::ToStdString(hostname_in);
    const std::string ae_title = Marshal::ToStdString(ae_title_in);

    const Networking::SocketConnectionFactory connection_factory(hostname, port, 30);
    const Networking::Service service(
        &connection_factory, 
        Networking::UI("2.4.6.0.1"), 
        Networking::AE("NativeWrapper"), 
        Networking::AE(ae_title)
    );

    // Takes ownership of objects
    Networking::StoreSCU scu(service, new MemoryImageSource(objects));

    Networking::Thread thread(&scu);
    thread.Start();
    thread.WaitUntilComplete();
}

//-------------------------------------------------------------------------------------------------

void DicomPusher::SaveData(
    OwnedObjects& objects,
    String^ folder_path_in
) {
    const std::wstring folder_path = Marshal::ToStdWstring(folder_path_in);

    const auto filename = folder_path + L"\\dcmfile";

    Networking::UI dicom_implementation_class_uid("1.2.392.200036.9116.31.100.1");

    int i = 0;

    std::deque<std::unique_ptr<Networking::Object>>::iterator iter;
    for (iter = objects->begin(); iter != objects->end(); ++iter) {

        std::unique_ptr<Networking::Object>& object_ptr = *iter;
        Networking::Object* object = object_ptr.get();
        object->AddImageMetaHeader(
            dicom_implementation_class_uid,
            Networking::UI::LittleEndianImplicitVR
        );
        object->RepairMetaHeader();        
        
        Networking::FileOutputBuffer buffer(filename + std::to_wstring(i++) + L".dcm");
        object->Save(buffer);
        buffer.FlushToDisk();
    }
}

//-------------------------------------------------------------------------------------------------

void DicomPusher::SaveFluoroData(
    Networking::Object& object,
    String^ folder_path_in,
    int frame_index
) {
    const std::wstring folder_path = Marshal::ToStdWstring(folder_path_in);

    const auto filename = folder_path + L"\\Fluoro_";

    Networking::UI dicom_implementation_class_uid("1.2.392.200036.9116.31.100.1");

    object.AddImageMetaHeader(dicom_implementation_class_uid, Networking::UI::LittleEndianImplicitVR);
    object.RepairMetaHeader();

    Networking::FileOutputBuffer buffer(filename + L"_" + std::to_wstring(frame_index++) + L".dcm");
    object.Save(buffer);
    buffer.FlushToDisk();
}
//-------------------------------------------------------------------------------------------------

