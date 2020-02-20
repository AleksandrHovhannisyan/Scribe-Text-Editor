#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QString>

namespace ProgrammingLanguage
{
    // Four common programming languages
    enum Language
    {
        None,
        CPP,
        C,
        Python,
        Java
    };

    QString toString(Language language);
}

#endif // LANGUAGE_H
