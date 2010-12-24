#include "palette.h"
#include "gridview.h"
#include "icongrid.h"
#include "mainwindow.h"

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


MainWindow::MainWindow( QWidget* parent )
    : KXmlGuiWindow( parent ),
      m_fileName( QString() )
{

    QDockWidget* paletteDock = new QDockWidget( i18n( "Palette" ), this );
    paletteDock->setObjectName( "paletteDock" );
    paletteDock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    paletteDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    m_palette = new Palette( paletteDock );
    paletteDock->setWidget( m_palette );
    addDockWidget( Qt::RightDockWidgetArea, paletteDock );
    ///viewMenu->addAction( paletteDock->toggleViewAction() );

    m_iconGrid = new IconGrid( m_palette );
    m_view = new GridView( m_iconGrid );
    setCentralWidget( m_view );

    setupActions();
}

MainWindow::~MainWindow()
{
//     kDebug() << "@@@@@@@@@";
}

void MainWindow::setupActions()
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

    KStandardAction::openNew( this, SLOT( newFile() ), actionCollection() );
    KStandardAction::open( this, SLOT( openFile() ), actionCollection() );
    KStandardAction::save( this, SLOT( saveFile() ), actionCollection() );
    KStandardAction::saveAs( this, SLOT( saveFileAs() ), actionCollection() );
    KStandardAction::print( this, SLOT( print() ), actionCollection() );
    KStandardAction::close( this, SLOT( close() ), actionCollection() );
    KStandardAction::quit( kapp, SLOT( quit() ), actionCollection() );

    KStandardAction::preferences( this, SLOT( slotConfigureSettings() ), actionCollection() );

    KStandardAction::undo( m_iconGrid, SLOT( undo() ), actionCollection() );
    KStandardAction::redo( m_iconGrid, SLOT( redo() ), actionCollection() );

    KStandardAction::cut( this, SLOT( cut() ), actionCollection() );
    KStandardAction::copy( this, SLOT( copy() ), actionCollection() );
    KStandardAction::paste( this, SLOT( paste() ), actionCollection() );
    KStandardAction::clear( this, SLOT( clear() ), actionCollection() );
    KStandardAction::selectAll( this, SLOT( selectAll() ), actionCollection() );

    KStandardAction::zoomIn( m_view, SLOT( zoomIn() ), actionCollection() );
    KStandardAction::zoomOut( m_view, SLOT( zoomOut() ), actionCollection() );
    KStandardAction::actualSize( m_view, SLOT( zoomReset() ), actionCollection() );
    setupGUI();
}

void MainWindow::newFile()
{
    m_fileName.clear();
//     textArea->clear();
}

void MainWindow::saveFileAs( const QString& outputFileName )
{
//     KSaveFile file( outputFileName );
//     file.open();

//     QByteArray outputByteArray;
//     outputByteArray.append( textArea->toPlainText().toUtf8() );
//     file.write( outputByteArray );
//     file.finalize();
//     file.close();
    m_iconGrid->toImage().save( outputFileName, "PNG" );

    m_fileName = outputFileName;
}

void MainWindow::saveFileAs()
{
    saveFileAs( KFileDialog::getSaveFileName() );
}

void MainWindow::saveFile()
{
    if( !m_fileName.isEmpty() ) {
        saveFileAs( m_fileName );
    }
    else {
        saveFileAs();
    }
}

void MainWindow::openFile()
{
    QString fileNameFromDialog = KFileDialog::getOpenFileName();

    QString tmpFile;
    if( KIO::NetAccess::download( fileNameFromDialog, tmpFile, this ) ) {
        QFile file( tmpFile );
        file.open( QIODevice::ReadOnly );
//         textArea->setPlainText( QTextStream( &file ).readAll() );
        m_fileName = fileNameFromDialog;

        KIO::NetAccess::removeTempFile( tmpFile );
    }
    else {
        KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
    }
}

void MainWindow::print()
{
//     QPrinter printer;
//     QPrintDialog printDialog( &printer, this );
//     printDialog.setWindowTitle( "Xxx"/*i18n( "Print %1", icon->url().section('/', -1) )*/ );
//     if ( printDialog.exec() ) {
//         int margin = 10, yPos = 0;
//         printer.setCreator( "KDE Icon Editor" );
// 
//         QPainter p;
//         p.begin( &printer );
//         QFontMetrics fm = p.fontMetrics();
//         
//         p.drawText( margin, margin + yPos, printer.width(), fm.lineSpacing(),
//                     Qt::TextExpandTabs | Qt::TextDontClip, icon->url() );
//                     yPos = yPos + fm.lineSpacing();
//         p.drawPixmap( margin, margin + yPos, grid->pixmap() );
//         p.end();
//     }
}
