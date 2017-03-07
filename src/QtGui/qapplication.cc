//#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "qapplication.h"
#include "qdesktopwidget.h"
#include "qscreen.h"
#include "../sc/fullscreenimageeditor.h"
#include "../sc/commondef.h"

using namespace v8;

// Persistent<Function> QApplicationWrap::constructor;
int QApplicationWrap::argc_ = 0;
char** QApplicationWrap::argv_ = NULL;

QApplicationWrap::QApplicationWrap() {
    //QDBG<<"addLibraryPath must be called before QApplication constructor.";
    //no need to add this lib path when package, just use @executable_path instead
    QCoreApplication::addLibraryPath("./");  //win32 and mac platform all need this for debug version
    q_ = new QApplication(argc_, argv_);
}

QApplicationWrap::~QApplicationWrap() {
    //no need to delete q_ manually, qt will do it
    //delete q_;
}

void QApplicationWrap::Initialize(Handle<Object> target) {

    // Prepare constructor template
    Isolate* isolate = target->GetIsolate();

    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "QApplicationWrap"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "exec", Exec);
    NODE_SET_PROTOTYPE_METHOD(tpl, "screenCapture", ScreenCapture);

    //constructor.Reset(isolate, tpl->GetFunction());
    target->Set(String::NewFromUtf8(isolate, "Main"), tpl->GetFunction());
}

void QApplicationWrap::New(const FunctionCallbackInfo<Value>& args) {
    // Isolate* isolate = args.GetIsolate();
    if (args.IsConstructCall()) {
        QApplicationWrap* obj = new QApplicationWrap();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        // const int argc = 1;
        // Local<Value> argv[argc] = { args[0] };
        // Local<Context> context = isolate->GetCurrentContext();
        // Local<Function> cons = Local<Function>::New(isolate, constructor);
        // Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
        // args.GetReturnValue().Set(instance);
    }
}

void QApplicationWrap::Exec(const FunctionCallbackInfo<Value>& args) {
    QApplicationWrap* w = ObjectWrap::Unwrap<QApplicationWrap>(args.This());
    QApplication* q = w->GetWrapped();

    q->exec();
}

void QApplicationWrap::ScreenCapture(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    QApplicationWrap* w = ObjectWrap::Unwrap<QApplicationWrap>(args.This());
    QApplication* q = w->GetWrapped();

    QString imgDir;
    if( (args.Length() == 2) && args[0]->IsString() ) {
        String::Utf8Value arg(args[0]->ToString());
        imgDir = QString::fromStdString(*arg);
		QDBG<<"imgDir: "<<imgDir.toStdString().c_str();
    }

    QPixmap image = q->primaryScreen()->grabWindow(q->desktop()->winId());
    double dpr = QApplication::desktop()->devicePixelRatio();
	QDBG<<"dpr: "<<dpr;
    if(dpr == 2.0)
    {
        //set 2.0 for retina screen, 1.0 for non-retina screen
        image.setDevicePixelRatio(dpr);
    }

    //print current lib path
    foreach (const QString &path, q->libraryPaths())
    {
        QDBG<<path;
    }

    FullScreenImageEditor scWindow(image, imgDir, dpr);
    // scWindow.showFullScreen();  //this will call memory increates each time, this method not used for OS X
    scWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  //lock the window
    //scWindow.setWindowFlags(Qt::FramelessWindowHint);  //hide the window title
    scWindow.show();  //should resize mainwindow to adapt the input image

    q->exec();

	QDBG<<"g_ImagePath: "<<g_ImagePath.toStdString().c_str();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, g_ImagePath.toStdString().data()));

    Local<Function> cb;
    const unsigned argc = 1;
    if( (args.Length() == 2) && !g_ImagePath.isEmpty() )
    {
        cb = Local<Function>::Cast(args[1]);
        QDBG<<"sc finished, return image url, run js cb.";
        Local<Value> argv[argc] = { String::NewFromUtf8(isolate, g_ImagePath.toStdString().data()) };
        cb->Call(Null(isolate), argc, argv);
        g_ImagePath = "";  //clear previous url
    }
    else if( (args.Length() == 2) && g_ImagePath.isEmpty() )
    {
        cb = Local<Function>::Cast(args[1]);
        QDBG<<"sc cancel, url is null, just exit.";
        Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "") };
        cb->Call(Null(isolate), argc, argv);
    }
    else
    {
        QDBG<<"other";
    }

}
