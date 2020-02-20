#include "finddialog.h"
#include <QHBoxLayout>


/* Initializes this FindDialog object.
 */
FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    initializeWidgets();

    // Ensures that the line edit gets the focus whenever the dialog is the active window
    setFocusProxy(findLineEdit);

    // Set up all the widgets and layouts
    initializeLayout();

    setWindowTitle(tr("Find and Replace"));

    connect(findNextButton, SIGNAL(clicked()), this, SLOT(on_findNextButton_clicked()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(on_replaceOperation_initiated()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(on_replaceOperation_initiated()));
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
    delete optionsLayout;
    delete verticalLayout;
}


/* Initializes all child widgets, such as the labels, checkboxes, and buttons.
 */
void FindDialog::initializeWidgets()
{
    findLabel = new QLabel(tr("Find what:    "));
    replaceLabel = new QLabel(tr("Replace with:"));
    findLineEdit = new QLineEdit();
    replaceLineEdit = new QLineEdit();
    findNextButton = new QPushButton(tr("&Find next"));
    replaceButton = new QPushButton(tr("&Replace"));
    replaceAllButton = new QPushButton(tr("&Replace all"));
    caseSensitiveCheckBox = new QCheckBox(tr("&Match case"));
    wholeWordsCheckBox = new QCheckBox(tr("&Whole words"));
}


/* Defines this FindDialog's layout and adds the widgets
 * to the appropriate child layouts.
 */
void FindDialog::initializeLayout()
{
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
}


/* Called when the user clicks the Find Next button. If the query is empty, it informs the user.
 * Otherwise, it emits an appropriate signal for startFinding with all relevant search criteria.
 */
void FindDialog::on_findNextButton_clicked()
{
    QString query = findLineEdit->text();

    if (query.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }

    bool caseSensitive = caseSensitiveCheckBox->isChecked();
    bool wholeWords = wholeWordsCheckBox->isChecked();
    emit(startFinding(query, caseSensitive, wholeWords));
}


/* Called when the user clicks the Replace or Replace All button. Emits the appropriate
 * signal (startReplacing or startReplacingAll, respectively), passing along all relevant
 * search and replace information.
 */
void FindDialog::on_replaceOperation_initiated()
{
    QString what = findLineEdit->text();

    if (what.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter a query."));
        return;
    }

    QString with = replaceLineEdit->text();
    bool caseSensitive = caseSensitiveCheckBox->isChecked();
    bool wholeWords = wholeWordsCheckBox->isChecked();
    bool replace = sender() == replaceButton;

    if (replace)
    {
        emit(startReplacing(what, with, caseSensitive, wholeWords));
    }
    else
    {
        emit(startReplacingAll(what, with, caseSensitive, wholeWords));
    }

}
