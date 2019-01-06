#ifndef DOCUMENTMETRICS_H
#define DOCUMENTMETRICS_H

struct DocumentMetrics
{
    DocumentMetrics() : charCount(0), wordCount(0), lineCount(1),
                        positionOfLastFindMatch(-1), currentColumn(1) {}
    int charCount;
    int wordCount;
    int lineCount; // TODO remove
    int positionOfLastFindMatch;
    int currentColumn;
};

#endif // DOCUMENTMETRICS_H
