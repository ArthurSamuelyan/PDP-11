#ifndef CODECONTAINER_H
#define CODECONTAINER_H

#include <QPlainTextEdit>
#include <QObject>
#include <QWidget>
#include <QRegExpValidator>

class LineArea;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeEditor( QWidget* parent = 0 );
    // methods for line area
    void lineAreaPaintEvent( QPaintEvent* event );
    uint lineAreaWidth();
    void lineAreaClicked( QMouseEvent* event );
    // methods for everybody else
    const QVector< bool >& breakpoints() { return _breakPoints; }

    void setCode( uint16_t* code );
    uint16_t* code();
protected:
    void resizeEvent( QResizeEvent* event );
private slots:
    void applyBlockCount( int newBlockCount );
    void highlightCurrentLine();
    void updateLineArea( const QRect&, int );
private:
    QWidget* _lineArea;
    QVector< bool > _breakPoints; // linewise breakpoints and wordwise breakpoints may not match
                                    // asm is an extra feature, not nesessary
    QRegExpValidator* codeValidator;

    // QWidget interface
};

class LineArea : public QWidget {
public:
    LineArea( CodeEditor* container ) : QWidget( container ) {
        _container = container;
    }
    QSize sizeHint() {
        return QSize( _container->lineAreaWidth(), 0 );
    }
protected:
    void paintEvent( QPaintEvent* event ) {
        _container->lineAreaPaintEvent( event );
    }
    void mousePressEvent(QMouseEvent *event) {
        _container->lineAreaClicked( event );
    }
private:
    CodeEditor* _container;
};

#endif // CODECONTAINER_H
