#include "mainwindow.h"  // Incluye la declaración de la clase MainWindow
#include "./ui_mainwindow.h"  // Incluye el archivo de diseño de interfaz de usuario para la clase MainWindow
#include <QMessageBox>  // Proporciona funcionalidad para mostrar mensajes de diálogo
#include <QFileDialog>  // Proporciona un cuadro de diálogo para abrir y guardar archivos
#include <QPushButton>  // Proporciona un botón interactivo
#include <QTextStream>  // Proporciona operaciones de lectura y escritura en streams de texto
#include <QFile>  // Proporciona operaciones para leer y escribir archivos
#include <QDir>  // Proporciona operaciones para manipular directorios y rutas de archivos
#include <QStandardPaths>  // Proporciona rutas estándar predefinidas en el sistema
#include <QDateTime>  // Proporciona funciones para trabajar con fechas y horas
#include "../ProyectoFinal/midocs.h"
#include <iostream>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QTextEdit>
using namespace std;


// Constructor de la clase MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->plainTextEdit); // Establece el widget de edición de texto como el widget central de la ventana principal
}

// Destructor de la clase MainWindow
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
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString folderPath = documentsPath + "/DocumentBlogs/LogDocs";

    QString nameDoc = QFileDialog::getOpenFileName(this, "Abrir", folderPath); // Abrir cuadro de diálogo para seleccionar un archivo en la carpeta DocumentBlogs

    if (nameDoc.isEmpty())
        return; // Si no se selecciona ningún archivo, se retorna sin hacer nada

    QFile doc(nameDoc); // Objeto QFile para manipular el archivo

    if (!doc.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", doc.errorString()); // Si ocurre un error al abrir el archivo, mostrar un mensaje de error
        return;
    }

    QTextStream io(&doc); // Objeto QTextStream para leer el contenido del archivo
    ui->plainTextEdit->setPlainText(io.readAll()); // Leer todo el contenido del archivo y establecerlo en el campo de texto

    doc.close(); // Cerrar el archivo

    setCurrentFileName(nameDoc); // Establecer el nombre del archivo seleccionado como el nombre actual del archivo
}

void MainWindow::on_actionGuardar_triggered()
{
    QString currentFileName = getCurrentFileName();

    if (currentFileName.isEmpty()) {
        on_actionGuardar_como_triggered();
        return;
    }

    QFile doc(currentFileName);
    if (!doc.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se puede abrir el archivo para escritura. Verifica los permisos de acceso.");
        return;
    }

    QTextStream io(&doc);
    io << ui->plainTextEdit->toPlainText();
    doc.flush();
    doc.close();

    if (doc.error() != QFile::NoError) {
        QMessageBox::critical(this, "Error", "Error al escribir en el archivo. Verifica los permisos de acceso.");
    }
}


