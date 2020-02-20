#ifndef PYTHONHIGHLIGHTER_H
#define PYTHONHIGHLIGHTER_H
#include "highlighter.h"
#include <QPair>


class PythonHighlighter : public Highlighter
{
public:
    PythonHighlighter(QTextDocument *parent = nullptr);

protected:
    virtual void highlightBlock(const QString &text) override;
    virtual bool highlightMultilineComments(const QString &text, QRegularExpression pattern, int in_state);

private:
    QPair<QRegularExpression, int> triple_single_quote;
    QPair<QRegularExpression, int> triple_double_quote;

    enum MultilineQuote
    {
        TRIPLE_SINGLE = 2,
        TRIPLE_DOUBLE = 3
    };
};

#endif // PYTHONHIGHLIGHTER_H
