#include "finddialog.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QtDebug> // TODO remove

/*
 *
 */
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    // Initialize all members
    findLabel = new QLabel(tr("Find what:"));
    lineEdit = new QLineEdit();
    findButton = new QPushButton(tr("&Find"));
    findNextButton = new QPushButton(tr("&Find next"));
    caseSensitiveCheckBox = new QCheckBox(tr("&Case sensitive"));
    wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
    queryText = "";

    // Ensures that the line edit gets the focus whenever the dialog is the active window
    setFocusProxy(lineEdit);

    // Set up all the  widgets and layouts
    horizontalLayout = new QHBoxLayout();
    optionsLayout = new QHBoxLayout();
    verticalLayout = new QVBoxLayout();

    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addLayout(optionsLayout);

    horizontalLayout->addWidget(findLabel);
    horizontalLayout->addWidget(lineEdit);
    horizontalLayout->addWidget(findButton);
    horizontalLayout->addWidget(findNextButton);

    optionsLayout->setSizeConstraint(QLayout::SetMaximumSize);
    optionsLayout->addWidget(caseSensitiveCheckBox);
    optionsLayout->addWidget(wholeWordsCheckBox);

    setLayout(verticalLayout);
    setWindowTitle(tr("Find"));

    connect(findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
    connect(findNextButton, SIGNAL(clicked()), this, SLOT(on_findNextButton_clicked()));
}


/* Performs all required memory cleanup operations.
 */
FindDialog::~FindDialog()
{
    delete findLabel;
    delete lineEdit;
    delete findButton;
    delete findNextButton;
    delete caseSensitiveCheckBox;
    delete wholeWordsCheckBox;
    delete horizontalLayout;
    delete verticalLayout;
    delete optionsLayout;
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
