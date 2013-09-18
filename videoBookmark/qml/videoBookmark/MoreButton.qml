import QtQuick 1.0

Image {
    source: "more.png"
    scale: quitMouse.pressed ? 0.8 : 1.0
    smooth: quitMouse.pressed
    MouseArea {
        id: quitMouse
        anchors.fill: parent
        anchors.margins: -10
        onClicked: {application.OtherApps();}
    }
}
