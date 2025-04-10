# Microsoft Developer Studio Project File - Name="Infested Soils" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Infested Soils - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Infested Soils.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Infested Soils.mak" CFG="Infested Soils - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Infested Soils - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Infested Soils - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Infested Soils - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Infested Soils - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Output/Infested Soils.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Infested Soils - Win32 Release"
# Name "Infested Soils - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# End Group
# Begin Group "RpgSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RpgSystem\CAbility.h
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CAttribute.cpp
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CAttribute.h
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CCharacterAttributes.cpp
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CCharacterAttributes.h
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CCharacterSheet.h
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CClass.cpp
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CClass.h
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CDice.cpp
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CDice.h
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\RpgSystem\CSkill.h
# End Source File
# End Group
# Begin Group "Library"

# PROP Default_Filter ""
# Begin Group "Basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\library\basic\basic.cpp
# End Source File
# Begin Source File

SOURCE=.\library\basic\basic.h
# End Source File
# Begin Source File

SOURCE=.\library\basic\CCharString.cpp
# End Source File
# Begin Source File

SOURCE=.\library\basic\CCharString.h
# End Source File
# Begin Source File

SOURCE=.\library\basic\CPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\library\basic\CPoint.h
# End Source File
# Begin Source File

SOURCE=.\library\basic\CRect.h
# End Source File
# Begin Source File

SOURCE=.\library\basic\CWideString.cpp
# End Source File
# Begin Source File

SOURCE=.\library\basic\CWideString.h
# End Source File
# End Group
# Begin Group "FileSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\library\filesystem\CFileLibManager.cpp
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\CFileLibManager.h
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\CFilePathManager.cpp
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\CFilePathManager.h
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\CFileStream.cpp
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\CFileStream.h
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\filesystem.cpp
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\filesystem.h
# End Source File
# Begin Source File

SOURCE=.\library\filesystem\IFileManager.h
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\library\win32\WinMain.cpp
# End Source File
# End Group
# End Group
# Begin Group "WorldSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WorldSystem\CArea.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldSystem\CArea.h
# End Source File
# End Group
# End Target
# End Project
