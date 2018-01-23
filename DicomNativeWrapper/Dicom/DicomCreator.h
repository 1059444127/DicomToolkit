#pragma once
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: dicom_creator_lib

**************************************************************************************************/

//-------------------------------------------------------------------------------------------------

class DicomCreator {

    //---------------------------------------------------------------------------------------------

private: // Non-copyable

    DicomCreator(const DicomCreator&);
    DicomCreator& operator=(const DicomCreator&);

    //---------------------------------------------------------------------------------------------

public: // Types

    enum IOD {
        eStandardCT = 0, 
        eEnhancedCT, 
        eStandardMR, 
        eEnhancedMR, 
        eXRayAngiographic, 
        eXRay3DAngiographic, 
        eUltrasoundEmbedded, 
        eUltrasoundNonEmbedded, 
        eFluoroscopy, 
    };

    typedef std::unique_ptr<std::deque<std::unique_ptr<Networking::Object>>> OwnedObjects;

    //---------------------------------------------------------------------------------------------

public: // Construction

    explicit DicomCreator(IOD);

    //---------------------------------------------------------------------------------------------

public: // Data

    const IOD m_iod;
    const std::unique_ptr<Networking::UI> m_study_instance_uid;
    const std::unique_ptr<Networking::UI> m_series_instance_uid;
    int m_number_of_frames;
    Networking::UInt16 m_rows;
    Networking::UInt16 m_columns;
    Networking::UInt16 m_pixel_data_vr_type;
    std::wstring m_patient_id;
    std::wstring m_study_id;
    std::wstring m_patients_birth_date;
    std::wstring m_patients_age;
    std::wstring m_patients_sex;
    std::wstring m_study_date;
    std::wstring m_patients_name;
    std::wstring m_patient_position;
    bool m_has_volume_private_tag;
    float m_volume_private_tag_bed_latitude;
    float m_volume_private_tag_bed_longitude;
    float m_volume_private_tag_bed_height;
    float m_volume_private_tag_ceiling_latitude;
    float m_volume_private_tag_ceiling_longitude;
    int m_volume_private_tag_arm_floor_position;
    bool m_tilted;
    bool m_uneven_spacing;
    bool m_view_from_head;

    std::wstring m_modality;
    Networking::UInt16 m_bits_allocated;
    bool m_pixel_representation_signed;    
    const Networking::UI* m_sop_class_uid;
    std::wstring m_volumetric_properties;

    std::string m_encoded_patient_id;    
    std::string m_encoded_study_description;
    std::wstring m_study_description;        
    std::string m_encoded_series_description;
    std::wstring m_series_description;        
    std::wstring m_specific_character_set;    
    
    //---------------------------------------------------------------------------------------------

public: // Functions

    // Methods to make DICOM objects
    OwnedObjects NewObjects() const;

    std::wstring GetFormattedPatientsBirthDate() const;
    std::wstring DicomCreator::GetFormattedStudyDate() const;

    static std::wstring FormatDateString(
        std::wstring in
    );

    std::unique_ptr<Networking::Object> NewFluoroObject(
        Networking::Object& owned_object,
        int frame_index
    );

private: // Types

    struct PixelData {
        Networking::UInt32 m_framelength_in_bytes;
        std::vector<Networking::Byte> m_pixel_data;
    };

    //---------------------------------------------------------------------------------------------

private: // Functions

    template<typename T>
    std::unique_ptr<PixelData> MakePixelData() const;

    static OwnedObjects MakeOwnedObjects(int count);
    std::unique_ptr<Networking::Object> MakeOwnedObject();

    // Methods to add tags from a particular module of the DICOM standard
    // These methods deliberately copy the structure of the DICOM standard
    void AddPatientModule(Networking::Object&) const;
    void AddGeneralStudyModule(Networking::Object&) const;
    void AddPatientStudyModule(Networking::Object&) const;
    void AddGeneralSeriesModule(Networking::Object&) const;
    static void AddGeneralImageModule(Networking::Object&, int instance_number);
    static void AddImagePlaneModule(
        Networking::Object&, 
        int index, 
        bool tilted,
        bool uneven_spacing,
        bool view_from_head,
        int num_rows
    );

    void AddPixelModule(
        Networking::Object& object,
        const int number_of_frames,
        const Networking::UInt32 framelength_in_bytes,
        const Networking::Byte* p_pixel_data
    )const;

    void AddImagePixelModule(
        Networking::Object&, 
        int number_of_frames, 
        Networking::UInt32 framelength_in_bytes, 
        const Networking::Byte* p_pixel_data
    ) const;
    void AddMultiframeFunctionalGroupsModule(
        Networking::Object&, 
        bool view_from_head,
        int num_rows
    ) const;
    static void AddPixelMeasuresMacro(Networking::Object&);
    static void AddPlaneOrientationPatientMacro(Networking::Object&, bool view_from_head);
    static void AddPlanePositionPatientMacro(
        Networking::Object&, 
        int frame,
        bool view_from_head,
        int num_rows
    );
    void AddSpecialisedImageModule(Networking::Object&) const;
    void AddCommonCTMRImageDescriptionMacro(Networking::Object&) const;
    void AddSOPCommonModule(Networking::Object&) const;
    static void AddFluoroFormatPrivateTag(Networking::Object&);
    void AddVolumeFormatPrivateTag(Networking::Object&) const;

    // Method to add a string tag using either its encoded or unencoded value, and check for failure
    void CreateStringTag(
        Networking::Object& object, 
        Networking::UInt32 tag_id, 
        const std::string& encoded_string, 
        const std::wstring& string
    ) const;

    // Method to add a string tag using its encoded value
    static void CreateStringTag(
        Networking::Object& object, 
        Networking::UInt32 tag_id, 
        const std::string& encoded_string
    );

    // Method to add a string tag, and check for failure
    static void CreateStringTag(
        Networking::Object& object, 
        Networking::UInt32 tag_id, 
        const std::wstring& string
    );
};

//-------------------------------------------------------------------------------------------------
