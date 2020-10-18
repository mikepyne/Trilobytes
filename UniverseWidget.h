#ifndef UNIVERSEWIDGET_H
#define UNIVERSEWIDGET_H

#include "Shape.h"
#include "Universe.h"

#include <QWidget>
#include <QTimer>

class UniverseWidget final : public QWidget, public UniverseFocusInterface {
    Q_OBJECT
public:
    explicit UniverseWidget(QWidget* parent);
    virtual ~UniverseWidget();

    virtual void SetFocus(const Point& focus) override;

    void SetUniverse(std::shared_ptr<Universe> universe);
    void SetFpsTarget(double fps);

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* event) override final;
    virtual void mousePressEvent(QMouseEvent* event) override final;
    virtual void mouseMoveEvent(QMouseEvent* event) override final;

    virtual void keyPressEvent(QKeyEvent* event) override final;

    virtual void paintEvent(QPaintEvent* event) override final;

private:
    QTimer renderThread_;

    // TODO update to using a matrix based transform
    qreal transformX_ = 0.0;
    qreal transformY_ = 0.0;
    qreal transformScale_ = 1.0;
    qreal dragX_ = 0.0;
    qreal dragY_ = 0.0;
    bool dragging_ = false;

    std::shared_ptr<Universe> universe_;

    Point TransformLocalToSimCoords(const Point& local);
    Point TransformSimToLocalCoords(const Point& sim);
};

#endif // UNIVERSEWIDGET_H
