#ifndef GOTODIALOG_H
#define GOTODIALOG_H
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

class GotoDialog : public QDialog
{
    Q_OBJECT

public:
    GotoDialog(QWidget *parent = nullptr);
    ~GotoDialog();

public slots:
    void onGotoResultReady(QString message) { QMessageBox::information(this, "Go To", message); }

private:
    QHBoxLayout *layout;
    QLabel *gotoLabel;
    QLineEdit *gotoLineEdit;
    QPushButton *gotoButton;

private slots:
    void on_gotoButton_clicked();

signals:
    void gotoLine(int);
};

#endif // GOTODIALOG_H
