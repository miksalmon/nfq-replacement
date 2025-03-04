#include "pch.h"
#include "WindowsSearch.h"
#if __has_include("WindowsSearch.g.cpp")
#include "WindowsSearch.g.cpp"
#endif

namespace winrt::NfqReplacementLib::implementation
{
    Windows::Foundation::Collections::IVector<hstring> WindowsSearch::GetFiles(hstring const& /*folderPath*/, hstring const& /*sortProperty*/)
    {
        auto files = winrt::single_threaded_vector<hstring>();
        files.Append(L"file1.txt");
        return files;
    }
}
