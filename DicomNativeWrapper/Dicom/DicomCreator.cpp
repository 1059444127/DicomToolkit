#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: DicomNativeWrapper

**************************************************************************************************/
#include "DicomCreator.h"

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#include "Networking/Common/MemoryInputBuffer.h"
#include "Networking/Common/MemoryOutputBuffer.h"
#include "Networking/Internationalization/StringDecodeManager.h"
#include "Networking/Message/DA.h"
#include "Networking/Message/DS.h"
#include "Networking/Message/Dictionary.h"
#include "Networking/Message/IS.h"
#include "Networking/Message/LO.h"
#include "Networking/Message/OB.h"
#include "Networking/Message/Object.h"
#include "Networking/Message/SL.h"
#include "Networking/Message/SQ.h"
#include "Networking/Message/UI.h"
#include "Networking/Message/US.h"
#include "Networking/Message/OW.h"
#include "Networking/Message/UN.h"
#include "PvBase/Convert.h"
#pragma warning(pop)

//-------------------------------------------------------------------------------------------------

DicomCreator::DicomCreator(IOD iod) :
    m_iod(iod),
    m_study_instance_uid(new Networking::UI),
    m_series_instance_uid(new Networking::UI),
    m_number_of_frames(4),
    m_rows(4),
    m_columns(4), 
    m_pixel_data_vr_type('OW'),
    m_patient_id(L"Patient ID"),
    m_study_id(L"Study ID"),
    m_patients_birth_date(L"19290529"),
    m_patients_age(L"083Y"),
    m_patients_sex(L"M"),
    m_study_date(L"20120704"),
    m_patients_name(L"Patient's Name"),
    m_patient_position(L"HFS"),
    m_has_volume_private_tag(false),
    m_volume_private_tag_bed_latitude(0),
    m_volume_private_tag_bed_longitude(0),
    m_volume_private_tag_bed_height(0),
    m_volume_private_tag_ceiling_latitude(0),
    m_volume_private_tag_ceiling_longitude(0),
    m_volume_private_tag_arm_floor_position(0),
    m_tilted(false),
    m_uneven_spacing(false),
    m_view_from_head(false)
{ 
    switch (iod) {
    case eStandardCT:
        m_modality = L"CT";
        m_bits_allocated = 16;
        m_pixel_representation_signed = true;
        m_sop_class_uid = &Networking::UI::CTImageStorage;
        break;

    case eEnhancedCT:
        m_modality = L"CT";
        m_bits_allocated = 16;
        m_pixel_representation_signed = true;
        m_sop_class_uid = &Networking::UI::EnhancedCTImageStorage;
        m_volumetric_properties = L"VOLUME";
        break;

    case eStandardMR:
        m_modality = L"MR";
        m_bits_allocated = 16;
        m_pixel_representation_signed = true;
        m_sop_class_uid = &Networking::UI::MRImageStorage;
        break;

    case eEnhancedMR:
        m_modality = L"MR";
        m_bits_allocated = 16;
        m_pixel_representation_signed = true;
        m_sop_class_uid = &Networking::UI::EnhancedMRImageStorage;
        m_volumetric_properties = L"VOLUME";
        break;

    case eXRayAngiographic:
        m_modality = L"XA";
        m_bits_allocated = 8;
        m_pixel_representation_signed = false;
        m_sop_class_uid = &Networking::UI::XAImageStorage;
        m_has_volume_private_tag = true;
        break;

    case eXRay3DAngiographic: 
        m_modality = L"XA";
        m_bits_allocated = 8;
        m_pixel_representation_signed = false;
        m_sop_class_uid = &Networking::UI::XRay3DAngiographicImageStorage;
        m_has_volume_private_tag = true;
        m_volumetric_properties = L"VOLUME";
        break;

    case eUltrasoundNonEmbedded:
    case eUltrasoundEmbedded:
        m_modality = L"US";
        m_bits_allocated = 8;
        m_pixel_representation_signed = false;

        // Infinix sends Ultrasound as X-Ray 3D Angiographic
        m_sop_class_uid = &Networking::UI::XRay3DAngiographicImageStorage;
        m_volumetric_properties = L"VOLUME";
        break;

    case eFluoroscopy:
        // Application only supports this size of fluoro images
        m_rows = 1024;
        m_columns = 1024;

        m_bits_allocated = 16;
        m_pixel_representation_signed = true;
        m_modality = L"XA";
        m_sop_class_uid = &Networking::UI::XAImageStorage;
        break;

    default:
        throw PvBase::Log(PvBase::Log::ARGUMENT_EXCEPTION) << __FUNCTION__ << 
            ": Unsupported IOD: " << iod << PvBase::Break();
    }
}

