#ifndef WINDOW_H
#define WINDOW_H

#include "emulatorsettings.h"

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
//#include <QScrollArea>
#include <QTextEdit>
#include <QLineEdit>

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

    void updateRegisters(); // User is not supposed to change values in registers. (?)
    void updateVideoMemory(); // Add shift later.

    uint16_t emulatorRegisters[ 8 ];
    uint16_t emulatorMemory[ MEM_SIZE ];
private slots:
    //void newFile();
    void openFile();
    void setDisplayAddress();
private:
    void setCurrentFileName( const QString& fileName ); //
    void loadFile( const QString& fileName );
    void createFileMenu(); // <-- test it!
    QString currentFileName;
    //void createStatusBar();

    //Ui::Window *ui;

    //QScrollArea* _programContainer;
    //QTextEdit* _programText;
    QLineEdit* searchLine;
    CodeEditor* codeEditor;

    QLabel* _registerNames[ 8 ];
    //QLabel* _registerValues[ 8 ];
    QLineEdit* _registerValues[ 8 ];

    ScreenWidget* _emulatorScreen;

    QPushButton* _startEmulation;
    QPushButton* _startWithBreakpoints;
    QPushButton* _stepEmulation;
    QPushButton* _pauseEmulation;
    QPushButton* _stopEmulation;

    int _register_radix;

    //uint16_t* emulatorMemory;
};

#endif // WINDOW_H
