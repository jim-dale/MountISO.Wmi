#pragma once

class DiskImageManager
{
private:
    bool m_verbose;
    IHResultStatus* m_statusReporter;

    std::wstring m_isoPath;
    _bstr_t m_isoObjectPath;

    CComPtr<IWbemLocator> m_locator;
    CComPtr<IWbemServices> m_service;

public:
    DiskImageManager(bool verbose, IHResultStatus* status)
    {
        m_verbose = verbose;
        m_statusReporter = status;
    }

    void Initialise(const std::wstring& isoPath)
    {
        m_isoPath = isoPath;
        m_isoObjectPath = DiskImageManager::BuildIsoObjectPath(m_isoPath.c_str());

        if (m_statusReporter->Succeeded())
        {
            m_locator = WmiManager::GetLocator(m_statusReporter);
        }
        if (m_statusReporter->Succeeded())
        {
            m_service = GetStorageService(m_locator);
        }
        if (m_statusReporter->Succeeded())
        {
            WmiManager::SetProxyBlanket(m_service, m_statusReporter);
        }
    }

    ~DiskImageManager()
    {
        m_service.Release();
        m_locator.Release();
    }

    int MountIso()
    {
        int result = WBEM_E_FAILED;

        CComPtr<IWbemClassObject> diskImageClass;
        if (m_statusReporter->Succeeded())
        {
            diskImageClass = GetDiskImageClass(m_service);
        }
        CComPtr<IWbemClassObject> inParamsDefinition;
        if (m_statusReporter->Succeeded())
        {
            inParamsDefinition = GetMountInParametersDefinition(diskImageClass);
        }
        if (m_statusReporter->Succeeded())
        {
            CComPtr<IWbemClassObject> inParams = SpawnInstance(inParamsDefinition);
            if (m_statusReporter->Succeeded())
            {
                _variant_t parameter((long)Access::ReadOnly);

                WmiManager::SetPropertyValue(inParams, L"Access", parameter, m_statusReporter);
            }
            if (m_statusReporter->Succeeded())
            {
                _variant_t parameter(false);

                WmiManager::SetPropertyValue(inParams, L"NoDriveLetter", parameter, m_statusReporter);
            }
            if (m_statusReporter->Succeeded())
            {
                CComPtr<IWbemClassObject> outParams = InvokeMethod(m_service, m_isoObjectPath, L"Mount", inParams);
                if (m_statusReporter->Succeeded())
                {
                    result = GetPropertyValueAs<int>(outParams, L"ReturnValue");
                }
            }
        }
        return result;
    }

    int DismountIso()
    {
        int result = WBEM_E_FAILED;

        CComPtr<IWbemClassObject> diskImageClass;
        if (m_statusReporter->Succeeded())
        {
            diskImageClass = GetDiskImageClass(m_service);
        }
        if (m_statusReporter->Succeeded())
        {
            CComPtr<IWbemClassObject> outParams = InvokeMethod(m_service, m_isoObjectPath, L"Dismount", nullptr);
            if (m_statusReporter->Succeeded())
            {
                result = GetPropertyValueAs<int>(outParams, L"ReturnValue");
            }
        }
        return result;
    }

    int GetDriveLetter()
    {
        int result = INVALID_DRIVE_LETTER;

        long lFlags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;
        _bstr_t query = DiskImageManager::BuildAssociatorsQuery(m_isoObjectPath);
        CComPtr<IEnumWbemClassObject> enumerator;

        HRESULT hr = m_service->ExecQuery(_bstr_t("WQL"), query, lFlags, nullptr, &enumerator);
        if (SUCCEEDED(hr))
        {
            ULONG returned = 0;

            for (;;)
            {
                CComPtr<IWbemClassObject> obj;

                hr = enumerator->Next(WBEM_INFINITE, 1, &obj, &returned);
                if (0 == returned || FAILED(hr))
                    break;

                result = GetPropertyValueAs<int>(obj, L"DriveLetter");

                if (m_verbose)
                {
                    wprintf_s(L"\nDrive letter = %d\n", result);
                    WmiManager::ShowProperties(obj);
                }
            }
        }
        if (FAILED(hr))
        {
            result = INVALID_DRIVE_LETTER;
            m_statusReporter->SetStatus(hr, L"Get drive letter failed.");
        }
        return result;
    }

    CComPtr<IWbemServices> GetStorageService(CComPtr<IWbemLocator> locator)
    {
        return WmiManager::GetService(locator, _bstr_t(LR"(\\.\ROOT\Microsoft\Windows\Storage)"), m_statusReporter);
    }

    CComPtr<IWbemClassObject> GetDiskImageObject(CComPtr<IWbemServices> service, const BSTR& objectPath)
    {
        return WmiManager::GetObject(service, objectPath, L"Get Disk Image Instance", m_statusReporter);
    }

    CComPtr<IWbemClassObject> GetDiskImageClass(CComPtr<IWbemServices> service)
    {
        return WmiManager::GetObject(service, L"MSFT_DiskImage", L"Get Disk Image Class", m_statusReporter);
    }

    CComPtr<IWbemClassObject> GetMountInParametersDefinition(CComPtr<IWbemClassObject> object)
    {
        CComPtr<IWbemClassObject> result;

        HRESULT hr = object->GetMethod(L"Mount", 0, &result, nullptr);
        if (FAILED(hr))
        {
            m_statusReporter->SetStatus(hr, L"Get Mount method In parameters definition for MSFT_DiskImage class");
        }
        return result;
    }

    CComPtr<IWbemClassObject> SpawnInstance(CComPtr<IWbemClassObject> object)
    {
        CComPtr<IWbemClassObject> result;

        HRESULT hr = object->SpawnInstance(0, &result);
        if (FAILED(hr))
        {
            m_statusReporter->SetStatus(hr, L"Spawn class instance");
        }
        return result;
    }

    CComPtr<IWbemClassObject> InvokeMethod(CComPtr<IWbemServices> service, const BSTR objectPath, const BSTR methodName, CComPtr<IWbemClassObject> inParameters)
    {
        CComPtr<IWbemClassObject> result;

        HRESULT hr = service->ExecMethod(objectPath, methodName, 0, nullptr, inParameters, &result, nullptr);
        if (FAILED(hr))
        {
            m_statusReporter->SetStatus(hr, L"Execute method");
        }
        return result;
    }

    template<typename T>
    T GetPropertyValueAs(CComPtr<IWbemClassObject> object, wchar_t const*const propertyName)
    {
        _variant_t value = WmiManager::GetPropertyValue(object, propertyName, m_statusReporter);
        return (T)(value);
    }

    /// <summary>
    /// BuildAssociatorsQuery builds a query to return the MSFT_Volume
    /// class instance, if one exists, for a given MSFT_DiskImage class instance.
    /// </summary>
    static _bstr_t BuildIsoObjectPath(wchar_t const*const isoPath)
    {
        _bstr_t result = L"MSFT_DiskImage.ImagePath='" + _bstr_t(isoPath) + L"',StorageType=1";
        return result;
    }

    /// <summary>
    /// BuildAssociatorsQuery builds a query to return the MSFT_Volume
    /// class instance, if one exists, for a given MSFT_DiskImage class instance.
    /// </summary>
    static _bstr_t BuildAssociatorsQuery(const _bstr_t& objectPath)
    {
        _bstr_t result = L"ASSOCIATORS OF {" + objectPath + L"}";
        result += " WHERE AssocClass = MSFT_DiskImageToVolume ResultClass = MSFT_Volume";
        return result;
    }
};
