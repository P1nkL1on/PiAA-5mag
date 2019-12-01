#include "ant.h"

#include <QChar>
#include <math.h>
#include <random>
#include <time.h>

Ant::Ant(const int nowInd, const int nPlacesToVisit):
    m_startInd(nowInd),
    m_nowInd(nowInd),
    m_nInds(nPlacesToVisit),
    m_visitedInds({nowInd}),
    m_path({nowInd})
{
}

Ant::~Ant()
{
}

Ant::MoveResult Ant::move(const std::vector<Path> &possiblePathes)
{
    // if moved everywhere and can finish
    if (m_visitedInds.size() == m_nInds)
        removeVisited(m_startInd);

    const auto notVisitedPathes = excludeVisited(possiblePathes);
    if (!notVisitedPathes.size())
        return lost;
    doMove(notVisitedPathes);

    return m_nowInd == m_startInd? finish : no;
}

void Ant::doMove(const std::vector<Path> &notVisitedPathes)
{
    std::vector<double> ps = {0};
    double pSumm = 0.0;
    for (const auto &path : notVisitedPathes){
        const double n = 1.0 / path.dist();
        const double t = path.feromon();
        const double p =
                pow(n, m_pathLengthWeigth)
                * pow(t, m_feromonWeigth);
        ps.push_back(pSumm + p);
        pSumm += p;
    }
    // normilizing
    for (auto &p : ps)
        p /= pSumm;
    const int decidedInd = decidePath(ps);
    const auto choosenPath = notVisitedPathes[decidedInd];
    const auto travelToInd = choosenPath.leadInt(m_nowInd);

    m_distTravelled += choosenPath.dist();
    m_visitedInds.push_back(travelToInd);
    m_path.push_back(travelToInd);
    m_nowInd = travelToInd;
}

int Ant::decidePath(const std::vector<double> &probabilities) const
{
    const double choosenP = (rand() % 1000) / 1000.0;
    int choosenPathInd = -1;
    for (uint i = 0; i < probabilities.size(); ++i)
        if (probabilities[i] > choosenP){
            choosenPathInd = i - 1;
            break;
        }
    Q_ASSERT(choosenPathInd >= 0);
    return choosenPathInd;
}

const std::vector<Path> Ant::excludeVisited(const std::vector<Path> &possiblePathes) const
{
    std::vector<Path> res;
    for (const auto &path : possiblePathes){
        // check path is correct
        const int leadToInd = path.leadInt(m_nowInd);
        Q_ASSERT(leadToInd >= 0);
        // if already visited this path, exclude it from deciding list
        if (containsVisited(leadToInd))
            continue;
        res.push_back(path);
    }
    return res;
}

void Ant::removeVisited(const int visitedInd)
{
    m_visitedInds.erase(std::remove(m_visitedInds.begin(), m_visitedInds.end(), visitedInd), m_visitedInds.end());
}

bool Ant::containsVisited(const int vistedInd) const
{
    return std::find(m_visitedInds.begin(), m_visitedInds.end(), vistedInd) != m_visitedInds.end();
}

double Ant::distTravelled() const
{
    return m_distTravelled;
}

std::vector<int> Ant::pathInds() const
{
    return m_path;
}

int Ant::nowInd() const
{
    return m_nowInd;
}
