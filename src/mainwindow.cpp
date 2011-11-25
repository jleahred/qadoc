#include "mainwindow.h"

#include <iostream>


#include <QPlainTextEdit>
#include <QTextStream>
#include <QFile>
#include <QSplitter>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QLabel>
#include <QStatusBar>
#include <QRegExp>
#include <QMessageBox>
#include <QClipboard>
#include <QLineEdit>



#include "forms/dialog_compile_options.h"



const char* build_options[][3]  = {
                                {"simple", "asciidoc", "html"}
                              , {"using source  (the command configurated on source)", "<source_command>" , "" }
                              , {"html icons embeded", "asciidoc --verbose -a data-uri -a icons -a toc -a max-width=55em", "html"}
                              , {"html chuncked", "a2x --verbose -d book -r . --icons --icons-dir .. -f chunked", "html"}
                              , {"pdf small", "a2x --verbose -d article --icons --dblatex-opts \"-T native -P doc.pdfcreator.show=0 -P doc.collab.show=0 -P latex.output.revhistory=0 -P doc.toc.show=1 -P table.title.top\" -f pdf ", "pdf"}
                              , {"pdf book", "a2x --verbose -d book --icons --dblatex-opts \"-T native -P doc.pdfcreator.show=0 -P doc.collab.show=0 -P latex.output.revhistory=0 -P doc.toc.show=1 -P table.title.top\" -f pdf ", "pdf"}
                              , {0, 0, 0}
                            };




QPlainTextEdit*   log_text;
void log (const QString& text)
{
    if (log_text)
        log_text->appendPlainText(text);
}




class status_bar
{
    static status_bar*  internal_instance;

public:

    static status_bar*  instance()   {   return internal_instance;  };

    static void init (QStatusBar* sb)
    {
        if (internal_instance != 0)
        {
            log ("Error initializing status bar");
            return;
        }
        internal_instance = new status_bar(sb);
    }

    void set_file_path(const QString& _file_path)
    {
        label_file_path->setText(_file_path);
    }

    void set_cursor_position (const QString& position)
    {
        label_cursor_position->setText(position);
    }

private:
    status_bar(QStatusBar* sb) : label_file_path(new QLabel), label_cursor_position(new QLabel)
    {
        sb->addWidget(label_cursor_position);
        sb->addWidget(label_file_path);
    }

    QLabel*      label_file_path;
    QLabel*      label_cursor_position;

};


status_bar* status_bar::internal_instance = 0;






QString get_full_file_path_from_link(QString link, QString current_doc_full_path)
{
    QString full_path;

    if (link.indexOf("/") == 0)
        return link;
    else
        return current_doc_full_path + "/" +  QUrl(link).encodedPath();
}










void setupFileMenu(QMainWindow* w)
{
    QMenu *fileMenu = new QMenu("&File", w);
    w->menuBar()->addMenu(fileMenu);

//    fileMenu->addAction("&New", w, SLOT(newFile()),
//                        QKeySequence::New);

    fileMenu->addAction("&Open...", w, SLOT(openNewTabFromFile()));//,QKeySequence::Open);

    fileMenu->addAction("&Save", w, SLOT(saveTab()),
                        QKeySequence::Save);

    fileMenu->addAction("&Close", w, SLOT(closeTab()),
                        QKeySequence::Close);

    fileMenu->addAction("E&xit", qApp, SLOT(quit()),
                        QKeySequence::Quit);
}


void setupCompileMenu(QMainWindow* w)
{
    QMenu *compileMenu = new QMenu("&Compile", w);
    w->menuBar()->addMenu(compileMenu);

    compileMenu->addAction("&Generate quick html preview", w, SLOT(generateHtml()));
    compileMenu->addAction("&Select option to build", w, SLOT(build_with_options()));
    compileMenu->addAction("Build with &Document options", w, SLOT(build_with_source_command()), QKeySequence(Qt::CTRL + Qt::Key_M, Qt::CTRL + Qt::Key_C, Qt::CTRL + Qt::Key_D));

}

