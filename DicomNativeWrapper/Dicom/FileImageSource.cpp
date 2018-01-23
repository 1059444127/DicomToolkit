#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

Module: DicomNativeWrapper

**************************************************************************************************/

#include "FileImageSource.h"

#include "Networking/Common/FileInputBuffer.h"
#include "Networking/Message/Dictionary.h"
#include "Networking/Message/Object.h"
#include "Networking/Message/UI.h"
#include "PvBase/FileSystemUtilities.h"

namespace DicomNativeWrapper {

    //-------------------------------------------------------------------------------------------------

    FileImageSource::FileImageSource(
        const std::string& path
    ) {
        if (PvBase::FileSystemUtilities::DirectoryExists(path)) {
            m_files = PvBase::FileSystemUtilities::FullFilePathsMatchingPattern(path, "*.*");
        } else if (PvBase::FileSystemUtilities::FileExists(path)) {
            m_files = std::vector<std::string>(1, path); 
        } else {
            throw PvBase::Log(PvBase::Log::ARGUMENT_OUT_OF_RANGE_EXCEPTION) << "Path not found: " << path;
        }
    }

    //-------------------------------------------------------------------------------------------------

    FileImageSource::~FileImageSource() {

    }

    //-------------------------------------------------------------------------------------------------

    std::vector<Networking::UI> FileImageSource::SopClasses() const {
        std::vector<Networking::UI> sop_classes;

        for (std::vector<std::string>::const_iterator i = m_files.begin(); m_files.end() != i; ++i) {
            Networking::FileInputBuffer buff(*i);
            Networking::Object obj(buff, Networking::Object::OmitPixelData);
            const Networking::UI& sop_class = obj.GetUI(Networking::Dictionary::SOPClassUID);

            if (sop_classes.end() == std::find(sop_classes.begin(), sop_classes.end(), sop_class)) {
                sop_classes.push_back(sop_class);
            }
        }

        return sop_classes;
    }

    //-------------------------------------------------------------------------------------------------

    int FileImageSource::EstimatedRemaining() {
        return static_cast<int>(m_files.size());
    }

    //-------------------------------------------------------------------------------------------------

    Networking::Object* FileImageSource::Next00() {
        if (m_files.empty()) {
            return NULL;

        } else {
            Networking::FileInputBuffer buff(m_files.back());
            m_files.pop_back();

            return new Networking::Object(buff, Networking::Object::LoadAllData);
        }
    }

    //-------------------------------------------------------------------------------------------------
}