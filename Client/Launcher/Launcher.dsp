# Microsoft Developer Studio Project File - Name="Launcher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Launcher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Launcher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Launcher.mak" CFG="Launcher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Launcher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Launcher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Launcher/Launcher", EODAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Launcher - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /force /out:"Launcher.exe"

!ELSEIF  "$(CFG)" == "Launcher - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /force /out:"Launcher_Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Launcher - Win32 Release"
# Name "Launcher - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APISocket.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\BB_CircularBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Launcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Launcher.rc
# End Source File
# Begin Source File

SOURCE=.\LauncherDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MacProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APISocket.h
# End Source File
# Begin Source File

SOURCE=.\BackgroundUtil.h
# End Source File
# Begin Source File

SOURCE=.\BB_CircularBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\Launcher.h
# End Source File
# Begin Source File

SOURCE=.\LauncherDlg.h
# End Source File
# Begin Source File

SOURCE=.\MacProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PacketDef.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Bkg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Btn_cancel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Btn_Connect.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Btn_exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Btn_join.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Edit_id.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Edit_PW.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Launcher.ico
# End Source File
# Begin Source File

SOURCE=.\res\Launcher.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Launcher : {5B776D49-B4CE-4EB5-A549-4EAB3088E6F8}
# 	1:15:IDD_TRANSPARENT:107
# 	2:16:Resource Include:resource.h
# 	2:18:DlgTransparent.cpp:DlgTransparent.cpp
# 	2:15:IDD_TRANSPARENT:IDD_TRANSPARENT
# 	2:16:DlgTransparent.h:DlgTransparent1.h
# 	2:10:ENUM: enum:enum
# 	2:22:CLASS: CDlgTransparent:CDlgTransparent
# 	2:19:Application Include:Launcher.h
# End Section