void setupEditMenu(QMainWindow* w)
{
    QMenu *menu = new QMenu("&Edit", w);
    w->menuBar()->addMenu(menu);

    menu->addAction("&Toggle view", w, SLOT(toggle_view()), QKeySequence(Qt::CTRL + Qt::Key_U));
    menu->addAction("&Copy full path file", w, SLOT(copy_full_path_file()), QKeySequence(Qt::CTRL + Qt::Key_M, Qt::CTRL + Qt::Key_C, Qt::CTRL + Qt::Key_F));
}








tab_page::tab_page(QWidget* w, QString _file_name, bool open_editing)
    : QWidget(w), file_info(_file_name)
{
    //  si el fichero no existe, preguntar si queremos crearlo
    if (file_info.exists() == false  ||  file_info.isFile() == false)
    {
        log ("file not exists, pending...");
        //return;
    }

    log ("file_info.fileName()  " +  file_info.baseName());
    QFile file(file_info.absoluteFilePath());
    bool opened = true;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        opened = false;

    adoc_editor = new asciidoc_editor(this);
    //connect(adoc_editor, SIGNAL(request_open_link(QString)), this, SLOT(link_clicked_asciidoc_editor_include_file(QString)));

    web_browser = new QWebView(this);
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(adoc_editor);
    splitter->addWidget(web_browser);

    splitter->setSizes(QList<int>() << this->size().width()/1 << this->size().width()/1);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(splitter);


    if (opened)
    {
        QTextStream in(&file);
        adoc_editor->clear();
        adoc_editor->setPlainText(in.readAll());
    }
    else
    {
        if (save()==false)
        {
            QMessageBox mb;
            mb.setText("I Could not create file");
            mb.setInformativeText("Beleave me, I try to create the file on " + file_info.absoluteFilePath() + " but has been imposible.\n"
                                  "Are you sure you know what you are doing?");
            mb.setIcon(QMessageBox::Critical);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.exec();
            log("error opening file...  " + file_info.absoluteFilePath());
            //return;
        }
    }


    web_browser->load(QUrl(get_html_output()));
    web_browser->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(web_browser, SIGNAL(linkClicked(QUrl)), this, SLOT(link_clicked(QUrl)));

    connect(&proc_compile, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(compilation_finished(int,QProcess::ExitStatus)));
    connect(&proc_compile, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilation_error(QProcess::ProcessError)));
    connect(&proc_compile, SIGNAL(readyReadStandardOutput()), this, SLOT(compilation_ready_read()));
    connect(&proc_compile, SIGNAL(readyReadStandardError()), this, SLOT(compilation_ready_read_standard_error()));

    connect(&proc_run, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(run_finished(int,QProcess::ExitStatus)));
    connect(&proc_run, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilation_error(QProcess::ProcessError)));
    connect(&proc_run, SIGNAL(readyReadStandardOutput()), this, SLOT(compilation_ready_read()));
    connect(&proc_run, SIGNAL(readyReadStandardError()), this, SLOT(compilation_ready_read_standard_error()));

    if (open_editing == false)
    {
        //adoc_editor->setMaximumWidth(0);
        adoc_editor->setVisible(false);
        //adoc_editor->setFocusPolicy(Qt::NoFocus);
        //web_browser->setMaximumWidth(QWIDGETSIZE_MAX);
        web_browser->setFocus();        //  it doesn't work??
        compile_page();
    }
}



bool tab_page::save ()
{
    QFile file(file_info.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        log("error opening file to write...  " + file_info.absoluteFilePath());
        return false;
    }
    else
    {
        QTextStream out(&file);
        out << adoc_editor->toPlainText();
        log ("saved document " + file_info.absoluteFilePath());

        generate_preview();

        return true;
    }
}

bool tab_page::generate_preview()
{
    log ("generating preview " + file_info.absoluteFilePath());
    compile_page();
    return true;
}


bool tab_page::compile_page()
{
    log("compile_page file... " + file_info.absoluteFilePath());
    QString command = "asciidoc -o \""+ get_html_output() +"\"";
    command += " \"" + file_info.absoluteFilePath() + "\"";

    log("executing ...  "+ command);
    proc_compile.start(command);

    return true;
}

QString tab_page::get_html_output(void)
{
    return "/tmp/adoc/" + file_info.baseName() + ".html";
}

