#include "groupdockwidget.h"
#include <sstream>

GroupDockWidget::GroupDockWidget(const std::string& title) : 
    QDockWidget(QString::fromStdString(title)),
    title_base(title)
{
}

void GroupDockWidget::update_title(const std::string& new_title)
{
    setWindowTitle(QString::fromStdString(new_title));
}
