# Add more folders to ship with the application, here
folder_01.source = qml/videoBookmark
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
TARGET="filMark"

symbian:TARGET.UID3 = 0x20044242
symbian:TARGET.CAPABILITY += PowerMgmt

autost =  "\"D:\\Symbian\\9.4\\S60_5th_Edition_SDK_v1.0\\epoc32\\data\\20044242.rsc\"		  -\"c:\\private\\101f875a\\import\\[20044243].rsc\""
autostart.pkg_postrules = autost
symbian:DEPLOYMENT += autostart

vendorinfo =  "%{\"Alexander Fokin\"}" \
    ":\"Alexander Fokin\""
vendor.pkg_prerules = vendorinfo
symbian:DEPLOYMENT += vendor

symbian:DEPLOYMENT.installer_header = "$${LITERAL_HASH}{\"filMark\"},(0x2002CCCF),1,3,0"
packageheader = "$${LITERAL_HASH}{\"filMark\"}, (0x20044243), 1, 3, 0, TYPE=SA"
info.pkg_prerules=packageheader
symbian:DEPLOYMENT+=info


locs.source=D:/Symbian/QtSDK/workspace/videoBookmark-build-desktop/lang
locs.target =
symbian:DEPLOYMENTFOLDERS+=locs


delcr =  "\"D:\\Symbian\\9.4\\S60_5th_Edition_SDK_v1.0\\epoc32\\release\\gcce\\urel\\filMarkSrv.exe\"		  -\"!:\\sys\\bin\\filMarkSrv.exe\""
uninst.pkg_postrules = delcr
symbian:DEPLOYMENT += uninst

symbian:ICON="filMark.svg"

symbian:LIBS+=-leuser


SOURCES += main.cpp\
 app.cpp \
localizer.cpp
HEADERS+= app.h\
localizer.h

include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
