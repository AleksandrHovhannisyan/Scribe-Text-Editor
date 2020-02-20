#include "searchhistory.h"


/* Records the given search term and its information in the current search history chain.
 * If the term starts a new chain, then the search history is cleared before the term is inserted.
 * Effectively, this means there will only ever be one item in the search history at any given point.
 */
void SearchHistory::add(QString term, int cursorPositionBeforeFirstSearch, int firstFoundAt)
{
    // If this search term breaks the current search "chain", clear the history
    if (!previouslyFound(term))
    {
        searchHistory.clear();
    }

    QPair<int,int> locations;
    locations.first = cursorPositionBeforeFirstSearch;
    locations.second = firstFoundAt;
    searchHistory.insert(term, locations);
}
