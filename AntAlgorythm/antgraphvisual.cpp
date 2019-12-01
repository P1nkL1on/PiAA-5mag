#include "antgraphvisual.h"

#include <QApplication>
#include <iostream>

AntGraphVisual::AntGraphVisual(const AntGraph &antGraph, AntDrawerWindow *drawer):
    AntGraph(antGraph),
    m_drawer(drawer)
{
}

void AntGraphVisual::showAntPath(const std::vector<int> &travelInds, const double &travelDist)
{
    AntGraph::showAntPath(travelInds, travelDist);
    const bool isFinished = travelDist > 0;
    drawPath(travelInds, (isFinished? 0 : 1), (isFinished? 1 : 0), 0);
}

void AntGraphVisual::finish()
{
    int bestInd = -1;
    double bestDist = std::numeric_limits<double>::max();
//    int bestCount = 0;
//        // compare by popularity
//        if (m_count[i] > bestCount){
//            bestCount = m_count[i];
//            bestInd = i;
//        }
        // compare by dist
    for (int i = 0; i < m_travelDists.size(); ++i)
        if (m_travelDists[i] < bestDist){
            bestDist = m_travelDists[i];
            bestInd = i;
        }
    for (auto &path : m_pathes)
        path.setFeromon(1);
    if (bestInd >= 0)
        drawPath(m_travels[bestInd], 1, .5, 0, 6, 2);

    std::cout << "Finished. Enter anything to restart... ";
    char pressToContinue;
    std::cin >> pressToContinue;

    m_drawer->close();
    m_drawer->deleteLater();
}

void AntGraphVisual::drawPath(const std::vector<int> &travelInds, const double r, const double g, const double b, int winPathWidth, int normaPathWidth)
{
    if (m_drawer == nullptr)
        return;
    // firstly draw all the visuals
    m_drawer->clearPathes();
    double maxFer;
    for (const auto &path : m_pathes)
        if (path.feromon() > maxFer)
            maxFer = path.feromon();

    for (const auto &path : m_pathes){
        const double ferPart = 1 - path.feromon() / maxFer * .5;
        const bool isInPath = isIn(path.startInd(), path.endInd(), travelInds);
        m_drawer->setPath(
                    path.startInd(),
                    path.endInd(),
                    isInPath? r : ferPart,
                    isInPath? g : ferPart,
                    isInPath? b : ferPart,
                    isInPath? winPathWidth : normaPathWidth);
    }
    m_drawer->renderNow();
    QApplication::processEvents();
}

bool AntGraphVisual::isIn(const int ind0, const int ind1, const std::vector<int> &inds) const
{
    for (uint i = 1; i < inds.size(); ++i)
        if ((inds[i - 1] == ind0 && inds[i] == ind1) || inds[i - 1] == ind1 && inds[i] == ind0)
            return true;
    return false;
}
