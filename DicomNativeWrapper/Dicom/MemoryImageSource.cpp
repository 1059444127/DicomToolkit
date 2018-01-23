#include "stdafx.h"
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/
#include "MemoryImageSource.h"

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4263) // 'function': member function does not override any base class virtual member function
#pragma warning(disable: 4264) // 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "Networking/Message/Dictionary.h"
#include "Networking/Message/Object.h"
#include "Networking/Message/UI.h"
#pragma warning(pop)

using namespace DicomNativeWrapper;

//-------------------------------------------------------------------------------------------------
    
MemoryImageSource::MemoryImageSource(
    OwnedObjects& images
) : 
    m_images(std::move(images))
{
    if (nullptr == m_images) {
        throw PvBase::Log(PvBase::Log::ARGUMENT_NULL_EXCEPTION) << __FUNCTION__ << ": Null argument" << PvBase::Break();
    }
}
    
//-------------------------------------------------------------------------------------------------

std::vector<Networking::UI> MemoryImageSource::SopClasses(
) const {
    std::set<Networking::UI> sop_classes;
    for (auto image = m_images->cbegin(); m_images->cend() != image; ++image) {
        sop_classes.insert((*image)->GetUI(Networking::Dictionary::SOPClassUID));
    }

    return std::vector<Networking::UI>(sop_classes.cbegin(), sop_classes.cend());
}
    
//-------------------------------------------------------------------------------------------------

int MemoryImageSource::EstimatedRemaining(
) {
    return static_cast<int>(m_images->size());
}
    
//-------------------------------------------------------------------------------------------------

Networking::Object* MemoryImageSource::Next00(
) {
    if (m_images->empty()) {
        return nullptr;
    }

    auto image = std::move(m_images->front());
    m_images->pop_front();

    return image.release();
}

//-------------------------------------------------------------------------------------------------

