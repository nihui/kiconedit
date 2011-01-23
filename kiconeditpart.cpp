#include "kiconeditpart.h"

#include "palette.h"
#include "gridview.h"
#include "icongrid.h"

#include <QDockWidget>
#include <QTextStream>

#include <KDebug>
#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <KFileDialog>
#include <KIO/NetAccess>
#include <KLocale>
#include <KMessageBox>
#include <KRuler>
#include <KSaveFile>
#include <KStandardAction>
#include <KToggleAction>

#include <KAboutData>
#include <KPluginFactory>

static KAboutData createAboutData()
{
    KAboutData aboutData(
        "kiconeditpart",
        "kiconeditpart",
        ki18n( "KIconEdit Part" ),
        "0.1",
        ki18n( "KIconEdit Part" ),
        KAboutData::License_GPL,
        ki18n( "Copyright (C) 2011 Ni Hui" )
    );
    aboutData.addAuthor( ki18n( "Ni Hui" ), KLocalizedString(), "shuizhuyuanluo@126.com" );
    return aboutData;
}

K_PLUGIN_FACTORY( KIconEditPartFactory, registerPlugin<KIconEditPart>(); )
K_EXPORT_PLUGIN( KIconEditPartFactory( createAboutData() ) )

KIconEditPart::KIconEditPart( QWidget* parentWidget, QObject* parent, const QVariantList& args )
: KParts::ReadWritePart(parent)
{
    setComponentData( KIconEditPartFactory::componentData() );

//     QDockWidget* paletteDock = new QDockWidget( i18n( "Palette" ), this );
//     paletteDock->setObjectName( "paletteDock" );
//     paletteDock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
//     paletteDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
//     m_palette = new Palette( paletteDock );
//     paletteDock->setWidget( m_palette );
//     addDockWidget( Qt::RightDockWidgetArea, paletteDock );
    ///viewMenu->addAction( paletteDock->toggleViewAction() );

    m_palette = new Palette;/// FIXME

    m_iconGrid = new IconGrid( m_palette );
    m_view = new GridView( m_iconGrid );
    setWidget( m_view );

    m_palette->setParent( m_view );/// FIXME

    setupActions();

    setXMLFile( "kiconeditpartui.rc" );
}

KIconEditPart::~KIconEditPart()
{
}

bool KIconEditPart::openFile()
{
}

bool KIconEditPart::saveFile()
{
}

void KIconEditPart::setupActions()
{
    KAction* resizeAction = new KAction( this );
    resizeAction->setText( i18n( "Resi&ze..." ) );
    resizeAction->setIcon( KIcon( "transform-scale" ) );
    ///clearAction->setShortcut( Qt::CTRL + Qt::Key_W );
    actionCollection()->addAction( "edit_resize", resizeAction );
    ///connect( resizeAction, SIGNAL( triggered( bool ) ), textArea, SLOT( clear() ) );

    QActionGroup* toolActionGroup = new QActionGroup( this );
    connect( toolActionGroup, SIGNAL(triggered(QAction*)), m_iconGrid, SLOT(slotToolChanged(QAction*)) );

    KToggleAction* freehandAction = new KToggleAction( this );
    freehandAction->setText( i18n( "Freehand" ) );
    freehandAction->setIcon( KIcon( "draw-freehand" ) );
    freehandAction->setData( QVariant::fromValue( KIconEdit::Freehand ) );
    actionCollection()->addAction( "tool_freehand", freehandAction );
    toolActionGroup->addAction( freehandAction );

    KToggleAction* lineAction = new KToggleAction( this );
    lineAction->setText( i18n( "Line" ) );
    lineAction->setIcon( KIcon( "draw-line" ) );
    lineAction->setData( QVariant::fromValue( KIconEdit::Line ) );
    actionCollection()->addAction( "tool_line", lineAction );
    toolActionGroup->addAction( lineAction );

    KToggleAction* eraserAction = new KToggleAction( this );
    eraserAction->setText( i18n( "Eraser" ) );
    eraserAction->setIcon( KIcon( "draw-eraser" ) );
    eraserAction->setData( QVariant::fromValue( KIconEdit::Eraser ) );
    actionCollection()->addAction( "tool_eraser", eraserAction );
    toolActionGroup->addAction( eraserAction );

    KToggleAction* colorpickerAction = new KToggleAction( this );
    colorpickerAction->setText( i18n( "Color Picker" ) );
    colorpickerAction->setIcon( KIcon( "color-picker" ) );
    colorpickerAction->setData( QVariant::fromValue( KIconEdit::Colorpicker ) );
    actionCollection()->addAction( "tool_colorpicker", colorpickerAction );
    toolActionGroup->addAction( colorpickerAction );

    KStandardAction::openNew( this, SLOT( newFile() ), actionCollection() );
    KStandardAction::open( this, SLOT( openFile() ), actionCollection() );
    KStandardAction::save( this, SLOT( saveFile() ), actionCollection() );
    KStandardAction::saveAs( this, SLOT( saveFileAs() ), actionCollection() );
    KStandardAction::print( this, SLOT( print() ), actionCollection() );
    KStandardAction::close( this, SLOT( close() ), actionCollection() );

    KStandardAction::undo( m_iconGrid, SLOT( undo() ), actionCollection() );
    KStandardAction::redo( m_iconGrid, SLOT( redo() ), actionCollection() );

    KStandardAction::cut( this, SLOT( cut() ), actionCollection() );
    KStandardAction::copy( this, SLOT( copy() ), actionCollection() );
    KStandardAction::paste( this, SLOT( paste() ), actionCollection() );
    KStandardAction::clear( m_iconGrid, SLOT( clear() ), actionCollection() );
    KStandardAction::selectAll( this, SLOT( selectAll() ), actionCollection() );

    KStandardAction::zoomIn( m_view, SLOT( zoomIn() ), actionCollection() );
    KStandardAction::zoomOut( m_view, SLOT( zoomOut() ), actionCollection() );
    KStandardAction::actualSize( m_view, SLOT( zoomReset() ), actionCollection() );
}

