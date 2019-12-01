#include "antfinder.h"

AntFinder::AntFinder(const int fromInd, const int toInd):
    Ant(fromInd, -1),
    m_toInd(toInd)
{
}

AntFinder::~AntFinder()
{
}

Ant::MoveResult AntFinder::move(const std::vector<Path> &possiblePathes)
{
    const auto notVisitedPathes = excludeVisited(possiblePathes);
    if (!notVisitedPathes.size())
        return lost;
    doMove(notVisitedPathes);

    return m_nowInd == m_toInd? finish : no;
}
