#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    inline QString getQueryText(){ return queryText; }
    void clearLineEdit() { lineEdit->clear(); }

signals:
    void queryTextReady(QString queryText);

public slots:
    void on_findButton_clicked();

private:
    QPushButton *findButton;
    QLineEdit *lineEdit;
    QString queryText;

};

#endif // FINDDIALOG_H
