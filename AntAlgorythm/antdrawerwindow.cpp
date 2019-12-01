#include "antdrawerwindow.h"


AntDrawerWindow::AntDrawerWindow(const QVector<QPointF> &points, const QSize &size, QWindow *parent):
    DrawerWindow(size, parent),
    m_points(points)
{
}

AntDrawerWindow::AntDrawerWindow(const AntDrawerWindow &antDrawerWindow):
    DrawerWindow(size(), parent()),
    m_points(antDrawerWindow.m_points),
    m_inds(antDrawerWindow.m_inds),
    m_colors(antDrawerWindow.m_colors)
{
}

void AntDrawerWindow::clearPathes()
{
    m_inds.clear();
    m_colors.clear();
    m_widths.clear();
}

void AntDrawerWindow::setPath(const int ind0, const int ind1, const double r, const double g, const double b, const int width)
{
    Q_ASSERT(ind0 >= 0 && ind0 < m_points.length());
    Q_ASSERT(ind1 >= 0 && ind1 < m_points.length());
    Q_ASSERT(ind0 != ind1);
    Q_ASSERT(r >= 0 && r <= 1);
    Q_ASSERT(g >= 0 && g <= 1);
    Q_ASSERT(b >= 0 && b <= 1);
    int R = int(r * 255), G = int(g * 255), B = int(b * 255);
    m_inds.append({ind0, ind1});
    m_colors.append(QColor(R, G, B));
    m_widths.append(width);
}

void AntDrawerWindow::render(QPainter *painter)
{
    Q_ASSERT(m_colors.length() * 2 == m_inds.length());
    for (int i = 0; i < m_colors.length(); ++i){
        painter->setPen(QPen(m_colors[i], m_widths[i]));
        painter->drawLine(m_points[m_inds[i * 2]], m_points[m_inds[i * 2 + 1]]);
    }
}
