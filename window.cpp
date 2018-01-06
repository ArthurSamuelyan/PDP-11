#include "window.h"
#include "ui_window.h"


Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi( this );

    //this->setStyleSheet( "background-color: #404450;");

    _programContainer = new CodeContainer;

    QVBoxLayout* _editorLayout = new QVBoxLayout;
    _editorLayout->addWidget( _programContainer );



    QVBoxLayout* _registerLayoutsLocal[ 8 ];     // = new QVBoxLayout* [ 8 ];
    QHBoxLayout* _registerLayoutGlobal = new QHBoxLayout;
    for( unsigned i = 0; i < 8; i++ ) {
        _registerNames[ i ] = new QLabel;
        _registerValues[ i ] = new QLabel;
        _registerLayoutsLocal[ i ] = new QVBoxLayout;

        _registerNames[ i ]->setStyleSheet( "background-color: #ffaa44; color: #000000;");
        _registerNames[ i ]->setText(
            QString( "<b><font color=#660000>R</font>%1</b>" ).arg( i, 0, 8 ) );
        _registerNames[ i ]->setAlignment( Qt::Alignment( Qt::AlignHCenter ) );
        _registerValues[ i ]->setText("<b>0000</b>");
        _registerValues[ i ]->setAlignment( Qt::Alignment( Qt::AlignHCenter ) );

        _registerLayoutsLocal[ i ]->addWidget( _registerNames[ i ] );
        _registerLayoutsLocal[ i ]->addWidget( _registerValues[ i ] );
        _registerLayoutGlobal->addLayout( _registerLayoutsLocal[ i ] );
    }

    _emulatorScreen = new ScreenWidget;
    _emulatorScreen->initScreen( 128, 128, 8, 3 );
    _emulatorScreen->testScreen();

    _startEmulation         = new QPushButton;
    _startWithBreakpoints   = new QPushButton;
    _stepEmulation          = new QPushButton;
    _pauseEmulation         = new QPushButton;
    _stopEmulation          = new QPushButton;
    QSpacerItem* _buttonsSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );

    QHBoxLayout* _buttonsLayout = new QHBoxLayout;
    _buttonsLayout->addWidget( _startEmulation );
    _buttonsLayout->addWidget( _startWithBreakpoints );
    _buttonsLayout->addWidget( _stepEmulation );
    _buttonsLayout->addWidget( _pauseEmulation );
    _buttonsLayout->addWidget( _stopEmulation );
    _buttonsLayout->addSpacerItem( _buttonsSpacer );

    /////////////////////

    QSpacerItem* _emulatorSpacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );

    QVBoxLayout* _emulatorLayout = new QVBoxLayout;
    _emulatorLayout->addLayout( _registerLayoutGlobal );
    _emulatorLayout->addWidget( _emulatorScreen );
    _emulatorLayout->addLayout( _buttonsLayout );
    _emulatorLayout->addSpacerItem( _emulatorSpacer );


    QHBoxLayout* _globalLayout = new QHBoxLayout;
    QSpacerItem* _globalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );

    _globalLayout->addLayout( _editorLayout );
    _globalLayout->addSpacerItem( _globalSpacer );
    _globalLayout->addLayout( _emulatorLayout );

    QWidget* cw = new QWidget( this );
    cw->setLayout( _globalLayout );
    setCentralWidget( cw );

    _register_radix = 8;
}

//QScrollArea


Window::~Window()
{
    delete ui;
}

void Window::setRegisters( uint16_t* registerValues )
{
    for( unsigned i = 0; i < 8; i++ ) {
        _registerValues[ i ]->setText( QString("<b>%1</b>").arg( registerValues[ i ], 4, _register_radix, QLatin1Char( '0' ) ) );
    }
}

void Window::setVideoMemory(uint16_t* videoMemory ) {
    _emulatorScreen->updateSource( videoMemory );
}
