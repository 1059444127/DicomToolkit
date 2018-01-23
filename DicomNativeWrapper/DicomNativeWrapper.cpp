#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/
#include "DicomNativeWrapper.h"

#include "Dicom/DicomPusher.h"
#include "Utilities/Marshal.h"

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "Networking/Message/Object.h"
#include "Networking/Message/UI.h"
#include "Networking/Common/FileInputBuffer.h"
#include "Networking/Message/Object.h"
#include "Networking/Message/Dictionary.h"
#include "PvBase/FileSystemUtilities.h"
#pragma warning(pop)

using namespace DicomNativeWrapper;

//-------------------------------------------------------------------------------------------------

DicomWrapper::DicomWrapper(IOD iod)
:   m_dicom_creator(new ::DicomCreator(static_cast< ::DicomCreator::IOD>(iod)))
{
}

//-------------------------------------------------------------------------------------------------

DicomWrapper::~DicomWrapper(
) {
    if (m_dicom_creator != nullptr) {
        delete m_dicom_creator;
        m_dicom_creator = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------

DicomWrapper::!DicomWrapper(
) {
    if (m_dicom_creator != nullptr) {
        delete m_dicom_creator;
        m_dicom_creator = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------

String^ DicomWrapper::FormatDateString(
    String^ in
) {
    return Marshal::ToSystemString(
        DicomCreator::FormatDateString(Marshal::ToStdWstring(in))
    );
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PatientsName::set(String^ value) {
    m_dicom_creator->m_patients_name = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::PatientsName::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_patients_name);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PatientPosition::set(String^ value) {
    m_dicom_creator->m_patient_position = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::PatientPosition::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_patient_position);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::EncodedPatientID::set(String^ value) {
    m_dicom_creator->m_encoded_patient_id = Marshal::ToStdString(value);
    // It's not valid to have both an encoded and an unencoded one.
    m_dicom_creator->m_patient_id.clear();
}

String^ DicomWrapper::EncodedPatientID::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_encoded_patient_id);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PatientID::set(String^ value) {
    m_dicom_creator->m_patient_id = Marshal::ToStdWstring(value);
    // It's not valid to have both an encoded and an unencoded one.
    m_dicom_creator->m_encoded_patient_id.clear();
}

String^ DicomWrapper::PatientID::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_patient_id);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PatientsBirthDate::set(String^ value) {
    m_dicom_creator->m_patients_birth_date = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::PatientsBirthDate::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_patients_birth_date);
}

String^ DicomWrapper::FormattedPatientsBirthDate::get() {
    return Marshal::ToSystemString(m_dicom_creator->GetFormattedPatientsBirthDate());
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PatientsSex::set(String^ value) {
    m_dicom_creator->m_patients_sex = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::PatientsSex::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_patients_sex);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::StudyDate::set(String^ value) {
    m_dicom_creator->m_study_date = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::StudyDate::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_study_date);
}

String^ DicomWrapper::FormattedStudyDate::get() {
    return Marshal::ToSystemString(m_dicom_creator->GetFormattedStudyDate());
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::StudyID::set(String^ value) {
    m_dicom_creator->m_study_id = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::StudyID::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_study_id);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::EncodedStudyDescription::set(String^ value) {
    m_dicom_creator->m_encoded_study_description = Marshal::ToStdString(value);
    // It's not valid to have both an encoded and an unencoded one.
    m_dicom_creator->m_study_description.clear();
}

String^ DicomWrapper::EncodedStudyDescription::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_encoded_study_description);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::StudyDescription::set(String^ value) {
    m_dicom_creator->m_study_description = Marshal::ToStdWstring(value);
    // It's not valid to have both an encoded and an unencoded one.
    m_dicom_creator->m_encoded_study_description.clear();
}

String^ DicomWrapper::StudyDescription::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_study_description);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PatientsAge::set(String^ value) {
    m_dicom_creator->m_patients_age = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::PatientsAge::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_patients_age);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::Modality::set(String^ value) {
    m_dicom_creator->m_modality = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::Modality::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_modality);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::EncodedSeriesDescription::set(String^ value) {
    m_dicom_creator->m_encoded_series_description = Marshal::ToStdString(value);
    // It's not valid to have both an encoded and an unencoded one.
    m_dicom_creator->m_series_description.clear();
}

String^ DicomWrapper::EncodedSeriesDescription::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_encoded_series_description);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::SeriesDescription::set(String^ value) {
    m_dicom_creator->m_series_description = Marshal::ToStdWstring(value);
    // It's not valid to have both an encoded and an unencoded one.
    m_dicom_creator->m_encoded_series_description.clear();
}

String^ DicomWrapper::SeriesDescription::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_series_description);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::NumberOfFrames::set(int value) {
    m_dicom_creator->m_number_of_frames = value;
}

