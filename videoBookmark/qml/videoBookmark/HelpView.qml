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
        height: (4.0*parent.height)/6.0 //help?(2.0*parent.height)/3.0:(1.0*parent.height)/3.0
        x:(1.0*parent.width)/12.0
        y:(1.0*parent.height)/6.0 //help?(1.0*parent.height)/6.0:(2.0*parent.height)/6.0
        opacity: 0.8;
        color: "black"
    }
    Flickable {
               id: flickArea
                anchors.fill: view
                contentWidth: helpText.width; contentHeight: helpText.height
                flickableDirection: Flickable.VerticalFlick
                clip: true

                Text{
                    width: view.width-20;
                    color: "white"
                    y:0
                    id: helpText;
                    font.pixelSize: 20;
                    text: strings[1];
                    x:10;
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
           }
}
