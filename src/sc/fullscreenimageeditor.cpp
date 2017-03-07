#include "fullscreenimageeditor.h"
#include "ui_fullscreenimageeditor.h"
#include "commondef.h"
#include "configmanager.h"
#include <QtMath>
#include <QPainter>
#include <QClipboard>
#include <QTime>
#include <QBitmap>

QString g_ImagePath;
QString g_ImageDir;
double g_dpr;

FullScreenImageEditor::FullScreenImageEditor(QPixmap &image, QString imgDir, double dpr, QWidget *parent) :
    QMainWindow(parent),
  ui(new Ui::FullScreenImageEditor)
{
    g_ImageDir = imgDir;
    g_dpr = dpr;

    ui->setupUi(this);
    selectedArea = QRect(-10000,-10000,0,0);
    screenImage = image.toImage();
    actualCanvas = screenImage;
    setCanvasMode(CanvasMode::IDLE);
    paintingStarted = false;
}

FullScreenImageEditor::~FullScreenImageEditor()
{

}

void FullScreenImageEditor::paintEvent(QPaintEvent *e)
{
    QPainter painter(&actualCanvas);
    painter.setBrush(QBrush(ConfigManager::instance()->maskColor()));
    painter.setPen(ConfigManager::instance()->maskColor());

    painter.drawImage(0, 0, screenImage);
    // paintingHistory should be drawn before the rectangle and the masks.
    painter.setBrush(QBrush());
//    paintingHistory.paint(&painter);

    painter.fillRect(0, 0, selectedArea.left(), screenImage.height(), ConfigManager::instance()->maskColor());
    painter.fillRect(selectedArea.right() + 1, 0, screenImage.width() - selectedArea.right(), screenImage.height(), ConfigManager::instance()->maskColor());
    painter.fillRect(selectedArea.left(), 0, selectedArea.width(), selectedArea.top(), ConfigManager::instance()->maskColor());
    painter.fillRect(selectedArea.left(), selectedArea.bottom() + 1, selectedArea.width(), screenImage.height()-selectedArea.bottom(), ConfigManager::instance()->maskColor());


    painter.setPen(ConfigManager::instance()->selectedBoarderColor());


    // I found that drawRect funciton thinks the WIDTH is rect.right - rect.height, which means
    // this function will draw a rectangle whose width is N with actually N+1 pixels.
    // So is height, thus here we make width and height -1
    painter.drawRect(selectedArea.left(), selectedArea.top(), selectedArea.width() - 1, selectedArea.height() - 1);


    // hide comand buttons if necessay. This is a wourk around. SetVisible may cause mouseMoveEvent not working.
//    if (getCanvasMode() == CanvasMode::MOVING || getCanvasMode() == CanvasMode::RESIZING
//             || getCanvasMode() == CanvasMode::IDLE || getCanvasMode() == CanvasMode::SELECTING) {
//        QRect geo(ui->cmdPanel->geometry());
//        geo.moveTo(-100000,-100000);
//        ui->cmdPanel->setGeometry(geo);
//    } else {
//        //moveCmdPanelToProperPosition();
//    }

    QPainter p(this);
    p.drawImage(0, 0, actualCanvas);
    QMainWindow::paintEvent(e);
}


void FullScreenImageEditor::mousePressEvent(QMouseEvent *e)
{
    mouseDownX = e->x();
    mouseDownY = e->y();
    if (e->button() == Qt::LeftButton) {
        if (getCanvasMode() == CanvasMode::IDLE) {
            selectedArea.setTop(e->y());
            selectedArea.setLeft(e->x());
            selectedArea.setBottom(e->y());
            selectedArea.setRight(e->x());
            setCanvasMode(CanvasMode::SELECTING);
            this->update();
        } else if (getCanvasMode() == CanvasMode::SELECTED) {
            if (mousePosition == MousePosition::Other) {
                if (isInSelectedArea(e->x(), e->y())) {
                    originalSelectLeft = selectedArea.left();
                    originalSelectTop = selectedArea.top();
                    setCanvasMode(CanvasMode::MOVING);
                } else {
                    canvasModeBackup = getCanvasMode();
                    setCanvasMode(CanvasMode::INVALID);
                }
            } else {
                // On selected area's boarder
                setCanvasMode(CanvasMode::RESIZING);
            }
        } else if (getCanvasMode() == CanvasMode::PAINTING) {
//            if (isInSelectedArea(e->x(), e->y())) {
//                paintingStarted = true;

//            } else {
//                canvasModeBackup = getCanvasMode();
//                setCanvasMode(CanvasMode::INVALID);
//            }
        }
    } else if (e->button() == Qt::RightButton) {
        if (getCanvasMode() == CanvasMode::IDLE) {
            myDispose();
            return;
        } else if (getCanvasMode() == CanvasMode::SELECTED) {
            setCanvasMode(CanvasMode::IDLE);
            this->update();
        } else
        {

        }
    }
}