bool  tab_page::toggle_view (void)
{
    //if (web_browser->maximumWidth() == 0)
    if (web_browser->isVisible() == false)
    {
        //adoc_editor->setMaximumWidth(0);
        //adoc_editor->setFocusPolicy(Qt::NoFocus);
        //web_browser->setMaximumWidth(QWIDGETSIZE_MAX);
        //web_browser->setFocus();
        adoc_editor->setVisible(false);
        web_browser->setVisible(true);
        web_browser->setFocus();
    }
    //else if (adoc_editor->maximumWidth() == 0)
    else if (adoc_editor->isVisible() == false)
    {
        //adoc_editor->setMaximumWidth(QWIDGETSIZE_MAX);
        //adoc_editor->setFocusPolicy(Qt::StrongFocus);
        //web_browser->setMaximumWidth(QWIDGETSIZE_MAX);
        //adoc_editor->setFocus();
        adoc_editor->setVisible(true);
        web_browser->setVisible(true);
        adoc_editor->setFocus();
    }
    else
    {
        //adoc_editor->setMaximumWidth(QWIDGETSIZE_MAX);
        //adoc_editor->setFocusPolicy(Qt::StrongFocus);
        //web_browser->setMaximumWidth(0);
        //adoc_editor->setFocus();
        adoc_editor->setVisible(true);
        web_browser->setVisible(false);
        adoc_editor->setFocus();
    }

    return true;
}

bool tab_page::copy_full_path_file(void)
{
    log("copying file... " + file_info.absoluteFilePath());
    QApplication::clipboard()->setText(file_info.absoluteFilePath());
    return true;
}


/*
void tab_page::link_clicked_asciidoc_editor_include_file    (const QString& file_name)
{
    QFileInfo newfile;
    newfile.setFile(file_info.absoluteDir(), file_name);
    emit request_open_link(newfile.absoluteFilePath());
}
*/


void tab_page::compilation_finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
    if (web_browser->url().toString() == "")
        web_browser->load(QUrl(get_html_output()));
    else
        web_browser->reload();
    log (web_browser->url().toString());
    log("FINISH html compilation, rendering preview");
}

void tab_page::run_finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
    save(); //  necessary to remove modification optimization on asciidoc to generate again the page
}


void tab_page::compilation_error   (QProcess::ProcessError /*error*/)
{
    log ("Error processing command");
}

void tab_page::compilation_ready_read   (void)
{
    log ("ready read standard input");
    QByteArray result = proc_compile.readAllStandardOutput();
    log (QString(result));
}

void tab_page::compilation_ready_read_standard_error   (void)
{
    log ("ready read standard input");
    QByteArray result = proc_compile.readAllStandardError();
    log (QString(result));
}

void tab_page::open_link_from_edit(const QString & surl)
{
    log ("clicked... " + surl);

    QString full_path = get_full_file_path_from_link(surl, file_info.absoluteDir().absolutePath());

    log ("try open file... " + full_path);
    emit new_tab_requested(full_path, true);
}

void tab_page::link_clicked(const QUrl & url)
{
    QString surl = url.toString();
    log ("clicked... " + surl);

    QString full_path;



    //  type of link
    if (surl.indexOf("eadoc:") == 0)
    {
        QString full_path = get_full_file_path_from_link(surl.mid(QString("eadoc:").size()), file_info.absoluteDir().absolutePath());

        //if (surl.indexOf("eadoc:/") == 0)
        //    full_path = surl.mid(QString("eadoc:").size());
        //else
        //    full_path = file_info.absoluteDir().absolutePath() + "/" +  surl.mid(QString("eadoc:").size());
        log ("try open file... " + full_path);
        emit new_tab_requested(full_path, true);
    }
    else if (surl.indexOf("adoc:") == 0)
    {
        QString full_path = get_full_file_path_from_link(surl.mid(QString("adoc:").size()), file_info.absoluteDir().absolutePath());
        //if (surl.indexOf("adoc:/") == 0)
        //    full_path = surl.mid(QString("adoc:").size());
        //else
        //    full_path = file_info.absoluteDir().absolutePath() + "/" + surl.mid(QString("adoc:").size());
        log ("try open file... " + full_path);
        emit new_tab_requested(full_path, false);
    }
    else if (surl.indexOf("run:") == 0)
    {
        //log(get_full_file_path_from_link(surl.mid(QString("run:").size()), file_info.absoluteDir().absolutePath()));
        //proc_compile.start(get_full_file_path_from_link(surl.mid(QString("run:").size()), file_info.absoluteDir().absolutePath()));
        QString command = QUrl(surl.mid(QString("run:").size())).toString();
        log("run clicked: " +command);
        proc_run.start(command);
        //QUrl t;
        //t.setPath("hola pajarito sín cola");
        //log(QString(t.toEncoded()));
    }
    else
    {
        log("not registered link");
        web_browser->load(url);
    }
}







