#ifndef DOCUMENTMETRICS_H
#define DOCUMENTMETRICS_H

struct DocumentMetrics
{
    DocumentMetrics() : charCount(0), wordCount(0), lineCount(1) {}
    int charCount;
    int wordCount;
    int lineCount;
};

#endif // DOCUMENTMETRICS_H
