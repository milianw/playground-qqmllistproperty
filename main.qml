import QtQuick 2.7
import QtQuick.Window 2.2

import foo.bar 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    function printList(name, list)
    {
        console.warn(name);
        console.log("length = ", list.length);
        console.log("list = ", list);
        console.log("typeof(list) = ", typeof(list));
        console.log("JSON.toString(list) = ", JSON.toString(list));
        for (var i in list) {
            console.log(i, typeof(list[i]), list[i]);
        }
    }

    Component.onCompleted: {
        printList("objects", myObject.objectList);
        console.log(myObject.objectList[0].objectName);
        console.log(myObject.objectList[1].objectName);

        printList("gadgets", myObject.gadgetList);
        //console.log(myObject.gadgetList[0].a, myObject.gadgetList[0].b);
        //console.log(myObject.gadgetList[1].a, myObject.gadgetList[1].b);

        printList("gadget vector", myObject.gadgetVector);
        //console.log(myObject.gadgetVector[0]);
        //console.log(myObject.gadgetVector[1]);

        Qt.quit();
    }
}
