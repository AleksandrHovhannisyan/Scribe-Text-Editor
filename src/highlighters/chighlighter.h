#ifndef CHIGHLIGHTER_H
#define CHIGHLIGHTER_H
#include "highlighter.h"


class CHighlighter : public Highlighter
{
public:
    CHighlighter(QTextDocument *parent = nullptr);
};

#endif // CHIGHLIGHTER_H
