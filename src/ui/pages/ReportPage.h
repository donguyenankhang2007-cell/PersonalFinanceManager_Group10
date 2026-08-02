#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>

class ReportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportPage(QWidget *parent = nullptr);

public slots:
    void loadReport();

private:
    QLabel *incomeLabel;
    QLabel *expenseLabel;
    QLabel *balanceLabel;
    QLabel *compareLabel;
    QSpinBox *monthSpin;
    QSpinBox *yearSpin;
};

#endif // REPORTPAGE_H
