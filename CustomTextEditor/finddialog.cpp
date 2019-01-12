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
    findLabel = new QLabel(tr("Find what:    "));
    replaceLabel = new QLabel(tr("Replace with:"));
    findLineEdit = new QLineEdit();
    replaceLineEdit = new QLineEdit();
    findNextButton = new QPushButton(tr("&Find next"));
    replaceButton = new QPushButton(tr("&Replace"));
    replaceAllButton = new QPushButton(tr("&Replace all"));
    caseSensitiveCheckBox = new QCheckBox(tr("&Match case"));
    wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
    queryText = "";

    // Ensures that the line edit gets the focus whenever the dialog is the active window
    setFocusProxy(findLineEdit);

    // Set up all the  widgets and layouts
    findHorizontalLayout = new QHBoxLayout();
    replaceHorizontalLayout = new QHBoxLayout();
    optionsLayout = new QHBoxLayout();
    verticalLayout = new QVBoxLayout();

    verticalLayout->addLayout(findHorizontalLayout);
    verticalLayout->addLayout(replaceHorizontalLayout);
    verticalLayout->addLayout(optionsLayout);

    findHorizontalLayout->addWidget(findLabel);
    findHorizontalLayout->addWidget(findLineEdit);
    replaceHorizontalLayout->addWidget(replaceLabel);
    replaceHorizontalLayout->addWidget(replaceLineEdit);

    optionsLayout->addWidget(caseSensitiveCheckBox);
    optionsLayout->addWidget(wholeWordsCheckBox);
    optionsLayout->addWidget(findNextButton);
    optionsLayout->addWidget(replaceButton);
    optionsLayout->addWidget(replaceAllButton);

    setLayout(verticalLayout);
    setWindowTitle(tr("Find and Replace"));

    connect(findNextButton, SIGNAL(clicked()), this, SLOT(on_findNextButton_clicked()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(on_replaceButton_clicked()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(on_replaceAllButton_clicked()));
}


/* Performs all required memory cleanup operations.
 */
FindDialog::~FindDialog()
{
    delete findLabel;
    delete replaceLabel;
    delete findLineEdit;
    delete replaceLineEdit;
    delete findNextButton;
    delete replaceButton;
    delete replaceAllButton;
    delete caseSensitiveCheckBox;
    delete wholeWordsCheckBox;
    delete findHorizontalLayout;
    delete replaceHorizontalLayout;
    delete verticalLayout;
    delete optionsLayout;
}


/* Called when the user clicks the Find Next button. If the query is empty, it informs the user.
 * Otherwise, it emits an appropriate signal for startFinding with all relevant search criteria.
 */
void FindDialog::on_findNextButton_clicked()
{
    QString query = findLineEdit->text();

    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }
    queryText = query;

    bool caseSensitive = caseSensitiveCheckBox->isChecked();
    bool wholeWords = wholeWordsCheckBox->isChecked();
    emit(startFinding(queryText, caseSensitive, wholeWords, "No results found."));
}


/* TODO document
 */
void FindDialog::on_replaceButton_clicked()
{
    QString query = findLineEdit->text();

    if(query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }
    queryText = query;

    QString replacementText = replaceLineEdit->text();
    bool caseSensitive = caseSensitiveCheckBox->isChecked();
    bool wholeWords = wholeWordsCheckBox->isChecked();

    emit(startReplacing(query, replacementText, caseSensitive, wholeWords));
}


/* TODO document
 */
void FindDialog::on_replaceAllButton_clicked()
{

}


/* TODO document
 */
void FindDialog::addToSearchHistory(QString term, int positionPriorToSearch, int firstMatchPosition)
{
    if(!previouslyFound(term))
    {
        searchHistory.clear();
    }
    QPair<int,int> locations;
    locations.first = positionPriorToSearch;
    locations.second = firstMatchPosition;
    searchHistory.insert(term, locations);
}
