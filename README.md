# MountISO.Wmi
Native Windows command line utility to mount an ISO image file, making it appear as a normal disk.

```
MountISO [-?] [-v] -m | -d | -q [drive:][path]filename
  [drive:][path][filename]
                 Specifies path to the ISO file to perform the action on.

  -?             Show this help information.
  -v             Switches on verbose mode to display additional debug information.
  -m             Mounts a previously created ISO disk image, making it appear as a normal disk.
  -d             Dismounts an ISO disk image so that it can no longer be accessed as a disk.
  -q             Gets the drive letter for a previously mounted ISO disk image.
```

If an ISO file is successfully mounted the program's exit code (ERRORLEVEL) is set to the numeric value of the drive letter e.g. 70 = F

The undocumented environment variable %%=ExitCodeAscii%% can then be used to get the ASCII drive letter for the mounted ISO file.

## Example usage:
```bat
@SETLOCAL
@SET INPUT=LOREM_IPSUM.iso
MountIso -v -m %%INPUT%%
@IF ERRORLEVEL 255 @GOTO :END
@SET ISODRVLETTER=%%=ExitCodeAscii%%
@IF \"%%ISODRVLETTER%%\"==\"\" @GOTO :END
@SET ISODRIVE=%%ISODRVLETTER%%:\\
@ECHO.
@ECHO The ISO file %%INPUT%% has been mounted to drive %%ISODRVLETTER%%
@ECHO.
DIR %%ISODRIVE%%
@ECHO.
@ECHO.
@ECHO Ready to Dismount ISO file
@PAUSE
MountIso -d %%INPUT%%
:END
@ENDLOCAL
```
