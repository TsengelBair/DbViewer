#include "dbwidget.h"

DbWidget::DbWidget(QString userName, QString hostName, QString password, QWidget *parent)
    : QWidget(parent)
{
    // Подключение
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setUserName(userName);
    db.setHostName(hostName);
    db.setPassword(password);

    if (db.open()) {
        qDebug() << "Успешное подключение к базе данных";
    } else {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
    }

    mainLayout = new QVBoxLayout();
    stackedWidget = new QStackedWidget();;
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

    // 1 окно
    firstWindow = new QWidget();
    firstWindowLayout = new QVBoxLayout(firstWindow);
    viewDbsBtn = new QPushButton("Доступные БД");
    connect(viewDbsBtn, &QPushButton::clicked, this, &DbWidget::on_view_dbs_btn_clicked);
    firstWindowLayout->addWidget(viewDbsBtn);
    stackedWidget->addWidget(firstWindow);

    // 2 окно
    secondWindow = new QWidget();
    secondWindowLayout = new QVBoxLayout(secondWindow);
    stackedWidget->addWidget(secondWindow);

    // 3 окно
    thirdWindow = new QWidget();
    thirdWindowLayout = new QVBoxLayout(thirdWindow);
    stackedWidget->addWidget(thirdWindow);

    // 4 окно
    fourthWindow = new QWidget();
    fourthWindowLayout = new QVBoxLayout(fourthWindow);
    // для отображения таблицы (в метод tableView.setModel() передадим имя модели -> схему выбранной таблицы)
    tableView = new QTableView();

    saveChanges = new QPushButton("Сохранить изменения");
    addRow = new QPushButton("Добавить запись");
    deleteRow = new QPushButton("Удалить запись");

    connect(saveChanges, &QPushButton::clicked, this, &DbWidget::on_save_changes_btn_clicked);
    connect(addRow, &QPushButton::clicked, this, &DbWidget::on_add_row_btn_clicked);
    connect(deleteRow, &QPushButton::clicked, this, &DbWidget::on_delete_row_btn_clicked);

    fourthWindowLayout->addWidget(tableView);
    fourthWindowLayout->addWidget(saveChanges);
    fourthWindowLayout->addWidget(addRow);
    fourthWindowLayout->addWidget(deleteRow);

    fourthWindow->setLayout(fourthWindowLayout);

    stackedWidget->addWidget(fourthWindow);
}

DbWidget::~DbWidget()
{
    if (db.isOpen()) {
        db.close();
    }
}

void DbWidget::on_view_dbs_btn_clicked()
{
    // Очистка от предудущих значений
    dbsNames.clear();
    while (secondWindowLayout->count() > 0){
        QWidget* widget = secondWindowLayout->itemAt(0)->widget();
        secondWindowLayout->removeWidget(widget);
        delete widget;
    }

    // сохраняем имена баз данных
    QSqlQuery query(db);
    query.prepare("SELECT datname FROM pg_database WHERE datistemplate = false;");

    if (!query.exec()) {
        qDebug() << "Ошибка:" << query.lastError().text();
        return;
    }

    QString dbName;
    while(query.next()){
        dbName = query.value(0).toString();
        if (dbName != "postgres"){
            dbsNames.push_back(dbName);
        }
    }

    // Отображаем имена баз данных в ui (как кнопки)
    for (const QString& dbName : dbsNames){
        QPushButton* btn = new QPushButton(dbName);
        secondWindowLayout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, &DbWidget::on_db_btn_clicked);
    }

    stackedWidget->setCurrentIndex(1);
}

void DbWidget::on_db_btn_clicked()
{
    tablesNames.clear();

    QPushButton* pushedBtn = qobject_cast<QPushButton*>(sender());
    if (pushedBtn){
        QString parsedDbName = pushedBtn->text();
        db.setDatabaseName(parsedDbName);

        // При изменении имени, нужно перезапустить (открыть заново)
        if (db.open()){
            QSqlQuery query(db);
            query.prepare("SELECT table_name FROM information_schema.tables WHERE table_schema = 'public';");

            if (!query.exec()) {
                qDebug() << "Ошибка:" << query.lastError().text();
                return;
            }

            QString tableName;
            while(query.next()){
                tableName = query.value(0).toString();
                tablesNames.push_back(tableName);
            }

            for (const QString& tableName : tablesNames){
                QPushButton* btn = new QPushButton(tableName);
                thirdWindowLayout->addWidget(btn);
                connect(btn, &QPushButton::clicked, this, &DbWidget::on_table_btn_clicked);
            }

            stackedWidget->setCurrentIndex(2);
        }
        else {
            qDebug() << "Failed to connect to database:" << db.lastError().text();
        }
    }
    else {
        qDebug() << "Failed";
    }
}

void DbWidget::on_table_btn_clicked()
{
    QPushButton* pushedBtn = qobject_cast<QPushButton*>(sender());
    if (pushedBtn){
        QString tableName = pushedBtn->text();

        tableModel = new QSqlTableModel(this, db);
        tableModel->setTable(tableName);
        tableModel->select();

        tableView->setModel(tableModel);
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked); // редактирование по двойному клику

        stackedWidget->setCurrentIndex(3);
    }
}

void DbWidget::on_save_changes_btn_clicked()
{
    if (tableModel) {
            if (tableModel->submitAll()) {
                qDebug() << "Changes saved successfully.";
            } else {
                qDebug() << "Failed to save changes:" << tableModel->lastError().text();
            }
        }
}

void DbWidget::on_add_row_btn_clicked()
{
    if (tableModel) {
            int row = tableModel->rowCount();
            tableModel->insertRow(row);
            tableView->selectRow(row);
            qDebug() << "Added new row at index:" << row;
        }
}

void DbWidget::on_delete_row_btn_clicked()
{
    if (tableModel) {
            QModelIndexList selected = tableView->selectionModel()->selectedRows();
            for (int i = 0; i < selected.count(); ++i) {
                tableModel->removeRow(selected.at(i).row());
            }
            if (tableModel->submitAll()) {
                qDebug() << "Deleted selected rows.";
            } else {
                qDebug() << "Failed to delete rows:" << tableModel->lastError().text();
            }
        }
}

