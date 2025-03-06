#pragma once

#include "WindowsSearchResult.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearchResult : WindowsSearchResultT<WindowsSearchResult>
    {
        WindowsSearchResult(WindowsSearchResultStatus status);
        WindowsSearchResult(WindowsSearchResultStatus status, Windows::Foundation::Collections::IVector<WindowsSearchResultItem> items);
        
        WindowsSearchResultStatus Status() const;

        Windows::Foundation::Collections::IVector<WindowsSearchResultItem> Items() const;

    private:
        WindowsSearchResultStatus m_status{WindowsSearchResultStatus::Success};
        Windows::Foundation::Collections::IVector<WindowsSearchResultItem> m_items;
    };
}

namespace winrt::NfqReplacementLib::factory_implementation
{
    struct WindowsSearchResult : WindowsSearchResultT<WindowsSearchResult, implementation::WindowsSearchResult>
    {
    };
}
