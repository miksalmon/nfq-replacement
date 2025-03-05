#include "pch.h"
#include "WindowsSearch.h"
#if __has_include("WindowsSearch.g.cpp")
#include "WindowsSearch.g.cpp"
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

namespace winrt::NfqReplacementLib::implementation
{
	__declspec(selectany) CLSID CLSID_CollatorDataSource = { 0x9E175B8B, 0xF52A, 0x11D8, 0xB9, 0xA5, 0x50, 0x50, 0x54, 0x50, 0x30, 0x30 };

	// Define column indices (manually mapped to your SELECT statement order)
	constexpr DBORDINAL COL_ItemName = 1;
	constexpr DBORDINAL COL_Path = 2;
	constexpr DBORDINAL COL_Kind = 3;
	constexpr DBORDINAL COL_ItemDate = 4;
	constexpr DBORDINAL COL_DateModified = 5;
	constexpr DBORDINAL COL_DateCreated = 6;

	// Define struct for row binding
	struct RowData
	{
		WCHAR itemName[512];
		WCHAR path[1024];
		WCHAR kind[128];
		FILETIME itemDate;
		FILETIME dateModified;
		FILETIME dateCreated;
		DBSTATUS status[6]; // One status per column
	};

	IVector<WindowsSearchResultItem> WindowsSearch::GetFiles(hstring const& folderPath, hstring const& sortProperty, bool isAscending)
	{
		com_ptr<ISearchManager> searchManager;
		HRESULT hr = CoCreateInstance(CLSID_CSearchManager, nullptr, CLSCTX_ALL, IID_PPV_ARGS(searchManager.put()));
		if (FAILED(hr))
		{
			std::cerr << "Failed to create ISearchManager instance." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		com_ptr<ISearchCatalogManager> catalogManager;
		hr = searchManager->GetCatalog(L"SystemIndex", catalogManager.put());
		if (FAILED(hr))
		{
			std::cerr << "Failed to get ISearchCatalogManager." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		// Check if the indexer is ready
		CatalogStatus dwStatus;
		CatalogPausedReason dwPausedReason;
		hr = catalogManager->GetCatalogStatus(&dwStatus, &dwPausedReason);
		if (FAILED(hr) || dwStatus != CATALOG_STATUS_IDLE)
		{
			std::cerr << "Indexer is not ready. Status: " << dwStatus << ", Paused Reason: " << dwPausedReason << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		com_ptr<ISearchQueryHelper> queryHelper;
		hr = catalogManager->GetQueryHelper(queryHelper.put());
		if (FAILED(hr))
		{
			std::cerr << "Failed to get ISearchQueryHelper." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		// Initialize OLE DB
		com_ptr<IDBInitialize> dbInitialize;
		hr = CoCreateInstance(CLSID_CollatorDataSource, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dbInitialize.put()));
		if (FAILED(hr))
		{
			std::cerr << "Failed to create IDBInitialize instance." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		hr = dbInitialize->Initialize();
		if (FAILED(hr))
		{
			std::cerr << "Failed to initialize IDBInitialize." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		// Execute the query
		winrt::com_ptr<IDBCreateSession> session = dbInitialize.as<IDBCreateSession>();
		winrt::com_ptr<::IUnknown> unkSessionPtr;
		hr = session->CreateSession(0, IID_IDBCreateCommand, unkSessionPtr.put());
		if (FAILED(hr))
		{
			std::cerr << "Failed to get IDBCreateSession." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		winrt::com_ptr<IDBCreateCommand> dbCreateCommand = unkSessionPtr.as<IDBCreateCommand>();

		com_ptr<::IUnknown> unknownCommandText;
		hr = dbCreateCommand->CreateCommand(nullptr, IID_ICommandText, unknownCommandText.put());
		if (FAILED(hr))
		{
			std::cerr << "Failed to create ICommandText." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		auto isAscendingStr = isAscending ? L"ASC" : L"DESC";
		std::wstring select = L"SELECT System.ItemNameDisplay, path, System.Kind, System.ItemDate, System.DateModified, System.DateCreated ";
		std::wstring from = L"FROM SystemIndex ";
		std::wstring where = std::format(L"WHERE(DIRECTORY = 'file:{}') ", folderPath);
		std::wstring orderBy = std::format(L"ORDER BY {} {}", sortProperty, isAscendingStr);
		if (sortProperty == L"System.ItemDate")
		{
			orderBy = std::format(
				L"ORDER BY CASE WHEN System.ItemDate IS NOT NULL THEN System.ItemDate "
				L"ELSE CASE WHEN System.DateModified < System.DateCreated THEN System.DateModified ELSE System.DateCreated END END",
				sortProperty);
		}

		auto query = select + from + where + orderBy;

		com_ptr<ICommandText> commandText = unknownCommandText.as<ICommandText>();
		hr = commandText->SetCommandText(DBGUID_DEFAULT, query.c_str());
		if (FAILED(hr))
		{
			std::cerr << "Failed to set command text." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		DBROWCOUNT rowCount = 0;
		com_ptr<::IUnknown> unknownRowset;
		hr = commandText->Execute(nullptr, IID_IRowset, nullptr, &rowCount, unknownRowset.put());
		if (FAILED(hr))
		{
			std::cerr << "Failed to execute command." << std::endl;
			return single_threaded_vector<WindowsSearchResultItem>();
		}

		com_ptr<IRowset> rowset = unknownRowset.as<IRowset>();
		com_ptr<IGetRow> getRow = rowset.as<IGetRow>();

		DBCOUNTITEM rowCountReturned;
		IVector<WindowsSearchResultItem> results = single_threaded_vector<WindowsSearchResultItem>();

		do
		{
			HROW rowBuffer[5000] = {}; // Request enough large batch to increase efficiency
			HROW* rowReturned = rowBuffer;
			hr = rowset->GetNextRows(DB_NULL_HCHAPTER, 0, ARRAYSIZE(rowBuffer), &rowCountReturned, &rowReturned);
			if (FAILED(hr))
			{
				std::cerr << "Failed to get next rows." << std::endl;
				return single_threaded_vector<WindowsSearchResultItem>();
			}


			// Setup bindings explicitly
			/*DBBINDING bindings[] = {
			{ COL_ItemName, offsetof(RowData, itemName), sizeof(std::wstring), DBTYPE_WSTR },
			{ COL_Path, offsetof(RowData, path), sizeof(std::wstring), DBTYPE_WSTR },
			{ COL_Kind, offsetof(RowData, kind), sizeof(std::wstring), DBTYPE_WSTR },
			{ COL_ItemDate, offsetof(RowData, itemDate), sizeof(FILETIME), DBTYPE_FILETIME },
			{ COL_DateModified, offsetof(RowData, dateModified), sizeof(FILETIME), DBTYPE_FILETIME },
			{ COL_DateCreated, offsetof(RowData, dateCreated), sizeof(FILETIME), DBTYPE_FILETIME },
			};*/
			DBBINDING bindings[6] = {};

            // Define bindings for each column
            bindings[0].iOrdinal = 1; // System.ItemName
            bindings[0].obValue = offsetof(RowData, itemName);
            bindings[0].dwPart = DBPART_VALUE;
            bindings[0].wType = DBTYPE_WSTR;
            bindings[0].cbMaxLen = sizeof(WCHAR[512]);

            bindings[1].iOrdinal = 2; // path
            bindings[1].obValue = offsetof(RowData, path);
            bindings[1].dwPart = DBPART_VALUE;
            bindings[1].wType = DBTYPE_WSTR;
            bindings[1].cbMaxLen = sizeof(WCHAR[1024]);

            bindings[2].iOrdinal = 3; // System.Kind
            bindings[2].obValue = offsetof(RowData, kind);
            bindings[2].dwPart = DBPART_VALUE;
            bindings[2].wType = DBTYPE_WSTR;
			bindings[2].cbMaxLen = sizeof(WCHAR[128]);

            bindings[3].iOrdinal = 4; // System.ItemDate
            bindings[3].obValue = offsetof(RowData, itemDate);
            bindings[3].dwPart = DBPART_VALUE;
            bindings[3].wType = DBTYPE_FILETIME;

            bindings[4].iOrdinal = 5; // System.DateModified
            bindings[4].obValue = offsetof(RowData, dateModified);
            bindings[4].dwPart = DBPART_VALUE;
            bindings[4].wType = DBTYPE_FILETIME;

            bindings[5].iOrdinal = 6; // System.DateCreated
            bindings[5].obValue = offsetof(RowData, dateCreated);
            bindings[5].dwPart = DBPART_VALUE;
            bindings[5].wType = DBTYPE_FILETIME;

			// Create accessor
			com_ptr<IAccessor> accessor = rowset.as<IAccessor>();

			HACCESSOR hAccessor = NULL;
			hr = accessor->CreateAccessor(DBACCESSOR_ROWDATA, ARRAYSIZE(bindings), bindings, sizeof(RowData), &hAccessor, nullptr);
			if (FAILED(hr))
			{
				std::cerr << "Failed to create accessor." << std::endl;
				return single_threaded_vector<WindowsSearchResultItem>();
			}

			// Fetch rows
			for (int i = 0; i < rowCountReturned; i++)
			{
				RowData rowData = {};
				hr = rowset->GetData(rowBuffer[i], hAccessor, &rowData);
				if (FAILED(hr))
				{
					std::cerr << "Failed to get data from rowset." << std::endl;
					continue;
				}

				WindowsSearchResultItem resultItem = WindowsSearchResultItem();

				// Convert and store values
				if (rowData.status[0] != DBSTATUS_S_ISNULL) resultItem.Name(winrt::hstring(rowData.itemName));
				if (rowData.status[1] != DBSTATUS_S_ISNULL) resultItem.Path(winrt::hstring(rowData.path));
				if (rowData.status[2] != DBSTATUS_S_ISNULL) resultItem.Kind(winrt::hstring(rowData.kind));
				if (rowData.status[3] != DBSTATUS_S_ISNULL) resultItem.ItemDate(winrt::clock::from_FILETIME(rowData.itemDate));
				if (rowData.status[4] != DBSTATUS_S_ISNULL) resultItem.DateModified(winrt::clock::from_FILETIME(rowData.dateModified));
				if (rowData.status[5] != DBSTATUS_S_ISNULL) resultItem.DateCreated(winrt::clock::from_FILETIME(rowData.dateCreated));

				results.Append(resultItem);
			}

			// Cleanup
			accessor->ReleaseAccessor(hAccessor, nullptr);
			

			rowset->ReleaseRows(rowCountReturned, rowReturned, nullptr, nullptr, nullptr);
		} while (rowCountReturned > 0);

		return results;
	}


}
