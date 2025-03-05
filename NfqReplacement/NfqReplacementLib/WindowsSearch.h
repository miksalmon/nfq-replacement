#pragma once

#include "WindowsSearch.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch>
    {
        static Windows::Foundation::Collections::IVector<WindowsSearchResultItem> GetFiles(hstring const& folderPath);
    };
}

namespace winrt::NfqReplacementLib::factory_implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch, implementation::WindowsSearch>
    {
    };
}
