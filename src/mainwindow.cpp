#include "mainwindow.h"
#include "QAction.h"
#include "QMenu.h"
#include "QTextEdit"
#include "QMenuBar"
#include "QMessageBox"
#include "groupdockwidget.h"
#include "personlistview.h"
#include "QTableView"
#include "QFileDialog"
#include "QPrinter"
#include "QPrintDialog"
#include "QPainter"
#include "QHeaderView"
#include "QApplication"
#include "QDesktopWidget"
#include <memory>
#include "Document.h"
#include "assert.h"
#include "personlistmodel.h"
#include "persontablemodel.h"
#include <cmath>
#include <sstream>
#include <algorithm>

namespace {
    std::auto_ptr<PersonListView> createPersonListView()
    {
        std::auto_ptr<PersonListView> listView(new PersonListView());
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listView->setDragEnabled(true);
        listView->setAcceptDrops(true);
        listView->setDropIndicatorShown(true);
        return listView;
    }
    
    std::auto_ptr<QTableView> createPersonTableView()
    {
        std::auto_ptr<QTableView> tableView(new QTableView());
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setDragEnabled(true);
        tableView->setAcceptDrops(true);
        tableView->setDropIndicatorShown(true);
        tableView->setDragDropOverwriteMode(false);
        tableView->setSortingEnabled(true);
        if(tableView->verticalHeader() != NULL)
            tableView->verticalHeader()->hide();
        return tableView;
    }
}

MainWindow::MainWindow() :
    view_menu(NULL),
    dock_widgets(),
    doc(),
    current_doc_path()
{
    std::auto_ptr<QAction> openAction(new QAction(tr("&Open..."), this));
    std::auto_ptr<QAction> saveAsAction(new QAction(tr("Save &As..."), this));
    std::auto_ptr<QAction> saveAction(new QAction(tr("&Save"), this));
    std::auto_ptr<QAction> printAction(new QAction(tr("&Print"), this));

    connect(openAction.get(), SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(saveAsAction.get(), SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(saveAction.get(), SIGNAL(triggered()), this, SLOT(onSave()));
    connect(printAction.get(), SIGNAL(triggered()), this, SLOT(onPrint()));

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction.release());
    fileMenu->addAction(saveAsAction.release());
    fileMenu->addAction(saveAction.release());
    fileMenu->addAction(printAction.release());
    
    view_menu = menuBar()->addMenu(tr("&View"));

    std::auto_ptr<QTableView> tableView = createPersonTableView();
    setCentralWidget(tableView.release());
    
    setWindowTitle(tr("Groups"));
}

void MainWindow::loadDocument(const std::string& filename)
{
    clear();
    doc.reset(new Document());
    current_doc_path = filename;
    doc->read_from_file(filename);

    QTableView* central_widget = dynamic_cast<QTableView*>(centralWidget());
    assert(central_widget != NULL);
    if(central_widget != NULL) {
        std::auto_ptr<PersonTableModel> model(new PersonTableModel(doc->get_unassigned_people()));
        QObject::connect(model.get(), SIGNAL(title_changed(const std::string&)),
                         this, SLOT(update_title(const std::string&)));
        update_title(model->makeTitle());
        central_widget->setModel(model.release());

    }
        
    std::vector<Group>& groups = doc->get_groups();

    for(std::vector<Group>::iterator iter = groups.begin();
        iter != groups.end();
        ++iter)
    {
        addGroup(*iter);
    }

    arrangeWindows();
}

void MainWindow::arrangeWindows()
{
    QDesktopWidget* desktop = QApplication::desktop();
    
    if(desktop == NULL)
        return;
    
    int screen_number = desktop->screenNumber(this);
    QRect screen_geometry = desktop->availableGeometry(screen_number);
    
    //Main window takes up right 1/3 of screen
    double dock_window_width_percent = 0.57;
    QRect rect = geometry();
    QRect frame_rect = frameGeometry();
    
    QRect new_rect;
    new_rect.setLeft(screen_geometry.left() + (dock_window_width_percent)*screen_geometry.width());
    new_rect.setWidth((1.0-dock_window_width_percent)*screen_geometry.width());
    new_rect.setTop(screen_geometry.top());
    new_rect.setHeight(screen_geometry.height());
    new_rect.adjust(rect.left() - frame_rect.left(),
                    rect.top() - frame_rect.top(),
                    rect.right() - frame_rect.right(),
                    rect.bottom() - frame_rect.bottom());
    setGeometry(new_rect);
    
    int num_cols = 5;
    int num_rows = 1 + (dock_widgets.size() / num_cols);
    int cur_col = 0;
    int cur_row = 0;    
    
    int col_width = ceil(dock_window_width_percent*screen_geometry.width()/num_cols);
    int row_height = 1 + screen_geometry.height()/num_rows;
    
    for(std::vector<QDockWidget*>::const_iterator iter = dock_widgets.begin();
        iter != dock_widgets.end(); 
        ++iter)
    {
        if(*iter != NULL) {
            (*iter)->setFloating(true);
            
            new_rect.setLeft(screen_geometry.left() + (cur_col * col_width));
            new_rect.setWidth(col_width);
            new_rect.setTop(cur_row * row_height + (cur_row == 0 ? 0 : 38));
            new_rect.setHeight(row_height - 20);
            (*iter)->setGeometry(new_rect);
            
            ++cur_col;
            if(cur_col == num_cols) {
                cur_col = 0;
                ++cur_row;
            }
        }
    }
}

void MainWindow::saveDocument(const std::string& filename)
{
    if(doc.get() != NULL) {
        current_doc_path = filename;
        doc->write_to_file(current_doc_path);
    }
}

QMenu* MainWindow::getViewMenu()
{
    return view_menu;
}

void MainWindow::clear()
{
    QMenu* mnu = getViewMenu();
    if(mnu != NULL)
        mnu->clear();

    for(std::vector<QDockWidget*>::const_iterator iter = dock_widgets.begin();
        iter != dock_widgets.end();
        ++iter)
    {
        removeDockWidget(*iter);
    }
    dock_widgets.clear();
}

void MainWindow::addGroup(Group& group)
{
    std::auto_ptr<GroupDockWidget> dock(new GroupDockWidget(group.get_name()));
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|
                          Qt::RightDockWidgetArea|
                          Qt::TopDockWidgetArea|
                          Qt::BottomDockWidgetArea);
    std::auto_ptr<PersonListView> lst = createPersonListView();
    std::auto_ptr<PersonListModel> model(new PersonListModel(group));
    QObject::connect(model.get(), SIGNAL(title_changed(const std::string&)),
                     dock.get(), SLOT(update_title(const std::string&)));
    dock->update_title(model->makeTitle());
    lst->updateBackgroundColor(model->getColor());
    lst->setModel(model.release());
    dock->setWidget(lst.release());
    
    QMenu* mnu = getViewMenu();
    if(mnu != NULL)
        mnu->addAction(dock->toggleViewAction());
    
    dock_widgets.push_back(dock.get());
    addDockWidget(Qt::LeftDockWidgetArea, dock.release());
}

