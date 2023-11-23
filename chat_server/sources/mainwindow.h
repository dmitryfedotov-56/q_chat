#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_Database(Database* dptr);
    void set_MessWindow(QWidget* w);

private slots:

    void on_messageButton_clicked();

    void on_lockButton_clicked();

    void on_unlockButton_clicked();

private:
    Ui::MainWindow *ui;
    Database* db = nullptr;
    QWidget* messwindow;
};

void database_Error();

#endif // MAINWINDOW_H
