// ============================================
// File: src/ui/pages/SettingsPage.h
// Mo ta: Trang cai dat — accent color, mat do bang,
//        che do sang/toi, an/hien bieu do,
//        sao luu/ khoi phuc database, thong tin app.
// ============================================
#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QVector>

class QPushButton;
class QCheckBox;
class QButtonGroup;
class QVBoxLayout;

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = nullptr);

private slots:
    void backupDatabase();
    void restoreDatabase();

private:
    void setupAppearance(QVBoxLayout *layout);
    void setupData(QVBoxLayout *layout);
    void setupAbout(QVBoxLayout *layout);
    void syncControls();

    QVector<QPushButton*> m_swatches;
    QVector<QPushButton*> m_avatarButtons;
    QButtonGroup *m_densityGroup;
    QButtonGroup *m_darkGroup;
    QCheckBox *m_chartsCheck;
};

#endif // SETTINGSPAGE_H
