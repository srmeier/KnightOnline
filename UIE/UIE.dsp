# Microsoft Developer Studio Project File - Name="UIE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=UIE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UIE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UIE.mak" CFG="UIE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UIE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "UIE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/UIE", IADAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UIE - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3UIE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"UIE.exe"

!ELSEIF  "$(CFG)" == "UIE - Win32 Debug"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "C:\Documents and Settings\Admin\kodev\DXF\DXSDK\include\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /D "_N3UIE" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"UIE_Debug.exe" /pdbtype:sept /libpath:"C:\Documents and Settings\Admin\kodev\DXF\DXSDK\lib"

!ENDIF 

# Begin Target

# Name "UIE - Win32 Release"
# Name "UIE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DlgChangeImage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnusedFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\HierarchyView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIList.cpp
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.cpp"
# End Source File
# Begin Source File

SOURCE=.\PropertyView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\UIE.cpp
# End Source File
# Begin Source File

SOURCE=.\UIE.rc
# End Source File
# Begin Source File

SOURCE=.\UIEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\UIEView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DlgChangeImage.h
# End Source File
# Begin Source File

SOURCE=.\DlgUnusedFileList.h
# End Source File
# Begin Source File

SOURCE=.\HierarchyView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIList.h
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.h"
# End Source File
# Begin Source File

SOURCE=.\PropertyView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UIE.h
# End Source File
# Begin Source File

SOURCE=.\UIEDoc.h
# End Source File
# Begin Source File

SOURCE=.\UIEView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UIE.ico
# End Source File
# Begin Source File

SOURCE=.\res\UIE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\UIEDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\zoomin.cur
# End Source File
# Begin Source File

SOURCE=.\res\zoomout.cur
# End Source File
# End Group
# Begin Group "N3Base"

# PROP Default_Filter ""
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3BASE\N3UIArea.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIArea.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIBase.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIBase.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIButton.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIButton.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIDef.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIEdit.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIImage.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIImage.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIManager.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIProgress.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIProgress.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIStatic.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIString.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UIString.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UITooltip.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UITooltip.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UITrackBar.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3UITrackBar.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\N3BASE\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\BitMapFile.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\DFont.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\DFont.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\LogWriter.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3AlphaPrimitiveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3AlphaPrimitiveManager.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Base.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3BaseFileAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3BaseFileAccess.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3EngTool.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Mng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndObj.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndObj.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndObjStream.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SndObjStream.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\WaveFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\WaveFile.h
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Group "DlgTexture"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTexture.h
# End Source File
# Begin Source File

SOURCE=.\TexViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\TexViewer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgAnimate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAnimate.h
# End Source File
# Begin Source File

SOURCE=.\DlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReplace.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
