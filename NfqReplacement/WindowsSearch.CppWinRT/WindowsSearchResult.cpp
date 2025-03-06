#include "pch.h"
#include "WindowsSearchResult.h"
#if __has_include("WindowsSearchResult.g.cpp")
#include "WindowsSearchResult.g.cpp"
#endif

namespace winrt::WindowsSearch::implementation
{
    WindowsSearchResult::WindowsSearchResult(WindowsSearchResultStatus status)
    {
        m_status = status;
        m_items = single_threaded_vector<WindowsSearchResultItem>();
    }

    WindowsSearchResult::WindowsSearchResult(WindowsSearchResultStatus status, Windows::Foundation::Collections::IVector<WindowsSearchResultItem> items)
    {
        m_status = status;
        m_items = items;
    }

    WindowsSearchResultStatus WindowsSearchResult::Status() const
    {
        return m_status;
    }

    Windows::Foundation::Collections::IVector<WindowsSearchResultItem> WindowsSearchResult::Items() const
    {
        return m_items;
    }
}
