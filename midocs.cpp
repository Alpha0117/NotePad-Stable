#include "midocs.h"
#include "ui_midocs.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QIcon>
#include <QDebug>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QHash>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QFileSystemWatcher>
#include <QTimer>
#include <iostream>
using namespace std;

MiDocs::MiDocs(QWidget* parent)
    : QDialog(parent), ui(new Ui::MiDocs)
{
    cout << "**********************" << endl;
    cout << "MIDOCS" << endl;
    cout << "**********************" << endl;

    ui->setupUi(this);

    // Obtener la ruta de la carpeta "DocumentBlogs" dentro de "Documentos" de Windows
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    folderPath = documentsPath + QDir::separator() + "DocumentBlogs";

    // Crear el objeto QFileSystemWatcher para monitorear cambios en la carpeta y subcarpetas
    fileWatcher = new QFileSystemWatcher(this);
    fileWatcher->addPath(folderPath);
    QStringList subfolderPaths = getAllSubfolderPaths(folderPath);
    fileWatcher->addPaths(subfolderPaths);

    // Configurar el QTreeWidget
    ui->slotArbol->setColumnCount(1);
    QStringList headerLabels;
    headerLabels << "Archivos";
    ui->slotArbol->setHeaderLabels(headerLabels);

    // Obtener la lista inicial de archivos y carpetas en la carpeta y agregarlos al QTreeWidget
    addFilesAndFoldersToTreeWidget(folderPath, ui->slotArbol->invisibleRootItem());

    // Conectar las señales del QFileSystemWatcher para actualizar el QTreeWidget cuando ocurran cambios
    connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateTreeWidget(QString)));
    connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateTreeWidget(QString)));

    // Crear el temporizador para actualizar periódicamente el QTreeWidget
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTreeWidget()));
    updateTimer->start(1000); // Actualizar cada 1 segundo

    // En el constructor MiDocs::MiDocs()
    connect(ui->btnMonitor, SIGNAL(clicked()), this, SLOT(updateTreeWidget()));

    connect(ui->btnBorrar, SIGNAL(clicked()), this, SLOT(on_btnBorrar_clicked()));

}

MiDocs::~MiDocs()
{
    delete ui;
}

//_______________________________________________________________________________________________________________

void MiDocs::addFilesAndFoldersToTreeWidget(const QString& folderPath, QTreeWidgetItem* parentItem)
{
    QDir folderDir(folderPath);

    // Obtener la lista de archivos y carpetas en la carpeta
    QFileInfoList fileInfoList = folderDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    // Recorrer la lista de archivos y carpetas
    foreach (const QFileInfo& fileInfo, fileInfoList) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText(0, fileInfo.fileName());

        if (fileInfo.isDir()) {
            // Si es una carpeta, llamar recursivamente a la función para agregar los archivos y carpetas dentro de ella
            addFilesAndFoldersToTreeWidget(fileInfo.filePath(), item);
        } else if (fileInfo.suffix() == "txt") {
            // Si es un archivo .txt, puedes realizar alguna acción específica
            // Por ejemplo, agregarlo a una lista o realizar alguna operación sobre él
            // Aquí simplemente se muestra un mensaje en la consola
            qDebug() << "Archivo .txt encontrado:" << fileInfo.filePath();
        }

        parentItem->addChild(item);
    }
}

