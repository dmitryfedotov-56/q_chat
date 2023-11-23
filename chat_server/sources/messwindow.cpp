#include "messwindow.h"
#include "ui_messwindow.h"
#include <QDebug>

//=========================================================================
//                           Window Constructor
//=========================================================================

MessWindow::MessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessWindow)
{
    ui->setupUi(this);
    /*
    ui->messageList->addItem("user0 -> user1 : Hello, user1");
    ui->messageList->addItem("user1 -> user0 : Hello, user0");
    */
}

MessWindow::~MessWindow()
{
    delete ui;
}

void MessWindow::set_Database(Database* dptr)
{
    db = dptr;  // store db pointer

    vector<Message> messages;
    std::string s;
    messages = db->getAllMessages();    // get all messages

    for(auto message : messages)        // fill in the list of messages
    {
        s = message.getSender();
        s += " -> ";
        if( message.getDest() == -1)
             s += "chat  ";
        else
             s += db->getUserName(message.getDest());
        s += " : ";
        s += message.getText();
        ui->messageList->addItem(s.c_str());
     };
 }

void MessWindow::set_MainWindow(QWidget* w)
{
    mainwindow = w;
};

//=========================================================================
//                           Window Buttons
//=========================================================================

void MessWindow::on_pushButton_clicked()
{
    this->close();
    mainwindow->show();
}

//=========================================================================
