# Microsoft Developer Studio Project File - Name="N3Viewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=N3Viewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3Viewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3Viewer.mak" CFG="N3Viewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3Viewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3Viewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/N3Viewer", YFBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3Viewer - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /D "_N3TOOL" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_N3TOOL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /machine:I386 /force /out:"N3Viewer.exe"

!ELSEIF  "$(CFG)" == "N3Viewer - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_N3TOOL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /D "_N3TOOL" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL" /d "_N3TOOL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d8.lib d3dx8.lib dsound.lib dxerr8.lib dxguid.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"N3Viewer_Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "N3Viewer - Win32 Release"
# Name "N3Viewer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\N3Base\BitMapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBrowsePath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPMeshCreateOption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPMeshEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Board.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBase.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBillBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXParticle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartParticles.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPlug.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeExtra.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ShapeMod.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Viewer.rc
# End Source File
# Begin Source File

SOURCE=.\N3ViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\N3ViewerView.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.cpp
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.cpp"
# End Source File
# Begin Source File

SOURCE=.\ShellTree.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSceneTree.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\N3Base\BitMapFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgBrowsePath.h
# End Source File
# Begin Source File

SOURCE=.\DlgPMeshCreateOption.h
# End Source File
# Begin Source File

SOURCE=.\DlgPMeshEdit.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_Macro.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Board.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBillBoard.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXParticle.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartParticles.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPlug.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeExtra.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ShapeMod.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=.\N3Viewer.h
# End Source File
# Begin Source File

SOURCE=.\N3ViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\N3ViewerView.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.h
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.h"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShellTree.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ViewProperty.h
# End Source File
# Begin Source File

SOURCE=.\ViewSceneTree.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\N3Viewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\N3Viewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\N3ViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\NodeIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\N3Viewer.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