QStringList MiDocs::getAllSubfolderPaths(const QString& folderPath)
{
    QDir folderDir(folderPath);
    QStringList subfolderPaths;

    // Obtener la lista de carpetas en la carpeta
    QFileInfoList fileInfoList = folderDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Recorrer la lista de carpetas
    foreach (const QFileInfo& fileInfo, fileInfoList) {
        QString subfolderPath = fileInfo.filePath();
        subfolderPaths.append(subfolderPath);

        // Obtener las subcarpetas recursivamente
        QStringList recursiveSubfolderPaths = getAllSubfolderPaths(subfolderPath);
        subfolderPaths.append(recursiveSubfolderPaths);
    }

    return subfolderPaths;
}
QStringList MiDocs::getAllFilePaths(const QString& folderPath, const QString& filter)
{
    QDir folderDir(folderPath);
    QStringList filePaths = folderDir.entryList(QDir::Files, QDir::Name);

    if (!filter.isEmpty()) {
        QStringList filteredFilePaths;
        foreach (const QString& filePath, filePaths) {
            if (filePath.contains(filter, Qt::CaseInsensitive)) {
                filteredFilePaths.append(folderPath + QDir::separator() + filePath);
            }
        }
        return filteredFilePaths;
    }

    for (int i = 0; i < filePaths.size(); ++i) {
        filePaths[i] = folderPath + QDir::separator() + filePaths[i];
    }

    return filePaths;
}

void MiDocs::updateTreeWidget(const QString& path)
{
    // Limpiar el QTreeWidget
    ui->slotArbol->clear();

    // Volver a agregar los archivos y carpetas actualizados
    addFilesAndFoldersToTreeWidget(folderPath, ui->slotArbol->invisibleRootItem());
}

void MiDocs::addSubfoldersToWatcher(const QString& folderPath)
{
    QDir folderDir(folderPath);

    // Obtener la lista de subcarpetas en la carpeta
    QStringList subfolderPaths = getAllSubfolderPaths(folderPath);

    // Agregar cada subcarpeta al QFileSystemWatcher
    foreach (const QString& subfolderPath, subfolderPaths) {
        fileWatcher->addPath(subfolderPath);
    }
}


void MiDocs::toggleMonitoring()
{
    if (isMonitoringEnabled) {
        // Desconectar el QFileSystemWatcher
        disconnect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateTreeWidget(QString)));
        disconnect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateTreeWidget(QString)));

        // Actualizar el estado del botón y la variable de control
        ui->btnMonitor->setText("Habilitar Monitoreo");
        isMonitoringEnabled = false;
    } else {
        // Conectar el QFileSystemWatcher
        connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateTreeWidget(QString)));
        connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateTreeWidget(QString)));

        // Actualizar el estado del botón y la variable de control
        ui->btnMonitor->setText("Deshabilitar Monitoreo");
        isMonitoringEnabled = true;
    }
}

//_______________________________________________________________________________________________________________


void MiDocs::on_btnBuscar_clicked()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString logDocsPath = documentsPath + "/DocumentBlogs/LogDocs";

    QString filePath = QFileDialog::getOpenFileName(this, "Seleccionar archivo", logDocsPath, "Archivos de texto (*.txt)");

    if (!filePath.isEmpty()) {
        // Verificar que el archivo seleccionado esté en la carpeta "LogDocs" dentro de la carpeta de documentos
        if (filePath.startsWith(logDocsPath)) {
            selectedFilePath=filePath;
            // Mostrar solo el nombre del archivo en el QListWidget
            QString fileName = QFileInfo(filePath).fileName();
            ui->listWidget->addItem(fileName);
        } else {
            QMessageBox::warning(this, "Error", "El archivo seleccionado no se encuentra en la carpeta LogDocs de DocumentBlogs.");
        }
    }
}

void MiDocs::on_btnModificar_clicked()
{
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();
    if (selectedItem) {

        QString currentItemText = selectedItem->text();
        QString modifiedFileName = currentItemText;
        modifiedFileName.remove(".txt"); // Quitar la extensión .txt

        bool ok;
        modifiedFileName = QInputDialog::getText(this, "Modificar nombre", "Nuevo nombre:", QLineEdit::Normal, modifiedFileName, &ok);

        if (ok && !modifiedFileName.isEmpty()) {
            //Agregar la extensión .txt al nuevo nombre
            modifiedFileName += ".txt";
            QString fileName = QFileInfo(selectedFilePath).fileName();
            selectedItem->setText(modifiedFileName);

            // Mostrar ventana emergente con el mensaje de éxito
            QMessageBox::information(this, "Éxito", "El nombre temporal del archivo ha sido asignado exitosamente, para guardar los cambios permanentemente porfavor dar al boton de guardar.");
        }
    } else {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ningún archivo.");
    }
}