//-------------------------------------------------------------------------------------------------

DicomCreator::OwnedObjects DicomCreator::NewObjects(
) const {

    // Create pixel data
    std::unique_ptr<const PixelData> pixel_data;
    switch (m_bits_allocated) {
    case 8:
        if (m_pixel_representation_signed) {
            pixel_data = MakePixelData<Networking::SByte>();
        } else {
            pixel_data = MakePixelData<Networking::UByte>();
        }
        break;
    case 16:
        if (m_pixel_representation_signed) {
            pixel_data = MakePixelData<Networking::SInt16>();
        } else {
            pixel_data = MakePixelData<Networking::UInt16>();
        }
        break;
    default:
        throw PvBase::Log(PvBase::Log::ARGUMENT_OUT_OF_RANGE_EXCEPTION) << __FUNCTION__ << 
            ": Unsupported bits allocated: " << m_bits_allocated << PvBase::Break();
    }
    const Networking::Byte* p_pixel_data = &pixel_data->m_pixel_data.front();

    // Create Objects
    OwnedObjects objects;
    switch (m_iod) {
    case eEnhancedCT:
    case eEnhancedMR:
    case eXRay3DAngiographic: 
    case eUltrasoundEmbedded: 
    case eUltrasoundNonEmbedded: 
        {
            // Create an empty Object
            objects = MakeOwnedObjects(1);
            Networking::Object& object = *objects->front();

            // Add the Specific Character Set first, before any tags which might modify it
            if (!m_specific_character_set.empty()) {
                CreateStringTag(object, Networking::Dictionary::SpecificCharacterSet, m_specific_character_set);
            }

            AddVolumeFormatPrivateTag(object);

            // Add all required modules from the DICOM standard
            AddPatientModule(object);
            AddGeneralStudyModule(object);
            AddPatientStudyModule(object);
            AddGeneralSeriesModule(object);
            AddImagePixelModule(object, m_number_of_frames, pixel_data->m_framelength_in_bytes, p_pixel_data);
            AddMultiframeFunctionalGroupsModule(object, m_view_from_head, m_rows);
            AddSpecialisedImageModule(object);
            AddSOPCommonModule(object);

            if (m_iod == eUltrasoundEmbedded) {
                // Embed the Ultrasound data in a private tag

                const Networking::Encoding encoding = Networking::Explicit;
                const Networking::UInt32 length = object.Length(encoding);

                // Write the Object out to a buffer
                Networking::MemoryOutputBuffer memory_output_buffer(
                    Networking::LittleEndian,
                    length
                );
                object.Encode(memory_output_buffer, encoding);

                // Read the data back from the buffer, as an OB tag
                Networking::MemoryInputBuffer memory_input_buffer(memory_output_buffer);
                std::unique_ptr<Networking::OB> embedded_data_vr(
                    new Networking::OB(memory_input_buffer, length)
                );

                // Replace the Object with an SC Object containing the private tag
                object.RemoveAllMatching(0x0000, 0x0000);
                object.Add(Networking::Dictionary::SOPClassUID, Networking::UI::SCImageStorage.Copy());
                object.Add(0x701B1000, embedded_data_vr.release());
                // Add patient id to outer object so it gets past the loader code in the test transceiver.
                AddPatientModule(object);
                AddGeneralStudyModule(object);
                AddGeneralSeriesModule(object);
            }
        }
        break;

    case eStandardCT:
    case eStandardMR:
    case eXRayAngiographic:
        {
            // Create a volume of empty Objects
            objects = MakeOwnedObjects(m_number_of_frames);

            int instance_number = 0;
            for(auto iter = objects->cbegin(); 
                objects->cend() != iter; 
                ++iter, 
                ++instance_number, 
                p_pixel_data += pixel_data->m_framelength_in_bytes
            ) {
                Networking::Object& object = **iter;

                // Add the Specific Character Set first, before any tags which might modify it
                if (!m_specific_character_set.empty()) {
                    CreateStringTag(object, Networking::Dictionary::SpecificCharacterSet, m_specific_character_set);
                }

                AddVolumeFormatPrivateTag(object);

                // Add all required modules from the DICOM standard
                AddPatientModule(object);
                AddGeneralStudyModule(object);
                AddPatientStudyModule(object);
                AddGeneralSeriesModule(object);
                AddGeneralImageModule(object, instance_number);
                AddImagePlaneModule(
                    object, 
                    instance_number, 
                    m_tilted, 
                    m_uneven_spacing, 
                    m_view_from_head,
                    m_rows
                );
                AddImagePixelModule(object, 1, pixel_data->m_framelength_in_bytes, p_pixel_data);
                AddSOPCommonModule(object);
            }
        }
        break;

    case eFluoroscopy:
        {
            // Create empty Objects
            objects = MakeOwnedObjects(m_number_of_frames);

            int instance_number = 0;
            for(auto iter = objects->cbegin(); 
                objects->cend() != iter; 
                ++iter, 
                ++instance_number, 
                p_pixel_data += pixel_data->m_framelength_in_bytes
            ) {
                Networking::Object& object = **iter;

                // Add all required modules from the DICOM standard
                AddPatientModule(object);
                AddGeneralStudyModule(object);
                AddPatientStudyModule(object);
                AddGeneralSeriesModule(object);
                AddGeneralImageModule(object, instance_number);
                AddImagePixelModule(object, 1, pixel_data->m_framelength_in_bytes, p_pixel_data);
                AddSOPCommonModule(object);
                AddFluoroFormatPrivateTag(object);
            }
        }
        break;

    default:
        throw PvBase::Log(PvBase::Log::ARGUMENT_EXCEPTION) << __FUNCTION__ << 
            ": Unsupported IOD: " << m_iod << PvBase::Break();
    }

    return objects;
}

