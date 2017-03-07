#ifndef FULLSCREENIMAGEEDITOR_H
#define FULLSCREENIMAGEEDITOR_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QRect>
#include <QPixmap>
//#include <QPainter>
//#include <QTextEdit>
//#include "paintinghistory.h"

namespace Ui {
class FullScreenImageEditor;
}


enum CanvasMode {
    INVALID = -1,
    IDLE = 0,
    SELECTING,
    SELECTED,
    RESIZING,
    MOVING,
    PAINTING
};

//enum PaintingMode {
//    PEN = 0,
//    OVAL,
//    ARROW,
//    RECT,
//    MOSAIC,
//    TEXT
//};

enum MousePosition {
    North = 1,
    East = 2,
    South = 4,
    West = 8,
    Other = 16,
    NorthWest = North | West,
    NorthEast = North | East,
    SouthEast = South | East,
    SouthWest = South | West
};

extern QString g_ImagePath;
extern QString g_ImageDir;
extern double g_dpr;

class FullScreenImageEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit FullScreenImageEditor(QPixmap &image, QString imgDir, double dpr, QWidget *parent = 0);
    ~FullScreenImageEditor();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    bool isInSelectedArea(int x, int y);
    void changeCursorIfNessesary(int mouseX, int mouseY);
    void resizingSelectedArea(int mouseX, int mouseY);
    void moveSelectedAreaLimitInScreen(int mouseX, int mouseY);
    void selectArea(int mouseX, int mouseY);
//    QPoint limitPointInSelectedArea(int x, int y);
    void myDispose();
//    void updateDrawing(QMouseEvent *);
    void setCanvasMode(CanvasMode m);

//    MosaicSize mosaicSize;
//    QPen currentPen;
    QImage screenImage;
    QImage actualCanvas;
    QRect selectedArea;
    CanvasMode _canvasMode;
    CanvasMode canvasModeBackup;
    bool paintingStarted;
//    PaintingMode paintingMode;
    MousePosition mousePosition;
    int mouseDownX;
    int mouseDownY;
    int originalSelectLeft;
    int originalSelectTop;
//    PaintingHistory paintingHistory;
//    QTextEdit *textEdit;

    CanvasMode getCanvasMode();

    Ui::FullScreenImageEditor *ui;

};

#endif // FULLSCREENIMAGEEDITOR_H
