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

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace winrt::NfqReplacementLib::implementation
{
	__declspec(selectany) CLSID CLSID_CollatorDataSource = { 0x9E175B8B, 0xF52A, 0x11D8, 0xB9, 0xA5, 0x50, 0x50, 0x54, 0x50, 0x30, 0x30 };

	IVector<hstring> WindowsSearch::GetFiles(hstring const& /*folderPath*/, hstring const& /*sortProperty*/)
	{
		IVector<hstring> results = single_threaded_vector<hstring>();
		results.Append(L"coucou.txt");

		return results;
		//com_ptr<ISearchManager> searchManager;
		//HRESULT hr = CoCreateInstance(CLSID_CSearchManager, nullptr, CLSCTX_ALL, IID_PPV_ARGS(searchManager.put()));
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to create ISearchManager instance." << std::endl;
		//	return single_threaded_vector<hstring>();
		//}

		//com_ptr<ISearchCatalogManager> catalogManager;
		//hr = searchManager->GetCatalog(L"SystemIndex", catalogManager.put());
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to get ISearchCatalogManager." << std::endl;
		//	return single_threaded_vector<hstring>();
		//}

		//// Check if the indexer is ready
		//CatalogStatus dwStatus;
		//CatalogPausedReason dwPausedReason;
		//hr = catalogManager->GetCatalogStatus(&dwStatus, &dwPausedReason);
		//if (FAILED(hr) || dwStatus != CATALOG_STATUS_IDLE)
		//{
		//	std::cerr << "Indexer is not ready. Status: " << dwStatus << ", Paused Reason: " << dwPausedReason << std::endl;
		//	return single_threaded_vector<hstring>();
		//}

		//com_ptr<ISearchQueryHelper> queryHelper;
		//hr = catalogManager->GetQueryHelper(queryHelper.put());
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to get ISearchQueryHelper." << std::endl;
		//	return single_threaded_vector<hstring>();
		//}

		//LPWSTR pwszQuery = nullptr;
		//std::wstring query = L"SELECT System.ItemName FROM SystemIndex WHERE System.ItemPathDisplay LIKE '" + std::wstring(folderPath) + L"\\*' ORDER BY " + std::wstring(sortProperty);
		//hr = queryHelper->GenerateSQLFromUserQuery(query.c_str(), &pwszQuery);
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to generate SQL query." << std::endl;
		//	return single_threaded_vector<hstring>();
		//}

		//// Initialize OLE DB
		//com_ptr<IDBInitialize> dbInitialize;
		//hr = CoCreateInstance(CLSID_CollatorDataSource, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dbInitialize.put()));
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to create IDBInitialize instance." << std::endl;
		//	CoTaskMemFree(pwszQuery);
		//	return single_threaded_vector<hstring>();
		//}

		//hr = dbInitialize->Initialize();
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to initialize IDBInitialize." << std::endl;
		//	CoTaskMemFree(pwszQuery);
		//	return single_threaded_vector<hstring>();
		//}

		//// Execute the query
		//com_ptr<IDBCreateCommand> dbCreateCommand;
		//hr = dbInitialize->QueryInterface(IID_PPV_ARGS(dbCreateCommand.put()));
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to get IDBCreateCommand." << std::endl;
		//	CoTaskMemFree(pwszQuery);
		//	return single_threaded_vector<hstring>();
		//}

		//com_ptr<::IUnknown> unknownCommandText;
		//hr = dbCreateCommand->CreateCommand(nullptr, IID_ICommandText ,unknownCommandText.put());
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to create ICommandText." << std::endl;
		//	CoTaskMemFree(pwszQuery);
		//	return single_threaded_vector<hstring>();
		//}

		//com_ptr<ICommandText> commandText = unknownCommandText.as<ICommandText>();
		//hr = commandText->SetCommandText(DBGUID_DEFAULT, pwszQuery);
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to set command text." << std::endl;
		//	CoTaskMemFree(pwszQuery);
		//	return single_threaded_vector<hstring>();
		//}

		//DBROWCOUNT rowCount = 0;
		//com_ptr<::IUnknown> unknownRowset;
		//hr = commandText->Execute(nullptr, IID_IRowset,nullptr, &rowCount, unknownRowset.put());
		//CoTaskMemFree(pwszQuery);
		//if (FAILED(hr))
		//{
		//	std::cerr << "Failed to execute command." << std::endl;
		//	return single_threaded_vector<hstring>();
		//}
		//com_ptr<IRowset> rowset = unknownRowset.as<IRowset>();
		//com_ptr<IGetRow> getRow = rowset.as<IGetRow>();
		//// Process the results
		//IVector<hstring> results = single_threaded_vector<hstring>();

  //      DBCOUNTITEM rowCountReturned;

		//do
		//{
		//	HROW rowBuffer[5000] = {}; // Request enough large batch to increase efficiency
		//	HROW* rowReturned = rowBuffer;
		//	hr = rowset->GetNextRows(DB_NULL_HCHAPTER, 0, ARRAYSIZE(rowBuffer), &rowCountReturned, &rowReturned);
		//	if (FAILED(hr))
		//	{
		//		std::cerr << "Failed to get next rows." << std::endl;
		//		return single_threaded_vector<hstring>();
		//	}

		//	for (int i = 0; i < rowCountReturned; i++)
		//	{
		//		com_ptr<::IUnknown> pUnkRow;
		//		hr = getRow->GetRowFromHROW(nullptr, rowBuffer[i], IID_IPropertyStore, pUnkRow.put());
		//		if (FAILED(hr))
		//		{
		//			std::cerr << "Failed to get row from HROW." << std::endl;
		//			return single_threaded_vector<hstring>();
		//		}

		//		com_ptr<IPropertyStore> propertyStore = pUnkRow.as<IPropertyStore>();

		//		PROPVARIANT propVar;
		//		PropVariantInit(&propVar);
		//		hr = propertyStore->GetValue(PKEY_ItemNameDisplay, &propVar);
		//		if (SUCCEEDED(hr))
		//		{
		//			results.Append(hstring(propVar.pwszVal));
		//			PropVariantClear(&propVar);
		//		}
		//	}

		//	rowset->ReleaseRows(rowCountReturned, rowReturned, nullptr, nullptr, nullptr);
		//} 
		//while (rowCountReturned > 0);		

		//return results;
	}
}
