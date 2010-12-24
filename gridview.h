#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QScrollArea>

class QResizeEvent;
class QWidget;
class KRuler;
class IconGrid;

class GridView : public QScrollArea
{
    Q_OBJECT
    public:
        GridView( IconGrid* iconGrid, QWidget* parent = 0 );
        ~GridView();
    public Q_SLOTS:
        void zoomIn();
        void zoomOut();
        void zoomReset();
        void zoomTo( double z );
    protected:
        virtual void mouseMoveEvent( QMouseEvent* event );
        virtual void resizeEvent( QResizeEvent* event );
    private:
        KRuler* m_hRuler;
        KRuler* m_vRuler;
        IconGrid* m_iconGrid;
        double m_scaleFactor;
};

#endif // GRIDVIEW_H