int DicomWrapper::NumberOfFrames::get() {
    return m_dicom_creator->m_number_of_frames;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::Rows::set(UInt16 value) {
    m_dicom_creator->m_rows = value;
}

UInt16 DicomWrapper::Rows::get() {
    return m_dicom_creator->m_rows;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::Columns::set(UInt16 value) {
    m_dicom_creator->m_columns = value;
}

UInt16 DicomWrapper::Columns::get() {
    return m_dicom_creator->m_columns;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::BitsAllocated::set(UInt16 value) {
    m_dicom_creator->m_bits_allocated = value;
}

UInt16 DicomWrapper::BitsAllocated::get() {
    return m_dicom_creator->m_bits_allocated;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PixelRepresentation_Signed::set(bool value) {
    m_dicom_creator->m_pixel_representation_signed = value;
}

bool DicomWrapper::PixelRepresentation_Signed::get() {
    return m_dicom_creator->m_pixel_representation_signed;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PixelDataVRType::set(String^ value) {

    if (String::IsNullOrEmpty(value)) {
        throw gcnew System::ArgumentNullException("value");
    }
    if (value->Length < 2) {
        throw gcnew System::ArgumentException("Not enough characters", "value");
    }

    const Char first_char = value[0];
    const Char second_char = value[1];
    m_dicom_creator->m_pixel_data_vr_type = 
        static_cast<Networking::UInt16>((first_char << 8) | second_char);
}

String^ DicomWrapper::PixelDataVRType::get() {
    array<Char>^ chars = gcnew array<Char>(2);
    chars[0] = (Char)((m_dicom_creator->m_pixel_data_vr_type & 0xF0) >> 8);
    chars[1] = (Char)(m_dicom_creator->m_pixel_data_vr_type & 0x0F);
    return gcnew String(chars);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumetricProperties::set(String^ value) {
    m_dicom_creator->m_volumetric_properties = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::VolumetricProperties::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_volumetric_properties);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::SOPClassUID::set(SOPClassUID_t value) {
    switch (value) {
    case SOPClassUID_t::eXAImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::XAImageStorage;
        return;
    case SOPClassUID_t::eXRay3DAngiographicImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::XRay3DAngiographicImageStorage;
        return;
    case SOPClassUID_t::eEnhancedMRImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::EnhancedMRImageStorage;
        return;
    case SOPClassUID_t::eMRImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::MRImageStorage;
        return;
    case SOPClassUID_t::eCTImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::CTImageStorage;
        return;
    case SOPClassUID_t::eEnhancedCTImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::EnhancedCTImageStorage;
        return;
    case SOPClassUID_t::eNMImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::NMImageStorage;
        return;
    case SOPClassUID_t::ePETImageStorage:
        m_dicom_creator->m_sop_class_uid = &Networking::UI::PETImageStorage;
        return;
    default:
        throw gcnew System::ArgumentOutOfRangeException("Unknown SOP Class UID: " + value.ToString());
    }
}

DicomWrapper::SOPClassUID_t DicomWrapper::SOPClassUID::get () {
    const Networking::UI& sop_class_uid = *m_dicom_creator->m_sop_class_uid;
    if (sop_class_uid == Networking::UI::XAImageStorage) {
        return SOPClassUID_t::eXAImageStorage;
    } else if (sop_class_uid == Networking::UI::XRay3DAngiographicImageStorage) {
        return SOPClassUID_t::eXRay3DAngiographicImageStorage;
    } else if (sop_class_uid == Networking::UI::EnhancedMRImageStorage) {
        return SOPClassUID_t::eEnhancedMRImageStorage;
    } else if (sop_class_uid == Networking::UI::MRImageStorage) {
        return SOPClassUID_t::eMRImageStorage;
    } else if (sop_class_uid == Networking::UI::CTImageStorage) {
        return SOPClassUID_t::eCTImageStorage;
    } else if (sop_class_uid == Networking::UI::EnhancedCTImageStorage) {
        return SOPClassUID_t::eEnhancedCTImageStorage;
    } else if (sop_class_uid == Networking::UI::NMImageStorage) {
        return SOPClassUID_t::eNMImageStorage;
    } else if (sop_class_uid == Networking::UI::PETImageStorage) {
        return SOPClassUID_t::ePETImageStorage;
    } else {
        throw gcnew System::InvalidOperationException(
            "Unknown SOP Class UID: " + Marshal::ToSystemString(sop_class_uid.Format())
        );
    }
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::SpecificCharacterSet::set(String^ value) {
    m_dicom_creator->m_specific_character_set = Marshal::ToStdWstring(value);
}

String^ DicomWrapper::SpecificCharacterSet::get() {
    return Marshal::ToSystemString(m_dicom_creator->m_specific_character_set);
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::Tilted::set(bool value) {
    m_dicom_creator->m_tilted = value;
}

bool DicomWrapper::Tilted::get() {
    return m_dicom_creator->m_tilted;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::UnevenSpacing::set(bool value) {
    m_dicom_creator->m_uneven_spacing = value;
}

bool DicomWrapper::UnevenSpacing::get() {
    return m_dicom_creator->m_uneven_spacing;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::ViewFromHead::set(bool value) {
    m_dicom_creator->m_view_from_head = value;
}

bool DicomWrapper::ViewFromHead::get() {
    return m_dicom_creator->m_view_from_head;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumeHasPrivateTag::set(bool value) {
    m_dicom_creator->m_has_volume_private_tag = value;
}

bool DicomWrapper::VolumeHasPrivateTag::get() {
    return m_dicom_creator->m_has_volume_private_tag;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumePrivateTagBedLatitude::set(float value) {
    m_dicom_creator->m_volume_private_tag_bed_latitude = value;
}

float DicomWrapper::VolumePrivateTagBedLatitude::get() {
    return m_dicom_creator->m_volume_private_tag_bed_latitude;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumePrivateTagBedLongitude::set(float value) {
    m_dicom_creator->m_volume_private_tag_bed_longitude = value;
}

float DicomWrapper::VolumePrivateTagBedLongitude::get() {
    return m_dicom_creator->m_volume_private_tag_bed_longitude;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumePrivateTagBedHeight::set(float value) {
    m_dicom_creator->m_volume_private_tag_bed_height = value;
}

float DicomWrapper::VolumePrivateTagBedHeight::get() {
    return m_dicom_creator->m_volume_private_tag_bed_height;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumePrivateTagCeilingLatitude::set(float value) {
    m_dicom_creator->m_volume_private_tag_ceiling_latitude = value;
}

float DicomWrapper::VolumePrivateTagCeilingLatitude::get() {
    return m_dicom_creator->m_volume_private_tag_ceiling_latitude;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumePrivateTagCeilingLongitude::set(float value) {
    m_dicom_creator->m_volume_private_tag_ceiling_longitude = value;
}

float DicomWrapper::VolumePrivateTagCeilingLongitude::get() {
    return m_dicom_creator->m_volume_private_tag_ceiling_longitude;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::VolumePrivateTagArmFloorPosition::set(int value) {
    m_dicom_creator->m_volume_private_tag_arm_floor_position = value;
}

int DicomWrapper::VolumePrivateTagArmFloorPosition::get() {
    return m_dicom_creator->m_volume_private_tag_arm_floor_position;
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::PushData(
    String^ hostname,
    String^ ae_title,
    UInt16 port
) {
    try {
        auto owned_objects = m_dicom_creator->NewObjects();

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

void DicomWrapper::SaveDefaultData(
    String^ folder_path
) {
    try {
        auto owned_objects = m_dicom_creator->NewObjects();
        // Takes ownership of owned_objects
        DicomPusher::SaveData(owned_objects, folder_path);

    } catch (const PvBase::Log& e) {
        throw gcnew System::InvalidOperationException(
            "PvBase::Log caught: " + Marshal::ToSystemString(e.ConcatenatedContents())
        );

    } catch (...) {
        throw gcnew System::InvalidOperationException("Exception caught");
    }
}

//-------------------------------------------------------------------------------------------------

void DicomWrapper::SplitFluoroscopy(
    String^ src_file_path,
    String^ folder_path
) {
    try {
        const std::string src_path = Marshal::ToStdString(src_file_path);

        // Only one source dicom is needed. This dicom contains multi frames.
        DicomPusher::OwnedObjects owned_objects(new std::deque<std::unique_ptr<Networking::Object>>(1));
        Networking::FileInputBuffer buff(src_path);
        owned_objects->at(0).reset(new Networking::Object(buff, Networking::Object::LoadAllData));

        const int number_of_frames = owned_objects->at(0)->GetInteger(Networking::Dictionary::NumberOfFrames);
        DicomPusher::OwnedObjects out_objects(new std::deque<std::unique_ptr<Networking::Object>>(number_of_frames));
        for (int frame_index = 0; frame_index < number_of_frames; ++frame_index) {
            std::unique_ptr<Networking::Object> object = std::make_unique<Networking::Object>();
            object = m_dicom_creator->NewFluoroObject(*owned_objects->at(0), frame_index);
            DicomPusher::SaveFluoroData(*object, folder_path, frame_index);
        }

    } catch (const PvBase::Log& e) {
        throw gcnew System::InvalidOperationException(
            "PvBase::Log caught: " + Marshal::ToSystemString(e.ConcatenatedContents())
        );

    } catch (...) {
        throw gcnew System::InvalidOperationException("Exception caught");
    }
}
//-------------------------------------------------------------------------------------------------


