
#include "personlistview.h"
#include "personlistmodel.h"
#include "QAction"
#include "QMenu"
#include "QColorDialog"

PersonListView::PersonListView(QWidget* parent /*= NULL*/) :
    QListView(parent)
{
}

void PersonListView::contextMenuEvent (QContextMenuEvent * event)
{
    
    std::auto_ptr<QAction> defineColorAction(new QAction(tr("&Define Color..."), this));
    
    connect(defineColorAction.get(), SIGNAL(triggered()), this, SLOT(onDefineColor()));
    
    QMenu contextMenu;
    contextMenu.addAction(defineColorAction.release());
    contextMenu.exec(QCursor::pos());
}

void PersonListView::onDefineColor()
{
    
    QAbstractItemModel* abs_model = model();
    PersonListModel* model = dynamic_cast<PersonListModel*>(abs_model);
    
    if(model != NULL) {
        QColor old_color = model->getColor();
        QColor new_color = QColorDialog::getColor(old_color, this);
        model->setColor(new_color);
        updateBackgroundColor(new_color);
    }
}

void PersonListView::updateBackgroundColor(const QColor& color)
{
    int gray = qGray(color.red(), color.green(), color.blue());
    
    QString style = "color: ";
    style += (gray < 128 ? "white" : "black");
    style += "; background-color: ";
    style += color.name();
    
    setStyleSheet(style);
}

