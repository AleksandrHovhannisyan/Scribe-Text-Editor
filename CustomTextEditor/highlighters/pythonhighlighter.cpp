#include "pythonhighlighter.h"


PythonHighlighter::PythonHighlighter(QTextDocument *parent) : Highlighter(parent)
{
    QStringList keywords;
    keywords << "\\band\\b" << "\\bas\\b" << "\\bassert\\b" << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
             << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b" << "\\belse\\b" << "\\bexcept\\b" << "\\bFalse\\b"
             << "\\bfinally\\b" << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b" << "\\bif\\b" << "\\bimport\\b"
             << "\\bin\\b" << "\\bis\\b" << "\\blambda\\b" << "\\bNone\\b" << "\\bnonlocal\\b" << "\\bnot\\b"
             << "\\bor\\b" << "\\bpass\\b" << "\\braise\\b" << "\\breturn\\b" << "\\bTrue\\b" << "\\btry\\b"
             << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";

    addKeywords(keywords);
    addRule(QRegularExpression("\\b[A-Z_][a-zA-Z0-9_]*\\b"), classFormat);
    addRule(QRegularExpression("(\".*\")|('.*')"), quoteFormat);
    addRule(QRegularExpression("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()"), functionFormat);
    addRule(QRegularExpression("#.*"), inlineCommentFormat);

    // Unique to PythonHighlighter
    triple_single_quote.first = QRegularExpression("'''");
    triple_single_quote.second = MultilineQuote::TRIPLE_SINGLE;
    triple_double_quote.first = QRegularExpression("\"\"\"");
    triple_double_quote.second = MultilineQuote::TRIPLE_DOUBLE;
}


void PythonHighlighter::highlightBlock(const QString &text)
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

    setCurrentBlockState(BlockState::NotInComment);

    // Handle multiline comments
    if(!highlightMultilineComments(text, triple_single_quote.first, triple_single_quote.second))
    {
        highlightMultilineComments(text, triple_double_quote.first, triple_double_quote.second);
    }
}


bool PythonHighlighter::highlightMultilineComments(const QString &text, QRegularExpression pattern, int in_state)
{
    int startIndex = 0;
    int offset = 0;

    if(previousBlockState() != in_state)
    {
        startIndex = text.indexOf(pattern);
        offset = pattern.match(text).capturedLength();
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = pattern.match(text, startIndex + offset);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        if(endIndex >= offset)
        {
            commentLength = endIndex - startIndex + offset + pattern.match(text).capturedLength();
            setCurrentBlockState(BlockState::NotInComment);
        }
        else
        {
            setCurrentBlockState(in_state);
            commentLength = text.length() - startIndex + offset;
        }

        setFormat(startIndex, commentLength, blockCommentFormat);
        startIndex = text.indexOf(pattern, startIndex + commentLength);
    }

    return currentBlockState() == in_state;
}
