﻿#pragma once

#include "WindowsSearchResult.g.h"

namespace winrt::FileSystem::implementation
{
    struct WindowsSearchResult : WindowsSearchResultT<WindowsSearchResult>
    {
        WindowsSearchResult(WindowsSearchResultStatus status);
        WindowsSearchResult(WindowsSearchResultStatus status, Windows::Foundation::Collections::IVector<FileSystemItem> items);
        
        WindowsSearchResultStatus Status() const;

        Windows::Foundation::Collections::IVector<FileSystemItem> Items() const;

    private:
        WindowsSearchResultStatus m_status{WindowsSearchResultStatus::Success};
        Windows::Foundation::Collections::IVector<FileSystemItem> m_items;
    };
}

namespace winrt::FileSystem::factory_implementation
{
    struct WindowsSearchResult : WindowsSearchResultT<WindowsSearchResult, implementation::WindowsSearchResult>
    {
    };
}
