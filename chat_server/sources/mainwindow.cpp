#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "messwindow.h"
#include <vector>
#include <string>
#include <QMessageBox>
#include <QDebug>

//=========================================================================
//                           Window Constructor
//=========================================================================

void database_Error()
{
    QMessageBox msgBox;               // error message
    msgBox.setWindowTitle("Error");
    msgBox.setText("database error!");
    msgBox.exec();
};

//=========================================================================
//                           Window Constructor
//=========================================================================

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::set_Database(Database* dptr)
{
    const char* name;
    std::string username;
    db = dptr;
    vector<std::string>users;
    users = db->getUserList();                      // fill in the list of users
    for(int i = 0; i < (int)users.size(); i++)
    {
        name = users[i].c_str();
        ui->userList->addItem(name);
        if(db->userLocked(username)){};
    };

    int count = ui->userList->count();
    for(int i = 0; i < count; i++)
    {
        QListWidgetItem * item = ui->userList->item(i);
        name = users[i].c_str();
        username = name;
        if(db->userLocked(username))
             item->setForeground(Qt::red);  // mark with red
    };

}

void MainWindow::set_MessWindow(QWidget* w)
{
    messwindow = w;
};

MainWindow::~MainWindow()
{
    delete ui;
}

//=========================================================================
//                           Window Buttons
//=========================================================================

void MainWindow::on_messageButton_clicked()     // open message list
{
    // ui->messageButton->setText("OK");
    this->close();
    messwindow->show();
}

static void userNotChosen()
{
    QMessageBox msgBox;               // error message
    msgBox.setWindowTitle("Error");
    msgBox.setText("User not chosen");
    msgBox.exec();
};

static void alreadyDone(bool locked)
{
    std::string text = "User is already ";
    if(locked)
        text += "locked";
    else
        text += "unlocked";

    QMessageBox msgBox;               // error message
    msgBox.setWindowTitle("Error");
    msgBox.setText(text.c_str());
    msgBox.exec();
};

void MainWindow::on_lockButton_clicked()        // lock user
{
  QListWidgetItem *item = ui->userList->currentItem();

  if(item == nullptr)                   // not chosen?
  {
      userNotChosen();
  } else                                // user is chosen
  {
      QMessageBox msgBox;
      QString username = item->text();

      std::string name = username.toStdString();
        if(db->userLocked(name))
      {
          alreadyDone(true);
          return;
      };

      msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      msgBox.setDefaultButton(QMessageBox::Cancel);
      msgBox.setWindowTitle("Warning");
      QString message = "Do you really want to lock " + username + " ?";
      msgBox.setText(message);

      int res = msgBox.exec();          // get confirmation

      if (res == QMessageBox::Ok)       // if confirmed
      {
         db->lockUser(name);            // lock user
         item->setForeground(Qt::red);  // mark with red
      };
   };
}

void MainWindow::on_unlockButton_clicked()  // unlock user
{
    QListWidgetItem *item = ui->userList->currentItem();

    if(item == nullptr)                   // not chosen?
    {
        userNotChosen();
    } else                                // user is chosen
    {
        QMessageBox msgBox;
        QString username = item->text();

        std::string name = username.toStdString();
          if(!db->userLocked(name))
        {
            alreadyDone(false);
            return;
        };

        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setWindowTitle("Warning");
        QString message = "Do you really want to unlock " + username + " ?";
        msgBox.setText(message);

        int res = msgBox.exec();            // get confirmation

        if (res == QMessageBox::Ok)         // if confirmed
        {
           db->unlockUser(name);              // unlock user
           item->setForeground(Qt::black);  // mark as unlocked
        };
     };
}

//=========================================================================


