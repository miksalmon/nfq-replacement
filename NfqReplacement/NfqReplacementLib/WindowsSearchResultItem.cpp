#include "pch.h"
#include "WindowsSearchResultItem.h"
#if __has_include("WindowsSearchResultItem.g.cpp")
#include "WindowsSearchResultItem.g.cpp"
#endif

namespace winrt::NfqReplacementLib::implementation
{
	winrt::hstring WindowsSearchResultItem::Name()
	{
		return m_name;
	}

	void WindowsSearchResultItem::Name(winrt::hstring const& v)
	{
		m_name = v;
	}

	winrt::hstring WindowsSearchResultItem::Path()
	{
		return m_path;
	}

	void WindowsSearchResultItem::Path(winrt::hstring const& v)
	{
		m_path = v;
	}

	winrt::hstring WindowsSearchResultItem::Kind()
	{
		return m_kind;
	}

	void WindowsSearchResultItem::Kind(winrt::hstring const& v)
	{
		m_kind = v;
	}

	Windows::Foundation::DateTime WindowsSearchResultItem::ItemDate()
	{
		return m_itemDate;
	}

	void WindowsSearchResultItem::ItemDate(Windows::Foundation::DateTime const& v)
	{
		m_itemDate = v;
	}

	Windows::Foundation::DateTime WindowsSearchResultItem::DateModified()
	{
		return m_dateModified;
	}

	void WindowsSearchResultItem::DateModified(Windows::Foundation::DateTime const& v)
	{
		m_dateModified = v;
	}

	Windows::Foundation::DateTime WindowsSearchResultItem::DateCreated()
	{
		return m_dateCreated;
	}

	void WindowsSearchResultItem::DateCreated(Windows::Foundation::DateTime const& v)
	{
		m_dateCreated = v;
	}
}
