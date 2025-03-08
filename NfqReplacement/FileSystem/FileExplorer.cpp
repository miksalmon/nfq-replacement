#include "pch.h"
#include "FileExplorer.h"
#if __has_include("FileExplorer.g.cpp")
#include "FileExplorer.g.cpp"
#endif

namespace winrt::FileSystem::implementation
{
    hstring FileExplorer::GetFolder()
    {
        std::vector<winrt::com_ptr<IWebBrowserApp>> webBrowserApps = GetWebBrowserAppsOrderedByZOrder();
        winrt::hstring folder = GetActiveFolderName(webBrowserApps);
        return folder;
    }

    Windows::Foundation::Collections::IVector<FileExplorerSort> FileExplorer::GetSortColumns(hstring const& folderPath)
    {
        std::vector<winrt::com_ptr<IWebBrowserApp>> webBrowserApps = GetWebBrowserAppsOrderedByZOrder();
        winrt::com_ptr<IFolderView2> folderView2 = GetFolderView(webBrowserApps, folderPath);

        int columnCount;
        folderView2->GetSortColumnCount(&columnCount);
        if (columnCount > 64)
        {
            columnCount = 64;
        }

        SORTCOLUMN columns[64];
        folderView2->GetSortColumns(columns, columnCount);

        Windows::Foundation::Collections::IVector<FileExplorerSort> sortColumns = winrt::single_threaded_vector<FileExplorerSort>();
        for (int i = 0; i < columnCount; i++)
        {
            bool ascending = columns[0].direction > 0 ? true : false;

            PWSTR sortProperty;
            PSGetNameFromPropertyKey(columns[0].propkey, &sortProperty);
            winrt::hstring propertyKey{ sortProperty };

            auto fileExplorerSort = FileExplorerSort(propertyKey, ascending);
            sortColumns.Append(fileExplorerSort);
        }

        return sortColumns;
    }

    winrt::hstring FileExplorer::GetActiveFolderName(std::vector<winrt::com_ptr<IWebBrowserApp>> const& webBrowserApps)
    {
        for (auto& webBrowserApp : webBrowserApps)
        {
            HWND rootHwnd;
            winrt::check_hresult(webBrowserApp->get_HWND((LONG_PTR*)&rootHwnd));
            winrt::com_ptr<IServiceProvider> serviceProvider;
            winrt::check_hresult(webBrowserApp->QueryInterface(IID_IServiceProvider, serviceProvider.put_void()));
            winrt::com_ptr<IShellBrowser> shellBrowser;
            winrt::check_hresult(serviceProvider->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, shellBrowser.put_void()));
            HWND shellBrowserHandle;
            winrt::check_hresult(shellBrowser->GetWindow(&shellBrowserHandle));
            auto activeTab = GetActiveTab(rootHwnd);
            if (activeTab == shellBrowserHandle)
            {
                winrt::com_ptr<IShellView> shellView;
                winrt::check_hresult(shellBrowser->QueryActiveShellView(shellView.put()));
                winrt::com_ptr<IFolderView> folderView;
                winrt::check_hresult(shellView->QueryInterface(IID_IFolderView, folderView.put_void()));
                winrt::com_ptr<IPersistFolder2> persistFolder;
                winrt::check_hresult(folderView->GetFolder(IID_IPersistFolder2, persistFolder.put_void()));
                LPITEMIDLIST pidlFolder;
                winrt::check_hresult(persistFolder->GetCurFolder(&pidlFolder));
                TCHAR g_szPath[MAX_PATH];
                g_szPath[0] = TEXT('\0');
                if (!SHGetPathFromIDList(pidlFolder, g_szPath))
                {
                    lstrcpyn(g_szPath, TEXT("<not a directory>"), MAX_PATH);
                }
                winrt::hstring fileExplorerFolderPath = g_szPath;
                return fileExplorerFolderPath;
            }
        }

