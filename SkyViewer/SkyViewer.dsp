# Microsoft Developer Studio Project File - Name="SkyViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SkyViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SkyViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SkyViewer.mak" CFG="SkyViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SkyViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SkyViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SkyViewer", WXDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SkyViewer - Win32 Release"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d8.lib d3dx8.lib dxguid.lib dxerr8.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"SkyViewer.exe"

!ELSEIF  "$(CFG)" == "SkyViewer - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_N3TOOL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib dxguid.lib dxerr8.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"SkyViewer_Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SkyViewer - Win32 Release"
# Name "SkyViewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FormViewProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.cpp"
# End Source File
# Begin Source File

SOURCE=.\SkyViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\SkyViewer.rc
# End Source File
# Begin Source File

SOURCE=.\SkyViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SkyViewerView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FormViewProperty.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.h"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SkyViewer.h
# End Source File
# Begin Source File

SOURCE=.\SkyViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\SkyViewerView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SkyViewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\SkyViewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SkyViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "N3Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\BitMapFile.h
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

SOURCE=..\N3BASE\N3AnimKey.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3AnimKey.h
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

SOURCE=..\N3BASE\N3Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Camera.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Cloud.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ColorChange.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ColorChange.h
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

SOURCE=..\N3BASE\N3GERain.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GERain.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GESnow.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GESnow.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GlobalEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GlobalEffect.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GlobalEffectMng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3GlobalEffectMng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3IMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3IMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Light.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Light.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Mng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Moon.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Moon.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3PMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3PMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Shape.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sky.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sky.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SkyMng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3SkyMng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Star.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Star.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sun.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Sun.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3Transform.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3VMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3VMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\StdAfxBase.h
# End Source File
# End Group
# End Target
# End Project
