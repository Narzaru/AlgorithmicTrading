#ifndef A8_ALGORITHMIC_TRADING_CHARTS_H_
#define A8_ALGORITHMIC_TRADING_CHARTS_H_

#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>

#include "models/include/point.h"

#if QT_VERSION < 0x060000
using namespace QtCharts;
#endif

class Charts : public QChartView {
  Q_OBJECT

 public:
  explicit Charts(QWidget *parent);

 public slots:
  void AttachSeries(const std::string &series_name,
                    const std::vector<Point> &points);
  void DetachSeries(const std::string &name);
  void DetachAllSeries();

 private:
  struct Bounds {
    QDateTime min_x;
    QDateTime max_x;
    double min_y;
    double max_y;
  };

  Bounds GetPointsBounds(const QList<QPointF> &points);
  void ResetBounds();
  void UpdateBounds();
  void RecalculateBounds(Bounds points_bounds);
  QList<QLineSeries *> series_;
  QDateTimeAxis x_axis_value_;
  QValueAxis y_axis_value_;
  Bounds bounds_;
};

#endif  // A8_ALGORITHMIC_TRADING_CHARTS_H_
