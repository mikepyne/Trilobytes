#include "UniverseWidget.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPalette>

UniverseWidget::UniverseWidget(QWidget* parent)
    : QWidget(parent)
    , renderThread_(this)
{
    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(true);
    QPalette p = palette();
    p.setColor(QPalette::ColorRole::Window, QColor(200, 255, 255));
    setPalette(p);

    renderThread_.setSingleShot(false);
    renderThread_.connect(&renderThread_, &QTimer::timeout, [&]()
    {
        if (updateToRender_) {
            update();
        }
    });

    SetFpsTarget(40);
}

UniverseWidget::~UniverseWidget()
{
}

void UniverseWidget::SuggestFocus(const Point& focus)
{
    transformX_ = -focus.x;
    transformY_ = -focus.y;
    updateToRender_ = true;
}

void UniverseWidget::SetUniverse(std::shared_ptr<Universe> universe)
{
    universe_ = universe;
}

void UniverseWidget::SetFpsTarget(double fps)
{
    if (fps <= 0.0) {
        renderThread_.stop();
    } else if (renderThread_.isActive()) {
        renderThread_.setInterval(1000.0 / fps);
    } else {
        renderThread_.start(1000.0 / fps);
    }
}

void UniverseWidget::wheelEvent(QWheelEvent* event)
{
    double d = 1.0 + (0.001 * double(event->angleDelta().y()));
    transformScale_ *= d;
    updateToRender_ = true;
    update();
}

void UniverseWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton) {
        dragging_ = false;
    }
}

void UniverseWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton) {
        dragging_ = true;
        dragX_ = event->x();
        dragY_ = event->y();
    } else if (event->button() == Qt::RightButton) {
        if (universe_) {
            Point simLocation = TransformLocalToSimCoords({ static_cast<double>(event->x()), static_cast<double>(event->y()) });
            universe_->SelectEntity(simLocation);
        }
    }
}

void UniverseWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging_) {
        transformX_ += ((event->x() - dragX_) / transformScale_);
        transformY_ += ((event->y() - dragY_) / transformScale_);
        dragX_ = event->x();
        dragY_ = event->y();
        updateToRender_ = true;
        update();
    }
}

void UniverseWidget::resizeEvent(QResizeEvent* /*event*/)
{
    updateToRender_ = true;
    update();
}

void UniverseWidget::paintEvent(QPaintEvent* event)
{
    updateToRender_ = false;

    QPainter p(this);
    p.setClipRegion(event->region());
    p.translate(width() / 2, height() / 2);
    p.scale(transformScale_, transformScale_);
    p.translate(transformX_, transformY_);

    // TODO add some indication that sim is still running when 0 FPS selected
    if (universe_) {
        universe_->Render(p);
    }
}

Point UniverseWidget::TransformLocalToSimCoords(const Point& local)
{
    double x = local.x;
    double y = local.y;
    // Sim is centred on screen
    x -= (width() / 2);
    y -= (height() / 2);
    // Sim is scaled
    x /= transformScale_;
    y /= transformScale_;
    // Sim is transformed
    x -= transformX_;
    y -= transformY_;
    return { x, y };
}

Point UniverseWidget::TransformSimToLocalCoords(const Point& sim)
{
    double x = sim.x;
    double y = sim.y;
    // Sim is transformed
    x += transformX_;
    y += transformY_;
    // Sim is scaled
    x *= transformScale_;
    y *= transformScale_;
    // Sim is centred on screen
    x += (width() / 2);
    y += (height() / 2);
    return { x, y };
}
