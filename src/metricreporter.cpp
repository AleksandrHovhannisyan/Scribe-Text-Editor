#include "metricreporter.h"


MetricReporter::MetricReporter(QWidget *parent) : QFrame(parent)
{
    // Note: since all of these get added as widgets, they will be
    // deallocated automatically by the QSplitter destructor
    wordLabel = new QLabel("Words: ");
    wordCountLabel = new QLabel();
    charLabel = new QLabel("Chars: ");
    charCountLabel = new QLabel();
    lineLabel = new QLabel("Line: ");
    lineCountLabel = new QLabel();
    columnLabel = new QLabel("Column: ");
    columnCountLabel = new QLabel();

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(wordLabel);
    layout->addWidget(wordCountLabel);
    layout->addWidget(charLabel);
    layout->addWidget(charCountLabel);
    layout->addWidget(lineLabel);
    layout->addWidget(lineCountLabel);
    layout->addWidget(columnLabel);
    layout->addWidget(columnCountLabel);
    setLayout(layout);
}


/* Updates the word count using the associated label.
 */
void MetricReporter::updateWordCount(int wordCount)
{
    wordCountLabel->setText(QString::number(wordCount));
}


/* Updates the char count using the associated label.
 */
void MetricReporter::updateCharCount(int charCount)
{
    charCountLabel->setText(QString::number(charCount));
}


/* Updates the line count using the associated label.
 */
void MetricReporter::updateLineCount(int current, int total)
{
    lineCountLabel->setText(QString::number(current) + tr("/") + QString::number(total));
}


/* Updates the column count using the associated label.
 */
void MetricReporter::updateColumnCount(int columnCount)
{
    columnCountLabel->setText(QString::number(columnCount));
}
