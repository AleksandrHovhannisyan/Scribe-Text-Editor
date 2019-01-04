#include "finddialog.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QtDebug> // TODO remove

/*
 *
 */
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    QLabel *findLabel = new QLabel(tr("Find what:"));
    lineEdit = new QLineEdit();

    findButton = new QPushButton(tr("&Find"));
    findNextButton = new QPushButton(tr("&Find next"));
    queryText = "";

    // TODO add a nested horizontal layout within a main vertical layout

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(findButton);
    layout->addWidget(findNextButton);

    setLayout(layout);
    setWindowTitle(tr("Find"));

    connect(findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
    connect(findNextButton, SIGNAL(clicked()), this, SLOT(on_findNextButton_clicked()));
}


/* Called when the user clicks the Find button. If the query is empty, it informs the user.
 * If the query matches the previous search, it asks the user if they would like to instead
 * conduct a Find Next. Emits the queryTextReady signal with a corresponding flag denoting
 * whether the user chose to Find or Find Next.
 */
void FindDialog::on_findButton_clicked()
{    
    QString query = lineEdit->text();

    if(queryText == query)
    {
        // TODO ask the user if they'd like to do a Find Next (Yes/No)
    }

    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }
    queryText = query;
    emit(queryTextReady(queryText, false));
}


/* Called when the user clicks the Find next button. If the query is empty, it informs the user.
 * Otherwise, it emits a signal for queryTextReady, with a flag of true to denote that the handler
 * should proceed with a Find Next operation.
 */
void FindDialog::on_findNextButton_clicked()
{
    QString query = lineEdit->text();
    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }
    queryText = query;
    emit(queryTextReady(queryText, true));
}
