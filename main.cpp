#include "window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window* w = new Window();
    w->show();

    uint16_t reg_vals[ 8 ] = { 0, 0, 10, 12, 0, 0, 0, 0 };
    w->setRegisters( reg_vals );

    //test video memory
    uint16_t video_mem[ 128 * 64 ];
    video_mem[ 16 * 64 + 16 ] = 0x00ff;
    //for( uint16_t i = 16; i < 32; i++)
    //    for( uint16_t j = 0; j < 64; j++) {
    //        video_mem[ i * 64 + j ] = 0xffff;
    //    }
    w->setVideoMemory( video_mem );

    //w->show();

    return app.exec();
}
