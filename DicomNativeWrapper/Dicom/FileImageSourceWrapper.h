/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

**************************************************************************************************/
// DicomNativeWrapper.h

#pragma once

#include "Networking/Common/SocketConnectionFactory.h"
#include "Networking/Common/Thread.h"
#include "Networking/Message/AE.h"
#include "Networking/Message/UI.h"
#include "Networking/Services/Service.h"
#include "Networking/Services/StoreSCU.h"
#include "PvBase/Convert.h"

#include "FileImageSource.h"

using namespace System;

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {

	public ref class FileImageSourceWrapper
	{
    public:
        FileImageSourceWrapper(FileImageSource* src)  {
            m_source = src;
        }

        ~FileImageSourceWrapper() {
            if (m_source) {
                delete m_source;
                m_source = nullptr;
            }
        }

        !FileImageSourceWrapper() {
            if (m_source) {
                delete m_source;
                m_source = nullptr;
            }
        }

        FileImageSource* GetSource() {
            return m_source;
        }

    private:

        FileImageSource* m_source;
	};

}

//-------------------------------------------------------------------------------------------------
