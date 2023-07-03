#ifndef MIDOCS_H
#define MIDOCS_H

#include <QDialog>
#include <QMap>
#include <QTimer>
#include <QListWidgetItem>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QFileSystemWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class MiDocs; }
QT_END_NAMESPACE

class MiDocs : public QDialog
{
    Q_OBJECT

public:
    explicit MiDocs(QWidget *parent = nullptr);
    ~MiDocs();

private slots:

    void on_btnBuscar_clicked();
    void on_btnModificar_clicked();
    void on_btnBorrar_clicked();
    void on_btnGuardar_clicked();
    void on_btnSallir_clicked();
//_______________________________________________________________________________________________________________

    void updateTreeWidget(const QString& path);
    void addFilesAndFoldersToTreeWidget(const QString& folderPath, QTreeWidgetItem* parentItem);
    QStringList getAllSubfolderPaths(const QString& folderPath);
    QStringList getAllFilePaths(const QString& folderPath, const QString& filter);
    void addSubfoldersToWatcher(const QString& folderPath);
    void toggleMonitoring();

private:
    Ui::MiDocs *ui;
    QString folderPath; // Variable para almacenar la ruta de la carpeta
    QFileSystemWatcher* fileWatcher; // Objeto para monitorear cambios en la carpeta
    QTimer* updateTimer; // Temporizador para actualizar el QTreeWidget
    bool isMonitoringEnabled;
    QString modifiedFileName;
    bool isEditingFileName; // Indica si se está editando el nombre del archivo en el QListWidget
    QListWidgetItem* selectedFileItem; // Almacena el elemento seleccionado en el QListWidget
    QString originalFileName; // Nombre original del archivo
    QString selectedFilePath;

};

#endif // MIDOCS_H
