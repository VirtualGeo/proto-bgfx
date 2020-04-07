#ifdef GUI_QT

#ifndef QWIDGETBGFX_H
#define QWIDGETBGFX_H

#include <QObject>
#include <QWidget>

class QWidgetBgfx : public QWidget
{
    Q_OBJECT
public:
    explicit QWidgetBgfx(QWidget *parent = nullptr);
    virtual ~QWidgetBgfx();

protected:
    void paintEvent(QPaintEvent * event) override;
    void resizeEvent(QResizeEvent *event) override;
    QPaintEngine * paintEngine() const override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:


private:
    bool m_updatePending = false;
    bool m_isInit = false;



};

#endif // QWIDGETBGFX_H


#endif
