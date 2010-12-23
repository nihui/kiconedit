#include "palette.h"

#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>

#include <KColorButton>
#include <KColorDialog>

class ForeBackDisplay : public QWidget
{
    friend class Palette;
    public:
        ForeBackDisplay( QWidget* parent = 0 );
        ~ForeBackDisplay();
    protected:
        virtual void resizeEvent( QResizeEvent* event );
    private:
        QPushButton* backgroundButton;
        QPushButton* foregroundButton;
};

ForeBackDisplay::ForeBackDisplay( QWidget* parent )
    : QWidget( parent )
{
    setMinimumHeight( 30 );
    backgroundButton = new QPushButton( this );
    foregroundButton = new QPushButton( this );
}

ForeBackDisplay::~ForeBackDisplay()
{
}

void ForeBackDisplay::resizeEvent( QResizeEvent* event )
{
    QWidget::resizeEvent( event );
    int buttonHeight = event->size().height() >= 60 ? 40 : event->size().height() * 2 / 3;
    backgroundButton->resize( event->size().width() * 2 / 3, buttonHeight );
    foregroundButton->resize( event->size().width() * 2 / 3, buttonHeight );
    backgroundButton->move( event->size().width() / 3, buttonHeight / 2 );
    foregroundButton->move( 0, 0 );
}

Palette::Palette( QWidget* parent )
    : QWidget( parent )
{
    m_foregroundColor = Qt::blue;
    m_backgroundColor = Qt::red;

    QVBoxLayout* widgetLayout = new QVBoxLayout;
    widgetLayout->setContentsMargins( 0, 0, 0, 0 );
    setLayout( widgetLayout );

    /// fore/back display
    display = new ForeBackDisplay;
    widgetLayout->addWidget( display );

    connect( display->backgroundButton, SIGNAL( clicked() ), this, SLOT( backgroundColorButtonClicked() ) );
    connect( display->foregroundButton, SIGNAL( clicked() ), this, SLOT( foregroundColorButtonClicked() ) );

    slotSetForegroundColor( m_foregroundColor );
    slotSetBackgroundColor( m_backgroundColor );

    /// color cells
    QGridLayout* colorCellsLayout = new QGridLayout;
    widgetLayout->addLayout( colorCellsLayout );

    QLabel* lb00 = new QLabel("A");
    QLabel* lb01 = new QLabel("B");
    QLabel* lb10 = new QLabel("C");
    QLabel* lb11 = new QLabel("D");
    QLabel* lb20 = new QLabel("E");
    QLabel* lb21 = new QLabel("F");
    colorCellsLayout->addWidget( lb00, 0, 0 );
    colorCellsLayout->addWidget( lb01, 0, 1 );
    colorCellsLayout->addWidget( lb10, 1, 0 );
    colorCellsLayout->addWidget( lb11, 1, 1 );
    colorCellsLayout->addWidget( lb20, 2, 0 );
    colorCellsLayout->addWidget( lb21, 2, 1 );
//     KColorButton* cells = new KColorButton( QColor( 60, 140, 160 ) );
//     widgetLayout->addWidget( cells );
}

Palette::~Palette()
{
}

QColor Palette::foregroundColor() const
{
    return m_foregroundColor;
}

QColor Palette::backgroundColor() const
{
    return m_backgroundColor;
}

void Palette::slotSetForegroundColor( const QColor& newColor )
{
    m_foregroundColor = newColor;
    display->foregroundButton->setPalette( QPalette( m_foregroundColor ) );
    emit foregroundColorChanged( m_foregroundColor );
}

void Palette::slotSetBackgroundColor( const QColor& newcolor )
{
    m_backgroundColor = newcolor;
    display->backgroundButton->setPalette( QPalette( m_backgroundColor ) );
    emit backgroundColorChanged( m_backgroundColor );
}

void Palette::foregroundColorButtonClicked()
{
    QColor c = m_foregroundColor;
    if ( KColorDialog::getColor( c, this ) != QDialog::Rejected ) {
        slotSetForegroundColor( c );
    }
}

void Palette::backgroundColorButtonClicked()
{
    QColor c = m_backgroundColor;
    if ( KColorDialog::getColor( c, this ) != QDialog::Rejected ) {
        slotSetBackgroundColor( c );
    }
}

