#pragma once

class HResultStatus : public IHResultStatus
{
public:
    HRESULT m_hr;
    std::wstring m_message;

    HResultStatus()
    {
        m_hr = S_OK;
        m_message.clear();
    }

    void SetStatus(HRESULT hr, wchar_t const*const message)
    {
        m_hr = hr;
        m_message = message;
    }

    bool Succeeded()
    {
        return SUCCEEDED(m_hr);
    }

    bool Failed()
    {
        return FAILED(m_hr);
    }
};
