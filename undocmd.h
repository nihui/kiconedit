#ifndef UNDOCMD_H
#define UNDOCMD_H

#include <QColor>
#include <QPoint>
#include <QUndoCommand>
#include <QVector>

#include <QDebug>

#include "icongrid.h"

class DrawCell : public QUndoCommand
{
    public:
        DrawCell( IconGrid* grid, int col, int row,
                  const QColor& newColor )
        : m_grid( grid ),
        m_pos( QPoint( col, row ) ),
        m_newColor( newColor ) {
            setText( "draw cell" );
            m_oldColor = m_grid->cellColorAt( col, row );
        }
        DrawCell( IconGrid* grid, const QPoint& pos,
                  const QColor& newColor )
        : m_grid( grid ),
        m_pos( pos ),
        m_newColor( newColor ) {
            setText( "draw cell" );
            m_oldColor = m_grid->cellColorAt( pos.x(), pos.y() );
        }
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
                  const QColor& newColor ) {
            setText( "draw line" );
            setLineColor( grid, QPoint( startCol, startRow ), QPoint( endCol, endRow ), newColor );
        }
        DrawLine( IconGrid* grid, const QPoint& startPos, const QPoint& endPos,
                  const QColor& newColor ) {
            setText( "draw line" );
            setLineColor( grid, startPos, endPos, newColor );
        }
        virtual ~DrawLine() {
            qDeleteAll( m_drawCells );
        }
        virtual void undo() {
            QVector<DrawCell*>::Iterator it = m_drawCells.begin();
            QVector<DrawCell*>::Iterator end = m_drawCells.end();
            while ( it != end ) {
                (*it)->undo();
                ++it;
            }
        }
        virtual void redo() {
            QVector<DrawCell*>::Iterator it = m_drawCells.begin();
            QVector<DrawCell*>::Iterator end = m_drawCells.end();
            while ( it != end ) {
                (*it)->redo();
                ++it;
            }
        }
    protected:
        void setLineColor( IconGrid* grid, const QPoint& startPos, const QPoint& endPos, const QColor& color ) {
            int deltaCol = qAbs( startPos.x() - endPos.x() );
            int deltaRow = qAbs( startPos.y() - endPos.y() );
            int colStep = ( startPos.x() < endPos.x() ) ? 1 : -1;
            int rowStep = ( startPos.y() < endPos.y() ) ? 1 : -1;
            int drawCol = startPos.x();
            int drawRow = startPos.y();
            if ( deltaCol >= deltaRow ) {
                double ey = (double)deltaRow / (double)deltaCol;
                for ( int i = 1; i <= deltaCol; ++i ) {
                    drawCol += colStep;
                    DrawCell* drawCell = new DrawCell( grid, drawCol, drawRow + qRound( ey * i ) * rowStep, color );
                    m_drawCells.append( drawCell );
                }
            }
            else {
                double ex = (double)deltaCol / (double)deltaRow;
                for ( int i = 1; i <= deltaRow; ++i ) {
                    drawRow += rowStep;
                    DrawCell* drawCell = new DrawCell( grid, drawCol + qRound( ex * i ) * colStep, drawRow, color );
                    m_drawCells.append( drawCell );
                }
            }
        }
    private:
        QVector<DrawCell*> m_drawCells;
};

#endif // UNDOCMD_H
