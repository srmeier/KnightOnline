========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : N3ME
========================================================================


AppWizard has created this N3ME application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your N3ME application.

N3ME.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

N3ME.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CN3MEApp application class.

N3ME.cpp
    This is the main application source file that contains the application
    class CN3MEApp.

N3ME.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

N3ME.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\N3ME.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file N3ME.rc.

res\N3ME.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.



/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in N3ME.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

N3MEDoc.h, N3MEDoc.cpp - the document
    These files contain your CN3MEDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CN3MEDoc::Serialize).

N3MEView.h, N3MEView.cpp - the view of the document
    These files contain your CN3MEView class.
    CN3MEView objects are used to view CN3MEDoc objects.



/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named N3ME.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////


* 맵툴에서 shape 오브젝트를 바꿀때 주의점
  - 이미 배치해놓은 오브젝트의 모델링 파일(shape)을 바꿀 필요가 있을때
	save한 폴더의 하위폴더인 mesh폴더에 있는 shape파일을 바꾸어주기만 하면
	맵상에 배치된 같은 shape파일을 쓰는 것들은 모두 바뀐다.
	하지만 source폴더의 shape도 바꾸어주어야 한다.(반드시..)

* game data 파일 확장자 설명
  - *.gmd : game map data, 아직 아무 내용없음
  - *.gtd : game terrain data, 지형 데이타파일
  - *.god : game object data, 지형에 배치된 object정보(object이름과 위치 회전 스케일값이 저장된 형식)
  - *.grm : game river main, 맵상에 보이는 모든 강의 정보
  - *.grp : game river patch, 맵의 한 구역(현재 128m) 안에 있는 강의 점정보들

* map editor save 파일 확장자 설명
  - *.n3m : n3 map eiditor file,
  - *.rvr : river data, 강 정보들
  - *.sdt : shape data text, object(shape)의 위치 회전 스케일 정보가 text형식으로 되어 있어서 직접 손으로 수정 가능
  - *.trn : terrain data, 지형정보가 들어있다.
