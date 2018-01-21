#include <QtGui>
#include <QRegExp>
//#include <QLineEdit>

#include "codeeditor.h"

CodeEditor::CodeEditor( QWidget* parent ) : QPlainTextEdit( parent ) {
    //QRegExp codeValidatorSyntax( "([0-7]{8}\n+){0, 1024}" );
    //codeValidator = new QRegExpValidator( codeValidatorSyntax, this );
    // Think of how to use.
    // Maybe in obtaining data from file
    // text -> regExp : buffer
    // buffer -> uint16_t* data
    // buffer -> via SetText -> text
    // Weird
    _lineArea = new LineArea(this);

    connect( this, SIGNAL( blockCountChanged(int) ), this, SLOT( applyBlockCount(int) ) );
    connect( this, SIGNAL( updateRequest(QRect,int) ), this, SLOT( updateLineArea(QRect,int) ) );
    connect( this, SIGNAL( cursorPositionChanged() ), this, SLOT( highlightCurrentLine() ) );

    _breakPoints.clear();
    applyBlockCount( 0 );
    highlightCurrentLine();
    setReadOnly( true ); // Uncomment this on release
}

uint CodeEditor::lineAreaWidth()
{
    //int digits;
    //int max = qMax( 1, blockCount() );
    //for( digits = 1; max >= 10; digits++ )  max /= 10;
    uint lineWidth = 3 + fontMetrics().width( QLatin1Char( '9' ) ) * ( 6 + 3 ); // +3 for breakpoint
    return lineWidth;
}

void CodeEditor::applyBlockCount( int ) {
    setViewportMargins( lineAreaWidth(), 0, 0, 0 );
    if( blockCount() > _breakPoints.size() ) {
        _breakPoints.append( QVector< bool >( blockCount() - _breakPoints.size(), false ) );
    } else if( blockCount() < _breakPoints.size() ) {
        _breakPoints.remove( blockCount(), _breakPoints.size() - blockCount() );
    }
}

void CodeEditor::updateLineArea( const QRect &rect, int dY ) {
    ( dY ) ? _lineArea->scroll( 0, dY ) : _lineArea->update(0, rect.y(), _lineArea->width(), rect.height());
    if( rect.contains( viewport()->rect() ) )
        applyBlockCount( 0 );
}

void CodeEditor::resizeEvent( QResizeEvent* event ) {
    QPlainTextEdit::resizeEvent( event );
    QRect _contentsRect = contentsRect();
    _lineArea->setGeometry( QRect( _contentsRect.left(), _contentsRect.top(), lineAreaWidth(), _contentsRect.height() ) );
}

void CodeEditor::highlightCurrentLine() { // add color as an arguement
    QList<QTextEdit::ExtraSelection> extraSelections;
    //if( !isReadOnly() ) {
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor( Qt::yellow ).lighter(160); // Change color later

    selection.format.setBackground( lineColor );
    selection.format.setProperty( QTextFormat::FullWidthSelection, true );
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append( selection );
    //}
    setExtraSelections(extraSelections);
}

void CodeEditor::lineAreaPaintEvent( QPaintEvent *event ) { // Alter it!
    QPainter painter( _lineArea );
    painter.fillRect( event->rect(), Qt::lightGray ); // Change color later!

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry( block ).translated( contentOffset() ).top();
    int bottom = top + (int)blockBoundingRect( block ).height();

    while ( block.isValid() && top <= event->rect().bottom() ) {
        if ( block.isVisible() && bottom >= event->rect().top() ) {
            QString number = QString( "%1 " ).arg( (blockNumber + currSectionStart) * 2, 6, 8, QLatin1Char( '0' ) );
            //QString number = QString( "* %1" ).arg( blockNumber + 1, 0, 10 );
            painter.setPen( Qt::black ); // change color?
            painter.drawText( 0, top, _lineArea->width(), fontMetrics().height(), Qt::AlignRight, number );
            if( _breakPoints[ blockNumber ] ) {
                painter.setPen( QPen( QBrush( QColor( Qt::red ).darker( 160 ) ), 3.0 ) );
                painter.setBrush( QBrush( QColor( Qt::red ).darker( 140 ) ) );
                //painter.drawText( 0, top, _lineArea->width(), fontMetrics().height(), Qt::AlignLeft, QString( " *" ) );
                painter.drawEllipse( QPoint( 7, top + 8 ), 4, 4 );

            }
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        blockNumber++;
    }
}

void CodeEditor::lineAreaClicked( QMouseEvent* event ) {
    if( isROM ) {
    //if( event->pos().x() < ( 3 + fontMetrics().width( QLatin1Char( '9' ) ) * 2 ) ) {
        QTextBlock currBlock = firstVisibleBlock();
        int top = (int)blockBoundingGeometry( currBlock ).translated( contentOffset() ).top();
        int bottom = top + (int)blockBoundingRect( currBlock ).height();
        int breakPointIndex = currBlock.blockNumber();
        //int blockHeight = (int)blockBoundingRect( firstBlock ).height();
        while( currBlock.isValid() && bottom < (int)event->pos().y() ) {
            currBlock = currBlock.next();
            top = bottom;
            bottom += (int)blockBoundingRect( currBlock ).height();
            breakPointIndex++;
        }
        //int breakPointIndex = currBlock.blockNumber() + ( (int)event->pos().y() - top ) / blockHeight;
        if( breakPointIndex < _breakPoints.size() ) {
            _breakPoints[ breakPointIndex ] ?
                _breakPoints[ breakPointIndex ] = false :
                _breakPoints[ breakPointIndex ] = true;
            _lineArea->repaint();
        }
    //}
    }
}

void CodeEditor::setEmulatorMemory( uint16_t* code ) {
    _emulatorMemory = code;
}
void CodeEditor::displayEmulatorMemory( uint16_t address ) {
    if( address >= RAM_START * 2 ) {
        currSectionStart = RAM_START;
        currSectionSize = RAM_SIZE;
        isROM = false;
    }
    if( address >= VRAM_START * 2 ) {
        currSectionStart = VRAM_START;
        currSectionSize = VRAM_SIZE;
        isROM = false;
    }
    if( address >= ROM_START * 2 ) {
        currSectionStart = ROM_START;
        currSectionSize = ROM_SIZE;
        isROM = true;
    }
    if( address >= IO_START * 2 ) {
        currSectionStart = IO_START;
        currSectionSize = IO_SIZE;
        isROM = false;
    }
    /// finish it!
    QString text = QString();
    for( uint16_t i = currSectionStart; i < currSectionStart + currSectionSize; i++) {

        text.append( QString( " %1\n" ).arg( _emulatorMemory[ i ], 6, 8, QLatin1Char( '0' ) )  );
    }
    setPlainText( text );

    QTextCursor cursor = textCursor();
    cursor.setPosition( address / 2 - currSectionStart );
    ///
}
