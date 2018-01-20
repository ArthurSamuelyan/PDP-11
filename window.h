#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>

#include "screenwidget.h"
#include "codeeditor.h"

#include <cstdlib>
#include <cstdint>


namespace Ui {
class Window;
}

// use them instead of hardcode!
#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 128
#define SCREEN_SCALE 3

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    void setRegisters( uint16_t* registerValues ); // User is not supposed to change values in registers. (?)
    void setVideoMemory( uint16_t* videoMemory ); // Add shift later.
private slots:
    void newFile();
    void openFile();
private:
    void setCurrentFileName( const QString& fileName ); //
    void loadFile( const QString& fileName );
    void createFileMenu(); // <-- test it!
    QString currentFileName;
    //void createStatusBar();

    //Ui::Window *ui;

    //QScrollArea* _programContainer;
    //QTextEdit* _programText;
    CodeEditor* codeEditor;

    QLabel* _registerNames[ 8 ];
    QLabel* _registerValues[ 8 ];

    ScreenWidget* _emulatorScreen;

    QPushButton* _startEmulation;
    QPushButton* _startWithBreakpoints;
    QPushButton* _stepEmulation;
    QPushButton* _pauseEmulation;
    QPushButton* _stopEmulation;

    int _register_radix;
};

#endif // WINDOW_H