//-------------------------------------------------------------------------------------------------

template<typename T>
std::unique_ptr<DicomCreator::PixelData> DicomCreator::MakePixelData(
) const {
    std::unique_ptr<PixelData> pixel_data(new PixelData());

    pixel_data->m_framelength_in_bytes = m_rows * m_columns * sizeof(T);
    pixel_data->m_pixel_data.resize(m_number_of_frames * pixel_data->m_framelength_in_bytes);

    T* p_pixel_data = reinterpret_cast<T*>(&pixel_data->m_pixel_data.front());

    // Knock up a quick cone:

    // These values must be valid in all pixel data types, 
    // and the object should be visible with the WL/WW values in the default preset
    const T air_value = 1;
    const T object_min_value = 60;
    const T object_max_value = 127;
    const T object_range = object_max_value - object_min_value;

    const auto x_range = m_rows - 1;
    const auto y_range = m_columns - 1;
    const auto z_range = m_number_of_frames - 1;

    const auto centre_x = x_range / 2;
    const auto centre_y = y_range / 2;

    const auto base_radius = min(x_range, y_range) / 2;

    for (int z = 0; z < m_number_of_frames; z++) {
        const auto radius = base_radius * (z_range - z) / z_range;
        const auto radius_squared = radius * radius;

        for (int x = 0; x < m_rows; x++) {
            const auto x_dist_from_centre = abs(x - centre_x);
            const auto x_dist_from_centre_squared = x_dist_from_centre * x_dist_from_centre;

            for (int y = 0; y < m_columns; y++) {
                const auto y_dist_from_centre = abs(y - centre_y);
                const auto y_dist_from_centre_squared = y_dist_from_centre * y_dist_from_centre;

                const auto dist_from_centre_squared = x_dist_from_centre_squared + y_dist_from_centre_squared;

                *p_pixel_data++ = 
                    dist_from_centre_squared < radius_squared ? 
                    static_cast<T>(object_min_value + object_range * y / y_range) : 
                    air_value;
            }
        }
    }

    return pixel_data;
}

