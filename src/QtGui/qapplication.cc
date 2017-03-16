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
    NODE_SET_PROTOTYPE_METHOD(tpl, "getFilePathFromClipboard", GetFilePathFromClipboard);

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

#if 0
struct thread_data {
    QPixmap image;
    QString dir;  
    double dpr;
    // QApplication * qa;
};

static void appCallSC(void* arg) {  
    thread_data * data = (thread_data *)arg;
    QDBG<<data->image.width()<<", "<<data->image.height();
    QDBG<<data->dir;
    printf("dpr: %f\n", data->dpr);
    // uv_thread_t *thread_id = (uv_thread_t *)arg;  
    uv_thread_t self_id = uv_thread_self();  
    // printf("main_thread_id:%lu\n", main_thread_id);  
    printf("sub_thread_id: %ld\n", self_id);  
    // *thread_id = uv_thread_self();  
    // QApplication * qq = data->qa;

    int argc_ = 0;
    char** argv_ = NULL;
    // QApplication * qq = new QApplication(argc_, argv_);
    QApplication qq(argc_, argv_);
    // if(qq)
    // {
    //     QDBG<<"qq exist.";
    // }


        FullScreenImageEditor scWindow(data->image, data->dir, data->dpr);
        scWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  //lock the window
        scWindow.show();  //should resize mainwindow to adapt the input image
        // g_IsRunning = true;

        qq.exec();
        // (data->qa)->exec();
}
#endif

void QApplicationWrap::ScreenCapture(const FunctionCallbackInfo<Value>& args) {

    // uv_thread_t sub_thread_id;
    // uv_thread_t main_thread_id = uv_thread_self();
    // printf("main_thread_id: %lu\n", main_thread_id);

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


    // thread_data args_in;
    // thread_data * pTmp = &args_in;
    // pTmp->image = image;
    // pTmp->dir = imgDir;
    // pTmp->dpr = dpr;    
    // pTmp->qa = new QApplication(argc_, argv_);
    // pTmp->qa = q;

    // QDBG<<g_IsRunning;
    // if(!g_IsRunning)
    // {
    //     uv_thread_create(&sub_thread_id, appCallSC, pTmp);
    //     uv_thread_join(&sub_thread_id);
    // }

    // QDBG<<isRunning_aaa;
    // if(isRunning_aaa == false)
    // {
        FullScreenImageEditor scWindow(image, imgDir, dpr);
        scWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  //lock the window
        scWindow.show();  //should resize mainwindow to adapt the input image

    //     isRunning_aaa = true;

    // // scWindow.setWindowFlags(Qt::FramelessWindowHint ); 
    // // scWindow.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    // // scWindow.showFullScreen();
    // }
    // else
    // {
    //     QDBG<<"bbbbbbb";
    // }

    // (pTmp->qa)->exec();
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
        // g_IsRunning = false; //reset
    }
    else if( (args.Length() == 2) && g_ImagePath.isEmpty() )
    {
        cb = Local<Function>::Cast(args[1]);
        QDBG<<"sc cancel, url is null, just exit.";
        Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "") };
        cb->Call(Null(isolate), argc, argv);
        // g_IsRunning = false; //reset
    }
    else
    {
        QDBG<<"other";
    }

}

void QApplicationWrap::GetFilePathFromClipboard(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();

    QString urls = "";
    const QMimeData * mimeData = QApplication::clipboard()->mimeData();
    if(mimeData->hasUrls())
    {
        QList<QUrl> list = mimeData->urls();
        for(int i=0; i<list.count(); i++)
        {
            QDBG<<list.at(i).fileName();
            QDBG<<list.at(i).path();
            if(!list.at(i).fileName().isEmpty())
            {
                urls += list.at(i).path() + ",";
            }
            else
            {
                QDBG<<"dir found, skip it.";
            }
        }
    }
    else
    {
        QDBG<<"no url in clipboard.";
    }

    QDBG<<"clipboard urls: "<<urls;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, urls.toStdString().data()));
}
