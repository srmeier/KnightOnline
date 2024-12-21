# Microsoft Developer Studio Project File - Name="N3FXE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=N3FXE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3FXE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3FXE.mak" CFG="N3FXE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3FXE - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3FXE - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/N3FXE", PSDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3FXE - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib strmiids.lib dxerr8.lib /nologo /subsystem:windows /machine:I386 /out:"N3FXE.exe"

!ELSEIF  "$(CFG)" == "N3FXE - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib d3d8.lib d3dx8.lib dsound.lib dxguid.lib strmiids.lib dxerr8.lib /nologo /subsystem:windows /debug /machine:I386 /out:"N3FXE_Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "N3FXE - Win32 Release"
# Name "N3FXE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\N3FXE.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXE.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\N3FXE.h
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

SOURCE=.\res\N3FXE.ico
# End Source File
# Begin Source File

SOURCE=.\res\N3FXE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\N3FXEDoc.ico
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

SOURCE=..\N3Base\DFont.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\DFont.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\LogWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\LogWriter.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\My_Macro.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AlphaPrimitiveManager.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimControl.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3AnimKey.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Base.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3BaseFileAccess.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Board.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Board.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Camera.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Chr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloak.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloak.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloud.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Cloud.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ColorChange.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ColorChange.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3EngTool.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPlug.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPlug.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GERain.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GERain.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GESnow.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GESnow.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffect.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffect.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffectMng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3GlobalEffectMng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3IMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Joint.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Light.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Mng.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Moon.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Moon.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshCreate.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3PMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3River2.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3River2.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3RiverPatch.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3RiverPatch.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Scene.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Shape.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMod.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMod.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sky.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sky.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3SndEng.h
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

SOURCE=..\N3Base\N3Star.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Star.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sun.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Sun.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TableBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Texture.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Transform.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3VMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\Pick.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\StdAfxBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\WaveFile.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\WaveFile.h
# End Source File
# End Group
# Begin Group "FX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXBundle.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXDef.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXGroup.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBase.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBase.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBillBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBillBoard.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartBottomBoard.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXParticle.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXParticle.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartParticles.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPartParticles.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3FXPMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.h
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgEditFxg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditFxg.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartBillBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartBillBoard.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartGround.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartGround.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartMesh.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartParticle.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditPartParticle.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditScript.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditScript.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewFileName.h
# End Source File
# Begin Source File

SOURCE=.\DlgNewPart.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNewPart.h
# End Source File
# Begin Source File

SOURCE=.\DlgPercent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPercent.h
# End Source File
# Begin Source File

SOURCE=.\DlgPutColor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPutColor.h
# End Source File
# Begin Source File

SOURCE=.\Ground.cpp
# End Source File
# Begin Source File

SOURCE=.\Ground.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\N3FXEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXEDoc.h
# End Source File
# Begin Source File

SOURCE=.\N3FXEView.cpp
# End Source File
# Begin Source File

SOURCE=.\N3FXEView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
