
#include "mainwindow.h"

#include <QTextStream>

#include <KDebug>
#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <KFileDialog>
#include <KIO/NetAccess>
#include <KLocale>
#include <KMessageBox>
#include <KSaveFile>
#include <KStandardAction>
#include <KToggleAction>

#include <KService>


MainWindow::MainWindow()
: KParts::MainWindow()
{
    setupActions();
    KService::Ptr service = KService::serviceByDesktopPath( "kiconeditpart.desktop" );
    if ( service ) {
        m_part = service->createInstance<KParts::ReadWritePart>( 0 );
        if ( m_part ) {
            setCentralWidget( m_part->widget() );
            setupGUI( ToolBar | Keys | StatusBar | Save );
            createGUI( m_part );
        }
        else {
            return;
        }
    }
    else {
        KMessageBox::error( this, "Service kiconeditpart.desktop not found." );
        qApp->quit();
        return;
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::load( const KUrl& url )
{
    m_part->openUrl( url );
}

void MainWindow::load()
{
    load( KFileDialog::getOpenUrl() );
}

void MainWindow::setupActions()
{
    KStandardAction::openNew( this, SLOT( newFile() ), actionCollection() );

    KStandardAction::open( this, SLOT( load() ), actionCollection() );

    KStandardAction::save( this, SLOT( saveFile() ), actionCollection() );
    KStandardAction::saveAs( this, SLOT( saveFileAs() ), actionCollection() );
    KStandardAction::print( this, SLOT( print() ), actionCollection() );
    KStandardAction::close( this, SLOT( close() ), actionCollection() );
    KStandardAction::quit( kapp, SLOT( quit() ), actionCollection() );

    KStandardAction::preferences( this, SLOT( slotConfigureSettings() ), actionCollection() );
}

// void MainWindow::newFile()
// {
//     m_fileName.clear();
// }
// 
// void MainWindow::saveFileAs( const QString& outputFileName )
// {
//     m_iconGrid->toImage().save( outputFileName, "PNG" );
// 
//     m_fileName = outputFileName;
// }
// 
// void MainWindow::saveFileAs()
// {
//     saveFileAs( KFileDialog::getSaveFileName() );
// }
// 
// void MainWindow::saveFile()
// {
//     if( !m_fileName.isEmpty() ) {
//         saveFileAs( m_fileName );
//     }
//     else {
//         saveFileAs();
//     }
// }
// 
// void MainWindow::openFile()
// {
//     QString fileNameFromDialog = KFileDialog::getOpenFileName();
// 
//     QString tmpFile;
//     if( KIO::NetAccess::download( fileNameFromDialog, tmpFile, this ) ) {
//         QFile file( tmpFile );
//         file.open( QIODevice::ReadOnly );
// //         textArea->setPlainText( QTextStream( &file ).readAll() );
//         m_fileName = fileNameFromDialog;
// 
//         KIO::NetAccess::removeTempFile( tmpFile );
//     }
//     else {
//         KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
//     }
// }
// 
// void MainWindow::print()
// {
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
// }
