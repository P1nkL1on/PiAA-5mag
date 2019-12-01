#ifndef ANTGRAPHVISUAL_H
#define ANTGRAPHVISUAL_H

#include "antgraph.h"
#include "antdrawerwindow.h"

class AntGraphVisual : public AntGraph
{
public:
    AntGraphVisual(const AntGraph &antGraph, AntDrawerWindow *drawer = nullptr);
    void showAntPath(const std::vector<int> &travelInds, const double &travelDist) override;
    void finish();

protected:
    void drawPath(
            const std::vector<int> &travelInds,
            const double r, const double g, const double b,
            int winPathWidth = 2, int normaPathWidth = 1);
    AntDrawerWindow *m_drawer = nullptr;

private:
    bool isIn(const int ind0, const int ind1, const std::vector<int> &inds) const;
};

#endif // ANTGRAPHVISUAL_H