tab_page*  get_current_tab_page (QTabWidget* tab)
{
    return dynamic_cast<tab_page*>(tab->currentWidget());
}











asciidoc_editor::asciidoc_editor(QWidget* w) :
              QPlainTextEdit(w)
              //QTextEdit(w)
            , highlighter(new Highlighter(this->document()))
{
    this->setFont(QFont ("Monospace", 9));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(on_cursor_position_changed()) );
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
}

void asciidoc_editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::blue).lighter(185);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        //textCursor().select(QTextCursor::BlockUnderCursor);
        selection.cursor = textCursor();
        //selection.cursor.select(QTextCursor::BlockUnderCursor);
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


void asciidoc_editor::on_cursor_position_changed(void)
{

    status_bar::instance()->set_cursor_position("(" + QString::number(this->textCursor().blockNumber()) + "," + QString::number(this->textCursor().columnNumber())+")");
}




void asciidoc_editor::check_click_on_link(void)
{
    QTextCursor text_cursor = textCursor();
    text_cursor.select(QTextCursor::BlockUnderCursor);
    log("mouse clicked with control and line  " + text_cursor.selectedText());
    //  check if clicked on link

    {
        QString line_clicked = text_cursor.selectedText();
        QRegExp re_link("include::(.*)\\ *\\[\\]\\ *");
        log("QString::number(re_link.indexIn(line_clicked))  " + QString::number(re_link.indexIn(line_clicked)));
        log ("111111111111111");
        if (re_link.indexIn(line_clicked)>=0)
        {
            log ("located1 and emiting signal... " + re_link.cap(1));
            //QString full_path = get_full_file_path_from_link(re_link.cap(1));
            //log("full path... " + full_path);
            emit request_open_link(re_link.cap(1), true);
            log ("22222222222222");
        }
    }
    {   //link:eadoc:qadoceditor.adoc[qAdocEditor]
        QString line_clicked = text_cursor.selectedText();
        QRegExp re_link("link:(eadoc|adoc):([^\\[\\n\\r]+)");
        if (re_link.indexIn(line_clicked)>=0)
        {
            log("clicked on <e>adoc link ...     " + re_link.cap(2));
            //QString full_path = get_full_file_path_from_link(re_link.cap(2));
            //log("full path... " + full_path);
            emit request_open_link(re_link.cap(2), true);
        }
    }
}

void asciidoc_editor::mousePressEvent ( QMouseEvent * e )
{
    QPlainTextEdit::mouseMoveEvent(e);
    //QTextEdit::mouseMoveEvent(e);

    if (e->modifiers()  &  Qt::ControlModifier)
    {
        check_click_on_link();
    }
}

