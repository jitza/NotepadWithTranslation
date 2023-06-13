#ifndef JENNYTEXTEDITOR_H
#define JENNYTEXTEDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>
#include <QFont>
#include <QFontDialog>
#include <QFile>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QTranslator>
#include <QActionGroup>

/*Name: Jenny Itza
  ID: 2020152085
  Course: CMPS2212 - GUI Programming
  Project Description: Implementing a notepad with diffrent functionalities including translatioin
  Date: 3/16/2022
*/

QT_BEGIN_NAMESPACE
namespace Ui { class JennyTextEditor; }
QT_END_NAMESPACE

class JennyTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    JennyTextEditor(const QString &fileName = QString(), QWidget *parent = nullptr);
    ~JennyTextEditor();

protected:
    void closeEvent(QCloseEvent *e);             //Implements a warning before closing a window

private slots:
    void onActionNew_triggered();                 //Opens new window
    void documentModified();                      //Checks if document has been modified
    void onActionSelectFont_triggered();          //Updated window's content font style
    void loadFile(const QString &fileName);       //Loads a file
    void setFileName(const QString &fileName);    //set window's file name
    void onActionOpen_triggered();                //Opens a file
    bool onActionSave_triggered();                //Saves a file
    bool onActionSaveAs_triggered();              //Save a file with a diffrent name
    void print();                                 //Prints file
    void switchLanguage(QAction *);               //Changes the application language
    void toolbar();                               //Toggles toolbar (Shows and Hides the window's toolbar)

private:
    Ui::JennyTextEditor *ui;
    QString savedText;               //Save the current window's content
    QString mFileName;               //Save the current window's file name
    QTranslator appTranslator;       //Save the translated files

};
#endif // JENNYTEXTEDITOR_H
