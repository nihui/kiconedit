#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class QWidget;
class GridView;
class IconGrid;
class Palette;

namespace KIconEdit {
    typedef enum {
        None        = 0,
        Freehand    = 1,
        Line        = 2,
        Rectangle   = 3,
        Ellipse     = 4
    } ToolType;
}

Q_DECLARE_METATYPE(KIconEdit::ToolType)

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    public:
        MainWindow( QWidget* parent = 0 );
        ~MainWindow();
    private slots:
        void newFile();
        void openFile();
        void saveFile();
        void saveFileAs();
        void saveFileAs( const QString& outputFileName );
        void print();
        void undo();
        void redo();
        void zoomIn();
        void zoomOut();
    private:
        QString m_fileName;
        GridView* m_view;
        IconGrid* m_iconGrid;
        Palette* m_palette;
        void setupActions();
};


#endif // MAINWINDOW_H
