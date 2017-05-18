# Microsoft Developer Studio Project File - Name="N3ME" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=N3ME - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3ME.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3ME.mak" CFG="N3ME - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3ME - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "N3ME - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/N3ME", DHBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3ME - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /D "_KNIGHT" /Fr /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 D3D8.lib D3DX8.lib dxerr8.lib dsound.lib dxguid.lib winmm.lib /nologo /subsystem:windows /map /machine:I386 /force /out:"N3ME.exe" /opt:ref
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "N3ME - Win32 Debug"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_N3TOOL" /D "_KNIGHT" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 D3D8.lib D3DX8.lib dxerr8.lib dsound.lib dxguid.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /force /out:"N3ME_debug.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "N3ME - Win32 Release"
# Name "N3ME - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\load.cur
# End Source File
# Begin Source File

SOURCE=.\res\move.cur
# End Source File
# Begin Source File

SOURCE=.\res\N3ME.ico
# End Source File
# Begin Source File

SOURCE=.\res\N3ME.rc2
# End Source File
# Begin Source File

SOURCE=.\res\N3MEDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\rotate.cur
# End Source File
# Begin Source File

SOURCE=.\res\SceneNodes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\transform.cur
# End Source File
# Begin Source File

SOURCE=.\res\zoom.cur
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

SOURCE=..\N3Base\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Eng.h
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3EngTool.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3EngTool.h
# End Source File
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

SOURCE=..\N3Base\N3FXPlug.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPlug.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMesh.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXPMeshInstance.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3FXShape.h
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

SOURCE=..\N3BASE\N3ShapeExtra.cpp
# End Source File
# Begin Source File

SOURCE=..\N3BASE\N3ShapeExtra.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3ShapeMgr.h
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=..\N3Base\N3Skin.h
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
# End Group
# Begin Group "N3ME"

# PROP Default_Filter ""
# Begin Group "Frames"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\N3ME.cpp
# End Source File
# Begin Source File

SOURCE=.\N3ME.h
# End Source File
# Begin Source File

SOURCE=.\N3ME.rc
# End Source File
# Begin Source File

SOURCE=.\N3MEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\N3MEDoc.h
# End Source File
# Begin Source File

SOURCE=.\N3MEView.cpp
# End Source File
# Begin Source File

SOURCE=.\N3MEView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BrushDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BrushDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddDTex.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddDTex.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddSoundGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddSoundGroup.h
# End Source File
# Begin Source File

SOURCE=.\DlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgBrowsePath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBrowsePath.h
# End Source File
# Begin Source File

SOURCE=.\DlgCtrlHeightScale.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCtrlHeightScale.h
# End Source File
# Begin Source File

SOURCE=.\DlgDelGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDelGroup.h
# End Source File
# Begin Source File

SOURCE=.\DlgDTexGroupView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDTexGroupView.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditEvent.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditEventAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditEventAttr.h
# End Source File
# Begin Source File

SOURCE=.\DlgEditWarp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEditWarp.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputAttr.h
# End Source File
# Begin Source File

SOURCE=.\DlgInputGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInputGroup.h
# End Source File
# Begin Source File

SOURCE=.\DlgLight.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLight.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoadEvt.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoadEvt.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoadNPCPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoadNPCPath.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoadTileSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoadTileSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgMakeNPCPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMakeNPCPath.h
# End Source File
# Begin Source File

SOURCE=.\DlgMakeWall.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMakeWall.h
# End Source File
# Begin Source File

SOURCE=.\DlgModifyDTex.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgModifyDTex.h
# End Source File
# Begin Source File

SOURCE=.\DlgPondProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPondProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgRegenUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRegenUser.h
# End Source File
# Begin Source File

SOURCE=.\DlgRiverProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRiverProperty.h
# End Source File
# Begin Source File

SOURCE=.\DlgSaveDivision.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSaveDivision.h
# End Source File
# Begin Source File

SOURCE=.\DlgSaveEvt.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSaveEvt.h
# End Source File
# Begin Source File

SOURCE=.\DlgSaveNewTileSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSaveNewTileSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgSaveNPCPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSaveNPCPath.h
# End Source File
# Begin Source File

SOURCE=.\DlgSceneGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSceneGraph.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetDTex.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetDTex.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetLightMap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetLightMap.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetSound.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetSound.h
# End Source File
# Begin Source File

SOURCE=.\DlgShapeList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgShapeList.h
# End Source File
# Begin Source File

SOURCE=.\DlgSowSeed.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSowSeed.h
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainSize.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTerrainSize.h
# End Source File
# Begin Source File

SOURCE=.\DlgUnusedFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUnusedFiles.h
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.cpp"
# End Source File
# Begin Source File

SOURCE="..\Common Control\PropertyList.h"
# End Source File
# Begin Source File

SOURCE=.\ShellTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellTree.h
# End Source File
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DTex.cpp
# End Source File
# Begin Source File

SOURCE=.\DTex.h
# End Source File
# Begin Source File

SOURCE=.\DTexGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DTexGroup.h
# End Source File
# Begin Source File

SOURCE=.\DTexGroupMng.cpp
# End Source File
# Begin Source File

SOURCE=.\DTexGroupMng.h
# End Source File
# Begin Source File

SOURCE=.\DTexMng.cpp
# End Source File
# Begin Source File