        return L"";
    }

    winrt::com_ptr<IFolderView2> FileExplorer::GetFolderView(std::vector<winrt::com_ptr<IWebBrowserApp>> const& webBrowserApps, const winrt::hstring& folderPath)
    {
        winrt::com_ptr<IFolderView2> folderView2;

        for (auto& webBrowserApp : webBrowserApps)
        {
            HWND rootHwnd;
            winrt::check_hresult(webBrowserApp->get_HWND((LONG_PTR*)&rootHwnd));
            winrt::com_ptr<IServiceProvider> serviceProvider;
            winrt::check_hresult(webBrowserApp->QueryInterface(IID_IServiceProvider, serviceProvider.put_void()));
            winrt::com_ptr<IShellBrowser> shellBrowser;
            winrt::check_hresult(serviceProvider->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, shellBrowser.put_void()));
            HWND shellBrowserHandle;
            winrt::check_hresult(shellBrowser->GetWindow(&shellBrowserHandle));
            auto activeTab = GetActiveTab(rootHwnd);
            if (activeTab == shellBrowserHandle)
            {
                winrt::com_ptr<IShellView> shellView;
                winrt::check_hresult(shellBrowser->QueryActiveShellView(shellView.put()));
                winrt::com_ptr<IFolderView> folderView;
                winrt::check_hresult(shellView->QueryInterface(IID_IFolderView, folderView.put_void()));
                winrt::com_ptr<IPersistFolder2> persistFolder;
                winrt::check_hresult(folderView->GetFolder(IID_IPersistFolder2, persistFolder.put_void()));
                LPITEMIDLIST pidlFolder;
                winrt::check_hresult(persistFolder->GetCurFolder(&pidlFolder));
                TCHAR g_szPath[MAX_PATH];
                g_szPath[0] = TEXT('\0');
                if (!SHGetPathFromIDList(pidlFolder, g_szPath))
                {
                    lstrcpyn(g_szPath, TEXT("<not a directory>"), MAX_PATH);
                }
                winrt::hstring fileExplorerFolderPath = g_szPath;

                if (folderPath == L"")
                {
                    winrt::check_hresult(shellView->QueryInterface(IID_IFolderView2, folderView2.put_void()));
                    return folderView2;
                }

                if (folderPath == fileExplorerFolderPath)
                {
                    winrt::check_hresult(shellView->QueryInterface(IID_IFolderView2, folderView2.put_void()));
                    return folderView2;
                }
            }
        }

        return folderView2;
    }

    std::vector<winrt::com_ptr<IWebBrowserApp>> FileExplorer::GetWebBrowserAppsOrderedByZOrder()
    {
        VARIANT v;
        V_VT(&v) = VT_I4;
        winrt::com_ptr<IDispatch> dispatch;
        BOOL fFound = FALSE;
        std::vector<std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t>> webBrowserAppZOrder;
        winrt::com_ptr<IShellWindows> shellWindows;
        winrt::check_hresult(CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, IID_PPV_ARGS(&shellWindows)));
        for (V_I4(&v) = 0; !fFound && shellWindows->Item(v, dispatch.put()) == S_OK; V_I4(&v)++)
        {
            winrt::com_ptr<IDispatch> disp;
            shellWindows->Item(v, disp.put());
            winrt::com_ptr<IWebBrowserApp> webBrowserApp;
            winrt::check_hresult(disp->QueryInterface(IID_IWebBrowserApp, webBrowserApp.put_void()));
            HWND hwnd;
            webBrowserApp->get_HWND((LONG_PTR*)&hwnd);
            uint32_t zOrder = GetZOrder(hwnd);
            webBrowserAppZOrder.push_back({ webBrowserApp, zOrder });
        }

        std::sort(webBrowserAppZOrder.begin(), webBrowserAppZOrder.end(),
            [](const std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t>& a, const std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t>& b) -> bool
            {
                return a.second < b.second;
            });

        std::vector<winrt::com_ptr<IWebBrowserApp>> webBrowserAppsOrdered(webBrowserAppZOrder.size());
        std::transform(webBrowserAppZOrder.begin(), webBrowserAppZOrder.end(), webBrowserAppsOrdered.begin(),
            [](std::pair<winrt::com_ptr<IWebBrowserApp>, uint32_t> pair) { return pair.first; });

        return webBrowserAppsOrdered;
    }

    uint32_t FileExplorer::GetZOrder(HWND hWnd)
    {
        uint32_t zOrder = 0;
        for (HWND h = hWnd; h != NULL; h = GetWindow(h, GW_HWNDPREV))
        {
            zOrder++;
        }

        return zOrder;
    }

    HWND FileExplorer::GetActiveTab(HWND hwnd)
    {
        auto activeTab = FindWindowEx(hwnd, NULL, L"ShellTabWindowClass", NULL);

        if (activeTab == NULL)
        {
            activeTab = FindWindowEx(hwnd, NULL, L"TabWindowClass", NULL);
        }

        return activeTab;
    }
}
