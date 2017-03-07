#win x86
#node-gyp configure
#node-gyp build



#mac x64
rm -rf build
node-gyp configure
node-gyp build

otool -L build/Release/screencapture.node
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore ./deps/qt-5.7.0/darwin/x64/lib/QtCore build/Release/screencapture.node
install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui ./deps/qt-5.7.0/darwin/x64/lib/QtGui build/Release/screencapture.node
install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets ./deps/qt-5.7.0/darwin/x64/lib/QtWidgets build/Release/screencapture.node
install_name_tool -change @rpath/QtDBus.framework/Versions/5/QtDBus ./deps/qt-5.7.0/darwin/x64/lib/QtDBus build/Release/screencapture.node
install_name_tool -change @rpath/QtPrintSupport.framework/Versions/5/QtPrintSupport ./deps/qt-5.7.0/darwin/x64/lib/QtPrintSupport build/Release/screencapture.node
otool -L build/Release/screencapture.node
