#ifndef A8_ALGORITHMIC_TRADING_MAINWINDOW_H_
#define A8_ALGORITHMIC_TRADING_MAINWINDOW_H_

#include <QFile>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

 signals:
  void OnFileLoaded(QString file_path);

 private slots:
  void OnLoadFileTriggered();

 private:
  Ui::MainWindow *ui_;
};

#endif  // A8_ALGORITHMIC_TRADING_MAINWINDOW_H_