void MainWindow::onOpen()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open a Groups Document"),
                                                    QDir::homePath(),
                                                    tr("YAML Files (*.yaml);;All Files (*)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty()) 
        loadDocument(fileName.toStdString());
}

void MainWindow::onSave()
{
    saveDocument(current_doc_path);
}

void MainWindow::onSaveAs()
{
    QString init_path = (current_doc_path.empty() ? QDir::homePath() : QString::fromStdString(current_doc_path));
    QString fileName = QFileDialog::getSaveFileName(this, 
                                                    tr("Save the Groups Document"),
                                                    init_path,
                                                    tr("YAML Files (*.yaml)"));
    if(!fileName.isEmpty())
        saveDocument(fileName.toStdString());
}

namespace {
    std::vector<std::string> populate_record(const std::string& group_name, const Person& person)
    {
        std::vector<std::string> record;
        record.push_back(person.get_first_name());
        record.push_back(person.get_last_name());
        record.push_back(person.get_ward());
        record.push_back(group_name);
        return record;
    }
    
    class Sorter
    {
    public:
        Sorter(const std::vector<size_t>& sort_order_in):sort_order(sort_order_in.begin(), sort_order_in.end()){}
        bool operator() (const std::vector<std::string>& left, const std::vector<std::string>& right)
        {
            bool rval = false;
            for(std::vector<size_t>::const_iterator sort_order_iter = sort_order.begin(); 
                sort_order_iter!=sort_order.end();
                ++sort_order_iter) {
                if(left[*sort_order_iter] != right[*sort_order_iter]) {
                    rval = left[*sort_order_iter] < right[*sort_order_iter];
                    break;
                }
            }
            return rval;
        }
    private:
        std::vector<size_t> sort_order;
        
    };
    
