#ifndef DRAWER_H
#define DRAWER_H

#include <QtGui>

class DrawerWindow : public QWindow
{
    Q_OBJECT
public:
    explicit DrawerWindow(const QSize &size, QWindow *parent = nullptr);
    virtual void render(QPainter *painter);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

private:
    QBackingStore *m_backingStore = nullptr;
};

#endif // DRAWER_H
