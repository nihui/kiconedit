#ifndef KICONEDITPART_H
#define KICONEDITPART_H

#include <KParts/ReadWritePart>

class GridView;
class IconGrid;
class Palette;

namespace KIconEdit {
    typedef enum {
        None        = 0,
        Freehand    = 1,
        Line        = 2,
        Eraser      = 3,
        Colorpicker = 4,
        Rectangle   = 5,
        Ellipse     = 6
    } ToolType;
}

Q_DECLARE_METATYPE(KIconEdit::ToolType)

class KIconEditPart : public KParts::ReadWritePart
{
    Q_OBJECT
    public:
        explicit KIconEditPart( QWidget* parentWidget, QObject* parent, const QVariantList& args );
        virtual ~KIconEditPart();
    protected:
        virtual bool openFile();
        virtual bool saveFile();
    private:
        void setupActions();
    private:
        GridView* m_view;
        IconGrid* m_iconGrid;
        Palette* m_palette;
};

#endif // KICONEDITPART_H
