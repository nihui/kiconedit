#ifndef UNDOCMD_H
#define UNDOCMD_H

#include <QColor>
#include <QPoint>
#include <QUndoCommand>

#include <QDebug>

#include "icongrid.h"

class DrawCell : public QUndoCommand
{
    public:
        DrawCell( IconGrid* grid, int col, int row,
                  const QColor& oldColor, const QColor& newColor )
                : m_grid( grid ),
                m_pos( QPoint( col, row ) ),
                m_oldColor( oldColor ),
                m_newColor( newColor ) { setText( "draw cell" ); }
        DrawCell( IconGrid* grid, const QPoint& pos,
                  const QColor& oldColor, const QColor& newColor )
                : m_grid( grid ),
                m_pos( pos ),
                m_oldColor( oldColor ),
                m_newColor( newColor ) { setText( "draw cell" ); }
        virtual void undo() {
            m_grid->setCellColor( m_pos, m_oldColor );
        }
        virtual void redo() {
            m_grid->setCellColor( m_pos, m_newColor );
        }
    private:
        IconGrid* m_grid;
        QPoint m_pos;
        QColor m_oldColor;
        QColor m_newColor;
};

class DrawLine : public QUndoCommand
{
    public:
        DrawLine( IconGrid* grid, int startCol, int startRow, int endCol, int endRow,
                  const QColor& oldColor, const QColor& newColor )
                : m_grid( grid ),
                m_startPos( QPoint( startCol, startRow ) ),
                m_endPos( QPoint( endCol, endRow ) ),
                m_oldColor( oldColor ),
                m_newColor( newColor ) { setText( "draw line" ); }
        DrawLine( IconGrid* grid, const QPoint& startPos, const QPoint& endPos,
                  const QColor& oldColor, const QColor& newColor )
                : m_grid( grid ),
                m_startPos( startPos ),
                m_endPos( endPos ),
                m_oldColor( oldColor ),
                m_newColor( newColor ) { setText( "draw line" ); }
        virtual void undo() {
            setLineColor( m_oldColor );
        }
        virtual void redo() {
            setLineColor( m_newColor );
        }
    protected:
        void setLineColor( const QColor& color ) {
            int deltaCol = qAbs( m_startPos.x() - m_endPos.x() );
            int deltaRow = qAbs( m_startPos.y() - m_endPos.y() );
            int colStep = ( m_startPos.x() < m_endPos.x() ) ? 1 : -1;
            int rowStep = ( m_startPos.y() < m_endPos.y() ) ? 1 : -1;
            int drawCol = m_startPos.x();
            int drawRow = m_startPos.y();
            if ( deltaCol >= deltaRow ) {
                double ey = (double)deltaRow / (double)deltaCol;
                for ( int i = 1; i <= deltaCol; ++i ) {
                    drawCol += colStep;
                    m_grid->setCellColor( QPoint( drawCol, drawRow + qRound( ey * i ) * rowStep ), color );
                }
            }
            else {
                double ex = (double)deltaCol / (double)deltaRow;
                for ( int i = 1; i <= deltaRow; ++i ) {
                    drawRow += rowStep;
                    m_grid->setCellColor( QPoint( drawCol + qRound( ex * i ) * colStep, drawRow ), color );
                }
            }
        }
    private:
        IconGrid* m_grid;
        QPoint m_startPos;
        QPoint m_endPos;
        QColor m_oldColor;
        QColor m_newColor;
};

#endif // UNDOCMD_H