SOURCE=.\DTexMng.h
# End Source File
# Begin Source File

SOURCE=.\EventCell.cpp
# End Source File
# Begin Source File

SOURCE=.\EventCell.h
# End Source File
# Begin Source File

SOURCE=.\EventMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\EventMgr.h
# End Source File
# Begin Source File

SOURCE=.\LightObjMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\LightObjMgr.h
# End Source File
# Begin Source File

SOURCE=.\LyTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\LyTerrain.h
# End Source File
# Begin Source File

SOURCE=.\LyTerrainDef.h
# End Source File
# Begin Source File

SOURCE=.\MapMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MapMng.h
# End Source File
# Begin Source File

SOURCE=.\NPCPath.cpp
# End Source File
# Begin Source File

SOURCE=.\NPCPath.h
# End Source File
# Begin Source File

SOURCE=.\NPCPathMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\NPCPathMgr.h
# End Source File
# Begin Source File

SOURCE=.\PondMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\PondMesh.h
# End Source File
# Begin Source File

SOURCE=.\PondMng.cpp
# End Source File
# Begin Source File

SOURCE=.\PondMng.h
# End Source File
# Begin Source File

SOURCE=.\PosDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\PosDummy.h
# End Source File
# Begin Source File

SOURCE=.\QTNode.cpp
# End Source File
# Begin Source File

SOURCE=.\QTNode.h
# End Source File
# Begin Source File

SOURCE=.\RegenUser.cpp
# End Source File
# Begin Source File

SOURCE=.\RegenUser.h
# End Source File
# Begin Source File

SOURCE=.\RiverMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\RiverMesh.h
# End Source File
# Begin Source File

SOURCE=.\RiverMng.cpp
# End Source File
# Begin Source File

SOURCE=.\RiverMng.h
# End Source File
# Begin Source File

SOURCE=.\RotDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\RotDummy.h
# End Source File
# Begin Source File

SOURCE=.\ScaleDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\ScaleDummy.h
# End Source File
# Begin Source File

SOURCE=.\SoundCell.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundCell.h
# End Source File
# Begin Source File

SOURCE=.\SoundMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundMgr.h
# End Source File
# Begin Source File

SOURCE=.\TransDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\TransDummy.h
# End Source File
# Begin Source File

SOURCE=.\VtxPosDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\VtxPosDummy.h
# End Source File
# Begin Source File

SOURCE=.\Wall.cpp
# End Source File
# Begin Source File

SOURCE=.\Wall.h
# End Source File
# Begin Source File

SOURCE=.\WallMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\WallMgr.h
# End Source File
# Begin Source File

SOURCE=.\WarpMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\WarpMgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgMapView.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMapView.h
# End Source File
# Begin Source File

SOURCE=.\help.doc
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SowSeedMng.cpp
# End Source File
# Begin Source File

SOURCE=.\SowSeedMng.h
# End Source File
# End Target
# End Project
# Section N3ME : {EC26A56F-9BC0-47D1-9CD5-AD20677F2E75}
# 	2:20:CLASS: CPropertyList:CPropertyList
# 	2:20:CLASS: CPropertyItem:CPropertyItem
# 	2:16:PropertyList.cpp:PropertyList.cpp
# 	2:19:Application Include:n3me.h
# 	2:14:PropertyList.h:PropertyList.h
# End Section
# Section N3ME : {6C6F5827-5C4B-4768-9AE9-BA4C6498D31D}
# 	1:16:IDD_SCENE_GRAPH1:102
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_SCENE_GRAPH:IDD_SCENE_GRAPH1
# 	2:15:DlgSceneGraph.h:DlgSceneGraph.h
# 	2:10:ENUM: enum:enum
# 	2:17:DlgSceneGraph.cpp:DlgSceneGraph.cpp
# 	2:19:Application Include:n3me.h
# 	2:21:CLASS: CDlgSceneGraph:CDlgSceneGraph
# End Section
# Section N3ME : {AD4878B9-F45B-461F-98D8-C3C661FF78E9}
# 	2:20:CLASS: CPropertyList:CPropertyList
# 	2:20:CLASS: CPropertyItem:CPropertyItem
# 	2:16:PropertyList.cpp:PropertyList1.cpp
# 	2:14:PropertyList.h:PropertyList1.h
# 	2:19:Application Include:n3me.h
# End Section
# Section N3ME : {53B0972A-3F58-4A16-A9D8-9CB4A0DE837B}
# 	1:8:IDD_BASE:106
# 	2:16:Resource Include:resource.h
# 	2:9:DlgBase.h:DlgBase1.h
# 	2:10:ENUM: enum:enum
# 	2:11:DlgBase.cpp:DlgBase1.cpp
# 	2:15:CLASS: CDlgBase:CDlgBase
# 	2:8:IDD_BASE:IDD_BASE
# 	2:19:Application Include:n3me.h
# End Section
# Section N3ME : {498F2CC1-DEF6-4910-B793-61F5F8F18A25}
# 	1:8:IDD_BASE:105
# 	2:16:Resource Include:resource.h
# 	2:9:DlgBase.h:DlgBase.h
# 	2:10:ENUM: enum:enum
# 	2:11:DlgBase.cpp:DlgBase.cpp
# 	2:15:CLASS: CDlgBase:CDlgBase
# 	2:8:IDD_BASE:IDD_BASE
# 	2:19:Application Include:n3me.h
# End Section
