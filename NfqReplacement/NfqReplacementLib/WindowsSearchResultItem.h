#pragma once

#include "WindowsSearchResultItem.g.h"

namespace winrt::NfqReplacementLib::implementation
{
    struct WindowsSearchResultItem : WindowsSearchResultItemT<WindowsSearchResultItem>
    {
		WindowsSearchResultItem() = default;

		winrt::hstring Name();
		void Name(winrt::hstring const& v);
		winrt::hstring Path();
		void Path(winrt::hstring const& v);

		winrt::hstring Kind();
		void Kind(winrt::hstring const& v);

		Windows::Foundation::DateTime ItemDate();
		void ItemDate(Windows::Foundation::DateTime const& v);

		Windows::Foundation::DateTime DateModified();
		void DateModified(Windows::Foundation::DateTime const& v);

		Windows::Foundation::DateTime DateCreated();
		void DateCreated(Windows::Foundation::DateTime const& v);

    private :
		winrt::hstring m_name;
		winrt::hstring m_path;
		winrt::hstring m_kind;
		Windows::Foundation::DateTime m_itemDate;
		Windows::Foundation::DateTime m_dateModified;
		Windows::Foundation::DateTime m_dateCreated;
    };
}

namespace winrt::NfqReplacementLib::factory_implementation
{
    struct WindowsSearchResultItem : WindowsSearchResultItemT<WindowsSearchResultItem, implementation::WindowsSearchResultItem>
    {
    };
}
