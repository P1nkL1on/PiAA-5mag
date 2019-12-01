#include "antgraph.h"

#include <QChar>
#include <iostream>
#include "antfinder.h"

AntGraph::AntGraph(
        const int nPlaces,
        const std::vector<Path> &possiblePathes,
        const double &expirePercent,
        const double &feromonPerAnt,
        const double &distWeigth,
        const double &feromonWeigth):
    m_pathes(possiblePathes),
    m_nPlaces(nPlaces),
    m_expirePerAnt(expirePercent),
    m_feromonPerAnt(feromonPerAnt),
    m_distWeigth(1 / distWeigth),
    m_feromonWeigth(feromonWeigth)
{
    Q_ASSERT(feromonPerAnt >= 0);
    Q_ASSERT(expirePercent >= 0 && expirePercent <= 1);
    Q_ASSERT(m_distWeigth >= 0 && m_distWeigth <= 1);
    Q_ASSERT(feromonWeigth >= 0);
}

void AntGraph::autoSpawnAnt(const int ind, const int maxAntCount)
{
     const double percentToBreak = .8;
     const int antsToConfirm = std::max(100, std::min(1000, maxAntCount / 100));

     int count = 0;
     bool isFoundOptimal = false;

     const int requiredCount = int(m_totalCount * percentToBreak);
     do{
         ++count;
         spawnAnt(ind);
         for (const int &founds : m_count)
             if (founds > requiredCount && m_totalCount > antsToConfirm){
                 isFoundOptimal = true;
                 break;
             }
     }while(!isFoundOptimal && count <= maxAntCount);
     if (!m_count.size()){
         std::cout << "Not enougth ants or bad graph topology!" << std::endl;
         return;
     }
     if (count >= maxAntCount){
         std::cout << "Finished cause max ant number limit!" << std::endl;
         return;
     }
     std::cout << "Find a probably optimal solution via " << count << " ants from " << maxAntCount << "!" << std::endl;
}

void AntGraph::autoSpawnAntFinder(const int ind, const int to, const int maxAntCount)
{
    int count = 0;
    do{
        ++count;
        spawnAntFinder(ind, to);
    }while(count <= maxAntCount);

}

void AntGraph::spawnAnt(const int ind)
{
    Ant ant(ind, m_nPlaces);
    auto moveResult = Ant::no;

    while(!(moveResult = ant.move(pathesFromInd(ant.nowInd()))));

    const bool isLost = moveResult == Ant::lost;
    if (!isLost)
        feromonPath(m_feromonPerAnt, ant.pathInds());
    showAntPath(ant.pathInds(), isLost? -1 : ant.distTravelled());
}

void AntGraph::spawnAntFinder(const int ind, const int to)
{
    AntFinder ant(ind, to);
    auto moveResult = Ant::no;
    while(!(moveResult = ant.move(pathesFromInd(ant.nowInd()))));

    const bool isLost = moveResult == Ant::lost;
    if (!isLost)
        feromonPath(m_feromonPerAnt, ant.pathInds());
    showAntPath(ant.pathInds(), isLost? -1 : ant.distTravelled());
}

void AntGraph::showAntPath(const std::vector<int> &travelInds, const double &travelDist)
{
//    for (auto i = travelInds.begin(); i != travelInds.end(); ++i)
//        std::cout << *i << ' ';
//    std::cout << "= " << travelDist << std::endl;
    // add it to scores
    if (travelDist >= 0)
        rememberScore(travelInds, travelDist);
}

std::vector<Path> AntGraph::pathesFromInd(const int ind) const
{
    std::vector<Path> pathes;
    for (const auto &path : m_pathes)
        if (path.containsInd(ind))
            pathes.push_back(path);
    return pathes;
}

void AntGraph::feromonPath(const double &feromonCount, const std::vector<int> &pathInds)
{
    std::vector<Path *> pathes;
    auto from = *pathInds.begin();
    for (auto path = pathInds.begin() + 1; path != pathInds.end(); ++path){
        const auto to = *path;
        Path *relatedPath = nullptr;
        for (auto j = m_pathes.begin(); j != m_pathes.end(); ++j)
            if (j->containsInd(from) && j->containsInd(to)){
                relatedPath = &*j;
                break;
            }
        from = to;
        pathes.push_back(relatedPath);
    }
    double summDist;
    for (const auto path : pathes)
        summDist += path->dist();
    const double dt = feromonCount / pow(summDist, 1);

    // expiring
    for (auto &path : m_pathes)
        path.setFeromon((1.0 - m_expirePerAnt) * path.feromon());

    // set success path
    for (const auto path : pathes){
        const auto t = path->feromon();
        path->setFeromon(t + dt * path->dist());
    }
}

bool AntGraph::isEquals(const std::vector<int> &path1, const std::vector<int> &path2) const
{
    if (path1.size() != path2.size())
        return false;
    const int n = path1.size();
    const int last = n - 1;
    if (path1[0] != path2[0] || path1[last] != path2[last])
        return false;
    bool isForwardSame = true;
    bool isReverseSame = true;
    for (int i = 0; i < n; ++i){
        if (isForwardSame && path1[i] != path2[i])
            isForwardSame = false;
        if (isReverseSame && path1[i] != path2[last - i])
            isReverseSame = false;
        if (!isForwardSame && !isReverseSame)
            return false;
    }
    return isForwardSame || isReverseSame;
}

void AntGraph::rememberScore(const std::vector<int> &travelInds, const double &travelDist)
{
    ++m_totalCount;
    int isSameAs = -1;
    for (uint i = 0; i < m_travels.size(); ++i)
        if (isEquals(m_travels[i], travelInds)){
            isSameAs = i;
            break;
        }
    if (isSameAs < 0){
        m_travels.push_back(travelInds);
        m_travelDists.push_back(travelDist);
        m_count.push_back(1);
    }else
        ++m_count[isSameAs];
}

void AntGraph::printScore() const
{
    for (uint i = 0; i < m_count.size(); ++i){
        const auto percent = int((double(m_count[i]) / m_totalCount) * 100.0);
        if (percent < 2)
            continue;
        std::cout << (i + 1) << ". "  << percent << "% = "<< m_travelDists[i] << std::endl;
    }
}
