#include "language.h"


QString ProgrammingLanguage::toString(Language language)
{
    switch (language)
    {
        case (Language::C):
            return "Language: C";
        case (Language::CPP):
            return "Language: C++";
        case (Language::Java):
            return "Language: Java";
        case (Language::Python):
            return "Language: Python";
        default:
            return "Language not selected";
    }
}
