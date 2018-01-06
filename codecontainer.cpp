#include <QtGui>
#include "codecontainer.h"

CodeContainer::CodeContainer( QWidget* parent ) : QPlainTextEdit( parent ) {
    _lineArea = new LineArea(this);

    connect( this, SIGNAL( blockCountChanged(int) ), this, SLOT( applyBlockCount(int) ) );
    connect( this, SIGNAL( updateRequest(QRect,int) ), this, SLOT( updateLineArea(QRect,int) ) );
    connect( this, SIGNAL( cursorPositionChanged() ), this, SLOT( highlightCurrentLine() ) );

    _breakPoints.clear();
    applyBlockCount( 0 );
    highlightCurrentLine();
}

uint CodeContainer::lineAreaWidth()
{
    int digits;
    int max = qMax( 1, blockCount() );
    for( digits = 1; max >= 10; digits++ )  max /= 10;
    uint lineWidth = 3 + fontMetrics().width( QLatin1Char( '9' ) ) * ( digits + 2 ); // +2 for breakpoint
    return lineWidth;
}

void CodeContainer::applyBlockCount( int ) {
    setViewportMargins( lineAreaWidth(), 0, 0, 0 );
    if( blockCount() > _breakPoints.size() ) {
        _breakPoints.append( QVector< bool >( blockCount() - _breakPoints.size(), false ) );
    } else if( blockCount() < _breakPoints.size() ) {
        _breakPoints.remove( blockCount(), _breakPoints.size() - blockCount() );
    }
}

void CodeContainer::updateLineArea( const QRect &rect, int dY ) {
    ( dY ) ? _lineArea->scroll( 0, dY ) : _lineArea->update(0, rect.y(), _lineArea->width(), rect.height());
    if( rect.contains( viewport()->rect() ) )
        applyBlockCount( 0 );
}

void CodeContainer::resizeEvent( QResizeEvent* event ) {
    QPlainTextEdit::resizeEvent( event );
    QRect _contentsRect = contentsRect();
    _lineArea->setGeometry( QRect( _contentsRect.left(), _contentsRect.top(), lineAreaWidth(), _contentsRect.height() ) );
}

void CodeContainer::highlightCurrentLine() { // add color as an arguement
    QList<QTextEdit::ExtraSelection> extraSelections;
    if( !isReadOnly() ) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor( Qt::yellow ).lighter(160); // Change color later

        selection.format.setBackground( lineColor );
        selection.format.setProperty( QTextFormat::FullWidthSelection, true );
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append( selection );
    }
    setExtraSelections(extraSelections);
}

void CodeContainer::lineAreaPaintEvent( QPaintEvent *event ) {
    QPainter painter( _lineArea );
    painter.fillRect( event->rect(), Qt::lightGray ); // Change color later!

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry( block ).translated( contentOffset() ).top();
    int bottom = top + (int)blockBoundingRect( block ).height();

    while ( block.isValid() && top <= event->rect().bottom() ) {
        if ( block.isVisible() && bottom >= event->rect().top() ) {
            QString number = QString::number( blockNumber + 1 );
            //QString number = QString( "* %1" ).arg( blockNumber + 1, 0, 10 );
            painter.setPen( Qt::black ); // change color?
            painter.drawText( 0, top, _lineArea->width(), fontMetrics().height(), Qt::AlignRight, number );
            if( _breakPoints[ blockNumber ] ) {
                painter.setPen( QPen( QBrush( QColor( Qt::red ).darker( 160 ) ), 3.0 ) );
                painter.setBrush( QBrush( QColor( Qt::red ).darker( 140 ) ) );
                //painter.drawText( 0, top, _lineArea->width(), fontMetrics().height(), Qt::AlignLeft, QString( " *" ) );
                painter.drawEllipse( QPoint( 7, ( top + bottom ) / 2 - 2 ), 4, 4 );

            }
        }
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        blockNumber++;
    }
}

void CodeContainer::lineAreaClicked( QMouseEvent* event ) {
    //if( event->pos().x() < ( 3 + fontMetrics().width( QLatin1Char( '9' ) ) * 2 ) ) {
        QTextBlock firstBlock = firstVisibleBlock();
        int top = (int)blockBoundingGeometry( firstBlock ).translated( contentOffset() ).top();
        int blockHeight = (int)blockBoundingRect( firstBlock ).height();
        int breakPointIndex = firstBlock.blockNumber() + ( (int)event->pos().y() - top ) / blockHeight;
        if( breakPointIndex < _breakPoints.size() ) {
            _breakPoints[ breakPointIndex ] ?
                _breakPoints[ breakPointIndex ] = false :
                _breakPoints[ breakPointIndex ] = true;
            _lineArea->repaint();
        }
    //}
}


