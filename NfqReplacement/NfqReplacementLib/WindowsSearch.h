#pragma once

#include "WindowsSearch.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch>
    {
        static WindowsSearchResult GetFiles(hstring const& folderPath);
    };
}

namespace winrt::NfqReplacementLib::factory_implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch, implementation::WindowsSearch>
    {
    };
}
