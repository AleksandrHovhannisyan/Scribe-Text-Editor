#include "highlighter.h"
#include <QtDebug>


/* Constructs a Highlighter object using the given list of patterns and the parent
 * QTextDocument widget (nullptr by default).
 */
Highlighter::Highlighter(QStringList keywordPatterns, QRegularExpression classPattern,
                         QRegularExpression quotePattern, QRegularExpression functionPattern,
                         QRegularExpression inlineCommentPattern, QRegularExpression blockCommentStart,
                         QRegularExpression blockCommentEnd, QTextDocument *parent) : QSyntaxHighlighter (parent)
{
    HighlightingRule rule;

    // Class highlighting rule
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = classPattern;
    rule.format = classFormat;
    rules.append(rule);

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    // Create a HighlightingRule for each pattern and store it in the internal vector
    foreach(const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        rules.append(rule);
    }

    // Quotation (e.g., chars and strings) highlighting rule
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = quotePattern;
    rule.format = quotationFormat;
    rules.append(rule);

    // Functions
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = functionPattern;
    rule.format = functionFormat;
    rules.append(rule);

    // Inline comments
    inlineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = inlineCommentPattern;
    rule.format = inlineCommentFormat;
    rules.append(rule);

    // Block comments
    blockCommentFormat.setForeground(Qt::darkGreen);
    this->blockCommentStart = blockCommentStart;
    this->blockCommentEnd = blockCommentEnd;
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

    formatMultilineComments(text);
}


/* Formats multiline comments per this Highlighter's
 * blockCommentStart and blockCommentEnd patterns.
 */
void Highlighter::formatMultilineComments(const QString &text)
{
    setCurrentBlockState(BlockState::NotInComment);

    qDebug() << "Previous state: " << previousBlockState();

    int startIndex = 0;

    // If the previous state was not in comment, then start searching from very beginning
    if(previousBlockState() != BlockState::InComment)
    {
        startIndex = text.indexOf(blockCommentStart);
    }

    //qDebug() << "Start index: " << startIndex;

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = blockCommentEnd.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        //qDebug() << "End index: " << endIndex;

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

        //qDebug() << "Comment length: " << commentLength << "\n";

        setFormat(startIndex, commentLength, blockCommentFormat);
        startIndex = text.indexOf(blockCommentStart, startIndex + commentLength);
   }
}


/* Returns a Highlighter object specific to the C language and its grammar and syntax.
 */
Highlighter *cHighlighter(QTextDocument *doc)
{
    QStringList keywords;

    keywords << "\\bauto\\b" << "\\bbreak\\b" << "\\bcase\\b" << "\\bchar\\b" << "\\bconst\\b"
             << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b"
             << "\\benum\\b" << "\\bextern\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b"
             << "\\bif\\b" << "\\bint\\b" << "\\blong\\b" << "\\bregister\\b" << "\\breturn\\b"
             << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstruct\\b"
             << "\\bswitch\\b" << "\\btypedef\\b" << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
             << "\\bvolatile\\b" << "\\bwhile\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('\\\\.')|('.{0,1}')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("//.*");
    QRegularExpression blockCommentStart("/\\*");
    QRegularExpression blockCommentEnd("\\*/");

    return new Highlighter(keywords, classPattern, quotePattern, functionPattern,
                           inlineCommentPattern, blockCommentStart, blockCommentEnd, doc);
}


/* Returns a Highlighter object specific to the C++ language and its grammar and syntax.
 */
