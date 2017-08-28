#pragma once

class WmiManager
{
public:
    static CComPtr<IWbemLocator> GetLocator(IHResultStatus* status)
    {
        CComPtr<IWbemLocator> result;

        HRESULT hr = CoCreateInstance(CLSID_WbemAdministrativeLocator,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            reinterpret_cast<void**>(&result)
        );
        if (FAILED(hr))
        {
            status->SetStatus(hr, L"Instantiation of IWbemLocator failed");
        }
        return result;
    }

    /// <summary>
    /// GetService connects to the local service with current credentials
    /// </summary>
    static CComPtr<IWbemServices> GetService(CComPtr<IWbemLocator> locator, const BSTR networkResource, IHResultStatus* status)
    {
        CComPtr<IWbemServices> result;

        HRESULT hr = locator->ConnectServer(networkResource,
            nullptr,
            nullptr,
            nullptr,
            WBEM_FLAG_CONNECT_USE_MAX_WAIT,
            nullptr,
            nullptr,
            &result
        );
        if (FAILED(hr))
        {
            status->SetStatus(hr, L"Connect server failed");
        }
        return result;
    }

    /// <summary>
    /// SetProxyBlanket sets the authentication information that will be used to make calls on the specified proxy.
    /// </summary>
    static void SetProxyBlanket(CComPtr<IWbemServices> service, IHResultStatus* status)
    {
        // Set the proxy so that impersonation of the client occurs.
        HRESULT hr = ::CoSetProxyBlanket(service,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            nullptr,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            nullptr,
            EOAC_NONE
        );
        if (FAILED(hr))
        {
            status->SetStatus(hr, L"Set Proxy Blanket failed");
        }
    }

    static CComPtr<IWbemClassObject> GetObject(CComPtr<IWbemServices> service, const BSTR objectPath, wchar_t const*const context, IHResultStatus* status)
    {
        CComPtr<IWbemClassObject> result;

        HRESULT hr = service->GetObject(objectPath, 0, nullptr, &result, nullptr);
        if (FAILED(hr))
        {
            status->SetStatus(hr, context);
        }
        return result;
    }

    static _variant_t GetPropertyValue(CComPtr<IWbemClassObject> object, wchar_t const*const propertyName, IHResultStatus* status)
    {
        _variant_t result;

        HRESULT hr = object->Get(propertyName, 0, result.GetAddress(), nullptr, 0);
        if (FAILED(hr))
        {
            status->SetStatus(hr, L"Get property value");
        }
        return result;
    }

    static void SetPropertyValue(CComPtr<IWbemClassObject> object, wchar_t const*const propertyName, _variant_t& value, IHResultStatus* status)
    {
        HRESULT hr = object->Put(propertyName, 0, static_cast<LPVARIANT>(&value), 0);
        if (FAILED(hr))
        {
            status->SetStatus(hr, L"Set property value");
        }
    }

    static void ShowProperties(CComPtr<IWbemClassObject> object)
    {
        long lFlags = WBEM_FLAG_ALWAYS;

        HRESULT hres = object->BeginEnumeration(lFlags);
        if (SUCCEEDED(hres))
        {
            _bstr_t name;
            _variant_t value;

            hres = object->Next(lFlags, name.GetAddress(), value.GetAddress(), nullptr, nullptr);
            while (SUCCEEDED(hres) && hres != WBEM_S_NO_MORE_DATA)
            {
                std::wstring typeName = VariantUtil::GetVariantType(value);
                std::wstring valueStr = VariantUtil::GetVariantValue(value);

                wprintf(_T("%s='%s' (%s)\n"), (PTCHAR)name, valueStr.c_str(), typeName.c_str());

                hres = object->Next(lFlags, name.GetAddress(), value.GetAddress(), nullptr, nullptr);
            }

            hres = object->EndEnumeration();
        }
    }
};
