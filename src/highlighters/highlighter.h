#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QtDebug>


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:

    Highlighter(QTextDocument *parent = nullptr);
    virtual void addKeywords(QStringList keywords);
    virtual void addRule(QRegularExpression pattern, QTextCharFormat format);

    QChar getCodeBlockStartDelimiter() const { return codeBlockStart; }
    QChar getCodeBlockEndDelimiter() const { return codeBlockEnd; }

protected:

    virtual void highlightBlock(const QString &text) override;
    virtual void highlightMultilineComments(const QString &text);

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    // Used for multi-line comment formatting
    enum BlockState
    {
        NotInComment = 0,
        InComment = 1
    };

    QVector<HighlightingRule> rules;

    // Block comments are handled separately from the other patterns
    QRegularExpression blockCommentStart;
    QRegularExpression blockCommentEnd;

    // For auto-indentation after a user hits ENTER
    QChar codeBlockStart;
    QChar codeBlockEnd;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat inlineCommentFormat;
    QTextCharFormat blockCommentFormat;
    QTextCharFormat quoteFormat;
    QTextCharFormat functionFormat;

};

#endif // HIGHLIGHTER_H
