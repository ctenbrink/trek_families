#ifndef mainwindow_h
#define mainwindow_h

#include "QMainWindow.h"
#include <vector>
#include "person.h"

class QMenu;
class Document;
class Group;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow();
    void loadDocument(const std::string& filename);
    void saveDocument(const std::string& filename);
    void clear();
    
public slots:
    void update_title(const std::string& new_title);

private slots:
    void onOpen();
    void onSave();
    void onSaveAs();
    void onPrint();
    
private:
    QMenu* getViewMenu();
    void addGroup(Group& group);
    void arrangeWindows();
    
    QMenu* view_menu;
    std::vector<QDockWidget*> dock_widgets;
    std::auto_ptr<Document> doc;
    std::string current_doc_path;
};

#endif
