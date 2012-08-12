#ifndef PERSONLISTVIEW_H
#define PERSONLISTVIEW_H

#include "QListView"

class QContextMenuEvent;

class PersonListView : public QListView
{
Q_OBJECT

public:
    PersonListView(QWidget* parent = NULL);
    void updateBackgroundColor(const QColor& color);
    
protected:
    virtual void contextMenuEvent (QContextMenuEvent * event);
    
private slots:
    void onDefineColor();
};
#endif //PERSONLISTVIEW_H
