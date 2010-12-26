#include "icongrid.h"
#include "palette.h"
#include "undocmd.h"

#include <stdlib.h>

#include <QDebug>

#include <QAction>
#include <QColormap>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QtCore/qmath.h>

IconGrid::IconGrid( Palette* palette, QWidget* parent )
    : QWidget( parent ), m_palette( palette )
{
    m_drawCommand = NULL;

    m_cols = 60;
    m_rows = 40;
    m_unitPixels = 10;
    m_tooltype = KIconEdit::None;

    /// FIXME:generate random colors
    for ( int i = 0; i < m_rows; ++i ) {
        for ( int j = 0; j < m_cols; ++j ) {
//             srand( (i+1) * 100 + j+1 );
//             int r = rand() % 255;
//             srand( (i+1) * 10 + j+1 );
//             int g = rand() % 255;
//             srand( (i+1) * 200 + j+1 );
//             int b = rand() % 255;
            m_colorArray.append( QColor(0,0,0,0)/*QColor( r, g, b )*/ );
        }
    }

    setMouseTracking( true );
    resize( m_cols*m_unitPixels, m_rows*m_unitPixels );

}

IconGrid::~IconGrid()
{
}

QImage IconGrid::toImage() const
{
    QImage image( m_cols, m_rows, QImage::Format_ARGB32 );
    for ( int i = 0; i < m_rows; ++i ) {
        for ( int j = 0; j < m_cols; ++j ) {
            QColor c = m_colorArray.at( i * m_cols + j );
            image.setPixel( j, i, c.rgba() );
        }
    }
    return image;
}

QColor IconGrid::currentColor() const
{
    return cellColorAt( m_currentRow, m_currentCol );
}

int IconGrid::currentCol() const
{
    return m_currentCol;
}

int IconGrid::currentRow() const
{
    return m_currentRow;
}

QColor IconGrid::cellColorAt( int col, int row ) const
{
    return m_colorArray.value( row * m_cols + col );
}

void IconGrid::setCellColor( int col, int row, const QColor& color )
{
    m_colorArray[ row * m_cols + col ] = color;
    update( col * m_unitPixels, row * m_unitPixels, m_unitPixels, m_unitPixels );
}

void IconGrid::setCellColor( const QPoint& pos, const QColor& color )
{
    setCellColor( pos.x(), pos.y(), color );
}

void IconGrid::clear()
{
    m_colorArray.fill( QColor( 0, 0, 0, 0 ) );
    update();
}

void IconGrid::undo()
{
    m_undoStack.undo();
}

void IconGrid::redo()
{
    m_undoStack.redo();
}

void IconGrid::slotUnitPixelsChanged( int newUnitPixels )
{
    m_unitPixels = newUnitPixels;
    resize( m_cols * m_unitPixels, m_rows * m_unitPixels );
}

void IconGrid::slotToolChanged( QAction* action )
{
    m_tooltype = action->data().value<KIconEdit::ToolType>();
}

void IconGrid::mouseMoveEvent( QMouseEvent* event )
{
    static int oldCol = 0;
    static int oldRow = 0;
    QWidget::mouseMoveEvent( event );

    if ( rect().contains( event->pos() ) ) {
        int x = event->x();
        int y = event->y();
        int oldCol_r = oldCol;
        int oldRow_r = oldRow;
        int newCol = x / m_unitPixels;
        int newRow = y / m_unitPixels;

        bool drawing = false;
        if ( newCol != oldCol ) {
            oldCol = newCol;
            emit cursorColChanged( newCol );
            emit cursorXChanged( newCol*m_unitPixels );
            drawing = true;
        }
        if ( newRow != oldRow ) {
            oldRow = newRow;
            emit cursorRowChanged( newRow );
            emit cursorYChanged( newRow*m_unitPixels );
            drawing = true;
        }
        /// moving within a single grid, no need to draw
        if ( !drawing )
            return;

        switch ( m_tooltype ) {
            case KIconEdit::Freehand:
                if ( event->buttons() == Qt::LeftButton ) {
                    if ( m_colorArray[ newRow * m_cols + newCol ] != m_palette->foregroundColor() ) {
                        /// FIXME:change the color
                        m_undoStack.push( new DrawLine( this, oldCol_r, oldRow_r, newCol, newRow,
                                                        m_palette->foregroundColor() ) );
                    }
                }
                break;
            case KIconEdit::Line: {
                if ( event->buttons() == Qt::LeftButton ) {
                    /// draw line in realtime
                    if ( m_drawCommand ) {
                        m_drawCommand->undo();
                        delete m_drawCommand;
                    }
                    m_drawCommand = new DrawLine( this, m_currentCol, m_currentRow, newCol, newRow,
                                                  m_palette->foregroundColor() );
                    m_drawCommand->redo();
                }
                break;
            }
            default:
                break;
        }
    }
}

