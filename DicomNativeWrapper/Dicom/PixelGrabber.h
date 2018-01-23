#pragma once
/**************************************************************************************************

    Copyright (C) 2015 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

using namespace System;

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper  {

    public ref class PixelGrabber abstract sealed
    {
    public:

        void static GrabPixels(
            String^ filename_in, 
            String^ filename_out
        );

    private:
        
        void static SaveFrame(
            BYTE* image_ptr, 
            INT rows, 
            INT columns, 
            const WCHAR* filename
        );

        CLSID static GetEncoderClsid(
            const WCHAR* format
        );
    };
}

//-------------------------------------------------------------------------------------------------
