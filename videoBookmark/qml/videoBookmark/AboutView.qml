import QtQuick 1.0
MouseArea{
    id: about
    anchors.fill: parent;
    visible: false;
    onClicked: {changeVisible();}
    function changeVisible(){
    if (visible){hideAnim.start();}
    else {showAnim.start();}
    }

    NumberAnimation {
        id:hideAnim
        target: view;
        property: "opacity";
        to: 0;
        duration: 300
        onCompleted: {about.visible=false;}
    }
    NumberAnimation {
        id:showAnim
        target: view;
        property: "opacity";
        from: 0
        to: 0.8
        duration: 300
        onStarted: {about.visible=true;}
    }
    Rectangle{
        anchors.fill: parent
        color: "black"
        opacity: 0.3
    }

    Rectangle {

        id: view
        width: (5.0*parent.width)/6.0
        height:txt.height+logo.height+30;
        x:(1.0*parent.width)/12.0
        y:(1.0*parent.height)/4.0 //help?(1.0*parent.height)/6.0:(2.0*parent.height)/6.0
        opacity: 0.8;
        color: "black"
    }
    Rectangle{
    color: "grey"
    opacity: view.opacity==0.8?1:view.opacity
    id: logo
    border.color: "grey"
    border.width: 1
    anchors.horizontalCenter: view.horizontalCenter
    y:view.y+10;
    width: im.width;
    height:  im.height+5
    radius: 0
        Image {
            id: im
            y: 5
            source: "logo.png"
        }
    }

    Text{
        width: view.width-10;
        color: "white"
        y: logo.y+logo.height+10;
        id: txt
        font.pixelSize: 20;
        text: strings[0];
        x:view.x+5;
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
