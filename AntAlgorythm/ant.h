#ifndef ANT_H
#define ANT_H

#include <vector>

#include "path.h"

class Ant
{
public:
    Ant(const int nowInd, const int nPlacesToVisit);
    virtual ~Ant();

    enum MoveResult {no = 0, finish, lost};
    virtual MoveResult move(const std::vector<Path> &possiblePathes);
    int nowInd() const;
    double distTravelled() const;
    std::vector<int> pathInds() const;

protected:
    virtual int decidePath(const std::vector<double> &probabilities) const;
    const std::vector<Path> excludeVisited(const std::vector<Path> &possiblePathes) const;
    void removeVisited(const int visitedInd);
    bool containsVisited(const int vistedInd) const;
    void doMove(const std::vector<Path> &notVisitedPathes);
    std::vector<int> m_visitedInds;
    int m_nowInd = -1;

private:
    int m_startInd = -1;
    int m_nInds = 0;
    std::vector<int> m_path;
    double m_distTravelled = 0.0;

    double m_pathLengthWeigth = 1.0;
    double m_feromonWeigth = 1.0;
};

#endif // ANT_H
