# Microsoft Developer Studio Project File - Name="db_perf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=db_perf - Win32 Debug x86
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "db_perf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "db_perf.mak" CFG="db_perf - Win32 Debug x86"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "db_perf - Win32 Release x86" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 Debug x86" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 ASCII Debug x86" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 ASCII Release x86" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - x64 Debug AMD64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - x64 Release AMD64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - x64 Debug IA64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - x64 Release IA64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 Debug AMD64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 Release AMD64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 Debug IA64" (based on "Win32 (x86) Console Application")
!MESSAGE "db_perf - Win32 Release IA64" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "db_perf - Win32 Release x86"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release/db_perf"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 libdb46.lib  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcmt" /libpath:"Release"

!ELSEIF  "$(CFG)" == "db_perf - Win32 Debug x86"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug/db_perf"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libdb46d.lib  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /pdb:none /debug /machine:I386 /nodefaultlib:"libcmtd" /fixed:no /libpath:"Debug"

!ELSEIF  "$(CFG)" == "db_perf - Win32 ASCII Debug x86"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_ASCII"
# PROP BASE Intermediate_Dir "Debug_ASCII/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_ASCII"
# PROP Intermediate_Dir "Debug_ASCII/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46d.lib  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /pdb:none /debug /machine:I386 /nodefaultlib:"libcmtd" /fixed:no
# ADD LINK32 libdb46d.lib  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /pdb:none /debug /machine:I386 /nodefaultlib:"libcmtd" /fixed:no /libpath:"Debug_ASCII"

!ELSEIF  "$(CFG)" == "db_perf - Win32 ASCII Release x86"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_ASCII"
# PROP BASE Intermediate_Dir "Release_ASCII/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_ASCII"
# PROP Intermediate_Dir "Release_ASCII/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46.lib  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcmt"
# ADD LINK32 libdb46.lib  kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libcmt" /libpath:"Release_ASCII"

!ELSEIF  "$(CFG)" == "db_perf - x64 Debug AMD64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_AMD64"
# PROP BASE Intermediate_Dir "Debug_AMD64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_AMD64"
# PROP Intermediate_Dir "Debug_AMD64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:AMD64 /nodefaultlib:"libcmtd" /fixed:no
# ADD LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:AMD64 /nodefaultlib:"libcmtd" /fixed:no /libpath:"Debug_AMD64"

!ELSEIF  "$(CFG)" == "db_perf - x64 Release AMD64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_AMD64"
# PROP BASE Intermediate_Dir "Release_AMD64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_AMD64"
# PROP Intermediate_Dir "Release_AMD64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:AMD64 /nodefaultlib:"libcmt"
# ADD LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:AMD64 /nodefaultlib:"libcmt" /libpath:"Release_AMD64"

!ELSEIF  "$(CFG)" == "db_perf - x64 Debug IA64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_IA64"
# PROP BASE Intermediate_Dir "Debug_IA64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_IA64"
# PROP Intermediate_Dir "Debug_IA64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:IA64 /nodefaultlib:"libcmtd" /fixed:no
# ADD LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:IA64 /nodefaultlib:"libcmtd" /fixed:no /libpath:"Debug_IA64"

!ELSEIF  "$(CFG)" == "db_perf - x64 Release IA64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_IA64"
# PROP BASE Intermediate_Dir "Release_IA64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_IA64"
# PROP Intermediate_Dir "Release_IA64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:IA64 /nodefaultlib:"libcmt"
# ADD LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:IA64 /nodefaultlib:"libcmt" /libpath:"Release_IA64"

!ELSEIF  "$(CFG)" == "db_perf - Win32 Debug AMD64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_AMD64"
# PROP BASE Intermediate_Dir "Debug_AMD64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_AMD64"
# PROP Intermediate_Dir "Debug_AMD64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:AMD64 /nodefaultlib:"libcmtd" /fixed:no
# ADD LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:AMD64 /nodefaultlib:"libcmtd" /fixed:no /libpath:"Debug_AMD64"

!ELSEIF  "$(CFG)" == "db_perf - Win32 Release AMD64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_AMD64"
# PROP BASE Intermediate_Dir "Release_AMD64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_AMD64"
# PROP Intermediate_Dir "Release_AMD64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:AMD64 /nodefaultlib:"libcmt"
# ADD LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:AMD64 /nodefaultlib:"libcmt" /libpath:"Release_AMD64"

!ELSEIF  "$(CFG)" == "db_perf - Win32 Debug IA64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_IA64"
# PROP BASE Intermediate_Dir "Debug_IA64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_IA64"
# PROP Intermediate_Dir "Debug_IA64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MDd /W3 /EHsc /Z7 /Od /I "." /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:IA64 /nodefaultlib:"libcmtd" /fixed:no
# ADD LINK32 libdb46d.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:IA64 /nodefaultlib:"libcmtd" /fixed:no /libpath:"Debug_IA64"

!ELSEIF  "$(CFG)" == "db_perf - Win32 Release IA64"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_IA64"
# PROP BASE Intermediate_Dir "Release_IA64/db_perf"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_IA64"
# PROP Intermediate_Dir "Release_IA64/db_perf"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD CPP /nologo /MD /W3 /EHsc /O2 /I "." /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE"  /Wp64 /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:IA64 /nodefaultlib:"libcmt"
# ADD LINK32 libdb46.lib  bufferoverflowU.lib kernel32.lib user32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:IA64 /nodefaultlib:"libcmt" /libpath:"Release_IA64"

!ENDIF 

# Begin Target

# Name "db_perf - Win32 Release x86"
# Name "db_perf - Win32 Debug x86"
# Name "db_perf - Win32 ASCII Debug x86"
# Name "db_perf - Win32 ASCII Release x86"
# Name "db_perf - x64 Debug AMD64"
# Name "db_perf - x64 Release AMD64"
# Name "db_perf - x64 Debug IA64"
# Name "db_perf - x64 Release IA64"
# Name "db_perf - Win32 Debug AMD64"
# Name "db_perf - Win32 Release AMD64"
# Name "db_perf - Win32 Debug IA64"
# Name "db_perf - Win32 Release IA64"
# Begin Source File

SOURCE=..\test_perf\db_perf.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_checkpoint.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_config.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_dbs.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_dead.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_debug.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_file.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_key.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_log.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_misc.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_op.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_parse.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_rand.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_spawn.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_stat.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_sync.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_thread.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_trickle.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_txn.c
# End Source File
# Begin Source File

SOURCE=..\test_perf\perf_util.c
# End Source File
# Begin Source File

SOURCE=..\clib\getopt.c
# End Source File
# End Target
# End Project
