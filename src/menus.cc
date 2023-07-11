#include "menus.h"

#include <QDateTime>
#include <QFileInfo>
#include <QMessageBox>
#include <cmath>

#include "ui_menus.h"

Menus::Menus(QWidget *parent)
    : QWidget{parent}, ui_{new Ui::Menus}, graph_days_increase_{0} {
  ui_->setupUi(this);
  graphs_.reserve(5);

  connect(ui_->draw_cubic_spline_button, SIGNAL(clicked(bool)), this,
          SLOT(OnDrawCubicSplineButtonClicked()));

  connect(ui_->draw_approximation_button, SIGNAL(clicked(bool)), this,
          SLOT(OnDrawApproximationButtonClicked()));

  connect(ui_->interpolation_clear_button, SIGNAL(clicked(bool)), this,
          SLOT(OnClearButtonClicked()));

  connect(ui_->clear_approximation_button, SIGNAL(clicked(bool)), this,
          SLOT(OnClearButtonClicked()));

  connect(ui_->approximation_clear_all_button, SIGNAL(clicked(bool)), this,
          SLOT(OnClearAllButtonClicked()));

  connect(ui_->interpolation_clear_all_button, SIGNAL(clicked(bool)), this,
          SLOT(OnClearAllButtonClicked()));

  connect(ui_->date_time_interpolation_date_time_edit,
          SIGNAL(dateTimeChanged(const QDateTime &)),
          SLOT(OnDateTimeChanged(const QDateTime &)));

  connect(ui_->date_time_approximate_date_time_edit,
          SIGNAL(dateTimeChanged(const QDateTime &)),
          SLOT(OnApproximationDateTimeChanged(const QDateTime &)));

  connect(ui_->extends_of_days_spin_box, SIGNAL(valueChanged(int)),
          SLOT(OnNumberOfDaysChanged(int)));
}

Menus::~Menus() { delete ui_; }

void Menus::OnDrawCubicSplineButtonClicked() {
  auto number_of_points{ui_->number_of_points_spin_box->value()};
  auto cubic_splines = function_.GetInterpolatedValues(number_of_points);

  SetEnabledApproximationTab(false);
  AddNewGraph(QString::number(number_of_points), cubic_splines);
}

void Menus::OnDrawApproximationButtonClicked() {
  auto points = function_.GetValues();
  auto max_x = points.MinMax<Points::XAxis>().second;

  auto end_date = static_cast<double>(
      QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(max_x))
          .addDays(graph_days_increase_)
          .toMSecsSinceEpoch());

  auto number_of_points{ui_->number_of_approximation_points_spin_box->value()};
  auto degree{ui_->polynomial_power_spin_box->value()};
  auto approximation =
      function_.GetApproximatedValues(number_of_points, degree, end_date);

  SetEnabledInterpolationTab(false);
  AddNewGraph(QString::number(degree), approximation);
}

void Menus::OnFileLoaded(const QString &file_path) {
  RemoveAllGraphs();

  csv::File file;
  file.Open(file_path.toStdString());
  if (file.GetStatus() != csv::Status::kOpenSuccess) {
    ShowErrorMessage("Can't open file " + file_path);
    return;
  }

  QFileInfo info(file_path);
  ui_->filename_label->setText(info.fileName());

  function_.LoadFromCSVFile(std::move(file), x_translate, y_translate);

  if (!function_.ArePointsCorrect()) {
    ShowErrorMessage("Invalid file data");
    return;
  }

  auto points = function_.GetValues();

  int number_of_points = static_cast<int>(points.Size());
  ui_->number_of_points_spin_box->setRange(number_of_points,
                                           number_of_points * 10);
  ui_->number_of_approximation_points_spin_box->setRange(number_of_points,
                                                         number_of_points * 10);
  SetEnabledWidget(true);

  auto [min_x, max_x] = points.MinMax<Points::XAxis>();
  ui_->date_time_interpolation_date_time_edit->setMinimumDateTime(
      QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(min_x)));
  ui_->date_time_interpolation_date_time_edit->setMaximumDateTime(
      QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(max_x)));
  ui_->date_time_approximate_date_time_edit->setMinimumDateTime(
      QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(min_x)));
  ui_->date_time_approximate_date_time_edit->setMaximumDateTime(
      QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(max_x))
          .addDays(graph_days_increase_));

  AddNewGraph(info.fileName(), points);
}

void Menus::ShowErrorMessage(const QString &message) {
  QMessageBox::critical(nullptr, "Error", message);
}

void Menus::OnClearButtonClicked() { RemoveAllGraphsExcludeMain(); }

void Menus::OnClearAllButtonClicked() { RemoveAllGraphs(); }

void Menus::OnDateTimeChanged(const QDateTime &date_time) {
  auto point = function_.InterpolatedPointAt(
      static_cast<double>(date_time.toMSecsSinceEpoch()));

  auto s = QString::number(point.y);

  ui_->interpolated_value_label->setText(s);
}

void Menus::AddNewGraph(const QString &name, const Points &points) {
  graphs_.push_back(name);
  emit OnNewSeries(name.toStdString(), points.GetRawPoints());
  if (graphs_.size() >= 5) {
    SetEnabledDrawButtons(false);
  }
}

void Menus::RemoveGraph(const QString &name) {
  for (int i = 0; i < graphs_.size(); ++i) {
    if (graphs_[i] == name) {
      emit OnDetachSeries(graphs_[i].toStdString());
      graphs_.removeAt(i);
    }
  }

  if (graphs_.size() < 5) {
    SetEnabledDrawButtons(true);
  }
  if (graphs_.size() <= 1) {
    SetEnabledInterpolationTab(true);
    SetEnabledApproximationTab(true);
  }
  if (graphs_.empty()) {
    SetEnabledWidget(false);
  }
}

void Menus::RemoveAllGraphsExcludeMain() {
  while (graphs_.size() != 1) {
    RemoveGraph(graphs_.back());
  }
}

void Menus::RemoveAllGraphs() {
  while (!graphs_.empty()) {
    RemoveGraph(graphs_.back());
  }
}

void Menus::SetEnabledApproximationTab(bool enabled) {
  ui_->tab_widget->setTabEnabled(1, enabled);
}

void Menus::SetEnabledInterpolationTab(bool enabled) {
  ui_->tab_widget->setTabEnabled(0, enabled);
}

void Menus::OnNumberOfDaysChanged(int days) {
  RemoveAllGraphsExcludeMain();
  graph_days_increase_ = days;
}

void Menus::SetEnabledWidget(bool enabled) {
  ui_->tab_widget->setEnabled(enabled);
}

void Menus::SetEnabledDrawButtons(bool enabled) {
  ui_->draw_cubic_spline_button->setEnabled(enabled);
  ui_->draw_approximation_button->setEnabled(enabled);
}

void Menus::OnApproximationDateTimeChanged(const QDateTime &date_time) {
  auto point = function_.ApproximatePointAt(
      static_cast<double>(date_time.toMSecsSinceEpoch()),
      ui_->polynomial_power_spin_box->value());

  auto s = QString::number(point.y);

  ui_->approximated_value_label->setText(s);
}
