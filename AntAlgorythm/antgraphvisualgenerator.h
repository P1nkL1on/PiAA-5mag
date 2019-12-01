#ifndef ANTGRAPHVISUALGENERATOR_H
#define ANTGRAPHVISUALGENERATOR_H

#include "antdrawerwindow.h"
#include "antgraphvisual.h"

class AntGraphVisualGenerator
{
public:
    AntGraphVisualGenerator(
            const bool generateTownMap,
            const QSize &fieldSize,
            const int nPlaces,
            const int nPathes,
            const double &feromonPerAnt = 10,
            const double &expirePercent = .1,
            const double &distWeigth = 1,
            const double &feromonWeigth = 1);
    AntGraphVisual graph() const;

protected:
    AntDrawerWindow *drawerWindow() const;
    void generate0(const QSize &fieldSize, const int nPlaces, const int nPathes);
    void generate1(const QSize &fieldSize);
private:
    const QSize m_fieldSize;
    QVector<QPointF> m_points;
    QVector<int> m_pathes;
    QVector<double> m_dists;
    const double m_feromonPerAnt = 0;
    const double m_expirePercent = 0;
    const double m_distWeigth = 0;
    const double m_eferomonWeigth = 0;
};

#endif // ANTGRAPHVISUALGENERATOR_H
