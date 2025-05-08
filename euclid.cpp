#include "Euclid.h"
//#include "ui_Euclid.h"
#include "ui_euclid.h"
#include <QMessageBox>
#include <QDebug>

Euclid::Euclid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Euclid)
{
    ui->setupUi(this);
    this->setWindowTitle("扩展 Euclid 算法计算");

    // 设置表格列标题
    QStringList headers = {"j", "sj", "tj", "qj+1", "rj+1"};
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止用户修改表格内容
}

Euclid::~Euclid()
{
    delete ui;
}

// **计算按钮点击事件**
void Euclid::on_calculateButton_clicked()
{
    // 获取用户输入
    bool ok1, ok2;
    int a = ui->inputA->text().toInt(&ok1);
    int b = ui->inputB->text().toInt(&ok2);

    // 验证输入是否合法
    if (!ok1 || !ok2 || a <= 0 || b <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入正整数！");
        return;
    }

    // 计算 Euclid 结果
    calculateEuclid(a, b);
}

// **计算扩展 Euclid 算法**
void Euclid::calculateEuclid(int a, int b)
{
    QVector<int> s_j, t_j, q_j1, r_j1;
    QVector<QVector<int>> tableData;

    r_j1.push_back(a);
    r_j1.push_back(b);
    s_j.push_back(1);
    s_j.push_back(0);
    t_j.push_back(0);
    t_j.push_back(1);

    int temp = a % b;
    int j = -3;

    for (int i = 1; temp != 0; i++)
    {
        int q_temp = r_j1[i-1] / r_j1[i];
        q_j1.push_back(q_temp);
        temp = r_j1[i-1] % r_j1[i];
        r_j1.push_back(temp);
        int sj_temp = -1 * q_temp * s_j[i] + s_j[i - 1];
        int tj_temp = -1 * q_temp * t_j[i] + t_j[i - 1];
        s_j.push_back(sj_temp);
        t_j.push_back(tj_temp);
    }

    int gcd = r_j1[r_j1.size() - 2];
    int s = s_j[s_j.size() - 2];
    int t = t_j[t_j.size() - 2];

    // **填充表格数据**
    for (; j + 3 < r_j1.size(); j++)
    {
        int ind = j + 3;

        QVector<int> rowData;
        rowData.append(j);

        if (ind == 0)
        {
            rowData.append(0);
            rowData.append(0);
            rowData.append(0);
            rowData.append(r_j1[ind]);
        }
        else if (ind == 1)
        {
            rowData.append(s_j[ind - 1]);
            rowData.append(t_j[ind - 1]);
            rowData.append(0);
            rowData.append(r_j1[ind]);
        }
        else
        {
            rowData.append(s_j[ind - 1]);
            rowData.append(t_j[ind - 1]);
            rowData.append(q_j1[ind - 2]);
            rowData.append(r_j1[ind]);
        }

        tableData.append(rowData);
    }

    // **更新 UI**
    ui->outputS->setText(QString::number(s));
    ui->outputT->setText(QString::number(t));
    ui->outputGCD->setText(QString::number(gcd));
    updateTable(tableData);
}

// **更新表格**
void Euclid::updateTable(const QVector<QVector<int>> &data)
{
    ui->tableWidget->setRowCount(data.size());

    for (int row = 0; row < data.size(); ++row) {
        for (int col = 0; col < data[row].size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(data[row][col]));
            ui->tableWidget->setItem(row, col, item);
        }
    }
}
