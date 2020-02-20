#ifndef JAVAHIGHLIGHTER_H
#define JAVAHIGHLIGHTER_H
#include "highlighter.h"


class JavaHighlighter : public Highlighter
{
public:
    JavaHighlighter(QTextDocument *parent = nullptr);
};

#endif // JAVAHIGHLIGHTER_H
