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
    queryText = "";

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(findButton);

    setLayout(layout);
    setWindowTitle(tr("Find"));

    connect(findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
}


/*
 *
 */
void FindDialog::on_findButton_clicked()
{
    QString query = lineEdit->text();

    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }

    queryText = query;

    emit(queryTextReady(queryText));
}

