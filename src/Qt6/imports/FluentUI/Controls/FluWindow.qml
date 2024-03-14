import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI

Window {
    default property list<QtObject> contentData
    property string windowIcon: FluApp.windowIcon
    property int launchMode: FluWindowType.Standard
    property var argument:({})
    property var background : com_background
    property bool fixSize: false
    property Component loadingItem: com_loading
    property bool fitsAppBarWindows: false
    property Item appBar: FluAppBar {
        title: window.title
        height: 30
        showDark: window.showDark
        showClose: window.showClose
        showMinimize: window.showMinimize
        showMaximize: window.showMaximize
        showStayTop: window.showStayTop
        icon: window.windowIcon
    }
    property color backgroundColor: {
        if(active){
            return FluTheme.windowActiveBackgroundColor
        }
        return FluTheme.windowBackgroundColor
    }
    property bool stayTop: false
    property bool showDark: false
    property bool showClose: true
    property bool showMinimize: true
    property bool showMaximize: true
    property bool showStayTop: false
    property bool autoMaximize: false
    property bool autoVisible: true
    property bool autoCenter: true
    property bool autoDestory: true
    property bool useSystemAppBar
    property color resizeBorderColor: {
        if(window.active){
            return FluTheme.dark ? "#333333" : "#6E6E6E"
        }
        return FluTheme.dark ? "#3D3D3E" : "#A7A7A7"
    }
    property int resizeBorderWidth: 1
    property var closeListener: function(event){
        if(autoDestory){
            destoryOnClose()
        }else{
            visible = false
            event.accepted = false
        }
    }
    signal showSystemMenu
    signal initArgument(var argument)
    signal firstVisible()
    property int _realHeight
    property int _realWidth
    property int _appBarHeight: appBar.height
    property var _windowRegister
    property string _route
    id:window
    color:"transparent"
    Component.onCompleted: {
        _realHeight = height
        _realWidth = width
        useSystemAppBar = FluApp.useSystemAppBar
        if(useSystemAppBar && autoCenter){
            moveWindowToDesktopCenter()
        }
        fixWindowSize()
        lifecycle.onCompleted(window)
        initArgument(argument)
        if(!useSystemAppBar){
            loader_frameless_helper.sourceComponent = com_frameless_helper
        }
        if(window.autoVisible){
            if(window.autoMaximize){
                window.showMaximized()
            }else{
                window.show()
            }
        }
    }
    Component.onDestruction: {
        lifecycle.onDestruction()
    }
    onShowSystemMenu: {
        if(loader_frameless_helper.item){
            loader_frameless_helper.item.showSystemMenu()
        }
    }
    onVisibleChanged: {
        if(visible && d.isFirstVisible){
            window.firstVisible()
            d.isFirstVisible = false
        }
        lifecycle.onVisible(visible)
    }
    onWidthChanged: {
        window.appBar.width = width
    }
    QtObject{
        id:d
        property bool isFirstVisible: true
    }
    Connections{
        target: window
        function onClosing(event){closeListener(event)}
    }
    Component{
        id:com_frameless_helper
        FluFramelessHelper{
            onLoadCompleted:{
                if(autoCenter){
                    window.moveWindowToDesktopCenter()
                }
            }
        }
    }
    Component{
        id:com_background
        Rectangle{
            color: window.backgroundColor
        }
    }
    Component{
        id:com_app_bar
        Item{
            data: window.appBar
        }
    }
    Component{
        id:com_loading
        Popup{
            id:popup_loading
            focus: true
            width: window.width
            height: window.height
            anchors.centerIn: Overlay.overlay
            closePolicy: {
                if(cancel){
                    return Popup.CloseOnEscape | Popup.CloseOnPressOutside
                }
                return Popup.NoAutoClose
            }
            Overlay.modal: Item {}
            onVisibleChanged: {
                if(!visible){
                    loader_loading.sourceComponent = undefined
                }
            }
            padding: 0
            opacity: 0
            visible:true
            Behavior on opacity {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 88
                    }
                    NumberAnimation{
                        duration:  167
                    }
                }
            }
            Component.onCompleted: {
                opacity = 1
            }
            background: Rectangle{
                color:"#44000000"
            }
            contentItem: Item{
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if (cancel){
                            popup_loading.visible = false
                        }
                    }
                }
                ColumnLayout{
                    spacing: 8
                    anchors.centerIn: parent
                    FluProgressRing{
                        Layout.alignment: Qt.AlignHCenter
                    }
                    FluText{
                        text:loadingText
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }
    }
    Component{
        id:com_border
        Rectangle{
            color:"transparent"
            border.width: window.resizeBorderWidth
            border.color: window.resizeBorderColor
        }
    }
    FluLoader{
        id:loader_frameless_helper
    }
    FluLoader{
        anchors.fill: parent
        sourceComponent: background
    }
    FluLoader{
        id:loader_app_bar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: {
            if(window.useSystemAppBar){
                return 0
            }
            return window.fitsAppBarWindows ? 0 : window.appBar.height
        }
        sourceComponent: window.useSystemAppBar ? undefined : com_app_bar
    }
    Item{
        data: window.contentData
        anchors{
            top: loader_app_bar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        clip: true
    }
    FluLoader{
        property string loadingText
        property bool cancel: false
        id:loader_loading
        anchors.fill: parent
    }
    FluInfoBar{
        id:info_bar
        root: window
    }
    FluWindowLifecycle{
        id:lifecycle
    }
    FluLoader{
        id:loader_border
        anchors.fill: parent
        sourceComponent: {
            if(window.useSystemAppBar){
                return undefined
            }
            if(FluTools.isWindows10OrGreater()){
                return undefined
            }
            if(window.visibility === Window.Maximized || window.visibility === Window.FullScreen){
                return undefined
            }
            return com_border
        }
    }
    function destoryOnClose(){
        lifecycle.onDestoryOnClose()
    }
    function showLoading(text = qsTr("Loading..."),cancel = true){
        loader_loading.loadingText = text
        loader_loading.cancel = cancel
        loader_loading.sourceComponent = com_loading
    }
    function hideLoading(){
        loader_loading.sourceComponent = undefined
    }
    function showSuccess(text,duration,moremsg){
        info_bar.showSuccess(text,duration,moremsg)
    }
    function showInfo(text,duration,moremsg){
        info_bar.showInfo(text,duration,moremsg)
    }
    function showWarning(text,duration,moremsg){
        info_bar.showWarning(text,duration,moremsg)
    }
    function showError(text,duration,moremsg){
        info_bar.showError(text,duration,moremsg)
    }
    function moveWindowToDesktopCenter(){
        screen = Qt.application.screens[FluTools.cursorScreenIndex()]
        var availableGeometry = FluTools.desktopAvailableGeometry(window)
        window.setGeometry((availableGeometry.width-window.width)/2+Screen.virtualX,(availableGeometry.height-window.height)/2+Screen.virtualY,window.width,window.height)
    }
    function fixWindowSize(){
        if(fixSize){
            window.maximumWidth =  window.width
            window.maximumHeight =  window.height
            window.minimumWidth = window.width
            window.minimumHeight = window.height
        }
    }
    function registerForWindowResult(path){
        return FluApp.createWindowRegister(window,path)
    }
    function onResult(data){
        if(_windowRegister){
            _windowRegister.onResult(data)
        }
    }
    function showMaximized(){
        if(FluTools.isWin()){
            if(loader_frameless_helper.item){
                loader_frameless_helper.item.showMaximized()
            }
        }else{
            window.visibility = Qt.WindowMaximized
        }
    }
}
