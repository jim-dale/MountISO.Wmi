#pragma once

class IHResultStatus
{
public:
    virtual void SetStatus(HRESULT hr, wchar_t const*const message) = 0;
    virtual bool Succeeded() = 0;
    virtual bool Failed() = 0;
};
