#ifndef ANDDRAWERWINDOW_H
#define ANDDRAWERWINDOW_H

#include <QVector2D>
#include <QVector>

#include "drawerwindow.h"


class AntDrawerWindow : public DrawerWindow
{
    Q_OBJECT
public:
    explicit AntDrawerWindow(
            const QVector<QPointF> &points,
            const QSize &size,
            QWindow *parent = nullptr);
    AntDrawerWindow(const AntDrawerWindow &antDrawerWindow);

    void clearPathes();
    void setPath(
            const int ind0,
            const int ind1,
            const double r = 0,
            const double g = 0,
            const double b = 0,
            const int width = 2);
    void render(QPainter *painter) override;

private:
    const QVector<QPointF> m_points;
    QVector<int> m_inds;
    QVector<QColor> m_colors;
    QVector<int> m_widths;
};

#endif // ANDDRAWERWINDOW_H
