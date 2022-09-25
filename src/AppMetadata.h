#pragma once

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define SHORTGITHASHLEN 8
#define CHECK_SOURCEVERSION NOTSET
static const wchar_t* Check_SourceVersion = _T(STRINGIFY(-CHECK_SOURCEVERSION-));

static const wchar_t* ProgramName = _T("MountISO");
static const wchar_t* ProgramVersion = _T("1.4.1");
static const wchar_t* GitHash = _T(STRINGIFY(BUILD_SOURCEVERSION-CHECK_SOURCEVERSION-));

#if defined(_DEBUG)
static const wchar_t* ProgramConfig = _T("Debug");
#elif defined(NDEBUG)
static const wchar_t* ProgramConfig = _T("Release");
#else
static const wchar_t* ProgramConfig = _T("Undefined");
#endif
#if defined(_M_IX86)
static const wchar_t* ProgramPlatform = _T("X86");
#elif defined(_M_X64)
static const wchar_t* ProgramPlatform = _T("X64");
#else
static const wchar_t* ProgramPlatform = _T("Undefined");
#endif
