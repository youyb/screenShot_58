/********************************************************************************
** Form generated from reading UI file 'fullscreenimageeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FULLSCREENIMAGEEDITOR_H
#define UI_FULLSCREENIMAGEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
//#include <QtWidgets/QButtonGroup>
//#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QPushButton>
//#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qdesktopwidget.h"
#include "qscreen.h"
#include "commondef.h"

QT_BEGIN_NAMESPACE

class Ui_FullScreenImageEditor
{
public:
    QWidget *centerWidget;
//    QWidget *cmdPanel;
//    QVBoxLayout *verticalLayout;

    void setupUi(QMainWindow *FullScreenImageEditor)
    {
        if (FullScreenImageEditor->objectName().isEmpty())
        {
            FullScreenImageEditor->setObjectName(QStringLiteral("FullScreenImageEditor"));
        }
        QPixmap screen = QApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId());
        QDBG<<"set MainWindow size: "<<screen.width()<<", "<<screen.height();
        FullScreenImageEditor->resize(screen.width(), screen.height());
        FullScreenImageEditor->setMouseTracking(true);
        FullScreenImageEditor->setAutoFillBackground(false);
        centerWidget = new QWidget(FullScreenImageEditor);
        centerWidget->setObjectName(QStringLiteral("centerWidget"));
        centerWidget->setMouseTracking(true);
//        cmdPanel = new QWidget(centerWidget);
//        cmdPanel->setObjectName(QStringLiteral("cmdPanel"));
//        cmdPanel->setGeometry(QRect(150, 120, 402, 101));
//        verticalLayout = new QVBoxLayout(cmdPanel);
//        verticalLayout->setSpacing(0);
//        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
//        verticalLayout->setContentsMargins(0, 0, 0, 0);

        FullScreenImageEditor->setCentralWidget(centerWidget);
//        cmdPanel->raise();

        retranslateUi(FullScreenImageEditor);
        QMetaObject::connectSlotsByName(FullScreenImageEditor);
    } // setupUi

    void retranslateUi(QMainWindow *FullScreenImageEditor)
    {
        FullScreenImageEditor->setWindowTitle(QApplication::translate("FullScreenImageEditor", "MainWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class FullScreenImageEditor: public Ui_FullScreenImageEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FULLSCREENIMAGEEDITOR_H
