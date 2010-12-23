#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QScrollArea>

class QResizeEvent;
class QWidget;
class KRuler;
class GridView : public QScrollArea
{
    Q_OBJECT
    public:
        GridView( QWidget* parent = 0 );
        ~GridView();
        KRuler* hRuler() const { return m_hRuler; }
        KRuler* vRuler() const { return m_vRuler; }
    protected:
        virtual void mouseMoveEvent( QMouseEvent* event );
        virtual void resizeEvent( QResizeEvent* event );
    private:
        KRuler* m_hRuler;
        KRuler* m_vRuler;
};

#endif // GRIDVIEW_H
