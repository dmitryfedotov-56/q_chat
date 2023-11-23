#include <iostream>
#include "CommandLineInterface.h"
#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"
#include "messwindow.h"
using namespace std;

#include <QDebug>

//=========================================================================
//                               Database Test
//=========================================================================

void db_Test(Database* db)
{
    vector<string> users = db->getUserList();
    for( auto s: users)qDebug() << s.c_str();

    int id = db->searchUserByName("user4");
    qDebug() << id;

    string name = db->getUserName(1);
    qDebug() << name.c_str();

    vector<Message>messages = db->getAllMessages();
    for( auto message : messages)
    {
        qDebug() << message.getSender().c_str() << " " << message.getText().c_str();
    };

    // db->addUser("user4", "pass4");

    id = db->checkPassword("user4", "pass4");
    qDebug() << id;

    // db->addChatMessage("user4", "test");

    // db->addPrivateMessage("user4", "user0", "Hello, user0!");

    qDebug() << "get chat messages";
    vector<string> chatmess = db->getChatMessages();
    for( auto s : chatmess)qDebug() << s.c_str();

    qDebug() << "get private messages";
    messages = db->getPrivateMessage(2);
    for( auto message : messages)
    {
         qDebug() << message.getSender().c_str() << " " << message.getText().c_str();
    };
};

//=========================================================================
//                               Main
//=========================================================================

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   Database db;
/*
   if(!db.database_Open())
   {
       qDebug() << "database error";
       database_Error();
       exit(0);
   };
*/
   // db_Test(&db);
   // return 0;
   MainWindow mainwindow;
   MessWindow messwindow;
   mainwindow.set_MessWindow(&messwindow);
   messwindow.set_MainWindow(&mainwindow);
   mainwindow.set_Database(&db);
   messwindow.set_Database(&db);
   mainwindow.show();

//	cout << "Usage:" << endl;
//	cout << "!login[username][password] - log in" << endl;
//	cout << "!logout - log out" << endl;
//	cout << "!register[username][password] - register new user" << endl;
//	cout << "!exit for exit" << endl;
//	cout << "!pm[(username)] message - write private message" << endl;
//	cout << "!getPM - receive private messages" << endl;
//  CommandLineInterface cli{};
//  cout << "chat ver. 0.1beta" << endl;
//  while (!cli->shouldExit())
//  {
//    cli->parseCommand();
//  }
/*
  QTranslator myappTranslator;
  myappTranslator.load("translations/my_ru.qm");
  a.installTranslator(&myappTranslator);

  QTranslator defaultTranslator;
  defaultTranslator.load("translations/qt_ru.qm");
  a.installTranslator(&defaultTranslator);

  MainWindow::createClient();
*/
  return a.exec();
}

//=========================================================================



