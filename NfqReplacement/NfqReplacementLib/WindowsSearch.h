#pragma once

#include "WindowsSearch.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch>
    {
        static Windows::Foundation::Collections::IVector<hstring> GetFiles(hstring const& folderPath, hstring const& sortProperty);
    };
}

namespace winrt::NfqReplacementLib::factory_implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch, implementation::WindowsSearch>
    {
    };
}
