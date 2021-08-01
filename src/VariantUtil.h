#pragma once

class VariantUtil
{
public:
    static std::wstring GetVariantType(const VARIANT& v)
    {
        std::wstring result(L"*NOT MAPPED*");

        switch (v.vt)
        {
        case VT_EMPTY:      result = L"VT_EMPTY";       break;
        case VT_NULL:       result = L"VT_NULL";        break;
        case VT_I2:         result = L"VT_I2";          break;
        case VT_I4:         result = L"VT_I4";          break;
        case VT_R4:         result = L"VT_R4";          break;
        case VT_R8:         result = L"VT_R8";          break;
        case VT_CY:         result = L"VT_CY";          break;
        case VT_DATE:       result = L"VT_DATE";        break;
        case VT_BSTR:       result = L"VT_BSTR";        break;
        case VT_DISPATCH:   result = L"VT_DISPATCH";    break;
        case VT_ERROR:      result = L"VT_ERROR";       break;
        case VT_BOOL:       result = L"VT_BOOL";        break;
        case VT_VARIANT:    result = L"VT_ARRAY";       break;
        case VT_UNKNOWN:    result = L"VT_UNKNOWN";     break;
        case VT_DECIMAL:    result = L"VT_DECIMAL";     break;
        case VT_UI1:        result = L"VT_UI1";         break;
        case VT_ARRAY:      result = L"VT_ARRAY";       break;
        case VT_BYREF:      result = L"VT_BYREF";       break;
        }
        return result;
    }

    static std::wstring GetVariantValue(const VARIANT& v)
    {
        std::wstring result(L"*NOT MAPPED*");

        switch (v.vt)
        {
        case VT_EMPTY:
            result = L"EMPTY";
            break;
        case VT_NULL:
            result = L"NULL";
            break;
        case VT_I2:
            result = std::to_wstring(v.iVal);
            break;
        case VT_I4:
            result = std::to_wstring(v.intVal);
            break;
        case VT_R4:
            result = std::to_wstring(v.fltVal);
            break;
        case VT_R8:
            result = std::to_wstring(v.dblVal);
            break;
        case VT_CY:
            result = std::to_wstring(v.cyVal.int64);
            break;
        case VT_BSTR:
            result = v.bstrVal;
            break;
        case VT_BOOL:
            result = bool_to_wstring(v.boolVal != FALSE);
            break;
        case VT_UI1:
            result = std::to_wstring(v.bVal);
            break;
        }
        return result;
    }
};
