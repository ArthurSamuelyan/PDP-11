#include "screenwidget.h"

ScreenWidget::ScreenWidget(QWidget *parent) : QWidget(parent) {
    _inited = false;
    _data = NULL;
    _backgroundColor = QColor( "#000000" );
    connect( this, SIGNAL( imageChanged() ), SLOT( repaint() ) );
}

ScreenWidget::~ScreenWidget() {
    if( _data != NULL )
        free( _data );
}

void ScreenWidget::generateColorTable() {
    _colorTable.clear();

    uint32_t mapping1[ 2 ] = { 0, 255 };
    uint32_t mapping2[ 4 ] = { 0, 85, 170, 255 };
    uint32_t mapping3[ 8 ] = { 0, 37, 74, 111, 148, 185, 222, 255 };

    uint32_t colorTableSize = 1 << _bitsPerPixel;
    for( uint32_t sourceColor = 0; sourceColor < colorTableSize; sourceColor++ ) {
        uint32_t resultColor = 0;

        switch( _bitsPerPixel ) {
        case 1:
            for( uint32_t channel = 0; channel < 3; channel++ )
                resultColor += mapping1[ sourceColor ] << ( channel * 8 );
            break;
        case 2:
            for( uint32_t channel = 0; channel < 3; channel++ )
                resultColor += mapping2[ sourceColor ] << ( channel * 8 );
            break;
        case 4:
            if( sourceColor == 7 || sourceColor == 8 ) {
                for( uint32_t channel = 0; channel < 3; channel++ )
                    resultColor += mapping2[ sourceColor - 6 ] << ( channel * 8 );
                break;
            }
            if( ( sourceColor ) / 8 ) {
                for( uint32_t channel = 0; channel < 3; channel++ )
                    if( sourceColor & ( 4 >> channel ) )  resultColor += mapping2[ 3 ] << ( channel * 8 );
            } else {
                for( uint32_t channel = 0; channel < 3; channel++ )
                    if( sourceColor & ( 4 >> channel ) )  resultColor += mapping2[ 2 ] << ( channel * 8 );
            }
            break;
        case 8:
            uint32_t _sourceColor = sourceColor % 256;
            uint32_t palette[ 3 ] = { 3, 3, 2 };

            for( uint32_t channel = 0; channel < 3; channel++) {
                uint32_t channelVolume = 1 << palette[ channel ];
                uint32_t colour = _sourceColor % channelVolume;
                _sourceColor /= channelVolume;
                if( palette[ channel ] == 3 )
                    resultColor += ( mapping3[ colour ] ) << ( channel * 8 );
                else
                    resultColor += ( mapping2[ colour ] ) << ( channel * 8 );
            }
            break;
        }

        _colorTable.push_back( QRgb( resultColor | 0xff000000 ));
    }
}

bool ScreenWidget::initScreen(
    uint32_t width,
    uint32_t height,
    uint32_t bitsPerPixel,
    uint32_t scale
) {
    if(
        width == 0 ||
        height == 0 ||
        scale == 0 ||
        (
            bitsPerPixel != 1 &&
            bitsPerPixel != 2 &&
            bitsPerPixel != 4 &&
            bitsPerPixel != 8
        )
    ) return false;

    _width = width;
    _height = height;
    _bitsPerPixel = bitsPerPixel;
    _scale = scale;
    setFixedSize( _width * _scale, _height * _scale );

    _rawBytesCount = _width * _height * _bitsPerPixel / 8;
    ( _bitsPerPixel == 2 || _bitsPerPixel == 4 ) ? _bytesCount = _width * _height : _bytesCount = _rawBytesCount;
    if( _data != NULL ) free( _data );
    _data = (uchar*)malloc( _bytesCount );
    for( uint32_t i = 0; i < _bytesCount; i++ )
        _data[ i ] = 0x00;

    generateColorTable();
    ( _bitsPerPixel == 1 ) ? _imageFormat = QImage::Format_Mono : _imageFormat = QImage::Format_Indexed8;
    _currImage = QImage( _data, _width, _height, _imageFormat );
    _currImage.setColorTable( _colorTable );

    _inited = true;
    return true;
}

void ScreenWidget::updateSource( uint16_t *vals ) {
    if( _inited ) {
        if( vals != NULL ) {
            if( _bitsPerPixel == 2 || _bitsPerPixel == 4 ) {
                uint32_t pixelsPerByte = 8 / _bitsPerPixel;
                for( uint32_t i = 0; i < _bytesCount; i++ )
                    ( _data )[ i ] =
                        ( ( (uchar*)(void*) vals )[ i / pixelsPerByte ] >> ( i % pixelsPerByte * _bitsPerPixel) ) &
                        ( ( 1 << _bitsPerPixel ) - 1 );
            } else {
                memcpy( (void*)_data, (void*)vals, _bytesCount ); // add a shift parameter later (+ 2 memcpy() functions )
            }
            _currImage = QImage( _data, _width, _height, _imageFormat );
            _currImage.setColorTable( _colorTable );
        } else {
            _currImage = QImage();
        }
        emit( imageChanged() );

    }
}

void ScreenWidget::paintEvent(QPaintEvent*) {
    QPainter painter( this );
    painter.setRenderHint( QPainter::Antialiasing );
    painter.save();
    if( _currImage.isNull() ) {
        painter.setBrush( _backgroundColor );
        painter.drawRect( rect() );
    } else {
        //QRect _targetRect = rect();
        painter.drawImage( rect(), _currImage, _currImage.rect() );
    }
    painter.restore();
}

void ScreenWidget::testScreen() {
    uint16_t colorsCount = 1 << _bitsPerPixel;
    if( colorsCount == 2 ) {
        uint16_t divider = _width / 8 / 2;
        if( divider == 0 ) divider = 1;
        for( uint16_t i = 0; i < _bytesCount; i++)
            _data[ i ] = i % ( _width / 8 ) / divider % 2 * 0xffff;
    } else {
        if( _width / colorsCount ) {
            for( uint32_t i = 0; i < _bytesCount; i++ )
                _data[ i ] = ( i % _width / ( _width / colorsCount ) ) % colorsCount;
        } else {
            uint32_t linesCount = colorsCount / _width;
            if( linesCount > _height ) linesCount = _height;
            for( uint32_t i = 0; i < _bytesCount; i++ )
                _data[ i ] = ( i % _width + i / ( _bytesCount / linesCount ) * _width ) % colorsCount;
        }
    }
    _currImage = QImage( (uchar*)_data, _width, _height, _imageFormat );
    _currImage.setColorTable( _colorTable );
    emit( imageChanged() );
}
