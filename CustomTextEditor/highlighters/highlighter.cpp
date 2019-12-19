#include "highlighter.h"
#include <QtDebug>


Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    quoteFormat.setForeground(Qt::darkGreen);
    inlineCommentFormat.setForeground(Qt::darkGreen);
    blockCommentFormat.setForeground(Qt::darkGreen);
}


/* Adds all keywords specified by the argument to this Highlighter's rules.
 */
void Highlighter::addKeywords(QStringList keywords)
{
    foreach(const QString &keyword, keywords)
    {
        addRule(QRegularExpression(keyword), keywordFormat);
    }
}


/* Creates a HighlightingRule based on the given regex pattern and its
 * corresponding format and adds it to the internal vector of rules.
 */
void Highlighter::addRule(QRegularExpression pattern, QTextCharFormat format)
{
    HighlightingRule rule;
    rule.pattern = pattern;
    rule.format = format;
    rules.append(rule);
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

        while (iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(BlockState::NotInComment);
    highlightMultilineComments(text);
}


/* Formats multiline comments per this Highlighter's
 * blockCommentStart and blockCommentEnd patterns.
 */
void Highlighter::highlightMultilineComments(const QString &text)
{
    int startIndex = 0;

    // If the previous state was not in comment, then start searching from very beginning
    if (previousBlockState() != BlockState::InComment)
    {
        startIndex = text.indexOf(blockCommentStart);
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = blockCommentEnd.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        // If we have not yet found the terminating pattern, we are still in comment
        if (endIndex == -1)
        {
           setCurrentBlockState(BlockState::InComment);
           commentLength = text.length() - startIndex;
        }
        else
        {
           commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(startIndex, commentLength, blockCommentFormat);
        startIndex = text.indexOf(blockCommentStart, startIndex + commentLength);
   }
}


