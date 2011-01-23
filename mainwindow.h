#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KParts/MainWindow>

class MainWindow : public KParts::MainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow();
        virtual ~MainWindow();
    public Q_SLOTS:
        void load( const KUrl& url );
        void load();
    private:
        void setupActions();
    private:
        KParts::ReadWritePart* m_part;
};

#endif // MAINWINDOW_H
