#ifndef A8_ALGORITHMIC_TRADING_MENUS_H_
#define A8_ALGORITHMIC_TRADING_MENUS_H_

#include <QDateTime>
#include <QLineSeries>
#include <QWidget>

#include "models/include/function.h"

namespace Ui {
class Menus;
}

class Menus : public QWidget {
  Q_OBJECT

 public:
  explicit Menus(QWidget *parent = nullptr);
  ~Menus() override;

 signals:
  void OnNewSeries(const std::string &name, const std::vector<Point> &points);
  void OnDetachSeries(const std::string &name);
  void OnClearSeries();

 private slots:
  void OnDrawCubicSplineButtonClicked();
  void OnDrawApproximationButtonClicked();
  void OnFileLoaded(const QString &file_path);
  void OnDateTimeChanged(const QDateTime &);
  void OnApproximationDateTimeChanged(const QDateTime &date_time);
  void OnClearButtonClicked();
  void OnClearAllButtonClicked();
  void OnNumberOfDaysChanged(int days);

 private:
  void AddNewGraph(const QString &name, const Points &points);
  void RemoveGraph(const QString &name);
  void RemoveAllGraphsExcludeMain();
  void RemoveAllGraphs();

  void SetEnabledDrawButtons(bool enabled);
  void SetEnabledWidget(bool enabled);
  void SetEnabledInterpolationTab(bool enabled);
  void SetEnabledApproximationTab(bool enabled);

  void ShowErrorMessage(const QString &message);

  const std::function<double(std::string_view)> x_translate =
      [](std::string_view sv) -> double {
    auto date_string = QString::fromStdString({sv.data(), sv.length()});
    auto date_time = QDateTime::fromString(date_string, "yyyy-MM-dd");
    if (!date_time.isValid()) {
      return std::numeric_limits<double>::quiet_NaN();
    } else {
      return static_cast<double>(date_time.toMSecsSinceEpoch());
    }
  };

  const std::function<double(std::string_view)> y_translate =
      [](std::string_view sv) -> double {
    double value{0.0};
    try {
      value = std::stod(std::string{sv.begin(), sv.end()});
    } catch (...) {
      value = std::numeric_limits<double>::quiet_NaN();
    }
    return value;
  };

  Ui::Menus *ui_;
  TabulatedFunction function_;
  int graph_days_increase_;
  QVector<QString> graphs_;
};

#endif  // A8_ALGORITHMIC_TRADING_MENUS_H_
