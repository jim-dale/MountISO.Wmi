
#define _WIN32_WINNT _WIN32_WINNT_WIN6
#define _WIN32_DCOM

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS              // some CString constructors will be explicit

#include <SDKDDKVer.h>
#include <windows.h>
#include <objbase.h>
#include <comdef.h>
#include <comutil.h>
#include <wbemidl.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

#include <strsafe.h>

#include <iostream>
#include <string>

#include "Access.h"
#include "Utility.h"
#include "VariantUtil.h"
#include "IHResultStatus.h"
#include "HResultStatus.h"

#include "ComManager.h"
#include "WmiManager.h"
#include "DiskImageManager.h"

#include "AppCommand.h"
#include "AppContext.h"
#include "ArgsProcessor.h"

#pragma comment(lib, "wbemuuid.lib")


int wmain(int argc, PWSTR argv[])
{
    _bstr_t DismountMethodName = L"Dismount";

    AppContext ctx = ArgsProcessor::Parse(argc, argv);

    int result = 0;
    bool attached = true;
    bool verbose = true;
    HResultStatus status;

    std::wstring isoPath = LR"(D:\Linux\ubuntu-12.04-desktop-amd64.iso)";

    _bstr_t isoObjectPath = DiskImageManager::BuildIsoObjectPath(isoPath.c_str());

    ComManager com;
    HRESULT hr = com.Initialise();
    if (SUCCEEDED(hr))
    {
        hr = com.InitialiseSecurity();
    }

    if (SUCCEEDED(hr))
    {
        DiskImageManager diskImageManager;

        {
            CComPtr<IWbemLocator> locator;
            if (diskImageManager.Succeeded())
            {
                locator = diskImageManager.GetLocator();
            }
            CComPtr<IWbemServices> service;
            if (diskImageManager.Succeeded())
            {
                service = diskImageManager.GetStorageService(locator);
            }
            if (diskImageManager.Succeeded())
            {
                diskImageManager.SetProxyBlanket(service);
            }




            if (false == attached)
            {
                CComPtr<IWbemClassObject> diskImageClass;
                if (diskImageManager.Succeeded())
                {
                    diskImageClass = diskImageManager.GetDiskImageClass(service);
                }

                CComPtr<IWbemClassObject> inParamsDefinition;
                if (diskImageManager.Succeeded())
                {
                    inParamsDefinition = diskImageManager.GetMountInParametersDefinition(diskImageClass);
                }

                CComPtr<IWbemClassObject> inParams;
                if (diskImageManager.Succeeded())
                {
                    inParams = diskImageManager.SpawnInstance(inParamsDefinition);
                }

                if (diskImageManager.Succeeded())
                {
                    _variant_t parameter((long)Access::ReadOnly);

                    diskImageManager.SetPropertyValue(inParams, L"Access", parameter);
                }
                if (diskImageManager.Succeeded())
                {
                    _variant_t parameter(false);

                    diskImageManager.SetPropertyValue(inParams, L"NoDriveLetter", parameter);
                }
                CComPtr<IWbemClassObject> outParams;
                if (diskImageManager.Succeeded())
                {
                    outParams = diskImageManager.InvokeMethod(service, isoObjectPath, L"Mount", inParams);
                }
                if (diskImageManager.Succeeded())
                {
                    int returnValue = diskImageManager.GetPropertyValueAs<int>(outParams, L"ReturnValue");

                    if (verbose)
                    {
                        wprintf(L"Mount return value = %d", returnValue);
                    }
                }
            }

            CComPtr<IWbemClassObject> diskImage;
            if (diskImageManager.Succeeded())
            {
                diskImage = diskImageManager.GetDiskImageObject(service, isoObjectPath);
            }
            if (diskImageManager.Succeeded())
            {
                if (verbose)
                {
                    diskImageManager.ShowProperties(diskImage);
                }

                attached = diskImageManager.GetPropertyValueAs<bool>(diskImage, L"Attached");

                CComPtr<IEnumWbemClassObject> enumerator;

                _bstr_t query = DiskImageManager::BuildAssociatorsQuery(isoObjectPath);

                long lFlags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;

                HRESULT hr = service->ExecQuery(_bstr_t("WQL"), query, lFlags, nullptr, &enumerator);
                if (SUCCEEDED(hr))
                {
                    ULONG returned = 0;

                    for (;;)
                    {
                        CComPtr<IWbemClassObject> obj;

                        hr = enumerator->Next(WBEM_INFINITE, 1, &obj, &returned);

                        if (returned == 0 || FAILED(hr))
                            break;

                        int driveLetter = diskImageManager.GetPropertyValueAs<int>(obj, L"DriveLetter");

                        result = driveLetter;

                        if (verbose)
                        {
                            diskImageManager.ShowProperties(obj);

                            wprintf_s(L"\n\nDrive letter = %d\n", driveLetter);
                        }
                    }
                }
            }
            //diskImageManager.ShowError();
        }
    }
    return result;
}
