#ifndef ANTGRAPH_H
#define ANTGRAPH_H

#include "ant.h"

class AntGraph
{
public:
    AntGraph(
            const int nPlaces,
            const std::vector<Path> &possiblePathes,
            const double &expirePercent,
            const double &feromonPerAnt,
            const double &distWeigth = 1,
            const double &feromonWeigth = 1);
    virtual void spawnAnt(const int ind);
    virtual void autoSpawnAnt(const int ind, const int maxAntCount = 1000);

    virtual void spawnAntFinder(const int ind, const int to);
    virtual void autoSpawnAntFinder(const int ind, const int to, const int maxAntCount = 1000);

    virtual void showAntPath(const std::vector<int> &travelInds, const double &travelDist);
    void printScore() const;

protected:
    std::vector<Path> pathesFromInd(const int ind) const;
    void feromonPath(const double &feromonCount, const std::vector<int> &pathInds);

protected:
    std::vector<Path> m_pathes;
    bool isEquals(const std::vector<int> &path1, const std::vector<int> &path2) const;
    void rememberScore(const std::vector<int> &travelInds, const double &travelDist);

    std::vector<std::vector<int>> m_travels;
    std::vector<double> m_travelDists;
    std::vector<int> m_count;

private:

    int m_nPlaces = 0;
    double m_expirePerAnt = .1;
    double m_feromonPerAnt = 0;
    const double m_distWeigth = 1;
    const double m_feromonWeigth = 1;

    // statistics
    int m_totalCount = 0;
};

#endif // ANTGRAPH_H
