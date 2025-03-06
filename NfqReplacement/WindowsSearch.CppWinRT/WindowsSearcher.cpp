#include "pch.h"
#include "WindowsSearcher.h"
#if __has_include("WindowsSearcher.g.cpp")
#include "WindowsSearcher.g.cpp"
#endif

#include <winrt/base.h>
#include <windows.h>
#include <searchapi.h>
#include <iostream>
#include <propsys.h>
#include <propkey.h>
#include <string>
#include <format>
#include <atldbcli.h> // For DBTYPE constants

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::WindowsSearch::implementation
{
    __declspec(selectany) CLSID CLSID_CollatorDataSource = { 0x9E175B8B, 0xF52A, 0x11D8, 0xB9, 0xA5, 0x50, 0x50, 0x54, 0x50, 0x30, 0x30 };

    WindowsSearchResult WindowsSearcher::GetFiles(hstring const& folderPath)
    {
        com_ptr<ISearchManager> searchManager;
        HRESULT hr = CoCreateInstance(CLSID_CSearchManager, nullptr, CLSCTX_ALL, IID_PPV_ARGS(searchManager.put()));
        if (FAILED(hr))
        {
            std::cerr << "Failed to create ISearchManager instance." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        com_ptr<ISearchCatalogManager> catalogManager;
        hr = searchManager->GetCatalog(L"SystemIndex", catalogManager.put());
        if (FAILED(hr))
        {
            std::cerr << "Failed to get ISearchCatalogManager." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        com_ptr<ISearchCrawlScopeManager> crawlScopeManager;
        hr = catalogManager->GetCrawlScopeManager(crawlScopeManager.put());
        if (FAILED(hr)) {
            std::cerr << "Failed to get CrawlScopeManager. Error: " << hr << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        // Check if the folder is included in the index
        BOOL isIncluded = FALSE;
        hr = crawlScopeManager->IncludedInCrawlScope(folderPath.c_str(), &isIncluded);

        if (FAILED(hr)) {
            std::cerr << "Error checking crawl scope: " << hr << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }
		// Check if the folder is being indexed
		if (!isIncluded)
		{
			std::cerr << "Folder is not indexed." << std::endl;
			return WindowsSearchResult(WindowsSearchResultStatus::FolderNotIndexed);
		}

        com_ptr<ISearchQueryHelper> queryHelper;
        hr = catalogManager->GetQueryHelper(queryHelper.put());
        if (FAILED(hr))
        {
            std::cerr << "Failed to get ISearchQueryHelper." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        // Initialize OLE DB
        com_ptr<IDBInitialize> dbInitialize;
        hr = CoCreateInstance(CLSID_CollatorDataSource, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dbInitialize.put()));
        if (FAILED(hr))
        {
            std::cerr << "Failed to create IDBInitialize instance." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        hr = dbInitialize->Initialize();
        if (FAILED(hr))
        {
            std::cerr << "Failed to initialize IDBInitialize." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        // Execute the query
        winrt::com_ptr<IDBCreateSession> session = dbInitialize.as<IDBCreateSession>();
        winrt::com_ptr<::IUnknown> unkSessionPtr;
        hr = session->CreateSession(0, IID_IDBCreateCommand, unkSessionPtr.put());
        if (FAILED(hr))
        {
            std::cerr << "Failed to get IDBCreateSession." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        winrt::com_ptr<IDBCreateCommand> dbCreateCommand = unkSessionPtr.as<IDBCreateCommand>();

        com_ptr<::IUnknown> unknownCommandText;
        hr = dbCreateCommand->CreateCommand(nullptr, IID_ICommandText, unknownCommandText.put());
        if (FAILED(hr))
        {
            std::cerr << "Failed to create ICommandText." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        std::wstring select = L"SELECT System.ItemNameDisplay, System.ItemPathDisplay, System.ItemType, System.ItemDate, System.Photo.DateTaken, System.DateCreated, System.DateModified, System.Size, System.Image.Dimensions, System.Keywords, System.Rating ";
        std::wstring from = L"FROM SystemIndex ";
        std::wstring where = std::format(L"WHERE(DIRECTORY = 'file:{}') ", folderPath);

        auto query = select + from + where;
        std::wcout << L"Query: " << query << std::endl;

        com_ptr<ICommandText> commandText = unknownCommandText.as<ICommandText>();
        hr = commandText->SetCommandText(DBGUID_DEFAULT, query.c_str());
        if (FAILED(hr))
        {
            std::cerr << "Failed to set command text." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        DBROWCOUNT rowCount = 0;
        com_ptr<::IUnknown> unknownRowset;
        hr = commandText->Execute(nullptr, IID_IRowset, nullptr, &rowCount, unknownRowset.put());
        if (FAILED(hr))
        {
            std::cerr << "Failed to execute command." << std::endl;
            return WindowsSearchResult(WindowsSearchResultStatus::UnknownError);
        }

        com_ptr<IRowset> rowset = unknownRowset.as<IRowset>();
        com_ptr<IGetRow> getRow = rowset.as<IGetRow>();

        DBCOUNTITEM rowCountReturned;
        IVector<FileSystemItem> results = single_threaded_vector<FileSystemItem>();

        do
        {
            HROW rowBuffer[5000] = {}; // Request enough large batch to increase efficiency
            HROW* rowReturned = rowBuffer;
            hr = rowset->GetNextRows(DB_NULL_HCHAPTER, 0, ARRAYSIZE(rowBuffer), &rowCountReturned, &rowReturned);
            if (FAILED(hr))
            {
                std::cerr << "Failed to get next rows." << std::endl;
                return WindowsSearchResult(WindowsSearchResultStatus::Incomplete, results);
            }

            for (int i = 0; i < rowCountReturned; i++)
            {
                com_ptr<::IUnknown> pUnkRow;
                hr = getRow->GetRowFromHROW(nullptr, rowBuffer[i], IID_IPropertyStore, pUnkRow.put());
                if (FAILED(hr))
                {
                    std::cerr << "Failed to get row from HROW." << std::endl;
                    return WindowsSearchResult(WindowsSearchResultStatus::Incomplete, results);
                }

                com_ptr<IPropertyStore> propertyStore = pUnkRow.as<IPropertyStore>();

                PROPVARIANT namePropVar;
                PropVariantInit(&namePropVar);
                hr = propertyStore->GetValue(PKEY_ItemNameDisplay, &namePropVar);

                PROPVARIANT pathPropVar;
                PropVariantInit(&pathPropVar);
                hr = propertyStore->GetValue(PKEY_ItemPathDisplay, &pathPropVar);

                PROPVARIANT typePropVar;
                PropVariantInit(&typePropVar);
                hr = propertyStore->GetValue(PKEY_ItemType, &typePropVar);

                PROPVARIANT itemDatePropVar;
                PropVariantInit(&itemDatePropVar);
                hr = propertyStore->GetValue(PKEY_ItemDate, &itemDatePropVar);

                PROPVARIANT dateTakenPropVar;
                PropVariantInit(&dateTakenPropVar);
                hr = propertyStore->GetValue(PKEY_Photo_DateTaken, &dateTakenPropVar);

                PROPVARIANT dateCreatedPropVar;
                PropVariantInit(&dateCreatedPropVar);
                hr = propertyStore->GetValue(PKEY_DateCreated, &dateCreatedPropVar);

                PROPVARIANT dateModifiedPropVar;
                PropVariantInit(&dateModifiedPropVar);
                hr = propertyStore->GetValue(PKEY_DateModified, &dateModifiedPropVar);

                PROPVARIANT sizePropVar;
                PropVariantInit(&sizePropVar);
                hr = propertyStore->GetValue(PKEY_Size, &sizePropVar);

                PROPVARIANT dimensionsPropVar;
                PropVariantInit(&dimensionsPropVar);
                hr = propertyStore->GetValue(PKEY_Image_Dimensions, &dimensionsPropVar);

                PROPVARIANT tagsPropVar;
                PropVariantInit(&tagsPropVar);
                hr = propertyStore->GetValue(PKEY_Keywords, &tagsPropVar);

                PROPVARIANT ratingPropVar;
                PropVariantInit(&ratingPropVar);
                hr = propertyStore->GetValue(PKEY_Rating, &ratingPropVar);

                auto item = FileSystemItem();
                item.Name(winrt::hstring(namePropVar.pwszVal));
                item.Path(winrt::hstring(pathPropVar.pwszVal));
                item.Type(winrt::hstring(typePropVar.pwszVal));
                item.ItemDate(winrt::clock::from_FILETIME(itemDatePropVar.filetime));
                item.DateTaken(winrt::clock::from_FILETIME(dateTakenPropVar.filetime));
                item.DateCreated(winrt::clock::from_FILETIME(dateCreatedPropVar.filetime));
                item.DateModified(winrt::clock::from_FILETIME(dateModifiedPropVar.filetime));
                item.Size(sizePropVar.uhVal.QuadPart);
                item.Dimensions(winrt::hstring(dimensionsPropVar.pwszVal != nullptr ? dimensionsPropVar.pwszVal : L""));	// TODO: fix ? in dimensions text

                IVector tagsVector = winrt::single_threaded_vector<winrt::hstring>();
                for (ULONG j = 0; j < tagsPropVar.calpwstr.cElems; j++) {
                    auto tag = tagsPropVar.calpwstr.pElems[j];
                    tagsVector.Append(winrt::hstring(tag));
                }
                item.Tags(tagsVector);
                item.Rating(ratingPropVar.uintVal);

                results.Append(item);

                PropVariantClear(&namePropVar);
                PropVariantClear(&pathPropVar);
                PropVariantClear(&typePropVar);
                PropVariantClear(&itemDatePropVar);
                PropVariantClear(&dateTakenPropVar);
                PropVariantClear(&dateCreatedPropVar);
                PropVariantClear(&dateModifiedPropVar);
                PropVariantClear(&sizePropVar);
                PropVariantClear(&dimensionsPropVar);
                PropVariantClear(&tagsPropVar);
                PropVariantClear(&ratingPropVar);
            }			

            rowset->ReleaseRows(rowCountReturned, rowReturned, nullptr, nullptr, nullptr);
        } while (rowCountReturned > 0);

        return WindowsSearchResult(WindowsSearchResultStatus::Success, results);
    }
}
