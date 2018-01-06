#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <cstdint>

// delete theese later
#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 128
#define BITS_PER_PIXEL 8
#define SCREEN_SCALE 3

class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenWidget(QWidget *parent = nullptr);
    ~ScreenWidget();
    bool initScreen(
        uint32_t width,
        uint32_t height,
        uint32_t bitsPerPixel,
        uint32_t scale
    );
    void updateSource( uint16_t* vals ); // add uint16_t begin
    void testScreen();
private:
    void generateColorTable();
    //void setMemory( uint16_t* vals );

    bool _inited;
    uint32_t _width; // > 0
    uint32_t _height; // > 0
    uint32_t _bitsPerPixel; // 1, 2, 4, 8 or (will not happen) 32
    uint32_t _scale; // > 0

    QColor _backgroundColor;
    QImage _currImage;
    QImage::Format _imageFormat;
    QVector< QRgb > _colorTable;
    //QColor _frameColor;

    uchar* _data;
    uint32_t _rawBytesCount;
    uint32_t _bytesCount;

signals:
    void imageChanged();
public slots:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SCREENWIDGET_H
