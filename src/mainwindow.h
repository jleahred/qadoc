#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "mqeditor.h"
#include <QPlainTextEdit>
#include <QFileInfo>
#include <QProcess>
#include <QtWebKit>


#include "highlighter.h"




class  asciidoc_editor : public MQEditor
{
    Q_OBJECT
public:

    asciidoc_editor(QWidget* w);

protected:
    void mousePressEvent ( QMouseEvent * e );
    void keyPressEvent ( QKeyEvent * event ) ;

signals:
    void request_open_link(const QString& file_name, bool open_editing);
    //void request_next_tab(void);
    //void request_previus_tab(void);

private:
    Highlighter* highlighter;

    void check_click_on_link(void);


private slots:
    void on_cursor_position_changed(void);
};



class tab_page : public QWidget
{
    Q_OBJECT
public:

    const QFileInfo     file_info;
    asciidoc_editor*    adoc_editor;
    QWebView*           web_browser;


    tab_page(QWidget* w, QString _file_name, bool open_editing);


    bool save ();
    bool generate_preview();

    bool    compile_page();
    QString get_html_output(void);

    bool  toggle_view (void);
    bool  copy_full_path_file(void);


private:
    QProcess proc_compile;
    QProcess proc_run;



private slots:
    //void link_clicked_asciidoc_editor_include_file(const QString& file_name);

    void compilation_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void run_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void compilation_error   (QProcess::ProcessError error);
    void compilation_ready_read   (void);
    void compilation_ready_read_standard_error   (void);

    void link_clicked(const QUrl & url);
    void open_link_from_edit(const QString & url);


signals:
    //void click_link_asciidoc_editor_include_file(const QString& file_name);
    void request_open_link(const QString& file_name);
    void new_tab_requested(const QString& file_name, bool edit_view);
};




class QLineEdit;
class UrlTab : public QWidget
{
    Q_OBJECT

public:
    UrlTab(QWidget *parent = 0);
    ~UrlTab(){};


    QLineEdit* string_path;
    QLineEdit* codded_path;
    QLineEdit* result_path;

private slots:
    void string_path_changed(QString);
    void codded_path_changed(QString);

};





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //void keyPressEvent ( QKeyEvent * event ) ;


private:
    QTabWidget* tab;
    void new_tab(QString file_name, bool open_editing);
    QProcess proc_compile;


public slots:
    void openNewTabFromFile(void);
    bool saveTab(void);
    bool closeTab(int tab_index=-1);        //  -1 means current tab
    void tab_changed(void);
    void generateHtml(void);
    void build_with_options(void);
    void build_with_source_command(void);

    void toggle_view(void);
    void copy_full_path_file(void);

    //void link_clicked_asciidoc_editor_include_file(const QString& file_name);
    void create_new_tab_from_file(const QString& file_name);

    void new_tab_requested(const QString& file_name, bool edit_view);
    void next_tab(void);
    void previus_tab(void);


    void compilation_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void compilation_error   (QProcess::ProcessError error);
    void compilation_ready_read   (void);
    void compilation_ready_read_standard_error   (void);


};

#endif // MAINWINDOW_H
