#include "mainwindow.h"

#include <QFile>
#include <QFileDialog>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  connect(ui_->menus,
          SIGNAL(OnNewSeries(const std::string &, const std::vector<Point> &)),
          ui_->chart,
          SLOT(AttachSeries(const std::string &, const std::vector<Point> &)));
  connect(ui_->menus, SIGNAL(OnClearSeries()), ui_->chart,
          SLOT(DetachAllSeries()));
  connect(ui_->load_file_action, SIGNAL(triggered(bool)), this,
          SLOT(OnLoadFileTriggered()));
  connect(this, SIGNAL(OnFileLoaded(QString)), ui_->menus,
          SLOT(OnFileLoaded(QString)));
  connect(ui_->menus, SIGNAL(OnDetachSeries(const std::string &)), ui_->chart,
          SLOT(DetachSeries(const std::string &)));
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::OnLoadFileTriggered() {
  QFile file;
  QFileDialog dialog(this);
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setFileMode(QFileDialog::ExistingFiles);
  auto file_path = QFileDialog::getOpenFileName(this, "", "", "*.csv");
  if (!file_path.isEmpty()) {
    emit OnFileLoaded(file_path);
  }
}
