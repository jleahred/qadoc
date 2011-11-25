#include "dialog_compile_options.h"
#include "ui_dialog_compile_options.h"

dialog_compile_options::dialog_compile_options( QString _file_info_baseName, QString _file_info_absoluteFilePath,
                                                QString _source_command, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_compile_options),
    file_info_baseName(_file_info_baseName),
    file_info_absoluteFilePath(_file_info_absoluteFilePath),
    source_command(_source_command)
{
    ui->setupUi(this);
}

dialog_compile_options::~dialog_compile_options()
{
    delete ui;
}

void dialog_compile_options::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void dialog_compile_options::add_option(const QString& description, QString command, const QString& extension)
{
    if (command == "<source_command>")
    {
        ui->lwOptions->addItem(description);
        if (source_command == "")
            commands << "undefined command in source";
        else
            commands << source_command;
        extensions << extension;
    }
    else
    {
        if (command.indexOf("asciidoc")==0)
        {
            command +=    " -o __builds/" + file_info_baseName + "." + extension
                        + " " + file_info_absoluteFilePath;
        }
        else        //  a2x
        {
            command +=    " -D __builds/ " + file_info_absoluteFilePath;
        }

        ui->lwOptions->addItem(description);
        commands << command;
        extensions << extension;
    }
}

void dialog_compile_options::on_lwOptions_itemChanged(QListWidgetItem* /*item*/)
{
}

void dialog_compile_options::on_lwOptions_currentRowChanged(int /*currentRow*/)
{
    QString command = commands.at(ui->lwOptions->currentRow());
    /*
    if (command.indexOf("asciidoc")==0)
    {
        command +=    " -o __builds/" + file_info_baseName + "." + get_selected_extension()
                    + " " + file_info_absoluteFilePath;
    }
    else        //  a2x
    {
        command +=    " -D __builds/ " + file_info_absoluteFilePath;
    }
    */
    ui->pteCommandLine->setPlainText(command);
}


QString dialog_compile_options::get_selected_command(void)
{
    return ui->pteCommandLine->toPlainText();
}


QString dialog_compile_options::get_selected_extension(void)
{
    return extensions.at(ui->lwOptions->currentRow());
}
