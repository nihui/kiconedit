#include "icongrid.h"
#include "gridview.h"

#include <QDebug>
#include <QResizeEvent>

#include <KLocale>
#include <KRuler>

static const int zoomLevels[10] = {
    1, 2, 4, 7, 10, 14, 18, 24, 32, 44
};

GridView::GridView( IconGrid* iconGrid, QWidget* parent )
    : QScrollArea( parent ), m_iconGrid( iconGrid ), m_zoomIndex(4)
{
    m_hRuler = new KRuler( Qt::Horizontal, this );
    m_hRuler->setEndLabel( i18n( "width" ) );
    m_hRuler->setOffset( 0 );
    m_hRuler->setRange( 0, 1000 );
    m_hRuler->setShowTinyMarks( true );
    m_hRuler->setShowLittleMarks( false );
    m_hRuler->setShowMediumMarks( true );
    m_hRuler->setShowBigMarks( true );
    m_hRuler->setShowEndMarks( true );

    m_vRuler = new KRuler( Qt::Vertical, this );
    m_vRuler->setEndLabel( i18n( "height" ) );
    m_vRuler->setOffset( 0 );
    m_vRuler->setRange( 0, 1000 );
    m_vRuler->setShowTinyMarks( true );
    m_vRuler->setShowLittleMarks( false );
    m_vRuler->setShowMediumMarks( true );
    m_vRuler->setShowBigMarks( true );
    m_vRuler->setShowEndMarks( true );

    setViewportMargins( m_hRuler->height(), m_vRuler->width(), 0, 0 );
    setWidget( m_iconGrid );

    connect( m_iconGrid, SIGNAL( cursorXChanged( int ) ), m_hRuler, SLOT( slotNewValue( int ) ) );
    connect( m_iconGrid, SIGNAL( cursorYChanged( int ) ), m_vRuler, SLOT( slotNewValue( int ) ) );
}

GridView::~GridView()
{
//     kDebug() << "#############################################~~~~~~~~~~~~";
}

void GridView::zoomIn()
{
    if ( ++m_zoomIndex > 9 )
        m_zoomIndex = 9;
    zoomTo( zoomLevels[ m_zoomIndex ] );
}

void GridView::zoomOut()
{
    if ( --m_zoomIndex < 0 )
        m_zoomIndex = 0;
    zoomTo( zoomLevels[ m_zoomIndex ] );
}

void GridView::zoomReset()
{
    m_zoomIndex = 4;
    zoomTo( zoomLevels[ m_zoomIndex ] );
}

void GridView::zoomTo( int zp )
{
    m_hRuler->setPixelPerMark( zp );
    m_vRuler->setPixelPerMark( zp );
    m_iconGrid->slotUnitPixelsChanged( zp );
}

void GridView::mouseMoveEvent( QMouseEvent* event )
{
    QAbstractScrollArea::mouseMoveEvent( event );
}

void GridView::resizeEvent( QResizeEvent* event )
{
    QAbstractScrollArea::resizeEvent( event );
    m_hRuler->setGeometry( m_vRuler->width(), 0, width(), m_hRuler->height() );
    m_vRuler->setGeometry( 0, m_hRuler->height(), m_vRuler->width(), height() );
}