void FullScreenImageEditor::mouseMoveEvent(QMouseEvent *e)
{
    if (getCanvasMode() == CanvasMode::SELECTING) {
        selectArea(e->x(), e->y());
        this->update();
    } else if (getCanvasMode() == CanvasMode::MOVING) {
        moveSelectedAreaLimitInScreen(e->x(), e->y());
        this->update();
    } else if (getCanvasMode() == CanvasMode::SELECTED) {
        changeCursorIfNessesary(e->x(), e->y());
    } else if (getCanvasMode() == CanvasMode::RESIZING) {
        resizingSelectedArea(e->x(), e->y());
        this->update();
    } else if (getCanvasMode() == CanvasMode::PAINTING && paintingStarted) {
        //updateDrawing(e);
        //this->update();
    }

}

#if 0
void FullScreenImageEditor::updateDrawing(QMouseEvent *e)
{
    switch (paintingMode) {
    case PaintingMode::PEN:
        dynamic_cast<PenPaintingStep *>((paintingHistory.peekStep()))->addPoint(limitPointInSelectedArea(e->x(), e->y()));
        break;
    case PaintingMode::OVAL:
        dynamic_cast<OvalPaintingStep *>((paintingHistory.peekStep()))->setSecondPoint(limitPointInSelectedArea(e->x(), e->y()));
        break;
    case PaintingMode::RECT:
        dynamic_cast<RectPaintingStep *>((paintingHistory.peekStep()))->setSecondPoint(limitPointInSelectedArea(e->x(), e->y()));
        break;
    case PaintingMode::TEXT:
//        dynamic_cast<TextPaintingStep *>((paintingHistory.peekStep()))->setSecondPoint(limitPointInSelectedArea(e->x(), e->y()));
        break;
    case PaintingMode::ARROW:
        dynamic_cast<ArrowPaintingStep *>((paintingHistory.peekStep()))->setSecondPoint(limitPointInSelectedArea(e->x(), e->y()));
        break;
    case PaintingMode::MOSAIC:
        dynamic_cast<MosaicPaintingStep *>((paintingHistory.peekStep()))->setSecondPoint(limitPointInSelectedArea(e->x(), e->y()));
        break;
    }
}


