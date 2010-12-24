#include "icongrid.h"
#include "gridview.h"

#include <QDebug>
#include <QResizeEvent>

#include <KLocale>
#include <KRuler>

GridView::GridView( IconGrid* iconGrid, QWidget* parent )
    : QScrollArea( parent ), m_iconGrid( iconGrid )
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
    zoomTo( 15.0 );
}

void GridView::zoomOut()
{
    zoomTo( 10.0 );
}

void GridView::zoomReset()
{
    zoomTo( 10.0 );
}

void GridView::zoomTo( double z )
{
    m_hRuler->setPixelPerMark( z );
    m_vRuler->setPixelPerMark( z );
    m_iconGrid->slotUnitPixelsChanged( z );
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
