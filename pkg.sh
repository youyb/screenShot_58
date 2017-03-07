install_name_tool -change ./deps/qt-5.7.0/darwin/x64/lib/QtCore @executable_path/../Resources/app/lib/QtCore screencapture.node

install_name_tool -change ./deps/qt-5.7.0/darwin/x64/lib/QtGui @executable_path/../Resources/app/lib/QtGui screencapture.node

install_name_tool -change ./deps/qt-5.7.0/darwin/x64/lib/QtWidgets @executable_path/../Resources/app/lib/QtWidgets screencapture.node

install_name_tool -change ./deps/qt-5.7.0/darwin/x64/lib/QtDBus @executable_path/../Resources/app/lib/QtDBus screencapture.node

install_name_tool -change ./deps/qt-5.7.0/darwin/x64/lib/QtPrintSupport @executable_path/../Resources/app/lib/QtPrintSupport screencapture.node