#ifndef BUILDING_NODE_EXTENSION
#define BUILDING_NODE_EXTENSION
#endif

#include <node.h>

//#include "QtCore/qsize.h"

#include "QtGui/qapplication.h"
//#include "QtGui/qlabel.h"
//#include "QtGui/qpushbutton.h"
//#include "QtGui/qwidget.h"

using namespace v8;

void Initialize(Handle<Object> target) {
  QApplicationWrap::Initialize(target);
  //QLabelWrap::Initialize(target);
  //QPushButtonWrap::Initialize(target);
  //QWidgetWrap::Initialize(target);
  //QMainWindowWrap::Initialize(target);
}

NODE_MODULE(screencapture, Initialize)
