#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMap>         // for search history
#include <QPair>        // for search history

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
    inline QString getQueryText(){ return queryText; }

signals:
    void startFinding(QString queryText, bool caseSensitive, bool wholeWords);
    void startReplacing(QString what, QString with, bool caseSensitive, bool wholeWords);
    void startReplacingAll(QString what, QString with, bool caseSensitive, bool wholeWords);

public slots:
    void on_findNextButton_clicked();
    void on_replaceOperation_initiated();

private:
    QLabel *findLabel;
    QLabel *replaceLabel;
    QPushButton *findNextButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
    QCheckBox *caseSensitiveCheckBox;
    QCheckBox *wholeWordsCheckBox;
    QString queryText;
    QHBoxLayout *findHorizontalLayout;
    QHBoxLayout *replaceHorizontalLayout;
    QHBoxLayout *optionsLayout;
    QVBoxLayout *verticalLayout;
};

#endif // FINDDIALOG_H
