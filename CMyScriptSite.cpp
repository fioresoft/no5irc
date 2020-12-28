#include "stdafx.h"
#include "CMyScriptSite.h"

void GetScriptEngines(CSimpleArray<CString> & vv)
{
	// get the component category manager for this machine
	ICatInformation* pci = 0;

	HRESULT hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		0, CLSCTX_SERVER, IID_ICatInformation, (void**)&pci);
	if (SUCCEEDED(hr))
	{
		CLSID catid = { 0 };
		//hr = CLSIDFromString(CT2COLE(_T("{F0B7A1A2-9847-11cf-8F20-00805F2CD064}")), &catid);	//CATID_ActiveScriptParser
		hr = CLSIDFromString(CT2COLE(_T("{F0B7A1A1-9847-11cf-8F20-00805F2CD064}")), &catid);	// CATID_ActiveScript
		ATLASSERT(SUCCEEDED(hr));
		// get the list of parseable script engines
		CATID rgcatidImpl[1];
		rgcatidImpl[0] = catid;// CATID_ActiveScriptParse;
		IEnumCLSID* pec = 0;

		hr = pci->EnumClassesOfCategories(1, rgcatidImpl, 0, 0,
			&pec);
		if (SUCCEEDED(hr))
		{
			// print the list of CLSIDs to the console as ProgIDs
			enum { CHUNKSIZE = 16 };
			CLSID rgclsid[CHUNKSIZE];
			ULONG cActual;

			do
			{
				hr = pec->Next(CHUNKSIZE, rgclsid, &cActual);
				if (FAILED(hr))
					break;
				if (hr == S_OK)
					cActual = CHUNKSIZE;
				for (ULONG i = 0; i < cActual; i++)
				{
					OLECHAR* pwszProgID = 0;
					if (SUCCEEDED(ProgIDFromCLSID(rgclsid[i], &pwszProgID)))
					{
						CString X = pwszProgID;
						vv.Add(X);
						CoTaskMemFree(pwszProgID);
					}
				}
			} while (hr != S_FALSE);
			pec->Release();
		}
		pci->Release();
	}
}