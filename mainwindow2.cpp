#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QTextStream>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->plainTextEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNuevo_triggered()
{
    QString currentFileName;

    if (ui->plainTextEdit->document()->isModified()) {
        // Verificar si hay cambios sin guardar en el documento actual
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Guardar cambios", "Desea guardar los cambios del documento actual?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            on_actionGuardar_triggered(); // Guardar los cambios del documento actual
        } else if (reply == QMessageBox::Cancel) {
            return; // Cancelar la creación del nuevo documento
        }
    }

    // Cerrar el documento actual
    ui->plainTextEdit->clear();
    currentFileName.clear();

    // Abrir el nuevo documento en blanco
    setWindowTitle("Nuevo Documento");
}


void MainWindow::on_actionNueva_ventana_triggered()
{
    MainWindow *newWindow = new MainWindow(); // Crear una nueva instancia de MainWindow
    newWindow->show(); // Mostrar la nueva ventana
}


void MainWindow::on_actionAbrir_triggered()
{
    QFile doc; // Objeto QFile para manipular el archivo
    QTextStream io; // Objeto QTextStream para leer el contenido del archivo
    QString nameDoc; // Variable para almacenar el nombre del archivo seleccionado

    nameDoc = QFileDialog::getOpenFileName(this, "Abrir"); // Abrir cuadro de diálogo para seleccionar un archivo

    if (nameDoc.isEmpty())
        return; // Si no se selecciona ningún archivo, se retorna sin hacer nada

    doc.setFileName(nameDoc); // Establecer el nombre del archivo en el objeto QFile
    doc.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text); // Abrir el archivo en modo de solo lectura y escritura

    if (!doc.isOpen()){
        QMessageBox::critical(this, "Error", doc.errorString()); // Si ocurre un error al abrir el archivo, mostrar un mensaje de error
    }

    io.setDevice(&doc); // Establecer el dispositivo de lectura en el objeto QTextStream
    ui->plainTextEdit->setPlainText(io.readAll()); // Leer todo el contenido del archivo y establecerlo en el campo de texto

    doc.close(); // Cerrar el archivo
}


void MainWindow::on_actionGuardar_triggered()
{
    QString currentFileName; // Variable para almacenar el nombre de archivo actual
    if (currentFileName.isEmpty()) {
        on_actionGuardar_como_triggered(); // Si no se ha guardado previamente, abre el cuadro de diálogo "Guardar como"
        return;
    }

    QFile doc(currentFileName); // Objeto QFile para manipular el archivo utilizando el nombre de archivo actual
    if (!doc.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", doc.errorString()); // Si ocurre un error al abrir el archivo, mostrar un mensaje de error
        return;
    }

    QTextStream io(&doc); // Objeto QTextStream para escribir en el archivo
    io << ui->plainTextEdit->toPlainText(); // Escribir el contenido del campo de texto en el archivo
    doc.flush(); // Vaciar el búfer y asegurarse de que todos los datos se escriban en el archivo
    doc.close(); // Cerrar el archivo
}


void MainWindow::on_actionGuardar_como_triggered()
{
    QFile doc; // Objeto QFile para manipular el archivo
    QTextStream io; // Objeto QTextStream para escribir en el archivo
    QString nameDoc; // Variable para almacenar el nombre de archivo

    nameDoc = QFileDialog::getSaveFileName(this, "Guardar"); // Abrir cuadro de diálogo para seleccionar la ruta de guardado

    if (nameDoc.isEmpty())
        return; // Si no se selecciona una ruta de guardado, se retorna sin hacer nada

    doc.setFileName(nameDoc); // Establecer el nombre del archivo en el objeto QFile
    doc.open(QIODevice::WriteOnly | QIODevice::Text); // Abrir el archivo en modo de solo escritura

    if (!doc.isOpen()){
        QMessageBox::critical(this, "Error", doc.errorString()); // Si ocurre un error al abrir el archivo, mostrar un mensaje de error
        return;
    }

    io.setDevice(&doc); // Establecer el dispositivo de escritura en el objeto QTextStream
    io << ui->plainTextEdit->toPlainText(); // Escribir el contenido del campo de texto en el archivo

    doc.flush(); // Vaciar el búfer y asegurarse de que todos los datos se escriban en el archivo
    doc.close(); // Cerrar el archivo
}


void MainWindow::on_actionSalir_triggered()
{
    close();
}


void MainWindow::on_actionCortar_triggered()
{
    ui->plainTextEdit->cut();
}


void MainWindow::on_actionCopiar_triggered()
{
    ui->plainTextEdit->copy();
}


void MainWindow::on_actionPegar_triggered()
{
    ui->plainTextEdit->paste();
}


void MainWindow::on_actionEliminar_triggered()
{
    // Obtén el objeto QPlainTextEdit correspondiente
    QPlainTextEdit* plainTextEdit = ui->plainTextEdit;

    // Verifica si se ha seleccionado texto
    if (!plainTextEdit->textCursor().selectedText().isEmpty())
    {
        // Elimina el contenido seleccionado
        QTextCursor cursor = plainTextEdit->textCursor();
        cursor.removeSelectedText();
    }
}


void MainWindow::on_actionAyuda_triggered()
{
    QString txt;
    txt = "Ayuda!\n:'c";
    QMessageBox::about(this, "Ayuda", txt);
}


void MainWindow::on_actionCC_triggered()
{
    QString txt;
    txt = "Mención a los autores de los iconos que fueron tomados para el proyecto";
    QMessageBox::about(this, "Todos los autores", txt);
}


void MainWindow::on_actionBy_triggered()
{
    QString txt;
    txt  = "Autor: Ruben David Renteria Cruz\n";
    txt += "Autor: Santiago Mazuera";
    QMessageBox::about(this, "Blog de Notas", txt);
}