    void printSorted(std::vector<std::vector<std::string> >& data,
                     std::vector<size_t> order,
                     QPrinter& printer,
                     QPainter& painter,
                     const std::string& sort_type)
    {
        printer.newPage();
        Sorter sorter(order);
        std::sort(data.begin(), data.end(), sorter);
        
        painter.setPen(Qt::black);
        QFontMetrics metrics(painter.font());
        int row_height = metrics.height() + 10;
        int num_rows_per_col = printer.pageRect().height() / row_height;
        int num_cols = (data.size() / num_rows_per_col) + 1;
        int col_width = printer.pageRect().width() / num_cols;
        
        int x = 0;
        int y = 0;
        for(std::vector<std::vector<std::string> >::const_iterator iter = data.begin(); iter != data.end(); ++iter) {
            const std::vector<std::string>& record = *iter;
            std::string s = record[1] + ", " + record[0] + " (" + record[2] + ") " + record[3];
            QString qstring = QString::fromStdString(s);
            painter.drawText(x,
                             y, 
                             col_width, 
                             row_height,
                             Qt::AlignLeft | Qt::AlignBottom,
                             qstring);
            y += row_height;
            if(y > printer.pageRect().height() - 10) {
                x += col_width;
                y = 0;
            }
        }
        
        std::stringstream s;
        s<<data.size() << " youth, sorted by "<<sort_type;
        QString qstring = QString::fromStdString(s.str());
        painter.drawText(x, y+(2*row_height), col_width, row_height, Qt::AlignLeft|Qt::AlignBottom, qstring);
    }
    
    void printSortedLists(const Document& doc, QPrinter& printer, QPainter& painter)
    {
        std::vector<std::vector<std::string> > data;
        
        const std::vector<Group>& groups = doc.get_groups();
        for(std::vector<Group>::const_iterator group_iter = groups.begin(); group_iter != groups.end(); ++group_iter){
            const std::vector<Person>& members = group_iter->get_members();
            for(std::vector<Person>::const_iterator person_iter = members.begin(); 
                person_iter != members.end(); 
                ++person_iter){
                data.push_back(populate_record(group_iter->get_name(), *person_iter));            
            }
        }
        const std::vector<Person>& unassigned = doc.get_unassigned_people();
        for(std::vector<Person>::const_iterator person_iter = unassigned.begin(); 
            person_iter != unassigned.end(); 
            ++person_iter){
            data.push_back(populate_record("Unassigned", *person_iter));            
        }
        
        std::vector<size_t> last_name_order;
        last_name_order.push_back(1);
        last_name_order.push_back(0);
        last_name_order.push_back(2);
        last_name_order.push_back(3);
        printSorted(data, last_name_order, printer, painter, "last name");
        
        std::vector<size_t> ward_order;
        ward_order.push_back(2);
        ward_order.push_back(1);
        ward_order.push_back(0);
        ward_order.push_back(3);
        printSorted(data, ward_order, printer, painter, "unit");
        
        std::vector<size_t> group_order;
        group_order.push_back(3);
        group_order.push_back(1);
        group_order.push_back(0);
        group_order.push_back(2);
        printSorted(data, group_order, printer, painter, "family");
    }
}

