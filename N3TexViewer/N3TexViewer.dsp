# Microsoft Developer Studio Project File - Name="N3TexViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=N3TexViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3TexViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3TexViewer.mak" CFG="N3TexViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3TexViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3TexViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/N3TexViewer", UNAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3TexViewer - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "_N3TOOL" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d8.lib d3dx8.lib dxerr8.lib dxguid.lib dsound.lib winmm.lib /nologo /subsystem:windows /machine:I386 /force /out:"N3TexViewer.exe" /force:multiple
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "N3TexViewer - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /D "_N3TOOL" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib dxerr8.lib dxguid.lib dsound.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"N3TexViewer_Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "N3TexViewer - Win32 Release"
# Name "N3TexViewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\N3Base\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\N3TexViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\N3TexViewer.rc
# End Source File
# Begin Source File

SOURCE=.\N3TexViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\N3TexViewerView.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\N3Base\BitMapFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgFormat.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.h
# End Source File
# Begin Source File

SOURCE=.\N3TexViewer.h
# End Source File
# Begin Source File

SOURCE=.\N3TexViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\N3TexViewerView.h
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\StdAfxBase.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\N3TexViewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\N3TexViewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\N3TexViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
