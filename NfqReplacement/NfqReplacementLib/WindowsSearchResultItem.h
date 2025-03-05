#pragma once

#include "WindowsSearchResultItem.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearchResultItem : WindowsSearchResultItemT<WindowsSearchResultItem>
    {
        WindowsSearchResultItem() = default;

        winrt::hstring Name();
        void Name(winrt::hstring const& v);

        winrt::hstring Type();
        void Type(winrt::hstring const& v);

        Windows::Foundation::DateTime ItemDate();
        void ItemDate(Windows::Foundation::DateTime const& v);

        Windows::Foundation::DateTime DateTaken();
        void DateTaken(Windows::Foundation::DateTime const& v);

        Windows::Foundation::DateTime DateModified();
        void DateModified(Windows::Foundation::DateTime const& v);

        Windows::Foundation::DateTime DateCreated();
        void DateCreated(Windows::Foundation::DateTime const& v);

        uint64_t Size();
        void Size(uint64_t v);

        winrt::hstring Dimensions();
        void Dimensions(winrt::hstring const& v);

        Windows::Foundation::Collections::IVector<winrt::hstring> Tags();
        void Tags(Windows::Foundation::Collections::IVector<winrt::hstring> const& v);

        uint32_t Rating();
        void Rating(uint32_t v);

    private:
        winrt::hstring m_name;
        winrt::hstring m_type;
        Windows::Foundation::DateTime m_itemDate;
        Windows::Foundation::DateTime m_dateModified;
        Windows::Foundation::DateTime m_dateCreated;
        Windows::Foundation::Collections::IVector<winrt::hstring> m_tags;
        uint64_t m_size;
        Windows::Foundation::DateTime m_dateTaken;
        winrt::hstring m_dimensions;
        uint32_t m_rating;
    };
}

namespace winrt::NfqReplacementLib::factory_implementation
{
    struct WindowsSearchResultItem : WindowsSearchResultItemT<WindowsSearchResultItem, implementation::WindowsSearchResultItem>
    {
    };
}
