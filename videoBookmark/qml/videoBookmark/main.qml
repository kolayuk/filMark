import QtQuick 1.0

Rectangle {
    id: root
    width: 360
    height: 640
    Image {
        id: bg
        anchors.fill: parent
        source: "bg.png"
    }
    Image {
        id: button
        property int server: serverlaunched
        anchors.centerIn: root
        source: server==1?"enabled.png":"disabled.png"
        MouseArea{
            anchors.fill: parent;
        onClicked: {
            button.server=button.server==1?0:1; application.Switch(button.server);}
        }
    }
    Image {
        x:225;
        y:595;
        height:38;
        width:38
        source: "about.png"
        scale: quitMouse.pressed ? 0.8 : 1.0
        smooth: quitMouse.pressed
        MouseArea {
            id: quitMouse
            anchors.fill: parent
            anchors.margins: -10
            onClicked: {helpview.changeVisible();}

        }
    }
    MoreButton {x:270; y:595; width:76; height:38;}
    QuitButton {x:308; y:14; width:38; height:38;}
    AboutView{id: aboutview;}
    HelpView{id: helpview;}
    Image {
        x:10
        y:parent.height-height-7
        id: logo
        source: "logo.png"
        Text {
            id: ver
            text: "v 1.3"
            font.bold: true;
            font.pixelSize: 18
            y:parent.height-height-2;
            x:parent.x+parent.width-5;
        }
        MouseArea{
            anchors.fill:  parent
        onClicked: {aboutview.changeVisible();}
        }
    }
}
