#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>

class ReportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportPage(QWidget *parent = nullptr);

public slots:
    // Tao va hien thi bao cao tai chinh
    void generateReport();

private:
    void setupUI();

    // Bo loc thoi gian
    QSpinBox *m_monthSpin;
    QSpinBox *m_yearSpin;

    // Labels hien thi ket qua bao cao
    QLabel *m_incomeValue;
    QLabel *m_expenseValue;
    QLabel *m_balanceValue;
    QLabel *m_comparisonValue;
};

#endif // REPORTPAGE_H
