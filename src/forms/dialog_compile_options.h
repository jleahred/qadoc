#ifndef DIALOG_COMPILE_OPTIONS_H
#define DIALOG_COMPILE_OPTIONS_H

#include <QDialog>

class QListWidgetItem;


namespace Ui {
    class dialog_compile_options;
}

class dialog_compile_options : public QDialog {
    Q_OBJECT
public:
    dialog_compile_options(QString _file_info_baseName, QString _file_info_absoluteFilePath, QString source_command, QWidget *parent = 0);
    ~dialog_compile_options();

    void add_option(const QString& description, QString command, const QString& extension );
    QString get_selected_command(void);
    QString get_selected_extension(void);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::dialog_compile_options *ui;

    QVector<QString>   commands;
    QVector<QString>   extensions;
    QString file_info_baseName;
    QString file_info_absoluteFilePath;
    QString source_command;

private slots:
    void on_lwOptions_currentRowChanged(int currentRow);
    void on_lwOptions_itemChanged(QListWidgetItem* item);
};

#endif // DIALOG_COMPILE_OPTIONS_H
