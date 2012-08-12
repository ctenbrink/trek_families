#ifndef GROUPDOCKWIDGET_H
#define GROUPDOCKWIDGET_H

#include "QDockWidget.h"

class GroupDockWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    GroupDockWidget(const std::string& title);

public slots:
    void update_title(const std::string& new_title);
    
private:
    std::string title_base;
};

#endif //GROUPDOCKWIDGET_H

