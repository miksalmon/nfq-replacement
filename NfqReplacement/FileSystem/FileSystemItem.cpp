#include "pch.h"
#include "FileSystemItem.h"
#if __has_include("FileSystemItem.g.cpp")
#include "FileSystemItem.g.cpp"
#endif

namespace winrt::FileSystem::implementation
{
	winrt::hstring FileSystemItem::Name() const
	{
		return m_name;
	}

	void FileSystemItem::Name(winrt::hstring const& v)
	{
		m_name = v;
	}

    winrt::hstring FileSystemItem::Path() const
    {
        return m_path;
    }

    void FileSystemItem::Path(winrt::hstring const& v)
    {
        m_path = v;
    }

	winrt::hstring FileSystemItem::Type() const
	{
		return m_type;
	}

	void FileSystemItem::Type(winrt::hstring const& v)
	{
		m_type = v;
	}

	Windows::Foundation::DateTime FileSystemItem::ItemDate() const
	{
		return m_itemDate;
	}

	void FileSystemItem::ItemDate(Windows::Foundation::DateTime const& v)
	{
		m_itemDate = v;
	}

	Windows::Foundation::DateTime FileSystemItem::DateTaken() const
	{
		return m_dateTaken;
	}

	void FileSystemItem::DateTaken(Windows::Foundation::DateTime const& v)
	{
		m_dateTaken = v;
	}

	Windows::Foundation::DateTime FileSystemItem::DateModified() const
	{
		return m_dateModified;
	}

	void FileSystemItem::DateModified(Windows::Foundation::DateTime const& v)
	{
		m_dateModified = v;
	}

	Windows::Foundation::DateTime FileSystemItem::DateCreated() const
	{
		return m_dateCreated;
	}

	void FileSystemItem::DateCreated(Windows::Foundation::DateTime const& v)
	{
		m_dateCreated = v;
	}

	uint64_t FileSystemItem::Size() const
	{
		return m_size;
	}

	void FileSystemItem::Size(uint64_t v)
	{
		m_size = v;
	}

	winrt::hstring FileSystemItem::Dimensions() const
	{
		return m_dimensions;
	}

	void FileSystemItem::Dimensions(winrt::hstring const& v)
	{
		m_dimensions = v;
	}

	Windows::Foundation::Collections::IVector<winrt::hstring> FileSystemItem::Tags() const
	{
		return m_tags;
	}

	void FileSystemItem::Tags(Windows::Foundation::Collections::IVector<winrt::hstring> const& v)
	{
		m_tags = v;
	}

	uint32_t FileSystemItem::Rating() const
	{
		return m_rating;
	}

	void FileSystemItem::Rating(uint32_t v)
	{
		m_rating = v;
	}
}
