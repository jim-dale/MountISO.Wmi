
#define _WIN32_WINNT _WIN32_WINNT_WIN6
#define _WIN32_DCOM

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS              // some CString constructors will be explicit

#include <SDKDDKVer.h>
#include <windows.h>
#include <objbase.h>
#include <comdef.h>
#include <comutil.h>
#include <wbemidl.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

#include <strsafe.h>

#include <iostream>
#include <string>

#define INVALID_DRIVE_LETTER 255

#include "AppMetadata.h"

#include "Access.h"
#include "Utility.h"
#include "VariantUtil.h"
#include "IHResultStatus.h"
#include "HResultStatus.h"

#include "ComManager.h"
#include "WmiManager.h"
#include "DiskImageManager.h"

#include "AppCommand.h"
#include "AppContext.h"
#include "ArgsProcessor.h"

#pragma comment(lib, "wbemuuid.lib")

int wmain(int argc, PWSTR argv[])
{
    AppContext ctx = ArgsProcessor::Parse(argc, argv);

    ctx.Validate();

    if (ctx.m_showHelp)
    {
        ArgsProcessor::ShowHelp();
    }
    else if (ctx.m_showVersion)
    {
        ArgsProcessor::ShowVersion();
    }
    else if (false == ctx.HasError())
    {
        if (ctx.m_verbose)
        {
            ArgsProcessor::ShowConfiguration(ctx);
        }

        ComManager com;
        HRESULT hr = com.Initialise();
        if (FAILED(hr))
        {
            ctx.m_status.SetStatus(hr, L"COM initialisation failed");
        }
        else
        {
            hr = com.InitialiseSecurity();
            if (FAILED(hr))
            {
                ctx.m_status.SetStatus(hr, L"COM security initialisation failed");
            }
        }

        if (ctx.m_status.Succeeded())
        {
            DiskImageManager diskImageManager(ctx.m_verbose, &ctx.m_status);

            diskImageManager.Initialise(ctx.m_isoPath);

            if (ctx.m_status.Succeeded())
            {
                switch (ctx.m_command)
                {
                case AppCommand::Mount:
                {
                    int returnValue = diskImageManager.MountIso();
                    if (ctx.m_verbose)
                    {
                        wprintf(L"Mount return value = %d\n", returnValue);
                    }

                    if (ctx.m_status.Succeeded())
                    {
                        ctx.m_driveLetter = diskImageManager.GetDriveLetter();
                        if (ctx.m_verbose)
                        {
                            wprintf(L"Mount drive letter = %d", ctx.m_driveLetter);
                        }
                    }
                }
                break;
                case AppCommand::Dismount:
                {
                    int returnValue = diskImageManager.DismountIso();
                    if (ctx.m_verbose)
                    {
                        wprintf(L"Dismount return value = %d\n", returnValue);
                    }
                }
                break;
                case AppCommand::Query:
                    ctx.m_driveLetter = diskImageManager.GetDriveLetter();
                    if (ctx.m_verbose)
                    {
                        wprintf(L"Query drive letter = %d\n", ctx.m_driveLetter);
                    }
                    break;
                }
            }
        }
    }
    if (ctx.HasError())
    {
        ArgsProcessor::ShowHelp();

        ctx.ShowError();
    }
    return ctx.m_driveLetter;
}
