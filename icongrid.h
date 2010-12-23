#ifndef ICONGRID_H
#define ICONGRID_H

#include <QColor>
#include <QImage>
#include <QUndoStack>
#include <QVector>
#include <QWidget>

#include "mainwindow.h"

class QMouseEvent;
class QPaintEvent;
class QPoint;
class Palette;

class IconGrid : public QWidget
{
    Q_OBJECT
    public:
        IconGrid( Palette* palette, QWidget* parent = 0 );
        ~IconGrid();
        QImage toImage() const;
        QColor currentColor() const;
        int currentCol() const;
        int currentRow() const;
        void setCellColor( int col, int row, const QColor& color );
        void setCellColor( const QPoint& pos, const QColor& color );
        void undo() { m_undoStack.undo(); }
        void redo() { m_undoStack.redo(); }
    signals:
        void currentColChanged( int newCol );
        void currentRowChanged( int newRow );
        void cursorColChanged( int newCol );
        void cursorRowChanged( int newRow );
        void cursorXChanged( int newX );
        void cursorYChanged( int newY );
    public slots:
        void slotUnitPixelsChanged( int newUnitPixels );
        void slotToolChanged( QAction* action );
    protected:
        virtual void mouseMoveEvent( QMouseEvent* event );
        virtual void mousePressEvent( QMouseEvent* event );
        virtual void mouseReleaseEvent( QMouseEvent* event );
        virtual void paintEvent( QPaintEvent* event );
    private:
        Palette* m_palette;
        KIconEdit::ToolType m_tooltype;
        QVector<QColor> m_colorArray;
        int m_cols;
        int m_rows;
        int m_unitPixels;
        int m_currentCol;
        int m_currentRow;
        QUndoStack m_undoStack;
};

#endif // ICONGRID_H
