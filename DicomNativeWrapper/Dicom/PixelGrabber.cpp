#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/
#include "PixelGrabber.h"
#include "../Utilities/Marshal.h"

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "Networking/Common/FileInputBuffer.h"
#include "Networking/Message/Dictionary.h"
#include "Networking/Message/Object.h"
#include "Networking/Message/UI.h"
#include "Networking/Message/UN.h"

#pragma warning(push)
// Disable warnings caused by GDI code
#pragma warning(disable: 4061) // enumerator 'identifier' in a switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4365) // 'action': conversion from 'type_1' to 'type_2', signed/unsigned mismatch
#include <gdiplus.h>
#pragma warning(pop)

#pragma warning(pop)

using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

using namespace DicomNativeWrapper;

//-------------------------------------------------------------------------------------------------

void PixelGrabber::GrabPixels(
    String^ filename_in_managed, 
    String^ filename_out_managed
) {
    const std::wstring filename_in = Marshal::ToStdWstring(filename_in_managed);
    const std::wstring filename_out = Marshal::ToStdWstring(filename_out_managed);

    // Read in the DICOM file
    std::unique_ptr<Networking::Object> dicom_object;
    try {
        Networking::FileInputBuffer buffer(filename_in);
        dicom_object.reset(new Networking::Object(buffer, Networking::Object::LoadAllData));
    } catch(...) {
        throw gcnew System::IO::IOException("Error whilst reading input DICOM file - aborting");
    }

    // Check the DICOM object is the Secondary Capture type we are expecting
    if (*dicom_object->Get(Networking::Dictionary::SOPClassUID) != 
        Networking::UI::MultiframeTrueColorSecondaryCaptureImageStorage
    ) {
        throw gcnew InvalidOperationException("Object is not a MultiframeTrueColorSC- aborting");
    }

    // Get the number of frames in this secondary capture
    const int number_of_frames = dicom_object->GetInteger(Networking::Dictionary::NumberOfFrames);

    // Get rows and columns. We assume equal for all frames and check the images are square and
    // size 1024 by 1024 - the only case we currently need.
    const int rows = dicom_object->GetInteger(Networking::Dictionary::Rows);
    const int columns = dicom_object->GetInteger(Networking::Dictionary::Columns);
    if ((rows != 1024) || (columns != 1024)) {
        throw gcnew InvalidOperationException("Image not 1024 by 1024 - aborting");
    }

    // Check for presence/value of some pixel data related tags
    const int pixel_rep = dicom_object->GetInteger(Networking::Dictionary::PixelRepresentation);
    // Currently support only 0 pixel rep
    if (pixel_rep != 0) {
        throw gcnew InvalidOperationException("Pixel rep should be 0 - aborting");
    }

    const int bits_alloc = dicom_object->GetInteger(Networking::Dictionary::BitsAllocated);
    // Currently support only 8 bits allocated
    if (bits_alloc != 8) {
        throw gcnew InvalidOperationException("Bits allocated should be 8 - aborting");
    }

    if (!dicom_object->Contains(Networking::Dictionary::PixelData)) {
        throw gcnew InvalidOperationException("Pixel data tag missing - aborting");
    }
    const std::unique_ptr<Networking::VR> pixel_data_vr(dicom_object->Detach(Networking::Dictionary::PixelData));

    // Check data type is 'UN'
    const Networking::UInt16 typ = pixel_data_vr->Type();
    if (typ != 'UN') {
        throw gcnew InvalidOperationException("Pixel data not of type 'UN' - aborting");
    }
    const Networking::UN* const pixel_data_un = dynamic_cast<Networking::UN*>(pixel_data_vr.get());

    // Now loop to dump pixel data from each frame as a bitmap.
    const int number_of_pixels = rows * columns;
    try {
        for (int frame = 0; frame < number_of_frames; frame++) {
            // Get ptr to first pixel of the current frame
            BYTE* const frame_ptr = static_cast<BYTE*>(pixel_data_un->Frame(frame));

            // Construct output filename
            std::wostringstream output_filename;
            output_filename << filename_out.c_str() << frame << L".bmp";

            // Switch blue and red as BMP format uses BGR, rather than RGB of DICOM
            for (int pixel = 0; pixel < number_of_pixels; pixel++) {
                const int pixel_offset = 3 * pixel;
                std::swap(frame_ptr[pixel_offset + 0], frame_ptr[pixel_offset + 2]);
            }

            // Save frame to disk
            SaveFrame(frame_ptr, rows, columns, output_filename.str().c_str());
        }
    } catch (...) {
        throw gcnew InvalidOperationException("Error whilst saving bitmaps to disk - aborting");
    }
}

//-------------------------------------------------------------------------------------------------

void PixelGrabber::SaveFrame(
    BYTE* const image_ptr, 
    const INT rows, 
    const INT columns, 
    const WCHAR* const filename
) {
    // Initialize GDI+.
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Ok) {
        throw gcnew InvalidOperationException("Failed to initialise GDI");
    }

    // Restrict the scope of bmap - 
    // Gdiplus has its own new and delete operators, so Gdiplus objects must not live beyond the call to GdiplusShutdown()
    {
        // Save frame to file as Bitmap
        Bitmap bmap(rows, columns, rows * 3, PixelFormat24bppRGB, image_ptr);
        const CLSID bmpClsid = GetEncoderClsid(L"image/bmp");
        if (bmap.Save(filename, &bmpClsid, nullptr) != Ok) {
            throw gcnew InvalidOperationException("Failed to save");
        }
    }

    // Shutdown GDI+
    GdiplusShutdown(gdiplusToken);
}

//-------------------------------------------------------------------------------------------------

CLSID PixelGrabber::GetEncoderClsid(
    const WCHAR* const format
) {
    UINT num; // number of image encoders
    UINT size; // size of the image encoder array in bytes
    if (GetImageEncodersSize(&num, &size) != Ok) {
        throw gcnew InvalidOperationException("Failed to get the number and size of encoders");
    }
    if (size == 0) {
        throw gcnew InvalidOperationException("Found no encoders");
    }

    ImageCodecInfo* const pImageCodecInfo = static_cast<ImageCodecInfo*>(malloc(size));
    if (pImageCodecInfo == nullptr) {
        throw gcnew InvalidOperationException("Failed to allocate space for codecs");
    }
    if (GetImageEncoders(num, size, pImageCodecInfo) != Ok) {
        free(pImageCodecInfo);
        throw gcnew InvalidOperationException("Failed to get codecs");
    }

    for (UINT j = 0; j < num; ++j) {
        #pragma warning(suppress: 6385) // Static Analysis doesn't like the array access here.
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            CLSID clsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return clsid; // Success
        }
    }

    free(pImageCodecInfo);
    throw gcnew InvalidOperationException("Failed to find a codec for the correct mimetype");
}

//-------------------------------------------------------------------------------------------------

