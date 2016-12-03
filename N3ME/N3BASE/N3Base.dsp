# Microsoft Developer Studio Project File - Name="N3Base" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=N3Base - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "N3Base.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "N3Base.mak" CFG="N3Base - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "N3Base - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "N3Base - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Noah3D New Project", PAEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "N3Base - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "N3Base - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "N3Base - Win32 Release"
# Name "N3Base - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\N3AnimControl.cpp
# End Source File
# Begin Source File

SOURCE=.\N3AnimKey.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Base.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Chr.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Eng.cpp
# End Source File
# Begin Source File

SOURCE=.\N3IMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Joint.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Light.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Moon.cpp
# End Source File
# Begin Source File

SOURCE=.\N3PMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\N3PMeshCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\N3PMeshInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\N3River2.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Shape.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Skin.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\N3Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\N3TransformCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\N3VMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Pick.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\My_3DStruct.h
# End Source File
# Begin Source File

SOURCE=.\My_3DStructExt.h
# End Source File
# Begin Source File

SOURCE=.\My_Macro.h
# End Source File
# Begin Source File

SOURCE=.\N3AnimControl.h
# End Source File
# Begin Source File

SOURCE=.\N3AnimKey.h
# End Source File
# Begin Source File

SOURCE=.\N3Base.h
# End Source File
# Begin Source File

SOURCE=.\N3Camera.h
# End Source File
# Begin Source File

SOURCE=.\N3Chr.h
# End Source File
# Begin Source File

SOURCE=.\N3DMesh.h
# End Source File
# Begin Source File

SOURCE=.\N3DUtil.h
# End Source File
# Begin Source File

SOURCE=.\N3DUtil.inl
# End Source File
# Begin Source File

SOURCE=.\N3Eng.h
# End Source File
# Begin Source File

SOURCE=.\N3IMesh.h
# End Source File
# Begin Source File

SOURCE=.\N3Joint.h
# End Source File
# Begin Source File

SOURCE=.\N3Light.h
# End Source File
# Begin Source File

SOURCE=.\N3LinkedList.h
# End Source File
# Begin Source File

SOURCE=.\N3Mesh.h
# End Source File
# Begin Source File

SOURCE=.\N3Mng.h
# End Source File
# Begin Source File

SOURCE=.\N3Moon.h
# End Source File
# Begin Source File

SOURCE=.\N3PMesh.h
# End Source File
# Begin Source File

SOURCE=.\N3PMeshCreate.h
# End Source File
# Begin Source File

SOURCE=.\N3PMeshInstance.h
# End Source File
# Begin Source File

SOURCE=.\N3River2.h
# End Source File
# Begin Source File

SOURCE=.\N3Scene.h
# End Source File
# Begin Source File

SOURCE=.\N3Shape.h
# End Source File
# Begin Source File

SOURCE=.\N3Skin.h
# End Source File
# Begin Source File

SOURCE=.\N3SMesh.h
# End Source File
# Begin Source File

SOURCE=.\N3SubScene.h
# End Source File
# Begin Source File

SOURCE=.\N3Texture.h
# End Source File
# Begin Source File

SOURCE=.\N3Transform.h
# End Source File
# Begin Source File

SOURCE=.\N3TransformCollision.h
# End Source File
# Begin Source File

SOURCE=.\N3VectorAndMatrix.h
# End Source File
# Begin Source File

SOURCE=.\N3VectorAndMatrix.inl
# End Source File
# Begin Source File

SOURCE=.\N3VMesh.h
# End Source File
# Begin Source File

SOURCE=.\Pick.h
# End Source File
# End Group
# Begin Group "Reference"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
