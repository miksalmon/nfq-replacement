#include "pch.h"
#include "FileExplorerSort.h"
#if __has_include("FileExplorerSort.g.cpp")
#include "FileExplorerSort.g.cpp"
#endif

namespace winrt::NfqReplacementLib::implementation
{
    FileExplorerSort::FileExplorerSort(const winrt::hstring& propertyKey, bool ascending) :
        m_propertyKey(propertyKey), m_ascending(ascending)
    {
    }

    winrt::hstring FileExplorerSort::PropertyKey()
    {
        return m_propertyKey;
    }

    void FileExplorerSort::PropertyKey(const winrt::hstring& propertyKey)
    {
        m_propertyKey = propertyKey;
    }

    bool FileExplorerSort::Ascending()
    {
        return m_ascending;
    }

    void FileExplorerSort::Ascending(bool ascending)
    {
        m_ascending = ascending;
    }
}
