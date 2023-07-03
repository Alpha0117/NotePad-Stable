#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getCurrentFileName() const;
    void setCurrentFileName(const QString& fileName);

private slots:
    void on_actionNuevo_triggered();
    void on_actionNueva_ventana_triggered();
    void on_actionAbrir_triggered();
    void on_actionGuardar_triggered();
    void on_actionGuardar_como_triggered();
    void on_actionSalir_triggered();
    void on_actionCortar_triggered();
    void on_actionCopiar_triggered();
    void on_actionPegar_triggered();
    void on_actionEliminar_triggered();
    void on_actionAyuda_triggered();
    void on_actionCC_triggered();
    void on_actionBy_triggered();
    void on_actionGestor_de_Archivos_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFileName;
};

#endif // MAINWINDOW_H