void asciidoc_editor::keyPressEvent ( QKeyEvent * event )
{
/*
    if (this->maximumWidth()==0)
    {
        log ("canceling pressing keys on non visible component");
        return;
    }
*/

    if ((event->key() == Qt::Key_Return  ||  event->key() == Qt::Key_Enter)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        check_click_on_link();
        return;
    }

    if ((event->key() == Qt::Key_Tab)  &&  (event->modifiers()  &  Qt::ControlModifier) )
        return;

    if ((event->key() == Qt::Key_Tab)  &&  (event->modifiers()  &  Qt::ControlModifier  & Qt::ShiftModifier) )
        return;

    if ((event->key() == Qt::Key_Tab)  )
    {
        insertPlainText("    ");
        return;
    }

    //  emacs movement keys...
    if ((event->key() == Qt::Key_P)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::PreviousBlock);
        //cursor.movePosition(QTextCursor::PreviousRow);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_N)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::NextBlock);
        //cursor.movePosition(QTextCursor::NextRow);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_P)  &&  (event->modifiers()  &  Qt::AltModifier) )
    {
        QTextCursor cursor = this->textCursor();
        for (int i=0; i<20; ++i)
            cursor.movePosition(QTextCursor::PreviousBlock);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_N)  &&  (event->modifiers()  &  Qt::AltModifier) )
    {
        QTextCursor cursor = this->textCursor();
        for (int i=0; i<20; ++i)
            cursor.movePosition(QTextCursor::NextBlock);
        this->setTextCursor(cursor);
        return;
    }

    if ((event->key() == Qt::Key_A)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::StartOfBlock);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_E)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::EndOfBlock);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_F)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::NextCharacter);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_B)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::PreviousCharacter);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_F)  &&  (event->modifiers()  &  Qt::AltModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::NextWord);
        this->setTextCursor(cursor);
        return;
    }
    if ((event->key() == Qt::Key_B)  &&  (event->modifiers()  &  Qt::AltModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::PreviousWord);
        this->setTextCursor(cursor);
        return;
    }

    if ((event->key() == Qt::Key_D)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        QTextCursor cursor = this->textCursor();
        cursor.deleteChar();
        return;
    }
    if ((event->key() == Qt::Key_K)  &&  (event->modifiers()  &  Qt::ControlModifier) )
    {
        this->textCursor().deleteChar();
        QTextCursor cursor = this->textCursor();
        while (this->textCursor().atBlockEnd() == false)
        {
            this->textCursor().deleteChar();
        }
        return;
    }


    QPlainTextEdit::keyPressEvent(event);
}




UrlTab::UrlTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("String path", this) );
    string_path = new QLineEdit(this);
    connect(string_path, SIGNAL(textChanged(QString)), this, SLOT(string_path_changed(QString)));
    layout->addWidget(string_path);

    layout->addWidget(new QLabel("Codded path", this) );
    codded_path = new QLineEdit(this);
    connect(codded_path, SIGNAL(textChanged(QString)), this, SLOT(codded_path_changed(QString)));
    layout->addWidget(codded_path);


    layout->addWidget(new QLabel("String path", this) );
    result_path = new QLineEdit(this);
    result_path->setReadOnly(true);
    layout->addWidget(result_path);
    layout->addStretch();
    setLayout(layout);
}



void UrlTab::string_path_changed(QString path)
{
    codded_path->setText(QUrl(path).encodedPath());
}

void UrlTab::codded_path_changed(QString codded_path)
{
    result_path->setText(QUrl(codded_path).toString());
}





//------------------------------------------------------------------------------------------
//      MainWindow
//------------------------------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // create /tmp/adoc/ folder
    if (QDir("/tmp/adoc").exists() == false)
    {
        log ("creating temp folder");
        QDir().mkdir("/tmp/adoc/");
    }
    //  delete files on open
    QDir temp_dir("/tmp/adoc/");
    for (unsigned i=2; i<temp_dir.count(); ++i)      //  2 . and .. cannot be deleted
    {
        QString file2delete = temp_dir[i];
        log ("deleting... " + file2delete);
        temp_dir.remove(file2delete);
    }

    setupFileMenu(this);
    setupEditMenu(this);
    setupCompileMenu(this);

    status_bar::init(statusBar());


    tab = new QTabWidget(this);
    tab->setMovable(true);
    tab->setTabsClosable(true);

    connect(tab, SIGNAL(currentChanged(int)), SLOT(tab_changed()));
    connect(tab, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));

    log_text  = new QPlainTextEdit(this);
    log_text->setFocusPolicy(Qt::ClickFocus);


    //  down tab
    QTabWidget*  down_tab = new QTabWidget(this);
    {
        down_tab->addTab(log_text, "log");
        down_tab->addTab(new UrlTab(this), "url");
    }


    QSplitter* main_splitter = new QSplitter(Qt::Vertical);
    main_splitter->addWidget(tab);
    main_splitter->addWidget(down_tab);

    setCentralWidget(main_splitter);

    new_tab("adoc/index.adoc", false);


    connect(&proc_compile, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(compilation_finished(int,QProcess::ExitStatus)));
    connect(&proc_compile, SIGNAL(error(QProcess::ProcessError)), this, SLOT(compilation_error(QProcess::ProcessError)));
    connect(&proc_compile, SIGNAL(readyReadStandardOutput()), this, SLOT(compilation_ready_read()));
    connect(&proc_compile, SIGNAL(readyReadStandardError()), this, SLOT(compilation_ready_read_standard_error()));


    QAction* act_next_tab = new QAction(this);
    act_next_tab->setShortcut(QKeySequence::NextChild);
    this->addAction(act_next_tab);

    QAction* act_prev_tab = new QAction(this);
    act_prev_tab->setShortcut(QKeySequence::PreviousChild);
    this->addAction(act_prev_tab);

    connect(act_next_tab, SIGNAL(triggered()), this, SLOT(next_tab()));
    connect(act_prev_tab, SIGNAL(triggered()), this, SLOT(previus_tab()));

}

