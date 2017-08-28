#pragma once

class AppContext
{
public:
    bool m_showHelp;
    bool m_verbose;
    HResultStatus m_status;
    std::wstring m_isoPath;
    AppCommand m_command;
    int m_driveLetter;

    AppContext()
    {
        m_showHelp = false;
        m_verbose = false;
        m_command = AppCommand::NotSet;
        m_driveLetter = INVALID_DRIVE_LETTER;
    }

    void Validate()
    {
        if (m_command == AppCommand::NotSet)
        {
            m_status.m_message = L"Specify an option to either Mount, Dismount or Query an ISO file.";
        }
        else if (m_isoPath.empty())
        {
            m_status.m_message = L"Specify the path to a valid ISO file.";
        }
        else
        {
            if (FileExists(m_isoPath.c_str()) == false)
            {
                HRESULT hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_FILE_NOT_FOUND);
                m_status.SetStatus(hr, m_isoPath.c_str());
            }
        }
    }

    bool IsValid()
    {
        return m_status.m_message.empty();
    }

    void ShowError()
    {
        if (m_status.Failed())
        {
            _com_error error(m_status.m_hr);

            wprintf_s(L"%s: %s\n", m_status.m_message.c_str(), error.ErrorMessage());
        }
        else if (m_status.m_message.empty() == false)
        {
            wprintf_s(L"%s\n", m_status.m_message.c_str());
        }
    }

private:
    static bool FileExists(LPCWSTR szPath)
    {
        DWORD dwAttrib = GetFileAttributes(szPath);

        return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }
};
