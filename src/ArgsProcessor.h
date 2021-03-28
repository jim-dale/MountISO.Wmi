#pragma once

class ArgsProcessor
{
public:
    static AppContext Parse(int argc, PWSTR argv[])
    {
        AppContext result;

        for (int i = 1; i < argc; i++)
        {
            std::wstring arg(argv[i]);

            if (arg.length() == 2 && (arg[0] == '-' || arg[0] == '/'))
            {
                wchar_t option = towlower(arg[1]);

                switch (option)
                {
                case '?':
                    result.m_showHelp = true;
                    break;
                case 'm':
                    result.m_command = AppCommand::Mount;
                    break;
                case 'd':
                    result.m_command = AppCommand::Dismount;
                    break;
                case 'q':
                    result.m_command = AppCommand::Query;
                    break;
                case 'v':
                    result.m_verbose = true;
                    break;
                default:
                    break;
                }
            }
            else
            {
                result.m_isoPath = arg;
            }
        }
        return result;
    }

    static void ShowHelp()
    {
        wprintf_s(L"MountISO [-?] [-v] -m | -d | -q [drive:][path]filename\n");
        wprintf_s(L"  [drive:][path][filename]\n");
        wprintf_s(L"                 Specifies path to the ISO file to perform the action on.\n\n");

        wprintf_s(L"  -?             Show this help information.\n");
        wprintf_s(L"  -v             Switches on verbose mode to display additional debug information.\n");
        wprintf_s(L"  -m             Mounts a previously created ISO disk image, making it appear as a normal disk.\n");
        wprintf_s(L"  -d             Dismounts an ISO disk image so that it can no longer be accessed as a disk.\n");
        wprintf_s(L"  -q             Gets the drive letter for a previously mounted ISO disk image.\n\n");

        wprintf_s(L"If an ISO file is successfully mounted the program's exit code (ERRORLEVEL) is set\n");
        wprintf_s(L"to the numeric value of the drive letter e.g. 70 = F\n");
        wprintf_s(L"The undocumented environment variable %%=ExitCodeAscii%% can then be used to get the\n");
        wprintf_s(L"ASCII drive letter for the mounted ISO file.\n\n");

        wprintf_s(L"Example usage:\n\n");

        wprintf_s(L"    @SETLOCAL\n");
        wprintf_s(L"    @SET INPUT=LOREM_IPSUM.iso\n\n");
        wprintf_s(L"    MountIso -v -m %%INPUT%%\n");
        wprintf_s(L"    @IF ERRORLEVEL 255 @GOTO :END\n\n");
        wprintf_s(L"    @SET ISODRVLETTER=%%=ExitCodeAscii%%\n");
        wprintf_s(L"    @IF \"%%ISODRVLETTER%%\"==\"\" @GOTO :END\n\n");
        wprintf_s(L"    @SET ISODRIVE=%%ISODRVLETTER%%:\\\n\n");
        wprintf_s(L"    @ECHO.\n");
        wprintf_s(L"    @ECHO The ISO file %%INPUT%% has been mounted to drive %%ISODRVLETTER%%\n");
        wprintf_s(L"    @ECHO.\n\n");
        wprintf_s(L"    DIR %%ISODRIVE%%\n\n");
        wprintf_s(L"    @ECHO.\n");
        wprintf_s(L"    @ECHO.\n");
        wprintf_s(L"    @ECHO Ready to Dismount ISO file\n");
        wprintf_s(L"    @PAUSE\n\n");
        wprintf_s(L"    MountIso -d %%INPUT%%\n\n");
        wprintf_s(L"    :END\n");
        wprintf_s(L"    @ENDLOCAL\n\n\n");
    }

    static void ShowConfiguration(const AppContext& ctx)
    {
        wprintf_s(L"ShowHelp: %s\n", bool_to_wstring(ctx.m_showHelp).c_str());
        wprintf_s(L"Verbose: %s\n", bool_to_wstring(ctx.m_verbose).c_str());
        wprintf_s(L"IsoPath: \"%s\"\n", ctx.m_isoPath.c_str());
        wprintf_s(L"Command: %s\n", appcommand_to_wstring(ctx.m_command));
    }


    static wchar_t const*const appcommand_to_wstring(AppCommand command)
    {
        static wchar_t const*const strings[] = { L"NotSet", L"Mount", L"Dismount", L"Query", L"Unknown" };

        wchar_t const* result = nullptr;

        switch (command)
        {
        case AppCommand::NotSet:
            result = strings[0];
            break;
        case AppCommand::Mount:
            result = strings[1];
            break;
        case AppCommand::Dismount:
            result = strings[2];
            break;
        case  AppCommand::Query:
            result = strings[3];
            break;
        default:
            result = strings[4];
            break;
        }
        return result;
    }
};
