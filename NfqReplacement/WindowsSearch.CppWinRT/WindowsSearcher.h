#pragma once

#include "WindowsSearcher.g.h"

namespace winrt::WindowsSearch::implementation
{
    struct WindowsSearcher : WindowsSearcherT<WindowsSearcher>
    {
        static WindowsSearchResult GetFiles(hstring const& folderPath);
    };
}

namespace winrt::WindowsSearch::factory_implementation
{
    struct WindowsSearcher : WindowsSearcherT<WindowsSearcher, implementation::WindowsSearcher>
    {
    };
}
