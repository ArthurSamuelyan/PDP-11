#include "window.h"
#include "ui_window.h"

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>



// Slots:

// TODO get rid of this function. We don't need editing only ability to open file!
//void Window::newFile() {
//    // if wannaSave()
//    codeEditor->clear();
//    setCurrentFileName( QString() );
//}

void Window::openFile() {
    // if wannaSave()
    QString fileName = QFileDialog::getOpenFileName( this );
    if( !fileName.isEmpty() )
        loadFile( fileName );
}

// Init functions:

void Window::setCurrentFileName( const QString& fileName ) {
    currentFileName = fileName;
    codeEditor->document()->setModified( false ); //
    setWindowModified( false );

    QString shownName = currentFileName;
    if( currentFileName.isEmpty() )
        shownName = "untitled.txt"; // "no file"
    setWindowFilePath( shownName );
}

void Window::loadFile( const QString& fileName ) {
    QFile file( fileName );
    //if( !file.open( QFile::ReadOnly | QFile::Text ) ) { // later set QFile::Binary !!! + convertation into text.
    if( !file.open( QFile::ReadOnly ) ) {
        QMessageBox::warning(
            this,
            tr( "Application" ),
            tr( "Cannot read file %1:\n%2." ).arg( QDir::toNativeSeparators( fileName ), file.errorString() )
        );
        return;
    }


    //QTextStream in( &file );
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor( Qt::WaitCursor );
#endif
    // fill ROM:
    file.read( (char*)(&emulatorMemory[ ROM_START ]), ROM_SIZE * sizeof( uint16_t) );
    codeEditor->setEmulatorMemory( emulatorMemory );
    codeEditor->displayEmulatorMemory( ROM_START * 2 );
    if( file.size() > ROM_SIZE * 2 ) {
        QMessageBox::warning(
            this,
            tr( "Application" ),
            tr( "File is too big. Loaded partly %1." ).arg( QDir::toNativeSeparators( fileName ) )
        );
    }
    //codeEditor->setPlainText( in.readAll() );
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFileName( fileName );
    statusBar()->showMessage( tr( "File loaded" ), 2000 );
}

void Window::createFileMenu() {
    QMenu *fileMenu = menuBar()->addMenu( tr( "&File" ) );
    //QToolBar *fileToolBar = addToolBar(tr("File"));

    //const QIcon newIcon = QIcon::fromTheme( "document-new", QIcon( ":/images/new.png" ) );
    //QAction *newAction = new QAction( newIcon, tr( "&New" ), this );
    //newAction->setShortcuts( QKeySequence::New );
    //newAction->setStatusTip( tr( "Create a new file" ) );
    //connect( newAction, &QAction::triggered, this, &Window::newFile );
    //fileMenu->addAction( newAction );
    //fileToolBar->addAction(newAction);

    const QIcon openIcon = QIcon::fromTheme( "document-open", QIcon( ":/images/open.png" ) );
    QAction *openAction = new QAction( openIcon, tr( "&Open..." ), this );
    openAction->setShortcuts( QKeySequence::Open );
    openAction->setStatusTip( tr( "Open an existing file" ) );
    connect( openAction, &QAction::triggered, this, &Window::openFile );
    fileMenu->addAction( openAction );
    //fileToolBar->addAction(openAct);
}

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    codeEditor( new CodeEditor )//,
    //ui(new Ui::Window)
{
    emulatorMemory = new uint16_t[ MEM_SIZE ];
    for( uint16_t i = 0; i < MEM_SIZE; i++ )
        emulatorMemory[ i ] = 0;

    //ui->setupUi( this );

    //this->setStyleSheet( "background-color: #404450;");

    //codeEditor = new CodeEditor;

    createFileMenu();

    searchLine = new QLineEdit;

    QVBoxLayout* _editorLayout = new QVBoxLayout;
    _editorLayout->addWidget( searchLine );
    _editorLayout->addWidget( codeEditor );



    QVBoxLayout* _registerLayoutsLocal[ 8 ];     // = new QVBoxLayout* [ 8 ];
    QHBoxLayout* _registerLayoutGlobal = new QHBoxLayout;
    for( unsigned i = 0; i < 8; i++ ) {
        _registerNames[ i ] = new QLabel;
        _registerValues[ i ] = new QLineEdit;
        _registerLayoutsLocal[ i ] = new QVBoxLayout;

        _registerNames[ i ]->setStyleSheet( "background-color: #ffaa44; color: #000000;");
        _registerNames[ i ]->setText(
            QString( "<b><font color=#660000>R</font>%1</b>" ).arg( i, 0, 8 ) );
        _registerNames[ i ]->setAlignment( Qt::Alignment( Qt::AlignHCenter ) );

        _registerValues[ i ]->setReadOnly( true );
        //_registerValues[ i ]->setBaseSize( 100, 20 );
        _registerValues[ i ]->setMaximumSize( 80, 20 ); // Change it?
        _registerValues[ i ]->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
        _registerValues[ i ]->setText("0000");
        _registerValues[ i ]->setAlignment( Qt::Alignment( Qt::AlignHCenter ) );

        _registerLayoutsLocal[ i ]->addWidget( _registerNames[ i ] );
        _registerLayoutsLocal[ i ]->addWidget( _registerValues[ i ] );
        _registerLayoutGlobal->addLayout( _registerLayoutsLocal[ i ] );
    }
    _register_radix = 8;

    _emulatorScreen = new ScreenWidget;
    _emulatorScreen->initScreen( 256, 256, 2, 2 );
    for( uint i = 0; i < 8; i++ )
        _registerValues[ i ]->setMaximumWidth( _emulatorScreen->width() / 8 - 2 * 5 );
    //_emulatorScreen->initScreen( 128, 128, 8, 3 );
    //_emulatorScreen->testScreen();

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


}

Window::~Window() {}

void Window::setRegisters( uint16_t* registerValues ) {
    for( unsigned i = 0; i < 8; i++ ) {
        _registerValues[ i ]->setText( QString("%1").arg( registerValues[ i ], 4, _register_radix, QLatin1Char( '0' ) ) );
    }
}

void Window::setVideoMemory(uint16_t* videoMemory ) {
    _emulatorScreen->updateSource( videoMemory );
}