QPoint FullScreenImageEditor::limitPointInSelectedArea(int x, int y)
{
    if (x < selectedArea.left()) {
        x = selectedArea.left();
    } else if (x > selectedArea.right()) {
        x = selectedArea.right();
    }

    if (y < selectedArea.top()) {
        y = selectedArea.top();
    } else if (y > selectedArea.bottom()) {
        y = selectedArea.bottom();
    }
    return QPoint(x, y);
}
#endif
void FullScreenImageEditor::resizingSelectedArea(int mouseX, int mouseY) {
    switch (mousePosition) {
    case MousePosition::NorthWest:
        selectedArea.setTopLeft(
                    QPoint(mouseX < selectedArea.right() ? mouseX : selectedArea.right(),
                           mouseY < selectedArea.bottom() ? mouseY : selectedArea.bottom()));
        break;
    case MousePosition::NorthEast:
        selectedArea.setTopRight(
                    QPoint(mouseX > selectedArea.left() ? mouseX : selectedArea.left(),
                           mouseY < selectedArea.bottom() ? mouseY : selectedArea.bottom()));
        break;
    case MousePosition::SouthEast:
        selectedArea.setBottomRight(
                    QPoint(mouseX > selectedArea.left() ? mouseX : selectedArea.left(),
                           mouseY > selectedArea.top() ? mouseY : selectedArea.top()));
        break;
    case MousePosition::SouthWest:
        selectedArea.setBottomLeft(
                    QPoint(mouseX < selectedArea.right() ? mouseX : selectedArea.right(),
                           mouseY > selectedArea.top() ? mouseY : selectedArea.top()));
        break;
    case MousePosition::North:
        selectedArea.setTop(mouseY < selectedArea.bottom() ? mouseY : selectedArea.bottom());
        break;
    case MousePosition::East:
        selectedArea.setRight(mouseX > selectedArea.left() ? mouseX : selectedArea.left());
        break;
    case MousePosition::South:
        selectedArea.setBottom(mouseY > selectedArea.top() ? mouseY : selectedArea.top());
        break;
    case MousePosition::West:
        selectedArea.setLeft(mouseX < selectedArea.right() ? mouseX : selectedArea.right());
        break;
    default:
        QDBG << "Imposible, mouse is not on the boarder of Selected Area.";
    }
}

void FullScreenImageEditor::changeCursorIfNessesary(int mouseX, int mouseY)
{
    if (qAbs(mouseX-selectedArea.left()) < 2) {
        if (qAbs(mouseY - selectedArea.top()) < 2) {
            setCursor(Qt::SizeFDiagCursor);
            mousePosition = MousePosition::NorthWest;
        } else if (qAbs(mouseY - selectedArea.bottom()) < 2) {
            setCursor(Qt::SizeBDiagCursor);
            mousePosition = MousePosition::SouthWest;
        } else if (mouseY > selectedArea.top() && mouseY < selectedArea.bottom()) {
            setCursor(Qt::SizeHorCursor);
            mousePosition = MousePosition::West;
        } else {
            setCursor(Qt::ArrowCursor);
            mousePosition = MousePosition::Other;
        }
    } else if (qAbs(mouseX-selectedArea.right()) < 2) {
        if (qAbs(mouseY - selectedArea.top()) < 2) {
            setCursor(Qt::SizeBDiagCursor);
            mousePosition = MousePosition::NorthEast;
        } else if (qAbs(mouseY - selectedArea.bottom()) < 2) {
            setCursor(Qt::SizeFDiagCursor);
            mousePosition = MousePosition::SouthEast;
        } else if (mouseY > selectedArea.top() && mouseY < selectedArea.bottom()) {
            setCursor(Qt::SizeHorCursor);
            mousePosition = MousePosition::East;
        } else {
            setCursor(Qt::ArrowCursor);
            mousePosition = MousePosition::Other;
        }
    } else if (mouseX > selectedArea.left() && mouseX < selectedArea.right()) {
        if (qAbs(mouseY - selectedArea.top()) < 2) {
            setCursor(Qt::SizeVerCursor);
            mousePosition = MousePosition::North;
        } else if (qAbs(mouseY - selectedArea.bottom()) < 2) {
            setCursor(Qt::SizeVerCursor);
            mousePosition = MousePosition::South;
        } else if (mouseY > selectedArea.top() && mouseY < selectedArea.bottom()) {
            setCursor(Qt::ArrowCursor);
            mousePosition = MousePosition::Other;
        } else {
            setCursor(Qt::ArrowCursor);
            mousePosition = MousePosition::Other;
        }
    } else {
        setCursor(Qt::ArrowCursor);
        mousePosition = MousePosition::Other;
    }
}

void FullScreenImageEditor::moveSelectedAreaLimitInScreen(int mouseX, int mouseY)
{
    int offX = mouseX - mouseDownX;
    int offY = mouseY - mouseDownY;
    int toX = offX + originalSelectLeft;
    int toY = offY + originalSelectTop;
    if (toX < 0)
        toX = 0;
    else if (toX + selectedArea.width() > screenImage.width())
        toX = screenImage.width() - selectedArea.width() - 1;
    if (toY < 0)
        toY = 0;
    else if (toY + selectedArea.height() > screenImage.height())
        toY = screenImage.height() - selectedArea.height() - 1;
    selectedArea.moveTo(toX, toY);
}

