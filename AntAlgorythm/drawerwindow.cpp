#include "drawerwindow.h"

DrawerWindow::DrawerWindow(const QSize &size, QWindow *parent):
    QWindow(parent),
    m_backingStore(new QBackingStore(this))
{
    setGeometry(100, 100, size.width(), size.height());
}

void DrawerWindow::render(QPainter *painter)
{
    painter->drawText(QRectF(0, 0, width(), height()), Qt::AlignCenter, QStringLiteral("QWindow"));
}

void DrawerWindow::renderLater()
{
    requestUpdate();
}

void DrawerWindow::renderNow()
{
    if (!isExposed())
        return;

    QRect rect(0, 0, width(), height());
    m_backingStore->beginPaint(rect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);

    painter.fillRect(0, 0, width(), height(), Qt::white);
    render(&painter);
    painter.end();

    m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

bool DrawerWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

void DrawerWindow::resizeEvent(QResizeEvent *event)
{
    m_backingStore->resize(event->size());
}

void DrawerWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        renderNow();
}
