#include "jennytexteditor.h"
#include "ui_jennytexteditor.h"

/*Name: Jenny Itza
  ID: 2020152085
  Course: CMPS2212 - GUI Programming
  Project Description: Implementing a notepad with diffrent functionalities including translatioin
  Date: 3/16/2022
*/

JennyTextEditor::JennyTextEditor(const QString &fileName, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::JennyTextEditor)

{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);                   //Set Text Editor as the central widget
    setWindowIcon(QIcon(":/images/new.png"));               //Set windows' icon to an external image

    //Connectors
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(onActionNew_triggered()));
    connect(ui->actionClose, SIGNAL(triggered()),this, SLOT(close()));
    connect(ui->actionOpen, SIGNAL(triggered()),this, SLOT(onActionOpen_triggered()));
    connect(ui->actionExit, SIGNAL(triggered()),QCoreApplication::instance(), SLOT(closeAllWindows()));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(documentModified()));
    connect(ui->actionSelectFont, SIGNAL(triggered()), this, SLOT(onActionSelectFont_triggered()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(aboutQt()));
    connect(ui->actionCut, SIGNAL(triggered()), ui->textEdit, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), ui->textEdit, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), ui->textEdit, SLOT(paste()));
    connect(ui->actionUndo, SIGNAL(triggered()), ui->textEdit, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), ui->textEdit, SLOT(redo()));
    connect(ui->actionCopy, SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(cutAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onActionSave_triggered()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(onActionSaveAs_triggered()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));
    connect(ui->actionToolbar, SIGNAL(triggered()), this, SLOT(toolbar()));

    setWindowTitle("Text Editor [*]"); //Set's window's title
    loadFile(fileName);               //Loads file with fileName

    if (appTranslator.load(":/test_en.qm"))
          qApp->installTranslator(&appTranslator);   //Installing  file translators

    // Actions to get the translation of the text editor application
        QActionGroup *languageActionGroup = new QActionGroup(ui->menuLanguage);         // pointer to an object that will group several actions
        languageActionGroup->addAction(ui->actionEnglish);                              // add the English and Spanish QAction to the object before created
        languageActionGroup->addAction(ui->actionSpanish);

        connect(languageActionGroup, SIGNAL(triggered(QAction *)),this, SLOT(switchLanguage(QAction *)));
}

JennyTextEditor::~JennyTextEditor()
{
    delete ui;
}

void JennyTextEditor::closeEvent(QCloseEvent *e)
{

    if(this->isWindowModified())   //If the window is modified show warning before closing
    {
     switch(QMessageBox::warning(this, tr("Document Modified"),
            tr("The document has been modified. "
            "Do you want to save your changes?\n"
            "You will lose and unsaved changes."),
             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
             QMessageBox::Cancel))
        {
            case QMessageBox::Yes:
                onActionSave_triggered();
                e->ignore();
            break;
            case QMessageBox::No:
                e->accept();
            break;
            case QMessageBox::Cancel:
                 e->ignore();
            break;

            default:
            //should never be reached
            break;
        }
     }
     else
     {
        e->accept();
     }
}

void JennyTextEditor::onActionNew_triggered()
{
    JennyTextEditor *n = new JennyTextEditor;  //Creates new window object and displays it
    n->show();
}

void JennyTextEditor::documentModified()
{
    setWindowModified(true);    //Window has been modified

}

void JennyTextEditor::onActionSelectFont_triggered()
{
    bool fontSelected;    //Store's user option

    QFont font = QFontDialog::getFont(&fontSelected, this);

    if(fontSelected){
        ui->textEdit->setFont(font);    //Changes window's text to the new font style
    }
}

void JennyTextEditor::loadFile(const QString &fileName)
{
    if(fileName.isEmpty()){
        setFileName(QString());
        return;

    }

    QFile file(fileName);                                   //Creates file object and if it didn't load properly displays warning

    if(!file.open((QIODevice::ReadOnly) | QFile::Text))
    {
        QMessageBox::warning(this, "..", "File not opened.");
        setFileName(QString());
        return;
    }

    QTextStream fileStream(&file);
    QString content = fileStream.readAll();                   //Reads file's content

    savedText = content;
    ui->textEdit->setText(content);                           //Displays it on the current window
    file.close();

    setFileName(fileName);
    setWindowModified(false);                                 //Indicates that the window hasn't been modified

}

void JennyTextEditor::setFileName(const QString &fileName)   //Accepts one parameter of type QString
{
    mFileName = fileName;                                    //The parameter is the passed to the current file name variable
    this->setWindowTitle(QString("%1[*] - %2")
                    .arg(mFileName.isNull()? tr("Untitled"): QFileInfo(mFileName).fileName(), //Validating mFileName
                              QApplication::applicationName()));

}

void JennyTextEditor::onActionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                       "Open document", QDir::currentPath(), "Text documents (*.txt)"); //Opens a file and get's its file name
    if(fileName.isNull()){
        return;
    }
    if(mFileName.isNull() && savedText == ui->textEdit->toPlainText()){      //Verifies if the current file name is empty and if the current window hasn't been modified

        loadFile(fileName);
        statusBar()->showMessage("File Opened", 2000);
    }
    else{
    JennyTextEditor *n = new JennyTextEditor(fileName, this);         //Creating new window with the filename
    n->show();

    }

}

bool JennyTextEditor::onActionSave_triggered()
{
    if(mFileName.isNull()){
        bool saveAs = onActionSaveAs_triggered();
        return saveAs;
    }
    QFile file(mFileName);     //Creating new file object with the current file name

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {        //Verifies if file wasn't opened
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        setFileName(QString());
        return false;
    }
    QTextStream fileStream(&file);
    QString text = ui->textEdit->toPlainText();          //The current window's content is saved on a variable

    fileStream << text;                                //Then it is passed to the fileSteam object
    statusBar()->showMessage("File Saved", 2000);

    file.close();
    setWindowModified(false);
    return true;
}

bool JennyTextEditor::onActionSaveAs_triggered()     //These two slots didn't crash on each other becasue I stored their return type in anoher variable and returned that variable instead
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Document"),
                                                    mFileName.isNull() ? QDir::currentPath() : mFileName,
                                                    "Text document (*.txt)");

    if(filename.isNull()){
        return false;
    }
    setFileName(filename);
    bool save = onActionSave_triggered();
    return save;

}

void JennyTextEditor::print()   //Printing of the document
{
    QTextDocument *document = ui->textEdit->document();

    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog * printDialog = new QPrintDialog(&printer, this);

    if(printDialog->exec() == QDialog::Accepted){
    document->print(&printer);
    statusBar()->showMessage("Document Printed", 2000);
    }
}

void JennyTextEditor::switchLanguage(QAction * action)
{

    qApp->removeTranslator(&appTranslator);                             // removes the current translation file

    QString locale;

    if (action->text() == "Spanish" || action->text() == "Espanol")     // check if the action selected is to change the language to Spanish
        locale = "es";                                                  // and assign the corresponding string
    else
        locale = "en";

    if (appTranslator.load(":/test_" + locale + ".qm"))
        qApp->installTranslator(&appTranslator);                            // add the translation file to be used

    ui->retranslateUi(this);                                            // perform the translation

    QString Lang = action->text();
    ui->statusbar->showMessage(tr("Language has changed to %1").arg(Lang), 1000);

}

void JennyTextEditor::toolbar()
{
//    if(ui->toolBar->isVisible()){
//        ui->toolBar->hide();
//      }
//     ui->toolBar->show();

       ui->toolBar->hide();
}



