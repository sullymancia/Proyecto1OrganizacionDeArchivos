#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CrearFile()
{
    //Crear el archivo con la direccion
     Archivo.setFileName(OpenFilePath);
     //Se abre el archivo binario y se verifica que este abierto
    if(Archivo.open(QIODevice::WriteOnly)){
        QDataStream out(&Archivo);
       // out.setVersion(QDataStream::Qt_4_9);
        //se ingresa un valor que despues de estar en el archivo se va a castear a binary
        //out<<"hola como estas";
        //se guarda pero no se cierra el archivo para seguir usando
        Archivo.flush();
    }

}

char* MainWindow::ReadFile()
{
    char* DataFile;
    //Crear el archivo con la direccion
     Archivo.setFileName(OpenFilePath);
     //Se abre el archivo binario y se verifica que este abierto
    if(!Archivo.open(QIODevice::ReadOnly)){
        qDebug ()<<"NO SE PUDO ABRIR";
    }else{

        QDataStream in(&Archivo);
        //se lee pero no se cierra el archivo para seguir usando
       in>>DataFile;
    }
    return DataFile;
}

void MainWindow::on_OpenFile_clicked()
{
    //se abre la dialogo de la ventana de busqueda de windows para abrir el archivo
    OpenFilePath = QFileDialog::getOpenFileName(this,"open File","C//","All file (*.*);;Text File (*.txt);;Music (*.mp3)");
    QMessageBox::information(this,tr("File Name"),OpenFilePath);
    qDebug()<<ReadFile();
}


void MainWindow::on_CreatFile_clicked()
{
    //se abre la dialogo de la ventana de busqueda de windows para abrir el guardar el archivo
    OpenFilePath = QFileDialog::getSaveFileName(this,"open File","C//","All file (*.*);;Text File (*.txt);;Music (*.mp3)");
    QMessageBox::information(this,tr("File Name"),OpenFilePath);
    CrearFile();
}
