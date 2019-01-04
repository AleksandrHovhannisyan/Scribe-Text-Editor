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

signals:
    void queryTextReady(QString queryText, bool findNext);

public slots:
    void on_findButton_clicked();
    void on_findNextButton_clicked();

private:
    QPushButton *findButton;
    QPushButton *findNextButton;
    QLineEdit *lineEdit;
    QString queryText;

};

#endif // FINDDIALOG_H
