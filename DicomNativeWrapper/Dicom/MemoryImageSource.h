#pragma once
/**************************************************************************************************

    Copyright (C) 2015-2016 Toshiba Medical Systems Corporation. All rights reserved.

Module: NativeWrapper

**************************************************************************************************/

#pragma warning(push)
// Disable warnings caused by VoxarDicomSDK code
#pragma warning(disable: 4625) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning(disable: 4626) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
#pragma warning ( disable : ALL_CODE_ANALYSIS_WARNINGS )
#include "Networking/Services/StoreSCU.h" // for Networking::StoreSCU::ImageSource
#pragma warning(pop)

namespace Networking { class Object; }
namespace Networking { class UI; }

//-------------------------------------------------------------------------------------------------

namespace DicomNativeWrapper {

class MemoryImageSource sealed : public Networking::StoreSCU::ImageSource
{

private: // Non-copyable

    MemoryImageSource(const MemoryImageSource&);
    MemoryImageSource operator=(const MemoryImageSource&);

    //---------------------------------------------------------------------------------------------

public: // Types
            
    typedef std::unique_ptr<std::deque<std::unique_ptr<Networking::Object>>> OwnedObjects;
            
public: // Construction and destruction
                        
    MemoryImageSource(
        // Pass a reference to work around Visual Studio defect 
        // 777768 "Linker error passing unique_ptr to function with C++/CLI" 
        // https://connect.microsoft.com/VisualStudio/feedback/details/777768/linker-error-passing-unique-ptr-to-function-with-c-cli
        OwnedObjects& images
    );
        
    public: // Networking::StoreSCU::ImageSource
            
    std::vector<Networking::UI> SopClasses(
    ) const;
            
    int EstimatedRemaining(
    );
            
    Networking::Object* Next00(
    );
            
private: // Data
            
    OwnedObjects m_images;

};

}

//-------------------------------------------------------------------------------------------------

