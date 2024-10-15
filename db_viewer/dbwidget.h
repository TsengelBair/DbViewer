#ifndef DBWIDGET_H
#define DBWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableView>
#include <QSqlTableModel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include <QDebug>
#include <QStatusBar>

class DbWidget : public QWidget
{
    Q_OBJECT

public:
    DbWidget(QString userName, QString hostName, QString password, QWidget *parent = nullptr);
    ~DbWidget();

private slots:
    void on_view_dbs_btn_clicked();
    void on_db_btn_clicked();
    void on_table_btn_clicked();
    void on_save_changes_btn_clicked();
    void on_add_row_btn_clicked();
    void on_delete_row_btn_clicked();

private:
    QSqlDatabase db;
    QVBoxLayout* mainLayout;
    QStackedWidget* stackedWidget;
    // 1 окно
    QWidget* firstWindow;
    QVBoxLayout* firstWindowLayout;
    QPushButton* viewDbsBtn;

    // 2 окно
    QWidget* secondWindow;
    QVBoxLayout* secondWindowLayout;
    QVector<QString>dbsNames;

    // 3 окно
    QWidget* thirdWindow;
    QVBoxLayout* thirdWindowLayout;
    QVector<QString>tablesNames;

    // 4 окно
    QWidget* fourthWindow;
    QVBoxLayout* fourthWindowLayout;
    QTableView* tableView;
    QSqlTableModel* tableModel;
    QPushButton* saveChanges;
    QPushButton* addRow;
    QPushButton* deleteRow;
};
#endif // DBWIDGET_H
