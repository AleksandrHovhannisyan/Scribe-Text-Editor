#include "deviceinputdialog.h"
#include "ui_deviceinputdialog.h"

DeviceInputDialog::DeviceInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceInputDialog)
{
    ui->setupUi(this);
}

DeviceInputDialog::~DeviceInputDialog()
{
    delete ui;
}

QString DeviceInputDialog::getText()
{
    DeviceInputDialog dialog;
    dialog.exec();

    return dialog.ui->lineEdit->text();
}
