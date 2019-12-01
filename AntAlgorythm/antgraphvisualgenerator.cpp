#include "antgraphvisualgenerator.h"

#include <iostream>
#include <random>

AntGraphVisualGenerator::AntGraphVisualGenerator(
        const bool generateTownMap,
        const QSize &fieldSize,
        const int nPlaces,
        const int nPathes,
        const double &feromonPerAnt,
        const double &expirePercent,
        const double &distWeigth,
        const double &feromonWeigth):
    m_fieldSize(fieldSize),
    m_feromonPerAnt(feromonPerAnt),
    m_expirePercent(expirePercent),
    m_distWeigth(distWeigth),
    m_eferomonWeigth(feromonWeigth)
{
    if (generateTownMap)
        generate1(fieldSize);
    else
        generate0(fieldSize, nPlaces, nPathes);
}

AntGraphVisual AntGraphVisualGenerator::graph() const
{
    std::vector<Path> pathes;
    for (int i = 0; i < m_pathes.length() / 2; ++i)
        pathes.push_back(Path(m_pathes[i * 2], m_pathes[i * 2 + 1], m_dists[i], 1));
    const auto graph = AntGraph(m_points.length(), pathes, m_expirePercent, m_feromonPerAnt, m_distWeigth, m_eferomonWeigth);
    auto drawer = drawerWindow();
    drawer->show();
    return AntGraphVisual(graph, drawer);
}

AntDrawerWindow *AntGraphVisualGenerator::drawerWindow() const
{
    return new AntDrawerWindow(m_points, m_fieldSize, nullptr);
}

void AntGraphVisualGenerator::generate0(const QSize &fieldSize, const int nPlaces, const int nPathes)
{
    const int offset = std::min(fieldSize.width(), fieldSize.height()) / 8;
    const double minDistance = 50;
    // generate a places

    for (int i = 0; i < nPlaces; ++i){
        QPointF newPoint;
        bool isOnMinDistance = false;
        do{
            const int x = offset + rand() % (fieldSize.width() - offset * 2);
            const int y = offset + rand() % (fieldSize.height() - offset * 2);
            newPoint = QPointF(x, y);
            isOnMinDistance = true;
            for (const QPointF &point : m_points)
                if (std::sqrt(std::pow(point.x() - x, 2) + std::pow(point.y() - y, 2)) < minDistance){
                    isOnMinDistance = false;
                    break;
                }
        }while(!isOnMinDistance);
        m_points << newPoint;
    }
    // generate a pathes
    for (int j = 0; j < nPathes; ++j){
        int ind0 = -1, ind1 = -1;
        bool isOk = false;
        do{
            ind0 = rand() % nPlaces;
            ind1 = rand() % nPlaces;
            isOk = ind0 != ind1;
            if (isOk)
                for (int i = 0; i < m_pathes.length() / 2; ++i)
                    if ((m_pathes[i * 2] == ind0 && m_pathes[i * 2 + 1] == ind1)
                            || (m_pathes[i * 2] == ind1 && m_pathes[i * 2 + 1] == ind0)){
                        isOk = false;
                        break;
                    }
        }while(!isOk);

        m_pathes << ind0 << ind1;
        m_dists << std::sqrt(std::pow(m_points[ind0].x() - m_points[ind1].x(), 2)
                             + std::pow(m_points[ind0].y() - m_points[ind1].y(), 2));
    }
}

void AntGraphVisualGenerator::generate1(const QSize &fieldSize)
{
    int minDistance = 50;

    const int w = int(fieldSize.width() / minDistance);
    const int h = int(fieldSize.height() / minDistance);

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            m_points.push_back(QPointF(j * minDistance + rand() % minDistance,
                                       i * minDistance + rand() % minDistance));

    for (int j = 0; j < h; ++j)
        for (int i = 0; i < w; ++i)
            if ((i + j) % 2 ){
                const int ind0  = j * w + i;
                for (int t = -1; t <= 1; t += 2)
                    if (i + t >= 0 && i + t < w)
                    {
                        const int ind1 = j * w + i + t;
                        m_pathes << ind0 << ind1;
                        m_dists << std::sqrt(std::pow(m_points[ind0].x() - m_points[ind1].x(), 2)
                                             + std::pow(m_points[ind0].y() - m_points[ind1].y(), 2));
                    }
                for (int g = -1; g <= 1; g += 2)
                    if (j + g >= 0 && j + g < h){
                        const int ind1 = j * w + i + g * w;
                        m_pathes << ind0 << ind1;
                        m_dists << std::sqrt(std::pow(m_points[ind0].x() - m_points[ind1].x(), 2)
                                             + std::pow(m_points[ind0].y() - m_points[ind1].y(), 2));
                    }
            }
}
