#pragma once

#include "FileExplorer.g.h"

#include <shlobj.h>

namespace winrt::WindowsSearch::implementation
{
    struct FileExplorer : FileExplorerT<FileExplorer>
    {
        static hstring GetFolder();
        static Windows::Foundation::Collections::IVector<FileExplorerSort> GetSortColumns(hstring const& folderPath);

    private:
        static winrt::hstring GetActiveFolderName(std::vector<winrt::com_ptr<IWebBrowserApp>> const& webBrowserApps);
        static winrt::com_ptr<IFolderView2> GetFolderView(std::vector<winrt::com_ptr<IWebBrowserApp>> const& webBrowserApps, const winrt::hstring& folderPath);

        static std::vector<winrt::com_ptr<IWebBrowserApp>> GetWebBrowserAppsOrderedByZOrder();
        static uint32_t GetZOrder(HWND hWnd);
        static HWND GetActiveTab(HWND hwnd);

    };
}

namespace winrt::WindowsSearch::factory_implementation
{
    struct FileExplorer : FileExplorerT<FileExplorer, implementation::FileExplorer>
    {
    };
}
