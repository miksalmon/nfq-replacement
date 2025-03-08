#pragma once

#include "FileExplorerSort.g.h"

namespace winrt::FileSystem::implementation
{
    struct FileExplorerSort : FileExplorerSortT<FileExplorerSort>
    {
        FileExplorerSort(const winrt::hstring& propertyKey, bool ascending);

        winrt::hstring PropertyKey();
        void PropertyKey(const winrt::hstring& propertyKey);

        bool Ascending();
        void Ascending(bool ascending);

    private:
        winrt::hstring m_propertyKey;
        bool m_ascending{ true };
    };
}

namespace winrt::FileSystem::factory_implementation
{
    struct FileExplorerSort : FileExplorerSortT<FileExplorerSort, implementation::FileExplorerSort>
    {
    };
}
