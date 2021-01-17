#ifndef UNIVERSEWIDGET_H
#define UNIVERSEWIDGET_H

#include "Shape.h"
#include "Universe.h"

// TODO QOpenGLWidget allows QPainter painting, but its messed up, consider moving over once everything is pixmap based
#include <QWidget>
#include <QTimer>

class UniverseWidget final : public QWidget, public UniverseObserver {
    Q_OBJECT
public:
    explicit UniverseWidget(QWidget* parent);
    virtual ~UniverseWidget();

    virtual void SuggestUpdate() override { updateToRender_ = true; }

    void SetUniverse(std::shared_ptr<Universe> universe);
    void SetFpsTarget(double fps);

signals:
    void EntitySelected(const std::shared_ptr<Entity>& newSelection);

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* event) override final;
    virtual void mousePressEvent(QMouseEvent* event) override final;
    virtual void mouseMoveEvent(QMouseEvent* event) override final;

    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void paintEvent(QPaintEvent* event) override final;

private:
    QTimer renderThread_;
    bool updateToRender_ = false;

    // TODO update to using a matrix based transform
    qreal transformX_ = 0.0;
    qreal transformY_ = 0.0;
    qreal transformScale_ = 1.0;
    qreal dragX_ = 0.0;
    qreal dragY_ = 0.0;
    bool dragging_ = false;

    std::shared_ptr<Universe> universe_;

    Point TransformLocalToSimCoords(const Point& local) const;
    Point TransformSimToLocalCoords(const Point& sim) const;
};

#endif // UNIVERSEWIDGET_H
