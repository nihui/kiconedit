#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QWidget>

class QPushButton;
class ForeBackDisplay;

class Palette : public QWidget
{
    Q_OBJECT
    public:
        Palette( QWidget* parent = 0 );
        ~Palette();
        QColor foregroundColor() const;
        QColor backgroundColor() const;
    public slots:
        void slotSetForegroundColor( const QColor& newColor );
        void slotSetBackgroundColor( const QColor& newcolor );
    signals:
        void foregroundColorChanged( const QColor& newColor );
        void backgroundColorChanged( const QColor& newColor );
    private slots:
        void foregroundColorButtonClicked();
        void backgroundColorButtonClicked();
    private:
        QColor m_foregroundColor;
        QColor m_backgroundColor;
        ForeBackDisplay* display;
};

#endif // PALETTE_H
