#include "jennytexteditor.h"

#include <QApplication>

/*Name: Jenny Itza
  ID: 2020152085
  Course: CMPS2212 - GUI Programming
  Project Description: Implementing a notepad with diffrent functionalities including translatioin
  Date: 3/16/2022
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Jenny Itza Text Editor");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("University of Belize CMPS2212: GUI Programming");
    a.setOrganizationDomain("online.ub.edu.bz");
    JennyTextEditor w;
    w.show();
    return a.exec();
}
