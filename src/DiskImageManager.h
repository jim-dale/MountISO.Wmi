#pragma once

class DiskImageManager
{
private:
    IHResultStatus* m_statusReporter;

public:
    void Initialise(IHResultStatus* status)
    {
        m_statusReporter = status;
    }

    CComPtr<IWbemServices> GetStorageService(CComPtr<IWbemLocator> locator)
    {
        return WmiManager::GetService(locator, _bstr_t(LR"(\\.\ROOT\Microsoft\Windows\Storage)"), m_statusReporter);
    }

    CComPtr<IWbemClassObject> GetDiskImageObject(CComPtr<IWbemServices> service, const BSTR objectPath)
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
