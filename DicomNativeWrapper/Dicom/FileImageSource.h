#pragma once
/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

Module: DicomNativeWrapper

**************************************************************************************************/

#include "Networking/Services/StoreSCU.h"

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {
    class FileImageSource : public Networking::StoreSCU::ImageSource
    {
    public: // Construction
        FileImageSource(const std::string& path);
        ~FileImageSource();

    public: //  Networking::StoreSCU::ImageSource
        std::vector<Networking::UI> SopClasses() const;
        int EstimatedRemaining();
        Networking::Object *Next00();

    private: // Data
        std::vector<std::string> m_files;
    };
}

//-------------------------------------------------------------------------------------------------
