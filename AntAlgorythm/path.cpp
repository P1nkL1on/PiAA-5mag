#include "path.h"

Path::Path(int startInd, int endInd, double dist, double feromon):
    m_startInd(startInd),
    m_endInd(endInd),
    m_dist(dist),
    m_feromon(feromon)
{
}

int Path::leadInt(const int fromInd) const
{
    return fromInd == startInd()? endInd() : fromInd == endInd()? startInd() : -1;
}

int Path::startInd() const
{
    return m_startInd;
}

int Path::endInd() const
{
    return m_endInd;
}

double Path::dist() const
{
    return m_dist;
}

double Path::feromon() const
{
    return m_feromon;
}

void Path::setFeromon(double feromon)
{
    m_feromon = feromon;
}

bool Path::containsInd(const int ind) const
{
    return startInd() == ind || endInd() == ind;
}
