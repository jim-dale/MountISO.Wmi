#pragma once

class AppContext
{
public:
    const int INVALID_DRIVE_LETTER = 255;

    bool m_showHelp;
    bool m_verbose;
    std::wstring m_isoPath;
    AppCommand m_command;
    int m_driveLetter;
    std::wstring m_errorMessage;

    AppContext()
    {
        m_command = AppCommand::NotSet;
        m_driveLetter = INVALID_DRIVE_LETTER;
    }

    void Validate()
    {
        if (m_command == AppCommand::NotSet)
        {
            m_errorMessage = L"Specify an option to either Mount or Dismount an ISO file.";
        }
        else if (m_isoPath.empty())
        {
            m_errorMessage = L"Specify the path to a valid ISO file.";
        }
        else
        {
            if (FileExists(m_isoPath.c_str()) == false)
            {
                m_errorMessage = L"The ISO file \"" + m_isoPath + L"\" was not found.";
            }
        }
        if (false == IsValid())
        {
            m_showHelp = true;
        }
    }

    bool IsValid()
    {
        return m_errorMessage.empty();
    }

    void ShowError(HRESULT hr, const std::wstring& context)
    {
        if (FAILED(hr))
        {
            _com_error error(hr);

            wprintf(_T("%s failed.\n"), context.c_str());
            wprintf(_T("%s\n"), error.ErrorMessage());
        }
    }

private:
    static bool FileExists(LPCWSTR szPath)
    {
        DWORD dwAttrib = GetFileAttributes(szPath);

        return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }
};
