#pragma once

#include "WindowsSearch.g.h"

namespace winrt::FileSystem::implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch>
    {
        static WindowsSearchResult GetFiles(hstring const& folderPath);
    };
}

namespace winrt::FileSystem::factory_implementation
{
    struct WindowsSearch : WindowsSearchT<WindowsSearch, implementation::WindowsSearch>
    {
    };
}
