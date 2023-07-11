#include "charts.h"

#include <QDateTime>
#include <QDebug>

Charts::Charts(QWidget *parent) : QChartView(parent) {
  chart()->setTitle("Stock quotes");
  chart()->legend()->setAlignment(Qt::AlignLeft);
  chart()->setAnimationOptions(QChart::SeriesAnimations);

  x_axis_value_.setTitleText("date");
  x_axis_value_.setFormat("dd-MM-yyyy");
  x_axis_value_.setTickCount(10);

  y_axis_value_.setTitleText("value");
  y_axis_value_.setLabelFormat("%g");
  y_axis_value_.setTickCount(10);

  chart()->addAxis(&x_axis_value_, Qt::AlignmentFlag::AlignBottom);
  chart()->addAxis(&y_axis_value_, Qt::AlignmentFlag::AlignLeft);

  x_axis_value_.setRange(QDateTime::fromMSecsSinceEpoch(0),
                         QDateTime::fromMSecsSinceEpoch(10));
  y_axis_value_.setRange(-5, 5);

  setRenderHint(QPainter::Antialiasing);

  ResetBounds();
}

void Charts::AttachSeries(const std::string &series_name,
                          const std::vector<Point> &points) {
  auto *series = new QLineSeries;
  series->setName(QString::fromStdString(series_name));
  if (series_.empty()) {
    series->setPointsVisible(true);
  }

  for (auto [x, y] : points) {
    series->append(x, y);
  }

  chart()->addSeries(series);
  series->attachAxis(&x_axis_value_);
  series->attachAxis(&y_axis_value_);
  series_.append(series);
  RecalculateBounds(GetPointsBounds(series->points()));
  UpdateBounds();
}

void Charts::DetachSeries(const std::string &name) {
  int index = 0;
  while (index < series_.length()) {
    if (auto series = series_.at(index); series->name().toStdString() == name) {
      series->detachAxis(&x_axis_value_);
      series->detachAxis(&y_axis_value_);
      chart()->removeSeries(series);
      delete series;
      series_.removeAt(index);
    } else {
      ++index;
    }
  }

  ResetBounds();
  for (auto series : series_) {
    RecalculateBounds(GetPointsBounds(series->points()));
  }
  UpdateBounds();
}

void Charts::DetachAllSeries() {
  while (!series_.empty()) {
    DetachSeries(series_.front()->name().toStdString());
  }
}

void Charts::ResetBounds() {
  bounds_ = Bounds{
      QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::max()),
      QDateTime::fromMSecsSinceEpoch(std::numeric_limits<qint64>::min()),
      std::numeric_limits<double>::max(), std::numeric_limits<double>::min()};
}

Charts::Bounds Charts::GetPointsBounds(const QList<QPointF> &points) {
  auto [min_x, max_x] = std::minmax_element(
      points.begin(), points.end(),
      [](const auto &p1, const auto &p2) -> bool { return p1.x() < p2.y(); });

  auto [min_y, max_y] = std::minmax_element(
      points.begin(), points.end(),
      [](const auto &p1, const auto &p2) -> bool { return p1.y() < p2.y(); });

  return {QDateTime::fromMSecsSinceEpoch(min_x->x()),
          QDateTime::fromMSecsSinceEpoch(max_x->x()), min_y->y(), max_y->y()};
}

void Charts::UpdateBounds() {
  if (bounds_.min_x > bounds_.max_x || bounds_.min_y > bounds_.max_y) {
    return;
  }
  x_axis_value_.setRange(bounds_.min_x, bounds_.max_x);
  y_axis_value_.setRange(bounds_.min_y, bounds_.max_y);
}

void Charts::RecalculateBounds(Charts::Bounds points_bounds) {
  auto qDateTimeCompareLess = [](QDateTime &dt1, QDateTime &dt2) {
    return dt1.toMSecsSinceEpoch() < dt2.toMSecsSinceEpoch();
  };
  bounds_.min_x = qDateTimeCompareLess(bounds_.min_x, points_bounds.min_x)
                      ? bounds_.min_x
                      : points_bounds.min_x;
  bounds_.max_x = !qDateTimeCompareLess(bounds_.max_x, points_bounds.max_x)
                      ? bounds_.max_x
                      : points_bounds.max_x;

  bounds_.min_y =
      bounds_.min_y < points_bounds.min_y ? bounds_.min_y : points_bounds.min_y;
  bounds_.max_y = bounds_.max_y >= points_bounds.max_y ? bounds_.max_y
                                                       : points_bounds.max_y;
}
