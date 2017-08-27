#pragma once

class ComManager
{
public:
    HRESULT Initialise()
    {
        return ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    }

    /// <summary>
    /// InitialiseSecurity setup process-wide security context
    /// </summary>
    HRESULT InitialiseSecurity()
    {
        HRESULT result = ::CoInitializeSecurity(nullptr,    // we're not a server
            -1,                                 // we're not a server
            nullptr,                            // we're not a server
            nullptr,                            // reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,          // let DCOM decide
            RPC_C_IMP_LEVEL_IMPERSONATE,
            nullptr,
            EOAC_NONE,
            nullptr);

        return result;
    }

    ~ComManager()
    {
        ::CoUninitialize();
    }
};
