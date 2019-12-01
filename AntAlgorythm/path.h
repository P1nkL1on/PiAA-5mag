#ifndef PATH_H
#define PATH_H


class Path
{
public:
    Path(int startInd,
         int endInd,
         double dist,
         double feromon = 0.0);
    int leadInt(const int fromInd) const;
    int startInd() const;
    int endInd() const;
    double dist() const;
    double feromon() const;
    void setFeromon(double feromon);
    bool containsInd(const int ind) const;

private:
    int m_startInd = -1;
    int m_endInd = -1;
    double m_dist = 0;
    double m_feromon = 0;
};

#endif // PATH_H
