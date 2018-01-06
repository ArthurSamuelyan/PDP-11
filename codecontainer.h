#ifndef CODECONTAINER_H
#define CODECONTAINER_H

#include <QPlainTextEdit>
#include <QObject>
#include <QWidget>

class LineArea;

class CodeContainer : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeContainer( QWidget* parent = 0 );

    void lineAreaPaintEvent( QPaintEvent* event );
    uint lineAreaWidth();
    void lineAreaClicked( QMouseEvent* event );
    QVector< bool > breakpoints() { return _breakPoints; }
protected:
    void resizeEvent( QResizeEvent* event );
private slots:
    void applyBlockCount( int newBlockCount );
    void highlightCurrentLine();
    void updateLineArea( const QRect&, int );
private:
    QWidget* _lineArea;
    QVector< bool > _breakPoints;

    // QWidget interface
};

class LineArea : public QWidget {
public:
    LineArea( CodeContainer* container ) : QWidget( container ) {
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
    CodeContainer* _container;
};

#endif // CODECONTAINER_H