void MainWindow::on_actionGuardar_como_triggered()
{
    QString currentFileName = getCurrentFileName();

    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString folderPath = documentsPath + "/DocumentBlogs/LogDocs";
    QDir folderDir(folderPath);

    if (!folderDir.exists()) {
        if (!folderDir.mkpath(folderPath)) {
            QMessageBox::critical(this, "Error", "No se puede crear la carpeta de documentos. Verifica los permisos de acceso.");
            return;
        }
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString year = QString::number(currentDateTime.date().year());
    QString month = QString("%1").arg(currentDateTime.date().month(), 2, 10, QLatin1Char('0'));
    QString day = QString("%1").arg(currentDateTime.date().day(), 2, 10, QLatin1Char('0'));
    QString hour = QString("%1").arg(currentDateTime.time().hour(), 2, 10, QLatin1Char('0'));
    QString minute = QString("%1").arg(currentDateTime.time().minute(), 2, 10, QLatin1Char('0'));
    QString subfolderPath = folderPath + "/" + year + "/" + month + "/" + day;
    QDir subfolderDir(subfolderPath);

    if (!subfolderDir.exists()) {
        if (!subfolderDir.mkpath(subfolderPath)) {
            QMessageBox::critical(this, "Error", "No se puede crear la subcarpeta de documentos. Verifica los permisos de acceso.");
            return;
        }
    }

    if (currentFileName.isEmpty()) {
        currentFileName = subfolderPath + "/Document_" + year + "-" + month + "-" + day + "_" + hour + "_" + minute + ".txt";
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar como", currentFileName, "Archivos de texto (*.txt)");

    if (fileName.isEmpty()) {
        return;
    }

    setCurrentFileName(fileName);

    QFile doc(fileName);
    if (!doc.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se puede abrir el archivo para escritura. Verifica los permisos de acceso.");
        return;
    }

    QTextStream io(&doc);
    io << ui->plainTextEdit->toPlainText();
    doc.flush();
    doc.close();

    if (doc.error() != QFile::NoError) {
        QMessageBox::critical(this, "Error", "Error al escribir en el archivo. Verifica los permisos de acceso.");
    }
}


QString MainWindow::getCurrentFileName() const
{
    return currentFileName;
}

void MainWindow::setCurrentFileName(const QString& fileName)
{
    currentFileName = fileName;
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

    // Sección "Archivo"
    txt = "Sección \"Archivo\":\n\n"
          "En la ventana principal del programa MainWindow, encontrarás varios botones y funcionalidades relacionadas con la manipulación de archivos. A continuación, se detallan cada uno de ellos junto con una explicación detallada de su función:\n\n"
          "Botón \"Nuevo\":\n"
          "Función: Crea un nuevo documento en blanco para comenzar a escribir una nueva nota.\n"
          "Atajo de teclado: Ctrl+N.\n"
          "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se abrirá un nuevo documento en blanco en el área de edición del programa. Aquí puedes comenzar a escribir tu nueva nota.\n\n"
          "Botón \"Nueva ventana\":\n"
          "Función: Abre una nueva ventana de MainWindow, lo que te permite trabajar en varias notas al mismo tiempo.\n"
          "Atajo de teclado: Ctrl+Shift+N.\n"
          "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se abrirá una nueva ventana de MainWindow. Esto te permite tener múltiples notas abiertas al mismo tiempo y trabajar en ellas de forma independiente.\n\n"
          "Botón \"Abrir\":\n"
          "Función: Permite seleccionar y abrir un archivo existente para editar una nota previamente guardada.\n"
          "Atajo de teclado: Ctrl+A.\n"
          "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se abrirá un cuadro de diálogo de \"Abrir archivo\". Aquí puedes buscar y seleccionar el archivo que deseas abrir. Una vez seleccionado, el contenido del archivo se cargará en el área de edición del programa, lo que te permite editar la nota existente.\n\n"
          "Botón \"Guardar\":\n"
          "Función: Guarda los cambios realizados en la nota actual.\n"
          "Atajo de teclado: Ctrl+G.\n"
          "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se guardarán los cambios realizados en la nota actual. Si la nota ya tiene un archivo asociado, los cambios se guardarán en ese archivo. Si es una nueva nota sin guardar, se abrirá el cuadro de diálogo \"Guardar archivo\" para que puedas especificar un nombre y ubicación para guardar el archivo.\n\n"
          "Botón \"Guardar como\":\n"
          "Función: Permite guardar la nota actual con un nombre o ubicación diferente.\n"
          "Atajo de teclado: Ctrl+Shift+S.\n"
          "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se abrirá el cuadro de diálogo \"Guardar archivo\". Aquí puedes especificar un nombre y ubicación para guardar el archivo actual. Esto te permite guardar la nota con un nombre o ubicación diferente sin afectar la versión original.\n\n"
          "Botón \"Salir\":\n"
          "Función: Cierra la aplicación.\n"
          "Atajo de teclado: Ctrl+Q.\n"
          "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se cerrará la aplicación MainWindow. Asegúrate de haber guardado tus cambios antes de salir, ya que cualquier cambio no guardado se perderá.\n\n"
          "Estas son las funcionalidades principales dentro de la sección \"Archivo\" en la ventana principal del programa MainWindow. Cada botón y función te proporciona opciones para crear, abrir, guardar y cerrar notas de manera eficiente. Utiliza estas funcionalidades de acuerdo a tus necesidades para administrar tus notas de manera efectiva.\n\n";

    // Sección "Editar"
    txt += "Sección \"Editar\":\n\n"
           "Dentro de la sección \"Editar\" en la ventana principal del programa MainWindow, encontrarás varios botones y funcionalidades relacionadas con la manipulación y edición del texto en el área de edición. A continuación, se detallan cada uno de ellos junto con una explicación detallada de su función:\n\n"
           "Botón \"Cortar\":\n"
           "Función: Elimina el texto seleccionado y lo copia al portapapeles.\n"
           "Atajo de teclado: Ctrl+X.\n"
           "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, el texto seleccionado en el área de edición se eliminará y se copiará al portapapeles. Esto te permite mover o transferir el texto seleccionado a otro lugar o documento.\n\n"
           "Botón \"Copiar\":\n"
           "Función: Copia el texto seleccionado al portapapeles.\n"
           "Atajo de teclado: Ctrl+C.\n"
           "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se copiará el texto seleccionado en el área de edición al portapapeles. Esto te permite duplicar el texto seleccionado sin eliminarlo del documento actual.\n\n"
           "Botón \"Pegar\":\n"
           "Función: Inserta el contenido del portapapeles en la posición del cursor.\n"
           "Atajo de teclado: Ctrl+V.\n"
           "Acción: Al hacer clic en este botón o utilizar el atajo de teclado correspondiente, se insertará el contenido del portapapeles en la posición del cursor en el área de edición. Esto te permite agregar texto previamente copiado o cortado desde otro lugar o documento.\n\n"
           "Botón \"Eliminar\":\n"
           "Función: Elimina el texto seleccionado sin copiarlo al portapapeles.\n"
           "Atajo de teclado: Supr o Delete.\n"
           "Acción: Al hacer clic en este botón o presionar la tecla \"Supr\" o \"Delete\", el texto seleccionado en el área de edición se eliminará sin copiarlo al portapapeles. Esto te permite eliminar rápidamente el texto seleccionado sin tener que copiarlo previamente.\n\n"
           "Estas son las funcionalidades principales dentro de la sección \"Editar\" en la ventana principal del programa MainWindow. Cada botón te brinda opciones para cortar, copiar, pegar y eliminar texto de manera eficiente. Utiliza estas funcionalidades según tus necesidades para editar y manipular el contenido de tu nota de forma efectiva.\n\n";

    // Sección "Ayuda"
    txt += "Sección \"Ayuda\":\n\n"
           "Dentro de la sección \"Ayuda\" en la ventana principal del programa MainWindow, encontrarás un botón con funcionalidades relacionadas con la asistencia y la obtención de información adicional sobre el programa. A continuación, se explica en detalle cada uno de ellos:\n\n"
           "Botón \"Ayuda\":\n"
           "Función: Proporciona información de ayuda sobre el programa.\n"
           "Acción: Al hacer clic en este botón, se mostrará un cuadro de diálogo con información de ayuda. Puede contener instrucciones generales sobre el uso del programa, consejos, advertencias o cualquier otra información relevante para ayudarte a comprender y utilizar correctamente el programa.\n\n"
           "Botón \"CC\" (Créditos):\n"
           "Función: Muestra los créditos de los autores de los iconos utilizados en el proyecto.\n"
           "Acción: Al hacer clic en este botón, se mostrará un cuadro de diálogo que contiene los nombres de los autores de los iconos utilizados en el programa. Esta función es útil para dar reconocimiento y agradecimiento a los creadores de los recursos visuales utilizados en la interfaz del programa.\n\n"
           "Botón \"By\" (Acerca de):\n"
           "Función: Muestra información sobre el autor del programa y posibles coautores.\n"
           "Acción: Al hacer clic en este botón, se mostrará un cuadro de diálogo que contiene información sobre el autor del programa, como su nombre. También puede incluir información adicional sobre coautores si los hubiera. Esta función te permite conocer quién desarrolló el programa y mostrar reconocimiento a las personas involucradas en su creación.\n\n"
           "En resumen, la sección \"Ayuda\" en la ventana principal del programa MainWindow proporciona acceso a información de ayuda, créditos de los autores de los iconos utilizados y detalles sobre el autor del programa. Estas funcionalidades están diseñadas para brindarte asistencia adicional y ayudarte a comprender mejor el programa, su origen y las personas involucradas en su desarrollo.\n\n";

    // Crear el cuadro de diálogo de Ayuda
    QDialog dialog(this);
    dialog.setWindowTitle("Ayuda");
    dialog.setMinimumSize(400, 400);

    // Crear un widget de texto y agregarlo al QScrollArea
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(txt);

    // Crear un QScrollArea y agregar el widget de texto a él
    QScrollArea scrollArea(&dialog);
    scrollArea.setWidget(textEdit);
    scrollArea.setWidgetResizable(true);

    // Crear un layout vertical y agregar el QScrollArea a él
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addWidget(&scrollArea);

    // Mostrar el cuadro de diálogo
    dialog.exec();
}


void MainWindow::on_actionCC_triggered()
{
    QString txt;
    txt = "Mención a los autores de los iconos que fueron tomados para el proyecto\n";

    QDialog dialog(this);
    dialog.setWindowTitle("Todos los autores");
    dialog.setMinimumSize(400, 400);

    // Crear un widget de texto y agregarlo al QScrollArea
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(txt);

    // Crear un QScrollArea y agregar el widget de texto a él
    QScrollArea scrollArea(&dialog);
    scrollArea.setWidget(textEdit);
    scrollArea.setWidgetResizable(true);

    // Crear un layout vertical y agregar el QScrollArea a él
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addWidget(&scrollArea);

    // Mostrar el cuadro de diálogo
    dialog.exec();
}


void MainWindow::on_actionBy_triggered()
{
    QString txt;
    txt  = "Autor: Ruben David Renteria Cruz\n";
    txt += "Autor: Santiago Mazuera\n\nAgradeciemientos:\nCoautor: Julian Polo";

    QDialog dialog(this);
    dialog.setWindowTitle("Creadores y autores intelectuales del proyecto");
    dialog.setMinimumSize(400, 400);

    // Crear un widget de texto y agregarlo al QScrollArea
    QTextEdit* textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(txt);

    // Crear un QScrollArea y agregar el widget de texto a él
    QScrollArea scrollArea(&dialog);
    scrollArea.setWidget(textEdit);
    scrollArea.setWidgetResizable(true);

    // Crear un layout vertical y agregar el QScrollArea a él
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->addWidget(&scrollArea);

    // Mostrar el cuadro de diálogo
    dialog.exec();
}


void MainWindow::on_actionGestor_de_Archivos_triggered()
{
    MiDocs miDocs;
    miDocs.setModal(true);
    miDocs.exec();
}