void MainWindow::onPrint()
{
    if(doc.get() == NULL)
        return;
    
    QPrinter printer(QPrinter::HighResolution);
    printer.setOrientation(QPrinter::Landscape); //force landscape
    
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print the Groups Document"));
    
    if (dialog->exec() != QDialog::Accepted)
        return;
    
    QPrinter::Orientation orientation = printer.orientation();
    int col_per_page = (orientation == QPrinter::Portrait ? 4 : 5);
    int row_per_page = (orientation == QPrinter::Portrait ? 5 : 4);
    int col_width = printer.pageRect().width() / col_per_page;
    int row_height = printer.pageRect().height() / row_per_page;
    int top_margin = 110;
    row_height -= 30;
    int x_text_margin = 80;
    int y_text_margin = 80;
    int x_box_margin = 20;
    int y_box_margin = 20;
    int space_after_header = 30;
    qreal x_radius = 80;
    qreal y_radius = 80;
    QFont header_font( "Helvetica", 9, 12);
    header_font.setBold(true);
    QFont body_font("Helvetica", 9, 2);
    QBrush brush(Qt::SolidPattern);
    QPainter painter;
    painter.begin(&printer);
    
    QPen orig_pen = painter.pen();

    
    size_t cur_col = 0;
    size_t cur_row = 0;
    std::vector<Company>::const_iterator company_iter = doc->get_companies().begin();
    
    while(company_iter != doc->get_companies().end()) {
        
        std::vector<Group> groups_in_company;
        std::vector<std::string> group_names = company_iter->get_all_members();
        for(std::vector<std::string>::const_iterator group_name_iter = group_names.begin(); 
            group_name_iter!= group_names.end(); 
            ++group_name_iter) {
            std::vector<Group>::iterator group_iter = doc->find_group(*group_name_iter);
            if(group_iter != doc->get_groups().end())
                groups_in_company.push_back(*group_iter);
            
        }
        int x = cur_col * col_width;
        int y = 0;

        painter.setFont(header_font);
        const RGBColor& color = company_iter->get_color();
        QColor qcolor(color.get_red(), color.get_green(), color.get_blue());
        painter.setPen(qcolor);
        QFontMetrics metrics(painter.font());
        int flags = Qt::AlignHCenter | Qt::AlignTop;
        QRect bounding_rect;
        std::string company_name = company_iter->get_name();
        QString company_name_text = QString::fromStdString(company_name);
        int txt_x = x+x_text_margin;
        int txt_y = y+y_text_margin;
        int txt_w = col_width - (2*x_text_margin);
        int txt_h = metrics.boundingRect(company_name_text).height();
        painter.drawText( txt_x,  
                          txt_y, 
                          txt_w,
                          txt_h,
                          flags, 
                          company_name_text, 
                          &bounding_rect);


        for(std::vector<Group>::const_iterator iter = groups_in_company.begin();
            iter != groups_in_company.end();
            ++iter) {
        
            const RGBColor& color = iter->get_color();
            QColor qcolor(color.get_red(), color.get_green(), color.get_blue());
            
            brush.setColor(QColor(color.get_red(), color.get_green(), color.get_blue()));   
            painter.setBrush(brush);
            QPen outline_pen;
            outline_pen.setColor(Qt::black);
            painter.setPen(outline_pen);
            
            y = (cur_row * row_height) + top_margin;        
            
            painter.setBackgroundMode(Qt::OpaqueMode);
            
            painter.drawRoundedRect(x+x_box_margin,
                                    y+y_box_margin,
                                    col_width - (2*x_box_margin), 
                                    row_height - (2*y_box_margin), 
                                    x_radius, 
                                    y_radius);
            painter.setBackgroundMode(Qt::TransparentMode);
            
            int gray = qGray(color.get_red(), color.get_green(), color.get_blue());
            painter.setPen(gray < 128 ? Qt::white : Qt::black);
            
            painter.setFont(header_font);
            QFontMetrics metrics(painter.font());
            int flags = Qt::AlignHCenter | Qt::AlignTop;
            const Group& group = *iter;
            QString header_text = QString::fromStdString(make_title_from_group(group));
            painter.drawText( x+x_text_margin,  
                             y+y_text_margin, 
                             col_width - (2*x_text_margin),
                             metrics.boundingRect(header_text).height(),
                             flags, 
                             header_text, 
                             &bounding_rect);
            
            std::string members;
            for(std::vector<Person>::const_iterator person_iter = group.get_members().begin(); person_iter != group.get_members().end(); ++person_iter) {
                members += (*person_iter).get_first_name() + " ";
                members += (*person_iter).get_last_name() + " ";
                members += "(" + (*person_iter).get_ward() +")";
                if((*person_iter).get_on_youth_committee())
                    members += " *";
                members += "\n";
            }
            
            painter.setFont(body_font);
            flags = Qt::AlignLeft | Qt::AlignTop;
            painter.drawText( x + x_text_margin,  
                             y + y_text_margin + bounding_rect.height() + space_after_header, 
                             col_width - (2 * x_text_margin),
                             row_height - (2 * y_text_margin),
                             flags, 
                             QString::fromStdString(members), 
                             &bounding_rect);
        
            ++cur_row;
        }
        cur_row = 0;
        ++cur_col;
        ++company_iter;
    }
    
    printSortedLists(*doc.get(), printer, painter);
    
    painter.end();
}

void MainWindow::update_title(const std::string& new_title)
{
    setWindowTitle(QString::fromStdString(new_title));
}

