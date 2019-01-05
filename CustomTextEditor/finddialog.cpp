#include "finddialog.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>


/* Initializes this FindDialog object.
 */
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    // Initialize all members
    findLabel = new QLabel(tr("Find what:"));
    lineEdit = new QLineEdit();
    findNextButton = new QPushButton(tr("&Find next"));
    caseSensitiveCheckBox = new QCheckBox(tr("&Match case"));
    wholeWordsCheckBox = new QCheckBox(tr("&Find whole words only"));
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

    optionsLayout->addWidget(caseSensitiveCheckBox);
    optionsLayout->addWidget(wholeWordsCheckBox);
    optionsLayout->addWidget(findNextButton);

    setLayout(verticalLayout);
    setWindowTitle(tr("Find"));

    connect(findNextButton, SIGNAL(clicked()), this, SLOT(on_findNextButton_clicked()));
}


/* Performs all required memory cleanup operations.
 */
FindDialog::~FindDialog()
{
    delete findLabel;
    delete lineEdit;
    delete findNextButton;
    delete caseSensitiveCheckBox;
    delete wholeWordsCheckBox;
    delete horizontalLayout;
    delete verticalLayout;
    delete optionsLayout;
}


/* Called when the user clicks the Find Next button. If the query is empty, it informs the user.
 * Otherwise, it emits an appropriate signal for queryTextReady with all relevant search criteria.
 */
void FindDialog::on_findNextButton_clicked()
{
    QString query = lineEdit->text();
    bool caseSensitive = caseSensitiveCheckBox->isChecked();
    bool wholeWords = wholeWordsCheckBox->isChecked();
    bool findNext = false;

    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }
    else if(query == queryText)
    {
        findNext = true;
    }
    queryText = query;

    emit(queryTextReady(queryText, findNext, caseSensitive, wholeWords));
}
