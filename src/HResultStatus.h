#pragma once

class HResultStatus : public IHResultStatus
{
public:
    HRESULT m_hr;
    std::wstring m_message;

    void HResultStatus::SetStatus(HRESULT hr, wchar_t const * const message)
    {
        m_hr = hr;
        m_message = message;
    }

    bool HResultStatus::Succeeded()
    {
        return SUCCEEDED(m_hr);
    }

    bool HResultStatus::Failed()
    {
        return FAILED(m_hr);
    }
};
