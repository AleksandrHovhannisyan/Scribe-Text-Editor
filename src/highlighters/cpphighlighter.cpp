#include "cpphighlighter.h"


CPPHighlighter::CPPHighlighter(QTextDocument *parent) : CHighlighter (parent)
{
    QStringList cppOnlyKeywords;

    cppOnlyKeywords <<  "\\basm\\b" << "\\bbool\\b" << "\\bcatch\\b" <<
                        "\\bclass\\b" << "\\bconst_cast\\b" << "\\bdelete\\b" <<
                        "\\bdynamic_cast\\b" << "\\bexplicit\\b" << "\\bfalse\\b" <<
                        "\\bfriend\\b" << "\\binline\\b" << "\\bmutable\\b" <<
                        "\\bnamespace\\b" << "\\bnew\\b" << "\\boperator\\b" <<
                        "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" <<
                        "\\breinterpret_cast\\b" << "\\bstatic_cast\\b" <<
                        "\\btemplate\\b" << "\\bthis\\b" << "\\bthrow\\b" <<
                        "\\btrue\\b" << "\\btry\\b" << "\\btypeid\\b" << "\\btypename\\b" <<
                        "\\bvirtual\\b" << "\\busing\\b" << "\\bwchar_t\\b";

    addKeywords(cppOnlyKeywords);
}