void FullScreenImageEditor::selectArea(int mouseX, int mouseY)
{
    int x1, x2, y1, y2;
    if (mouseX > mouseDownX) {
        x1 = mouseDownX;
        x2 = mouseX;
    } else {
        x2 = mouseDownX;
        x1 = mouseX;
    }
    if (mouseY > mouseDownY) {
        y1 = mouseDownY;
        y2 = mouseY;
    } else {
        y2 = mouseDownY;
        y1 = mouseY;
    }

    selectedArea.setTop(y1);
    selectedArea.setLeft(x1);
    selectedArea.setBottom(y2);
    selectedArea.setRight(x2);
}


void FullScreenImageEditor::mouseReleaseEvent(QMouseEvent *e)
{
    if (getCanvasMode() == CanvasMode::INVALID) {
        setCanvasMode(canvasModeBackup);
        return;
    }
    if (e->button() == Qt::LeftButton) {
        if (getCanvasMode() == CanvasMode::SELECTING) {
            selectArea(e->x(), e->y());
            setCanvasMode(CanvasMode::SELECTED);
            this->update();
        } else if (getCanvasMode() == CanvasMode::MOVING) {
            moveSelectedAreaLimitInScreen(e->x(), e->y());
            setCanvasMode(CanvasMode::SELECTED);
            this->update();
        } else if (getCanvasMode() == CanvasMode::RESIZING) {
            resizingSelectedArea(e->x(), e->y());
            setCanvasMode(CanvasMode::SELECTED);
            this->update();
        } else if (getCanvasMode() == CanvasMode::PAINTING && paintingStarted) {
            // && paintingStarted is used to avoid some situations that trigger mouseRelease without mousePress
//            updateDrawing(e);
//            if (paintingMode == PaintingMode::MOSAIC) {
//                dynamic_cast<MosaicPaintingStep *>(paintingHistory.peekStep())->endSelect();
//            }
//            paintingStarted = false;

//            this->update();
        }
    }
}

void FullScreenImageEditor::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        QImage img;
        QDBG<<g_dpr;
//        QDBG<<selectedArea.width()<<", "<<selectedArea.height();
        if( (selectedArea.width() == 1) && (selectedArea.height() == 1) )
        {
            //exit if user not select rect
            myDispose();
            return;
        }
        if(g_dpr == 2.0)
        {
            QRect newRect = QRect(selectedArea.left()*2, selectedArea.top()*2,
                                  selectedArea.width()*2, selectedArea.height()*2);
            img = screenImage.copy(newRect);
        }
        else
        {
            img = screenImage.copy(selectedArea);
        }

        QApplication::clipboard()->clear();

        QApplication::clipboard()->setImage(img);
        QString defaultName = "Screenshot" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png";
        g_ImagePath = g_ImageDir.append(defaultName);
        img.save(g_ImagePath, "png");
        myDispose();
    }
}

void FullScreenImageEditor::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        myDispose();
    }
}

bool FullScreenImageEditor::isInSelectedArea(int x, int y)
{
    return x > selectedArea.left() && x < selectedArea.right() && y > selectedArea.top() && y < selectedArea.bottom();
}

void FullScreenImageEditor::myDispose()
{
    this->close();
}

void FullScreenImageEditor::setCanvasMode(CanvasMode m)
{
    switch (m) {
    case CanvasMode::IDLE:
        selectedArea.setTop(-10000);
        selectedArea.setLeft(0-10000);
        selectedArea.setBottom(0);
        selectedArea.setRight(0);
//        paintingHistory.clear();
        break;
    case CanvasMode::PAINTING:
        paintingStarted = false;
        break;
    case CanvasMode::INVALID:
    case CanvasMode::SELECTING:
    case CanvasMode::SELECTED:
    case CanvasMode::RESIZING:
    case CanvasMode::MOVING:
        break;
    }
    _canvasMode = m;
//    ui->cmdPanel->setVisible(ui->cmdPanel->isVisible() == false);
}

CanvasMode FullScreenImageEditor::getCanvasMode()
{
    return _canvasMode;
}
