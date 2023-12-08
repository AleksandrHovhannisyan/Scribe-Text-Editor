#ifndef DEVICEINPUTDIALOG_H
#define DEVICEINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class DeviceInputDialog;
}

class DeviceInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceInputDialog(QWidget *parent = nullptr);
    ~DeviceInputDialog();

    static QString getText();

private:
    Ui::DeviceInputDialog *ui;
};

#endif // DEVICEINPUTDIALOG_H
