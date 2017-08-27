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
        wprintf_s(L"MountISO [-?] [-v] -m | -u [drive:][path]filename");
        //            Console.WriteLine("  [drive:][path][filename]");
        //            Console.WriteLine("                 Specifies path to the ISO file to perform the action on.");
        //            Console.WriteLine();
        //            Console.WriteLine("  -?             Show this help information.");
        //            Console.WriteLine("  -v             Switches on verbose mode to display additional debug information.");
        //            Console.WriteLine("  -m             Mounts a previously created ISO disk image, making it appear as a normal disk.");
        //            Console.WriteLine("  -d             Dismounts an ISO disk image so that it can no longer be accessed as a disk.");
        //            Console.WriteLine();
        //            Console.WriteLine("If an ISO file is successfully mounted the program's exit code (ERRORLEVEL) is set");
        //            Console.WriteLine("to the numeric value of the driver letter e.g. 70 = F");
        //            Console.WriteLine("The undocumented environment variable %=ExitCodeAscii% can then be used to get the");
        //            Console.WriteLine("ASCII drive letter for the mounted ISO file.");
        //            Console.WriteLine("");
        //            Console.WriteLine("See the 'example-usage.cmd' script for an example of how to use the command.");
        //            Console.WriteLine("");
    }

            static void ShowConfiguration(AppContext ctx)
            {
    //            Console.WriteLine($"ShowHelp: {ctx.ShowHelp}");
    //            Console.WriteLine($"Verbose: {ctx.Verbose}");
    //            Console.WriteLine($"IsoPath: \"{ctx.IsoPath}\"");
    //            Console.WriteLine($"Command: {ctx.Command}");
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
