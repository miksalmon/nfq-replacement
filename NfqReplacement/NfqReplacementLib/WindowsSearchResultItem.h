#pragma once

#include "WindowsSearchResultItem.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearchResultItem : WindowsSearchResultItemT<WindowsSearchResultItem>
    {
        WindowsSearchResultItem() = default;

        winrt::hstring Name() const;
        void Name(winrt::hstring const& v);

        winrt::hstring Type() const;
        void Type(winrt::hstring const& v);

        Windows::Foundation::DateTime ItemDate() const;
        void ItemDate(Windows::Foundation::DateTime const& v);

        Windows::Foundation::DateTime DateTaken() const;
        void DateTaken(Windows::Foundation::DateTime const& v);

        Windows::Foundation::DateTime DateModified() const;
        void DateModified(Windows::Foundation::DateTime const& v);

        Windows::Foundation::DateTime DateCreated() const;
        void DateCreated(Windows::Foundation::DateTime const& v);

        uint64_t Size() const;
        void Size(uint64_t v);

        winrt::hstring Dimensions() const;
        void Dimensions(winrt::hstring const& v);

        Windows::Foundation::Collections::IVector<winrt::hstring> Tags() const;
        void Tags(Windows::Foundation::Collections::IVector<winrt::hstring> const& v);

        uint32_t Rating() const;
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
