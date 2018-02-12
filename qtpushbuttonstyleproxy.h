#ifndef QTPUSHBUTTONSTYLEPROXY_H
#define QTPUSHBUTTONSTYLEPROXY_H

#include <QProxyStyle>

class QtPushButtonStyleProxy :  public QProxyStyle
{
public:
    QtPushButtonStyleProxy(): QProxyStyle() {}

    virtual void drawItemText(QPainter *painter, const QRect &rect,
            int flags, const QPalette &pal, bool enabled,
            const QString &text, QPalette::ColorRole textRole) const
        {
            flags |= Qt::TextWordWrap;
            QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
        }

    private:
        Q_DISABLE_COPY(QtPushButtonStyleProxy)
    };

#endif // QTPUSHBUTTONSTYLEPROXY_H
