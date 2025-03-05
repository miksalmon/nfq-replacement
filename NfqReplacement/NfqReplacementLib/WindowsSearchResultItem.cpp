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

	winrt::hstring WindowsSearchResultItem::Type()
	{
		return m_type;
	}

	void WindowsSearchResultItem::Type(winrt::hstring const& v)
	{
		m_type = v;
	}

	Windows::Foundation::DateTime WindowsSearchResultItem::ItemDate()
	{
		return m_itemDate;
	}

	void WindowsSearchResultItem::ItemDate(Windows::Foundation::DateTime const& v)
	{
		m_itemDate = v;
	}

	Windows::Foundation::DateTime WindowsSearchResultItem::DateTaken()
	{
		return m_dateTaken;
	}

	void WindowsSearchResultItem::DateTaken(Windows::Foundation::DateTime const& v)
	{
		m_dateTaken = v;
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

	uint64_t WindowsSearchResultItem::Size()
	{
		return m_size;
	}

	void WindowsSearchResultItem::Size(uint64_t v)
	{
		m_size = v;
	}

	winrt::hstring WindowsSearchResultItem::Dimensions()
	{
		return m_dimensions;
	}

	void WindowsSearchResultItem::Dimensions(winrt::hstring const& v)
	{
		m_dimensions = v;
	}

	Windows::Foundation::Collections::IVector<winrt::hstring> WindowsSearchResultItem::Tags()
	{
		return m_tags;
	}

	void WindowsSearchResultItem::Tags(Windows::Foundation::Collections::IVector<winrt::hstring> const& v)
	{
		m_tags = v;
	}

	uint32_t WindowsSearchResultItem::Rating()
	{
		return m_rating;
	}

	void WindowsSearchResultItem::Rating(uint32_t v)
	{
		m_rating = v;
	}
}
