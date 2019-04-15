#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:

    Highlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter (parent) {}
    virtual void setKeywords(QStringList keywords);
    virtual void setKeywordFormat();
    virtual void setClassPattern(QRegularExpression classPattern);
    virtual void setClassFormat();
    virtual void setFunctionPattern(QRegularExpression functionPattern);
    virtual void setFunctionFormat();
    virtual void setQuotePattern(QRegularExpression quotePattern);
    virtual void setQuoteFormat();
    virtual void setInlineCommentPattern(QRegularExpression inlineCommentPattern);
    virtual void setInlineCommentFormat();
    virtual void setBlockCommentStartPattern(QRegularExpression blockCommentStart);
    virtual void setBlockCommentEndPattern(QRegularExpression blockCommentEnd);
    virtual void setBlockCommentFormat();
    virtual void addRule(QRegularExpression pattern, QTextCharFormat format);

protected:

    virtual void highlightBlock(const QString &text) override;
    virtual void highlightMultilineComments(const QString &text);

private:

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> rules;

    QRegularExpression blockCommentStart;
    QRegularExpression blockCommentEnd;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat inlineCommentFormat;
    QTextCharFormat blockCommentFormat;
    QTextCharFormat quoteFormat;
    QTextCharFormat functionFormat;
};


/* Used for multi-line comment formatting */
enum BlockState
{
    NotInComment,
    InComment
};


/* Used to build specific types of highlighters */

Highlighter *cHighlighter(QTextDocument *doc);
Highlighter *cppHighlighter(QTextDocument *doc);
Highlighter *javaHighlighter(QTextDocument *doc);
Highlighter *pythonHighlighter(QTextDocument *doc);


#endif // HIGHLIGHTER_H