Highlighter *cppHighlighter(QTextDocument *doc)
{
    QStringList keywords;

    keywords << "\\basm\\b" << "\\belse\\b" << "\\bnew\\b" << "\\bthis\\b" <<
                "\\bauto\\b" << "\\benum\\b" << "\\boperator\\b" << "\\bthrow\\b" <<
                "\\bbool\\b" << "\\bexplicit\\b" << "\\bprivate\\b" << "\\btrue\\b" <<
                "\\bbreak\\b" << "\\bexport\\b" << "\\bprotected\\b" << "\\btry\\b" <<
                "\\bcase\\b" << "\\bextern\\b" << "\\bpublic\\b" << "\\btypedef\\b" <<
                "\\bcatch\\b" << "\\bfalse\\b" << "\\bregister\\b" << "\\btypeid\\b" <<
                "\\bchar\\b" << "\\bfloat\\b" << "\\breinterpret_cast\\b" << "\\btypename\\b" <<
                "\\bclass\\b" << "\\bfor\\b" << "\\breturn\\b" << "\\bunion\\b" <<
                "\\bconst\\b" << "\\bfriend\\b" << "\\bshort\\b" << "\\bunsigned\\b" <<
                "\\bconst_cast\\b" << "\\bgoto\\b" << "\\bsigned\\b" << "\\busing\\b" <<
                "\\bcontinue\\b" << "\\bif\\b" << "\\bsizeof\\b" << "\\bvirtual\\b" <<
                "\\bdefault\\b" << "\\binline\\b" << "\\bstatic\\b" << "\\bvoid\\b" <<
                "\\bdelete\\b" << "\\bint\\b" << "\\bstatic_cast\\b" << "\\bvolatile\\b" <<
                "\\bdo\\b" << "\\blong\\b" << "\\bstruct\\b" << "\\bwchar_t\\b" <<
                "\\bdouble\\b" << "\\bmutable\\b" << "\\bswitch\\b" << "\\bwhile\\b" <<
                "\\bdynamic_cast\\b" << "\\bnamespace\\b" << "\\btemplate\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('\\\\.')|('.{0,1}')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()"); // TODO account for special operator overload cases
    QRegularExpression inlineCommentPattern("//.*");
    QRegularExpression blockCommentStart("/\\*");
    QRegularExpression blockCommentEnd("\\*/");

    return new Highlighter(keywords, classPattern, quotePattern, functionPattern,
                           inlineCommentPattern, blockCommentStart, blockCommentEnd, doc);
}


/* Returns a Highlighter object specific to the Java language and its grammar and syntax.
 */
Highlighter *javaHighlighter(QTextDocument *doc)
{
    QStringList keywords;

    keywords << "\\babstract\\b" << "\\bassert\\b" << "\\bboolean\\b" << "\\bbreak\\b" << "\\bbyte\\b"
             << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bcontinue\\b"
             << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bextends\\b"
             << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b"
             << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b" << "\\bint\\b" << "\\binterface\\b"
             << "\\blong\\b" << "\\bnative\\b" << "\\bnew\\b" << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b"
             << "\\bpublic\\b" << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b" << "\\bstrictfp\\b" << "\\bsuper\\b"
             << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\bthis\\b" << "\\bthrow\\b" << "\\bthrows\\b" << "\\btransient\\b"
             << "\\btry\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('\\\\.')|('.{0,1}')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("//.*");
    QRegularExpression blockCommentStart("/\\*");
    QRegularExpression blockCommentEnd("\\*/");

    return new Highlighter(keywords, classPattern, quotePattern, functionPattern,
                           inlineCommentPattern, blockCommentStart, blockCommentEnd, doc);
}


/* Returns a Highlighter object specific to the Python language and its grammar and syntax.
 */
Highlighter *pythonHighlighter(QTextDocument *doc)
{
    QStringList keywords;

    keywords << "\\band\\b" << "\\bas\\b" << "\\bassert\\b" << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
             << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b" << "\\belse\\b" << "\\bexcept\\b" << "\\bFalse\\b"
             << "\\bfinally\\b" << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b" << "\\bif\\b" << "\\bimport\\b"
             << "\\bin\\b" << "\\bis\\b" << "\\blambda\\b" << "\\bNone\\b" << "\\bnonlocal\\b" << "\\bnot\\b"
             << "\\bor\\b" << "\\bpass\\b" << "\\braise\\b" << "\\breturn\\b" << "\\bTrue\\b" << "\\btry\\b"
             << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('.*')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("#.*");

    // TODO change for Python
    QRegularExpression blockCommentStart("'''");
    QRegularExpression blockCommentEnd("'''");

    return new Highlighter(keywords, classPattern, quotePattern, functionPattern,
                           inlineCommentPattern, blockCommentStart, blockCommentEnd, doc);
}