//-------------------------------------------------------------------------------------------------

DicomCreator::OwnedObjects DicomCreator::MakeOwnedObjects(
    const int count
) {
    OwnedObjects objects(new std::deque<std::unique_ptr<Networking::Object>>(static_cast<size_t>(count)));
    for (auto object = objects->begin(); objects->end() != object; ++object) {
        object->reset(new Networking::Object());
    }
    return objects;
}

//-------------------------------------------------------------------------------------------------

std::unique_ptr<Networking::Object> DicomCreator::MakeOwnedObject(
) {
    std::unique_ptr<Networking::Object> object = std::make_unique<Networking::Object>();
    object.reset(new Networking::Object());
    return object;
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddPatientModule(
    Networking::Object& object
) const {
    CreateStringTag(object, Networking::Dictionary::PatientsName, m_patients_name);
    CreateStringTag(object, Networking::Dictionary::PatientPosition, m_patient_position);
    CreateStringTag(object, Networking::Dictionary::PatientID, m_encoded_patient_id, m_patient_id);
    CreateStringTag(object, Networking::Dictionary::PatientsBirthDate, m_patients_birth_date);
    CreateStringTag(object, Networking::Dictionary::PatientsSex, m_patients_sex);
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddVolumeFormatPrivateTag(
    Networking::Object& object
) const {

    if (!m_has_volume_private_tag) {
        return;
    }

    // Tag must have at least 20 elements to be accepted by the loader    
    // Initialise to 0 since we don't care about most of these values
    std::vector<Networking::SInt32> private_tag_content(48, 0);
    private_tag_content[6] = m_volume_private_tag_ceiling_longitude;
    private_tag_content[7] = m_volume_private_tag_ceiling_latitude;
    private_tag_content[12] = m_volume_private_tag_arm_floor_position;
    private_tag_content[13] = m_volume_private_tag_bed_height;
    private_tag_content[14] = m_volume_private_tag_bed_longitude;
    private_tag_content[15] = m_volume_private_tag_bed_latitude;

    object.Add(0x70790010, new Networking::LO("TOSHIBA_MEC_XA3"));
    object.Add(0x7079104e, new Networking::SL(private_tag_content));
}    

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddGeneralStudyModule(
    Networking::Object& object
) const {
    object.Add(Networking::Dictionary::StudyInstanceUID, m_study_instance_uid->Copy());
    CreateStringTag(object, Networking::Dictionary::StudyDate, m_study_date);
    CreateStringTag(object, Networking::Dictionary::StudyID, m_study_id);
    if (!m_encoded_study_description.empty() || !m_study_description.empty()) {
        CreateStringTag(object, Networking::Dictionary::StudyDescription, m_encoded_study_description, m_study_description);
    }
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddPatientStudyModule(
    Networking::Object& object
) const {
    if (!m_patients_age.empty()) {
        CreateStringTag(object, Networking::Dictionary::PatientsAge, m_patients_age);
    }
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddGeneralSeriesModule(
    Networking::Object& object
) const {
    CreateStringTag(object, Networking::Dictionary::Modality, m_modality);
    object.Add(Networking::Dictionary::SeriesInstanceUID, m_series_instance_uid->Copy());
    if (!m_encoded_series_description.empty() || !m_series_description.empty()) {
        CreateStringTag(object, Networking::Dictionary::SeriesDescription, m_encoded_series_description, m_series_description);
    }
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddGeneralImageModule(
    Networking::Object& object, 
    const int instance_number
) {
    object.Add(Networking::Dictionary::InstanceNumber, new Networking::IS(instance_number));
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddImagePlaneModule(
    Networking::Object& object,
    const int index,
    bool tilted,
    bool uneven_spacing,
    bool view_from_head,
    int num_rows
) {
    object.Add(Networking::Dictionary::PixelSpacing, new Networking::DS(1.0, 1.0));

    Networking::DS* orientation;
    if (view_from_head) {
        orientation = new Networking::DS(
            -1, 0, 0, 
             0, 1, 0
        );
    } else {
        orientation = new Networking::DS(
            1, 0, 0,
            0, 1, 0
        );
    }
    object.Add(Networking::Dictionary::ImageOrientationPatient, orientation);

    double z_pos = index;
    if (uneven_spacing) {
        // Just arbitrarily move the second slice
        if (index == 1) {
            z_pos = 1.8;
        }
    }

    double x_pos = 0;
    double y_pos = 0;
    if (tilted) {
        // Set the x and y positions to be the same as the z, to get a 45 degree tilt.
        x_pos = z_pos;
        y_pos = z_pos;
    }

    // Make the position top-left even for view_from_head slices
    if (view_from_head) {
        x_pos = (num_rows-2) - x_pos;
    }

    object.Add(Networking::Dictionary::ImagePositionPatient, new Networking::DS(
        x_pos, 
        y_pos, 
        z_pos
    ));
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddImagePixelModule(
    Networking::Object& object, 
    const int number_of_frames, 
    const Networking::UInt32 framelength_in_bytes, 
    const Networking::Byte* p_pixel_data
) const {
    object.Add(Networking::Dictionary::SamplesPerPixel, new Networking::US(1));
    CreateStringTag(object, Networking::Dictionary::PhotometricInterpretation, L"MONOCHROME2");
    object.Add(Networking::Dictionary::Rows, new Networking::US(m_rows));
    object.Add(Networking::Dictionary::Columns, new Networking::US(m_columns));
    object.Add(Networking::Dictionary::BitsAllocated, new Networking::US(m_bits_allocated));
    object.Add(Networking::Dictionary::PixelRepresentation, new Networking::US(m_pixel_representation_signed ? 1u : 0u));

    // Create a PixelData tag
    Networking::MemoryInputBuffer buffer(
        p_pixel_data, 
        number_of_frames * framelength_in_bytes, 
        Networking::LittleEndian, 
        false
    );

    std::unique_ptr<Networking::VR> pixel_data(Networking::VR::NewPixelDataFromBuffer(
        buffer, 
        m_pixel_data_vr_type, 
        buffer.Size(), 
        number_of_frames, 
        static_cast<Networking::UInt32>(framelength_in_bytes)
    ));

    // Add the PixelData tag
    object.Add(Networking::Dictionary::PixelData, pixel_data.release());
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddMultiframeFunctionalGroupsModule(
    Networking::Object& object,
    bool view_from_head,
    int num_rows
) const {
    // Create and add the Shared Functional Groups Sequence
    std::unique_ptr<Networking::Object> sequence_item(new Networking::Object());
    AddPixelMeasuresMacro(*(sequence_item.get()));
    AddPlaneOrientationPatientMacro(*(sequence_item.get()), view_from_head);
    AddPlanePositionPatientMacro(*(sequence_item.get()), 1, view_from_head, num_rows);
    object.Add(
        Networking::Dictionary::SharedFunctionalGroupsSequence, 
        new Networking::SQ(sequence_item.release())
    );

    // Create and add the Per-frame Functional Groups Sequence
    std::unique_ptr<Networking::SQ> sequence(new Networking::SQ());
    for (int frame = 0; m_number_of_frames > frame; ++frame) {
        std::unique_ptr<Networking::Object> obj(new Networking::Object());
        AddPlanePositionPatientMacro(*(obj.get()), frame, view_from_head, num_rows);
        sequence->PushBack(obj.release());
    }
    object.Add(Networking::Dictionary::PerframeFunctionalGroupsSequence, sequence.release());

    object.Add(Networking::Dictionary::NumberOfFrames, new Networking::IS(m_number_of_frames));
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddPixelMeasuresMacro(
    Networking::Object& object
) {
    std::unique_ptr<Networking::Object> sequence_item(new Networking::Object());
    sequence_item->Add(Networking::Dictionary::PixelSpacing, new Networking::DS(1.0, 1.0));
    object.Add(
        Networking::Dictionary::PixelMeasuresSequence, 
        new Networking::SQ(sequence_item.release())
    );
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddPlaneOrientationPatientMacro(
    Networking::Object& object,
    bool view_from_head
) {
    std::unique_ptr<Networking::Object> sequence_item(new Networking::Object());


    Networking::DS* orientation;
    if (view_from_head) {
        orientation = new Networking::DS(
            -1, 0, 0,
            0, 1, 0
        );
    } else {
        orientation = new Networking::DS(
            1, 0, 0,
            0, 1, 0
        );
    }

    sequence_item->Add(
        Networking::Dictionary::ImageOrientationPatient, 
        orientation
    );
    object.Add(
        Networking::Dictionary::PlaneOrientationSequence, 
        new Networking::SQ(sequence_item.release())
    );
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddPlanePositionPatientMacro(
    Networking::Object& object,
    const int frame,
    bool view_from_head,
    int num_rows
) {
    std::unique_ptr<Networking::Object> sequence_item(new Networking::Object());

    // Make the position top-left even for view_from_head slices
    double x = 0;
    if (view_from_head) {
        x = num_rows-2;
    }

    sequence_item->Add(Networking::Dictionary::ImagePositionPatient, new Networking::DS(x, 0.0, frame));
    object.Add(
        Networking::Dictionary::PlanePositionSequence, 
        new Networking::SQ(sequence_item.release())
    );
}

//-------------------------------------------------------------------------------------------------

// This method serves to create the Enhanced CT Image Module, Enhanced MR Image Module and X-Ray 3D Image Module;
// since we only read one tag from each module we can get away with using the same method to create them.
void DicomCreator::AddSpecialisedImageModule(
    Networking::Object& object
) const {
    AddCommonCTMRImageDescriptionMacro(object);
    CreateStringTag(object, Networking::Dictionary::PhotometricInterpretation, L"MONOCHROME2");
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddCommonCTMRImageDescriptionMacro(
    Networking::Object& object
) const {
    CreateStringTag(object, Networking::Dictionary::VolumetricProperties, m_volumetric_properties);
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddSOPCommonModule(
    Networking::Object& object
) const {
    object.Add(Networking::Dictionary::SOPClassUID, m_sop_class_uid->Copy());
    object.Add(Networking::Dictionary::SOPInstanceUID, new Networking::UI());
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddFluoroFormatPrivateTag(
    Networking::Object& object
) {
    object.Add(0x70790010, new Networking::LO("TOSHIBA_MEC_XA3"));
    object.Add(0x7079104e, new Networking::SL());
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::CreateStringTag(
    Networking::Object& object, 
    const Networking::UInt32 tag_id, 
    const std::string& encoded_string, 
    const std::wstring& string
) const {
    if (!encoded_string.empty()) {
        if (!string.empty()) {
            throw PvBase::Log(PvBase::Log::ARGUMENT_EXCEPTION) << __FUNCTION__ << 
                ": Both encoded and unencoded values are specified for tag " << tag_id << PvBase::Break();
        }
        CreateStringTag(object, tag_id, encoded_string);
    } else {
        CreateStringTag(object, tag_id, string);
    }
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::CreateStringTag(
    Networking::Object& object, 
    const Networking::UInt32 tag_id, 
    const std::string& encoded_string
) {
    Networking::StringDecodeManager decode_manager;

    // Write the encoded string out to a buffer
    Networking::MemoryOutputBuffer memory_output_buffer(
        Networking::LittleEndian,
        static_cast<Networking::Size_T>(encoded_string.length())
    );
    memory_output_buffer.WriteS(encoded_string);

    // Read the encoded string back from the buffer, as a DICOM tag
    Networking::MemoryInputBuffer memory_input_buffer(memory_output_buffer);
    std::unique_ptr<Networking::VR> tag(Networking::VR::NewVrFromBuffer(
        memory_input_buffer, 
        Networking::Implicit, 
        Networking::Dictionary::Instance().Type(tag_id),
        memory_input_buffer.Size(), 
        decode_manager
    ));

    // Decode the tag
    decode_manager.Decode();

    // Add the tag
    object.Add(tag_id, tag.release());
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::CreateStringTag(
    Networking::Object& object, 
    const Networking::UInt32 tag_id, 
    const std::wstring& string
) {
    if (!object.CreateStringTag(tag_id, string)) {
        throw PvBase::Log() << __FUNCTION__ << 
            "Failed to create tag " << tag_id << " containing " << string << PvBase::Break();
    }
}

//-------------------------------------------------------------------------------------------------

std::wstring DicomCreator::GetFormattedPatientsBirthDate() const {
    return FormatDateString(m_patients_birth_date);
}

//-------------------------------------------------------------------------------------------------

std::wstring DicomCreator::GetFormattedStudyDate() const {
    return FormatDateString(m_study_date);
}

//-------------------------------------------------------------------------------------------------

std::wstring DicomCreator::FormatDateString(
    std::wstring in
) {
    Networking::DA date_object(PvBase::Convert::WstringToString(in));
    return date_object.Format();
}

//-------------------------------------------------------------------------------------------------

std::unique_ptr<Networking::Object> DicomCreator::NewFluoroObject(
    Networking::Object& object,
    int frame_index
) {
    const Networking::VR* nt_pixel_data = object.Get(Networking::Dictionary::PixelData);
    const void* frame_data = nullptr;
    switch (object.Type(Networking::Dictionary::PixelData)) {
        case 'UN':
            frame_data = (dynamic_cast<const Networking::UN*>(nt_pixel_data))->Frame(frame_index);
            break;
        case 'OB':
            frame_data = (dynamic_cast<const Networking::OB*>(nt_pixel_data))->Frame(frame_index);
            break;
        case 'OW':
            frame_data = (dynamic_cast<const Networking::OW*>(nt_pixel_data))->Frame(frame_index);
            break;
        default:
            break;
    }

    std::unique_ptr<PixelData> pixel_data(new PixelData());
    pixel_data->m_framelength_in_bytes = m_rows * m_columns * sizeof(Networking::SInt16);
    pixel_data->m_pixel_data.resize(1 * pixel_data->m_framelength_in_bytes);  // one frame

    const Networking::Byte* p_pixel_data = &pixel_data->m_pixel_data.front();
    p_pixel_data = reinterpret_cast<const Networking::Byte*>(frame_data);

    // Create Objects
    std::unique_ptr<Networking::Object> owned_object;
    owned_object = MakeOwnedObject();

    int instance_number = 0;
    Networking::Object& tmp_object = *owned_object.get();
    tmp_object.Copy(object);
    tmp_object.Remove(Networking::Dictionary::PixelData);
    tmp_object.Remove(Networking::Dictionary::SOPClassUID);
    tmp_object.Remove(Networking::Dictionary::SOPInstanceUID);
    tmp_object.Remove(Networking::Dictionary::NumberOfFrames);

    // Create a PixelData tag
    AddPixelModule(tmp_object, 1, pixel_data->m_framelength_in_bytes, p_pixel_data);
    AddSOPCommonModule(tmp_object);
    AddGeneralImageModule(tmp_object, frame_index);
    //tmp_object.Remove(0x70790010);
    //tmp_object.Remove(0x7079104e);
    // Add all required modules from the DICOM standard
    //AddPatientModule(tmp_object);
    //AddGeneralStudyModule(tmp_object);
    //AddPatientStudyModule(tmp_object);
    //AddGeneralSeriesModule(tmp_object);
    //AddImagePixelModule(tmp_object, 1, pixel_data->m_framelength_in_bytes, p_pixel_data);
    //AddFluoroFormatPrivateTag(tmp_object);

    return owned_object;
}

//-------------------------------------------------------------------------------------------------

void DicomCreator::AddPixelModule(
    Networking::Object& object, 
    const int number_of_frames, 
    const Networking::UInt32 framelength_in_bytes, 
    const Networking::Byte* p_pixel_data
) const {
    // Create a PixelData tag
    Networking::MemoryInputBuffer buffer(
        p_pixel_data, 
        number_of_frames * framelength_in_bytes, 
        Networking::LittleEndian, 
        false
    );

    std::unique_ptr<Networking::VR> pixel_data(Networking::VR::NewPixelDataFromBuffer(
        buffer, 
        m_pixel_data_vr_type, 
        buffer.Size(), 
        number_of_frames, 
        static_cast<Networking::UInt32>(framelength_in_bytes)
    ));

    // Add the PixelData tag
    object.Add(Networking::Dictionary::PixelData, pixel_data.release());
    object.Add(Networking::Dictionary::NumberOfFrames, new Networking::IS(number_of_frames));
}
//-------------------------------------------------------------------------------------------------

