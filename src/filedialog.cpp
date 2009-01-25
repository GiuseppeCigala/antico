////////////////////////////////////////
//  File      : filedialog.cpp        //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "filedialog.h"

////////////////////////////////////////

Filedialog::Filedialog(const QString &text, const QString &type, QWidget *parent) : QDialog(parent)
{
    button_type = type;
    win_text = text;
    setSizeGripEnabled(true);
    setContentsMargins(0, 10, 0, 0);
    read_settings();
    init();
    show();
}

Filedialog::~Filedialog()
{}

void Filedialog::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Other");
    delete_file_pix = stl_path + style->value("delete_file_pix").toString();
    open_with_pix = stl_path + style->value("open_with_pix").toString();
    style->endGroup(); //Other
}

void Filedialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(0, 0, width(), height());
}

void Filedialog::init()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QHBoxLayout *hor_layout = new QHBoxLayout();
    win_label = new QLabel("<b>" + win_text + "</b>", this); // show window text
    win_label->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    line_path = new QLineEdit(this); // show selection path
    hidden_files = new QRadioButton("Show hidden files", this);
    preview_label = new QLabel(this); // show pixmap preview
    preview_label->setMaximumSize(32, 32);
    preview_label->setScaledContents(true);
    hor_layout->addWidget(preview_label);
    hor_layout->addWidget(line_path);
    dir_model = new QDirModel(this);
    dir_model->setReadOnly(false);
    dir_model->setSupportedDragActions(Qt::LinkAction);
    QCompleter *completer = new QCompleter(this);
    completer->setModel(dir_model);
    line_path->setCompleter(completer);
    Fileicon *prov = new Fileicon(); // get the files icon
    dir_model->setIconProvider(prov);
    tree_view = new QTreeView(this);
    tree_view->setModel(dir_model);
    tree_view->setDragEnabled(true);
    layout->addLayout(hor_layout);
    layout->addWidget(hidden_files);
    layout->addWidget(win_label);
    layout->addWidget(tree_view);

    connect(tree_view, SIGNAL(clicked(QModelIndex)), this, SLOT(show_path(QModelIndex)));
    connect(tree_view, SIGNAL(clicked(QModelIndex)), this, SLOT(show_preview(QModelIndex)));
    connect(hidden_files, SIGNAL(toggled(bool)), this, SLOT(show_hidden(bool)));
    connect(line_path, SIGNAL(returnPressed()), this, SLOT(path_completer()));
    connect(completer, SIGNAL(activated(QModelIndex)), this, SLOT(update_tree(QModelIndex)));
  
    if (button_type == "OK_Cancel") // on new creation
    {
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);
        connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    }
    if (button_type == "Close") // on navigation
    {
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
        layout->addWidget(buttonBox);
        connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    }
    // show the Category apps list for open the file
    main_menu = new QMenu();
    open_menu = main_menu->addMenu(QIcon(open_with_pix), "Open with");
    cat_menu = new Categorymenu(open_menu);
    QAction *del_file = main_menu->addAction(QIcon(delete_file_pix), "Delete");
    connect(del_file, SIGNAL(triggered()), this, SLOT(del_file()));
}

void Filedialog::contextMenuEvent(QContextMenuEvent *event)
{
    if(tree_view->currentIndex().isValid() && tree_view->geometry().contains(event->pos()))
    {
        cat_menu->set_cmd_arguments(get_selected_path() + get_selected_name()); // set the file path+name as argument
        main_menu->exec(event->globalPos());
    }
}

void Filedialog::del_file()
{
    QModelIndex selection = tree_view->currentIndex();
    QString name = get_selected_name();

    if (dir_model->isDir(selection)) // test if is a directory
    {
        if (dir_model->rmdir(selection))
        {
            Msgbox *msg = new Msgbox(this);
            msg->setText("<b>" + name + "</b>" + " deleted");
            msg->setIcon(QMessageBox::Information);
        }
    }
    else if (dir_model->remove(selection)) // else is a file
    {
        Msgbox *msg = new Msgbox(this);
        msg->setText("<b>" + name + "</b>" + " deleted");
        msg->setIcon(QMessageBox::Information);
    }
}

void Filedialog::set_path(const QString &pth) // for folder navigation from deskfolder
{
    tree_view->setRootIndex(dir_model->index(pth));
    line_path->setText(pth);
}

void Filedialog::show_path(const QModelIndex &index)
{
    line_path->setText(dir_model->filePath(index));
}

void Filedialog::show_preview(const QModelIndex &index) // show file pixmap preview
{
    preview_label->setPixmap(QPixmap(dir_model->filePath(index)));
}

void Filedialog::path_completer() // on user button press in line_path 
{
    qDebug() << "Path completer:" << line_path->text();
    QModelIndex index = dir_model->index(line_path->text());
    
    if(index.isValid())
    {
        tree_view->setCurrentIndex(index);
    }
    else
    {
        reject();
    }
}

void Filedialog::update_tree(const QModelIndex &index) // update the tree on path completer
{
    tree_view->scrollTo(index);
    tree_view->setCurrentIndex(index);
}

void Filedialog::set_filter(QDir::Filters fltr)
{
    dir_model->setFilter(fltr);
}

void Filedialog::set_name_filters(const QStringList &fltr)
{
    dir_model->setNameFilters(fltr);
}

void Filedialog::set_read_only(bool b)
{
    dir_model->setReadOnly(b);
}

void Filedialog::show_hidden(bool select)
{
    if (select)
        dir_model->setFilter(dir_model->filter() | QDir::Hidden); // add hidden files to filter
    else
        dir_model->setFilter(dir_model->filter() ^ QDir::Hidden); // remove hidden files from filter
}

QString Filedialog::get_selected_path() const
{
    QModelIndex selection = tree_view->currentIndex();

    if (dir_model->isDir(selection)) // is a directory
    {
        QString path = dir_model->filePath(selection);
        qDebug() << "Selected path:" << path;
        return path;
    }
    else // is a file
    {
        QString filepath = dir_model->filePath(selection);
        QFileInfo fileinfo(filepath);
        QString path = fileinfo.absolutePath(); // remove the file name from path
        path.append("/"); // add slash at the end
        qDebug() << "Selected path:" << path;
        return path;
    }
}

QString Filedialog::get_selected_name() const
{
    QString name = dir_model->fileName(tree_view->currentIndex());
    qDebug() << "Selected name:" << name;
    return name;
}

QString Filedialog::get_selected_icon() const
{
    QString file = get_selected_name();
    QFileInfo fileinfo(file);
    Fileicon *prov = (Fileicon *)dir_model->iconProvider();
    QString icon = prov->type(fileinfo); // get the file icon
    return icon;
}

void Filedialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    { 
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
    }
}

void Filedialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Filedialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}


