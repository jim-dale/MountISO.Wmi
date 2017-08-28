#pragma once

class ArgsProcessor
{
public:
    static AppContext Parse(int argc, PWSTR argv[])
    {
        AppContext result;

        //            for(var arg in args)
        //            {
        //                if (String.IsNullOrWhiteSpace(arg))
        //                {
        //                    continue;
        //                }
        //                if (arg.Length == 2 && (arg[0] == '-' || arg[0] == '\\'))
        //                {
        //                    char option = Char.ToLowerInvariant(arg[1]);
        //
        //                    switch (option)
        //                    {
        //                    case '?':
        //                        result.ShowHelp = true;
        //                        break;
        //                    case 'm':
        //                        result.Command = Command.Mount;
        //                        break;
        //                    case 'd':
        //                        result.Command = Command.Dismount;
        //                        break;
        //                    case 'v':
        //                        result.Verbose = true;
        //                        break;
        //                    default:
        //                        break;
        //                    }
        //                }
        //                else
        //                {
        //                    result.IsoPath = arg;
        //                }
        //            }
        return result;
    }

    static void ShowHelp()
    {
        wprintf_s(L"MountISO [-?] [-v] -m | -u [drive:][path]filename\n");
        wprintf_s(L"  [drive:][path][filename]\n");
        wprintf_s(L"                 Specifies path to the ISO file to perform the action on.\n\n");

        wprintf_s(L"  -?             Show this help information.\n");
        wprintf_s(L"  -v             Switches on verbose mode to display additional debug information.\n");
        wprintf_s(L"  -m             Mounts a previously created ISO disk image, making it appear as a normal disk.\n");
        wprintf_s(L"  -d             Dismounts an ISO disk image so that it can no longer be accessed as a disk.\n\n");

        wprintf_s(L"If an ISO file is successfully mounted the program's exit code (ERRORLEVEL) is set\n");
        wprintf_s(L"to the numeric value of the driver letter e.g. 70 = F\n");
        wprintf_s(L"The undocumented environment variable %%=ExitCodeAscii%% can then be used to get the\n");
        wprintf_s(L"ASCII drive letter for the mounted ISO file.\n\n");

        wprintf_s(L"See the 'example-usage.cmd' script for an example of how to use the command.\n\n");
    }

    static void ShowConfiguration(const AppContext& ctx)
    {
        wprintf_s(L"ShowHelp: %s\n", bool_to_wstring(ctx.m_showHelp).c_str());
        wprintf_s(L"Verbose: %s\n", bool_to_wstring(ctx.m_verbose).c_str());
        wprintf_s(L"IsoPath: \"%s\"", ctx.m_isoPath.c_str());
        // TODO: wprintf_s(L"Command: {ctx.Command}");
    }

    //        public static void ShowRunState(AppContext ctx)
    //        {
    //            string errorMessage = String.IsNullOrEmpty(ctx.ErrorMessage) ? "(Not Set)" : ctx.ErrorMessage;
    //            string exception = (ctx.Exception == null) ? "(Not Set)" : ctx.Exception.ToString();
    //
    //            Console.WriteLine($"Status: {ctx.Status}");
    //            Console.WriteLine($"DriveLetter: {ctx.DriveLetter}");
    //            Console.WriteLine($"Error Message: {errorMessage}");
    //            Console.WriteLine($"Exception: {exception}");
    //        }
};
