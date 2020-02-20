#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>

class FindDialog : public QDialog
{
    Q_OBJECT

public:

    FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

signals:

    void startFinding(QString queryText, bool caseSensitive, bool wholeWords);
    void startReplacing(QString what, QString with, bool caseSensitive, bool wholeWords);
    void startReplacingAll(QString what, QString with, bool caseSensitive, bool wholeWords);

public slots:

    void on_findNextButton_clicked();
    void on_replaceOperation_initiated();
    void onFindResultReady(QString message) { QMessageBox::information(this, "Find and Replace", message); }

private:

    void initializeWidgets();
    void initializeLayout();

    QLabel *findLabel;
    QLabel *replaceLabel;
    QPushButton *findNextButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QLineEdit *findLineEdit;
    QLineEdit *replaceLineEdit;
    QCheckBox *caseSensitiveCheckBox;
    QCheckBox *wholeWordsCheckBox;

    QHBoxLayout *findHorizontalLayout;
    QHBoxLayout *replaceHorizontalLayout;
    QHBoxLayout *optionsLayout;
    QVBoxLayout *verticalLayout;
};

#endif // FINDDIALOG_H