MainWindow::~MainWindow()
{
    log_text = 0;
    //  try to save all pages
    for (int i=0; i<tab->count(); ++i)
    {
        tab->setCurrentIndex(i);
        saveTab();
    }
}


void MainWindow::new_tab(QString file_name, bool open_editing)
{
    if (file_name == "")
        file_name = QFileDialog::getOpenFileName(this,
            "Open File", "", "asciidoc (*.adoc) ;; all (*)");


    //  if its already open, just activate it
    {
        for (int i=0; i< tab->count(); ++i)
        {
            QString tab_full_file_path = dynamic_cast<tab_page*>(tab->widget(i))->file_info.absoluteFilePath();
            if (file_name == tab_full_file_path)
            {
                log ("detected tab with this file, activating...");
                tab->setCurrentIndex(i);
                return;
            }
        }
        dynamic_cast<tab_page*>(tab->currentWidget());
    }

    if (!file_name.isEmpty()) {
        tab_page* ntp = new tab_page(tab, file_name, open_editing);
        tab->insertTab(tab->currentIndex()+1, ntp, QFileInfo(file_name).baseName());
        connect(ntp->adoc_editor, SIGNAL(request_open_link(QString, bool)), ntp, SLOT(open_link_from_edit(const QString)));
        connect(ntp, SIGNAL(new_tab_requested(QString,bool)), this, SLOT(new_tab_requested(QString,bool)));
        tab->setCurrentIndex(tab->currentIndex()+1);
        if (open_editing)
            ntp->adoc_editor->setFocus();
        else
            ntp->web_browser->setFocus();
    }
}

void MainWindow::new_tab_requested(const QString& file_name, bool edit_view)
{
    new_tab(file_name, edit_view);
}


void MainWindow::next_tab(void)
{
    if (tab->currentIndex() < tab->count())
        tab->setCurrentIndex(tab->currentIndex()+1);
}

void MainWindow::previus_tab(void)
{
    if (tab->currentIndex() >0)
        tab->setCurrentIndex(tab->currentIndex()-1);

}






void MainWindow::openNewTabFromFile()
{
    new_tab("", true);
}

bool MainWindow::saveTab(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
        return tp->save();
    else
    {
        log("error saving file... invalid dynamic_cast");
        return false;
    }
}

