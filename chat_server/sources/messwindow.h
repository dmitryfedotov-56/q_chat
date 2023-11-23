#ifndef MESSWINDOW_H
#define MESSWINDOW_H

#include <QMainWindow>
#include "database.h"

namespace Ui {
class MessWindow;
}

class MessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessWindow(QWidget *parent = nullptr);
    ~MessWindow();

    void set_Database(Database* dptr);
    void set_MainWindow(QWidget* w);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MessWindow *ui;
    Database* db = nullptr;
    QWidget* mainwindow;
};

#endif // MESSWINDOW_H
