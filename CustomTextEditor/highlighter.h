#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QStringList keywordPatterns, QRegularExpression classPattern,
                QRegularExpression quotePattern, QRegularExpression functionPattern,
                QRegularExpression inlineCommentPattern, QRegularExpression blockCommentStart,
                QRegularExpression blockCommentEnd, QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;
    void formatMultilineComments(const QString &text);

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
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};


/* Used for multi-line comment formatting */
enum BlockState
{
    NotInComment,
    InComment
};

Highlighter *cHighlighter(QTextDocument *doc);
Highlighter *cppHighlighter(QTextDocument *doc);
Highlighter *javaHighlighter(QTextDocument *doc);
Highlighter *pythonHighlighter(QTextDocument *doc);


#endif // HIGHLIGHTER_H