void IconGrid::mousePressEvent( QMouseEvent* event )
{
    QWidget::mousePressEvent( event );

    if ( rect().contains( event->pos() ) ) {
        if ( event->buttons() == Qt::LeftButton ) {
            int x = event->x();
            int y = event->y();
            m_currentCol = x / m_unitPixels;
            m_currentRow = y / m_unitPixels;
            switch ( m_tooltype ) {
                case KIconEdit::Freehand:
                    /// FIXME:change the color
//                     qDebug() << "BEGIN";
                    m_undoStack.beginMacro( "freehand" );
                    m_undoStack.push( new DrawCell( this, m_currentCol, m_currentRow,
                                                    m_palette->foregroundColor() ) );
                    break;
                case KIconEdit::Line: {
//                     qWarning() << "BEGIN";
                    m_undoStack.beginMacro( "line" );
                    m_undoStack.push( new DrawCell( this, m_currentCol, m_currentRow,
                                                    m_palette->foregroundColor() ) );
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void IconGrid::mouseReleaseEvent( QMouseEvent* event )
{
    QWidget::mouseReleaseEvent( event );

//     if ( rect().contains( event->pos() ) ) {
    if ( event->buttons() == Qt::LeftButton ) {
//             int x = event->x();
//             int y = event->y();
//             m_currentCol = x / m_unitPixels;
//             m_currentRow = y / m_unitPixels;
//            /// FIXME:
//
        switch ( m_tooltype ) {
            case KIconEdit::Freehand:
    //             qDebug() << "END";
                m_undoStack.endMacro();
                break;
            case KIconEdit::Line:
    //             qWarning() << "END";
                m_undoStack.push( m_drawCommand );
                m_drawCommand = NULL;
                m_undoStack.endMacro();
                break;
            default:
                break;
        }
    }
//     }
}

void IconGrid::paintEvent( QPaintEvent* event )
{
    QWidget::paintEvent( event );
    const QRect urect = event->rect();

    int leftCol = urect.left() / m_unitPixels;
    int rightCol = urect.right() / m_unitPixels;
    int topRow = urect.top() / m_unitPixels;
    int bottomRow = urect.bottom() / m_unitPixels;
//     qWarning() << leftCol << rightCol << topRow << bottomRow;

    QPainter painter( this );

    int index = topRow * m_cols + leftCol;
    for ( int i = topRow; i <= bottomRow; ++i ) {
        for ( int j = leftCol; j <= rightCol; ++j ) {
            int xStart = m_unitPixels * j;
            int yStart = m_unitPixels * i;
            /// draw transparent indicator background
            painter.fillRect( xStart + m_unitPixels / 2, yStart,
                              m_unitPixels / 2, m_unitPixels / 2,
                              Qt::lightGray );
            painter.fillRect( xStart, yStart + m_unitPixels / 2,
                              m_unitPixels / 2, m_unitPixels / 2,
                              Qt::lightGray );
            /// draw icon pixels if not fully transparent
            if ( m_colorArray.at( index ).alpha() != 0 ) {
                painter.fillRect( xStart, yStart,
                                  m_unitPixels, m_unitPixels,
                                  m_colorArray.at( index ) );
            }
            index++;
        }
        index += m_cols - rightCol + leftCol - 1;
    }

    /// draw grid
    for ( int i = topRow; i <= bottomRow; ++i ) {
        painter.drawLine( 0, m_unitPixels*i, m_unitPixels*m_cols, m_unitPixels*i );
    }
    for ( int i = leftCol; i <= rightCol; ++i ) {
        painter.drawLine( m_unitPixels*i, 0, m_unitPixels*i, m_unitPixels*m_rows );
    }
}
