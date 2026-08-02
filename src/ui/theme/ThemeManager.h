// ============================================
// File: src/ui/theme/ThemeManager.h
// Mo ta: Quan ly giao dien chung (theme) cho toan app.
//        Ho tro doi accent color, mat do bang, che do sang/toi
//        (dark mode), luu qua QSettings.
// ============================================
#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QColor>

struct AccentPreset
{
    QString name;
    QColor  accent;      // Màu chính
    QColor  dark;        // mau dam hon (hover)
    QColor  soft;        // mau nhat de lam nen/highlight
    QColor  light;       // Màu rất nhạt làm nền card
};

enum class TableDensity
{
    Comfortable,
    Compact
};

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    static ThemeManager& instance();

    // === Accent ===
    const QVector<AccentPreset>& presets() const { return m_presets; }
    int accentIndex() const { return m_accentIndex; }
    void setAccentIndex(int index);
    QColor accent() const;

    // === Table density ===
    TableDensity density() const { return m_density; }
    void setDensity(TableDensity density);
    int tableRowHeight() const;

    // === Charts visibility ===
    bool chartsEnabled() const;
    void setChartsEnabled(bool enabled);

    // === Dark mode ===
    bool darkMode() const { return m_darkMode; }
    void setDarkMode(bool dark);

    // === Avatar ===
    QString avatarPath() const { return m_avatarPath; }
    void setAvatarPath(const QString &path);

    // === Màu nền theo theme (dùng cho charts/QLabel động) ===
    QColor textColor() const;
    QColor bodyTextColor() const;
    QColor mutedColor() const;
    QColor cardColor() const;
    QColor gridColor() const;

    // === Stylesheet ===
    QString buildStylesheet() const;
    void applyTo(QWidget* root);

signals:
    void themeChanged();
    void densityChanged();
    void chartsEnabledChanged();
    void avatarChanged(const QString &path);

private:
    ThemeManager();
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    void loadSettings();
    void saveSettings() const;
    bool isDark() const { return m_darkMode; }

    QVector<AccentPreset> m_presets;
    int m_accentIndex = 0;
    TableDensity m_density = TableDensity::Comfortable;
    bool m_chartsEnabled = true;
    bool m_darkMode = false;
    QString m_avatarPath = ":/icons/user.svg";
};

#endif // THEMEMANAGER_H