void MiDocs::on_btnBorrar_clicked()
{
    if (!selectedFilePath.isEmpty()) {
        // Mostrar un mensaje de confirmación antes de eliminar el archivo
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmación", "¿Deseas eliminar el archivo?", QMessageBox::Yes | QMessageBox::No);

        // Verificar la respuesta del usuario
        if (reply == QMessageBox::Yes) {
            // Verificar si el archivo original existe
            if (QFile::exists(selectedFilePath)) {
                // Obtener el nombre del archivo
                QString fileName = QFileInfo(selectedFilePath).fileName();

                // Obtén la ruta de la carpeta "DeletedDocs" dentro de "DocumentBlogs" en la carpeta "Documentos" de Windows
                QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
                QString deletedDocsFolderPath = documentsPath + QDir::separator() + "DocumentBlogs" + QDir::separator() + "DeletedDocs";

                // Verificar si la carpeta "DeletedDocs" existe. Si no existe, créala
                QDir deletedDocsFolder(deletedDocsFolderPath);
                if (!deletedDocsFolder.exists()) {
                    if (!deletedDocsFolder.mkpath(deletedDocsFolderPath)) {
                        // Mostrar un mensaje de error si no se pudo crear la carpeta
                        QMessageBox::critical(this, "Error", "No se pudo crear la carpeta 'DeletedDocs'.");
                        return;
                    }
                }

                // Obtén la ruta completa de destino del archivo en la carpeta "DeletedDocs"
                QString destinationFilePath = deletedDocsFolderPath + QDir::separator() + fileName;

                // Mueve el archivo a la carpeta "DeletedDocs"
                if (QFile::rename(selectedFilePath, destinationFilePath)) {
                    // Mostrar un mensaje de éxito
                    QMessageBox::information(this, "Éxito", "El archivo se ha eliminado correctamente.");

                    // Eliminar el ítem del QListWidget
                    delete ui->listWidget->takeItem(ui->listWidget->currentRow());

                    // Limpiar la variable selectedFilePath
                    selectedFilePath.clear();
                } else {
                    // Mostrar un mensaje de error si no se pudo mover el archivo
                    QMessageBox::critical(this, "Error", "No se pudo eliminar el archivo correctamente.");
                }
            } else {
                // Mostrar un mensaje de error si el archivo original no existe
                QMessageBox::critical(this, "Error", "El archivo original no existe.");
            }
        } else {
            // No realizar ninguna acción si se selecciona "No"
            return;
        }
    }
}

void MiDocs::on_btnGuardar_clicked()
{
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();
    if (selectedItem) {
        QString currentItemText = selectedItem->text();
        QString modifiedFileName = currentItemText;
        modifiedFileName.remove(".txt");

        QString folderPath = ui->listWidget->currentItem()->data(Qt::UserRole).toString();
        QString originalFilePath = selectedFilePath;
        QString newFileName = modifiedFileName + ".txt";
        QFile file(originalFilePath);
        QDir path(originalFilePath);

        if (file.exists()) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Guardar archivo", "¿Desea renombrar el archivo?",
                                                                       QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {

                if (file.rename(originalFilePath.replace(path.dirName(), newFileName))) {
                    QMessageBox::information(this, "Éxito", "El nuevo nombre del archivo se ha asignado correctamente.");
                }
                else {
                    QMessageBox::critical(this, "Error", "No se pudo guardar el archivo con el nuevo nombre.");
                }
            }
            else {
                return;
            }
        }
    }
    else {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ningún archivo.");
    }

    // Limpiar el QListWidget
    ui->listWidget->clear();
}

void MiDocs::on_btnSallir_clicked()
{
    // Cerrar la ventana
    close();
}
