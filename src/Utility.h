#pragma once

std::wstring bool_to_wstring(bool value)
{
    return (value) ? L"true" : L"false";
}
