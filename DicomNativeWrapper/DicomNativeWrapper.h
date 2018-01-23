#pragma once
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

#include "Dicom/DicomCreator.h"

using namespace System;

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {

public ref class DicomWrapper sealed {

public: // Types

    // This must match ::DicomCreator::IOD in dicom_creator_lib
    enum class IOD {
        eStandardCT = ::DicomCreator::eStandardCT, 
        eEnhancedCT = ::DicomCreator::eEnhancedCT, 
        eStandardMR = ::DicomCreator::eStandardMR, 
        eEnhancedMR = ::DicomCreator::eEnhancedMR, 
        eXRayAngiographic = ::DicomCreator::eXRayAngiographic, 
        eXRay3DAngiographic = ::DicomCreator::eXRay3DAngiographic, 
        eUltrasoundEmbedded = ::DicomCreator::eUltrasoundEmbedded, 
        eUltrasoundNonEmbedded = ::DicomCreator::eUltrasoundNonEmbedded, 
        eFluoroscopy = ::DicomCreator::eFluoroscopy, 
    };

    enum class SOPClassUID_t {
        eXAImageStorage,
        eXRay3DAngiographicImageStorage,
        eEnhancedMRImageStorage,
        eMRImageStorage,
        eCTImageStorage,
        eEnhancedCTImageStorage,
        eNMImageStorage,
        ePETImageStorage
    };
    
public: // Construction and destruction

    DicomWrapper(
        IOD
    );

    inline ~DicomWrapper(
    );

public: // Methods

    void PushData(
        String^ hostname,
        String^ ae_title,
        UInt16 port
    );

    void SplitFluoroscopy(
        String^ src_file_path,
        String^ folder_path
    );

    void SaveDefaultData(
        String^ folder_path
    );

public: // Properties

    static String^ FormatDateString(
        String^ in
    );

    property String^ PatientsName {
        void set(String^ value);
        String^ get();
    }

    property String^ PatientPosition {
        void set(String^ value);
        String^ get();
    }

    property String^ EncodedPatientID {
        void set(String^ value);
        String^ get();
    }

    property String^ PatientID {
        void set(String^ value);
        String^ get();
    }

    property String^ PatientsBirthDate {
        void set(String^ value);
        String^ get();
    }

    property String^ FormattedPatientsBirthDate {
        String^ get();
    }

    property String^ PatientsSex {
        void set(String^ value);
        String^ get();
    }

    property String^ StudyDate {
        void set(String^ value);
        String^ get();
    }

    property String^ FormattedStudyDate {
        String^ get();
    }

    property String^ StudyID {
        void set(String^ value);
        String^ get();
    }

    property String^ EncodedStudyDescription {
        void set(String^ value);
        String^ get();
    }

    property String^ StudyDescription {
        void set(String^ value);
        String^ get();
    }

    property String^ PatientsAge {
        void set(String^ value);
        String^ get();
    }

    property String^ Modality {
        void set(String^ value);
        String^ get();
    }

    property String^ EncodedSeriesDescription {
        void set(String^ value);
        String^ get();
    }

    property String^ SeriesDescription {
        void set(String^ value);
        String^ get();
    }

    property int NumberOfFrames {
        void set(int value);
        int get();
    }

    property UInt16 Rows {
        void set(UInt16 value);
        UInt16 get();
    }

    property UInt16 Columns {
        void set(UInt16 value);
        UInt16 get();
    }

    property UInt16 BitsAllocated {
        void set(UInt16 value);
        UInt16 get();
    }

    property bool PixelRepresentation_Signed {
        void set(bool value);
        bool get();
    }

    property String^ PixelDataVRType {
        void set(String^ value);
        String^ get();
    }

    property String^ VolumetricProperties {
        void set(String^ value);
        String^ get();
    }

    property SOPClassUID_t SOPClassUID {
        void set(SOPClassUID_t value);
        SOPClassUID_t get();
    }

    property String^ SpecificCharacterSet {
        void set(String^ value);
        String^ get();
    }

    property bool Tilted {
        void set(bool value);
        bool get();
    }

    property bool UnevenSpacing {
        void set(bool value);
        bool get();
    }

    property bool ViewFromHead {
        void set(bool value);
        bool get();
    }

    property bool VolumeHasPrivateTag {
        void set(bool value);
        bool get();
    }

    property float VolumePrivateTagBedLatitude {
        void set(float value);
        float get();
    }

    property float VolumePrivateTagBedLongitude {
        void set(float value);
        float get();
    }

    property float VolumePrivateTagBedHeight {
        void set(float value);
        float get();
    }

    property float VolumePrivateTagCeilingLatitude {
        void set(float value);
        float get();
    }

    property float VolumePrivateTagCeilingLongitude {
        void set(float value);
        float get();
    }

    property int VolumePrivateTagArmFloorPosition {
        void set(int value);
        int get();
    }

protected: // Construction and destruction

    inline !DicomWrapper(
    );

private: // Data

    ::DicomCreator* m_dicom_creator;

};

}

//-------------------------------------------------------------------------------------------------
