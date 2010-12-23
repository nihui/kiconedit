#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>

#include "mainwindow.h"

int main( int argc, char** argv )
{
    KAboutData aboutData( "kiconedit", 0, ki18n( "KIconEdit" ), "1.0.0" );
    aboutData.setShortDescription( ki18n( "KDE Icon Editor" ) );
    aboutData.setLicense( KAboutData::License_GPL );
    aboutData.setCopyrightStatement( ki18n( "(c) 1998 Thomas Tanghus" ) );

    aboutData.addAuthor( ki18n("Ni Hui"), KLocalizedString(), "shuizhuyuanluo@126.com" );
    aboutData.addAuthor( ki18n("Thomas Tanghus"), KLocalizedString(), "tanghus@kde.org" );
    aboutData.addAuthor( ki18n("John Califf"), KLocalizedString(), "jcaliff@compuzone.net" );
    aboutData.addAuthor( ki18n("Laurent Montel"), KLocalizedString(), "montel@kde.org" );
    aboutData.addAuthor( ki18n("Aaron J. Seigo"), KLocalizedString(), "aseigo@olympusproject.org" );
    aboutData.addCredit( ki18n("Nadeem Hasan"),
                         ki18n("Rewrote UI to use XMLGUI\nLots of fixes and cleanup"),
                         "nhasan@nadmm.com" );
    aboutData.addCredit( ki18n("Adrian Page"),
                         ki18n("Bug fixes and GUI tidy up"),
                         "Adrian.Page@tesco.net" );

    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;

    MainWindow* window = new MainWindow;
    window->show();

    return app.exec();
}