bool MainWindow::closeTab(int tab2close)
{
    int previus_tab = tab->currentIndex();
    if (tab2close != -1)
    {
        tab->setCurrentIndex(tab2close);
    }

    if (saveTab() == false)
    {
        QMessageBox msgBox;
        msgBox.setText("I couldn't save the document. If you close it, the modifications will be lost.\n"
                       "Are you sure to close anyway (under your entirely responsability, don't cry after it)?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort  | QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDefaultButton(QMessageBox::No);
        if (msgBox.exec() != QMessageBox::Yes)
        {
           log("canceled close tab due to errors on savetab");
           return false;
        }
        else
           log("I warned you. The tab is closing without save it (don't cry).");
    }

    {
        tab_page* tp = get_current_tab_page(tab);
        if (tp != 0)
        {
            log("deleting tab... " + tp->file_info.absoluteFilePath());
            delete tp;
        }
        else
        {
            log("error saving file... invalid dynamic_cast");
            return false;
        }
    }

    {
        //  set focus to new active tab
        tab_page* tp = get_current_tab_page(tab);
        if (tp != 0)
        {
            log("seting focus... " + tp->file_info.absoluteFilePath());
            tp->adoc_editor->setFocus();;
        }
        else
        {
            log("error seting focus file... invalid dynamic_cast");
            return false;
        }
    }

    if (tab2close != -1)
    {
        if (previus_tab >= tab2close)
            --previus_tab;
        tab->setCurrentIndex(previus_tab);
    }

    return true;
}


void MainWindow::tab_changed(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
        status_bar::instance()->set_file_path(tp->file_info.absoluteFilePath());
    else
        log ("event  tab_changed invalid cast");

}


void MainWindow::create_new_tab_from_file(const QString& file_name)
{
    new_tab(file_name, true);
}



void MainWindow::generateHtml(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
    {
        tp->generate_preview();
    }
    else
    {
        log("error generateHtml file... invalid dynamic_cast");
    }
}



void MainWindow::toggle_view(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
    {
        tp->toggle_view();
    }
    else
    {
        log("error toggle_view file... invalid dynamic_cast");
    }
}

void MainWindow::copy_full_path_file(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
    {
        tp->copy_full_path_file();
    }
    else
    {
        log("error copy_full_path_file file... invalid dynamic_cast");
    }
}





QString get_compilation_command_in_text(QString text, QString full_file_name)
{
    QString result;
    QRegExp rx("[\n\r]+// *compile:([^\n\r]+)");
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        //log (rx.cap(1) + "  ---");
        result += rx.cap(1);
        pos += rx.matchedLength();
    }
    if (result != "")
        return result + " " + full_file_name;
    else
        return "";
}

void MainWindow::build_with_options(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
    {
        QString in_source_command (get_compilation_command_in_text(tp->adoc_editor->toPlainText(), tp->file_info.absoluteFilePath()));
        dialog_compile_options dialog(tp->file_info.baseName(), tp->file_info.absoluteFilePath(), in_source_command);
        int contador = 0;
        while (build_options[contador][0] != 0)
        {
            dialog.add_option(QString(build_options[contador][0]), QString(build_options[contador][1]), build_options[contador][2]);
            ++contador;
        }
        if (dialog.exec() == QDialog::Accepted)
        {
            QString command = dialog.get_selected_command();
            log ("compiling... " + command);
            proc_compile.start(command);
            /*
            QString command = dialog.get_selected_command();
            if (command.indexOf("asciidoc")==0)
            {
                command +=    " -o __builds/" + tp->file_info.baseName() + "." + dialog.get_selected_extension()
                            + " " + tp->file_info.absoluteFilePath();
            }
            else        //  a2x
            {
                command +=    " -D __builds/ " + tp->file_info.absoluteFilePath();
            }
            log ("compiling... " + command);
            proc_compile.start(command);
            */
        }
    }
    else
    {
        log("error build_with_options file... invalid dynamic_cast");
    }
}

void MainWindow::build_with_source_command(void)
{
    tab_page* tp = get_current_tab_page(tab);
    if (tp != 0)
    {
        QString command = get_compilation_command_in_text(tp->adoc_editor->toPlainText(), tp->file_info.absoluteFilePath());
        if (command == "" )
        {
            log ("Error, there are not compile command in document");
            return;
        }

        log ("compiling with document configuration... " + command);
        proc_compile.start(command);
    }
    else
    {
        log("error build_with_options document... invalid dynamic_cast");
    }
}



void MainWindow::compilation_finished(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
    log("FINISH compilation");
}

void MainWindow::compilation_error   (QProcess::ProcessError /*error*/)
{
    log ("Error processing command");
}

void MainWindow::compilation_ready_read   (void)
{
    log ("ready read standard input");
    QByteArray result = proc_compile.readAllStandardOutput();
    log (QString(result));
}

void MainWindow::compilation_ready_read_standard_error   (void)
{
    log ("ready read standard input");
    QByteArray result = proc_compile.readAllStandardError();
    log (QString(result));
}

/*
void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    log ("key press---- "  + QString::number(event->key()));
    QMainWindow::keyPressEvent(event);
}
*/


