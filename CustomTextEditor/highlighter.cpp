#include "highlighter.h"


/* Constructs a Highlighter object using the given list of keyword patterns and
 * a parent QTextDocument widget (nullptr by default).
 */
Highlighter::Highlighter(QStringList keywordPatterns, QTextDocument *parent) : QSyntaxHighlighter (parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    // Create a HighlightingRule for each pattern and store it in the internal vector
    foreach(const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        rules.append(rule);
    }

    // Class highlighting rule
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    rules.append(rule);

    // Quotation (e.g., chars and strings) highlighting rule
    quotationFormat.setForeground(Qt::darkGreen);

    // TODO fix escape char regex, e.g. for '\n'
    rule.pattern = QRegularExpression("(\".*\")|('\\.{1}')|('.{0,1}')");       // TODO python uses single quotes for strings too, so pass as param
    rule.format = quotationFormat;
    rules.append(rule);

    // Functions
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    rules.append(rule);

    // TODO Python uses # for single-line and '''...''' / """...""" for multi-line, so we need to pass these in as params

    // Single-line comments
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    rules.append(rule);

    // Multi-line comments
    multilineCommentFormat.setForeground(Qt::darkGreen);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}


/* Called whenever blocks of text change within the document. Used to apply custom
 * formatting/syntax highlighting to the given text.
 * @param text - the text to be parsed for pattern matches
 */
void Highlighter::highlightBlock(const QString &text)
{
    // Try to find matches for all rules (except comments) and apply their formatting
    foreach(const HighlightingRule &rule, rules)
    {
        QRegularExpressionMatchIterator iterator = rule.pattern.globalMatch(text);

        while(iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // The rest of the code is for dealing with comments

    setCurrentBlockState(0);
    int startIndex = 0;

    if(previousBlockState() != 1)
    {
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0)
    {
           QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
           int endIndex = match.capturedStart();
           int commentLength = 0;

           if (endIndex == -1)
           {
               setCurrentBlockState(1);
               commentLength = text.length() - startIndex;
           }
           else
           {
               commentLength = endIndex - startIndex + match.capturedLength();
           }

           setFormat(startIndex, commentLength, multilineCommentFormat);
           startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
       }

}
