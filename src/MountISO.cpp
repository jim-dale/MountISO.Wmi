
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

    if (ctx.IsValid())
    {
        ComManager com;
        HRESULT hr = com.Initialise();
        if (SUCCEEDED(hr))
        {
            hr = com.InitialiseSecurity();
            if (FAILED(hr))
            {
                ctx.m_status.SetStatus(hr, L"COM security initialisation failed.");
            }
        }
        else
        {
            ctx.m_status.SetStatus(hr, L"COM initialisation failed.");
        }
        if (SUCCEEDED(hr))
        {
            DiskImageManager diskImageManager(ctx.m_verbose, &ctx.m_status);

            diskImageManager.Initialise(isoPath);

            if (ctx.m_status.Succeeded())
            {
                switch (ctx.m_command)
                {
                case AppCommand::Mount:
                {
                    int returnValue = diskImageManager.MountIso();
                    if (ctx.m_verbose)
                    {
                        wprintf(L"Mount return value = %d", returnValue);
                    }

                    if (ctx.m_status.Succeeded())
                    {
                        ctx.m_driveLetter = diskImageManager.GetDriveLetter();
                    }
                }
                break;
                case AppCommand::Dismount:
                {
                    int returnValue = diskImageManager.DismountIso();
                    if (ctx.m_verbose)
                    {
                        wprintf(L"Dismount return value = %d", returnValue);
                    }
                }
                break;
                case AppCommand::Query:
                    ctx.m_driveLetter = diskImageManager.GetDriveLetter();
                    break;
                }
            }
        }
    }
    ctx.ShowError();

    return ctx.m_driveLetter;
}




//CComPtr<IWbemClassObject> diskImage;
//if (status.Succeeded())
//{
//    diskImage = diskImageManager.GetDiskImageObject(service, isoObjectPath);
//}
//if (status.Succeeded())
//{
//    if (verbose)
//    {
//        diskImageManager.ShowProperties(diskImage);
//    }
//
//    attached = diskImageManager.GetPropertyValueAs<bool>(diskImage, L"Attached");
