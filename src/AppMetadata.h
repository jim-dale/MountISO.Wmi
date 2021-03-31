#pragma once

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define VERSION "1.3.0"
#define SHORTGITHASHLEN 8

#ifndef BUILDNUM
#define BUILDNUM InternalOnly
#endif

static const PTCHAR ProgramName = _T("MountISO");
static const PTCHAR ProgramVersion = _T(VERSION);
static const PTCHAR GitHash = _T(STRINGIFY(BUILDNUM));

#if defined(_DEBUG)
static const PTCHAR ProgramConfig = _T("Debug");
#elif defined(NDEBUG)
static const PTCHAR ProgramConfig = _T("Release");
#else
static const PTCHAR ProgramConfig = _T("Undefined");
#endif
#if defined(_M_IX86)
static const PTCHAR ProgramPlatform = _T("X86");
#elif defined(_M_X64)
static const PTCHAR ProgramPlatform = _T("X64");
#else
static const PTCHAR ProgramPlatform = _T("Undefined");
#endif
