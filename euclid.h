#ifndef EUCLID_H
#define EUCLID_H

#include <QDialog>
#include <QVector>

namespace Ui {
class Euclid;
}

class Euclid : public QDialog
{
    Q_OBJECT

public:
    explicit Euclid(QWidget *parent = nullptr);
    ~Euclid();

private slots:
    void on_calculateButton_clicked(); // 计算按钮槽函数

private:
    Ui::Euclid *ui;
    void calculateEuclid(int a, int b); // 计算扩展 Euclid
    void updateTable(const QVector<QVector<int>> &data); // 更新表格
};

#endif // EUCLID_H
