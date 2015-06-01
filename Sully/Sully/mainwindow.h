#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QFile Archivo;
    QString OpenFilePath;
    void CrearFile();
    char* ReadFile();
private slots:
      void on_OpenFile_clicked();

      void on_CreatFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
