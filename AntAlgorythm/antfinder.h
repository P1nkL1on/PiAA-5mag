#ifndef ANTFINDER_H
#define ANTFINDER_H

#include "ant.h"

class AntFinder : public Ant
{
public:
    AntFinder(const int fromInd, const int toInd);
    ~AntFinder() override;
    Ant::MoveResult move(const std::vector<Path> &possiblePathes) override;

protected:
    int m_toInd;
};

#endif // ANTFINDER_H
