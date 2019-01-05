#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    inline QString getQueryText(){ return queryText; }

signals:
    void queryTextReady(QString queryText, bool findNext, bool caseSensitive, bool wholeWords);

public slots:
    void on_findNextButton_clicked();

private:
    QLabel *findLabel;
    QPushButton *findNextButton;
    QLineEdit *lineEdit;
    QCheckBox *caseSensitiveCheckBox;
    QCheckBox *wholeWordsCheckBox;
    QString queryText;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *optionsLayout;
    QVBoxLayout *verticalLayout;


};

#endif // FINDDIALOG_H
