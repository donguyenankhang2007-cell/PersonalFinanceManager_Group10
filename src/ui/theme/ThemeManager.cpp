// ============================================
// File: src/ui/theme/ThemeManager.cpp
// Mo ta: Xay dung va ap dung stylesheet toan cuc.
//        QSS dung token (@BG, @CARD, @ACCENT, ...) de ho tro
//        ca che do sang (light) va toi (dark). Moi doi tuong
//        dung objectName de duoc QSS dinh hinh.
// ============================================
#include "ThemeManager.h"

#include <QSettings>
#include <QWidget>
#include <QApplication>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QFile>

namespace {
const char* kOrg   = "PFM";
const char* kApp   = "FinanceManager";
}

ThemeManager& ThemeManager::instance()
{
    static ThemeManager mgr;
    return mgr;
}

ThemeManager::ThemeManager()
{
    m_presets = {
        { "Blue",    QColor("#2563EB"), QColor("#1D4ED8"), QColor("#DBEAFE"), QColor("#EFF6FF") },
        { "Indigo",  QColor("#6366F1"), QColor("#4F46E5"), QColor("#E0E7FF"), QColor("#EEF2FF") },
        { "Emerald", QColor("#10B981"), QColor("#059669"), QColor("#D1FAE5"), QColor("#ECFDF5") },
        { "Coral",   QColor("#F43F5E"), QColor("#E11D48"), QColor("#FFE4E6"), QColor("#FFF1F2") },
        { "Violet",  QColor("#8B5CF6"), QColor("#7C3AED"), QColor("#EDE9FE"), QColor("#F5F3FF") },
        { "Amber",   QColor("#F59E0B"), QColor("#D97706"), QColor("#FEF3C7"), QColor("#FFFBEB") },
    };
    loadSettings();
}

void ThemeManager::loadSettings()
{
    QSettings s(kOrg, kApp);
    m_accentIndex = s.value("accentIndex", 0).toInt();
    if (m_accentIndex < 0 || m_accentIndex >= m_presets.size())
        m_accentIndex = 0;
    m_density = s.value("tableDensity", 0).toInt()
                    ? TableDensity::Compact
                    : TableDensity::Comfortable;
    m_chartsEnabled = s.value("chartsEnabled", true).toBool();
    m_darkMode = s.value("darkMode", false).toBool();
    m_avatarPath = s.value("avatarPath", ":/icons/user.svg").toString();
}

void ThemeManager::saveSettings() const
{
    QSettings s(kOrg, kApp);
    s.setValue("accentIndex", m_accentIndex);
    s.setValue("tableDensity", static_cast<int>(m_density));
    s.setValue("chartsEnabled", m_chartsEnabled);
    s.setValue("darkMode", m_darkMode);
    s.setValue("avatarPath", m_avatarPath);
}

void ThemeManager::setAvatarPath(const QString &path)
{
    if (m_avatarPath == path)
        return;
    m_avatarPath = path;
    saveSettings();
    emit avatarChanged(m_avatarPath);
}

void ThemeManager::setAccentIndex(int index)
{
    if (index < 0 || index >= m_presets.size() || index == m_accentIndex)
        return;
    m_accentIndex = index;
    saveSettings();
    emit themeChanged();
}

QColor ThemeManager::accent() const
{
    return m_presets.at(m_accentIndex).accent;
}

void ThemeManager::setDensity(TableDensity density)
{
    if (density == m_density)
        return;
    m_density = density;
    saveSettings();
    emit densityChanged();
}

int ThemeManager::tableRowHeight() const
{
    return m_density == TableDensity::Comfortable ? 38 : 28;
}

bool ThemeManager::chartsEnabled() const
{
    return m_chartsEnabled;
}

void ThemeManager::setChartsEnabled(bool enabled)
{
    if (enabled == m_chartsEnabled)
        return;
    m_chartsEnabled = enabled;
    saveSettings();
    emit chartsEnabledChanged();
}

void ThemeManager::setDarkMode(bool dark)
{
    if (dark == m_darkMode)
        return;
    m_darkMode = dark;
    saveSettings();
    emit themeChanged();
}

QColor ThemeManager::textColor() const
{
    return isDark() ? QColor("#F1F5F9") : QColor("#0F172A");
}

QColor ThemeManager::bodyTextColor() const
{
    return isDark() ? QColor("#E2E8F0") : QColor("#1E293B");
}

QColor ThemeManager::mutedColor() const
{
    return isDark() ? QColor("#94A3B8") : QColor("#64748B");
}

QColor ThemeManager::cardColor() const
{
    return isDark() ? QColor("#1E293B") : QColor("#FFFFFF");
}

QColor ThemeManager::gridColor() const
{
    return isDark() ? QColor("#2A3750") : QColor("#E4E7EC");
}

QString ThemeManager::buildStylesheet() const
{
    const AccentPreset& p = m_presets.at(m_accentIndex);
    QString tablePadding = m_density == TableDensity::Comfortable
                               ? "9px" : "4px";

    // === Bảng màu theo chế độ sáng/tối ===
    QString bg, card, border, text, textBody, textMuted, inputBg,
            inputBorder, headerBg, headerText, tableAlt, grid,
            progressBg, scrollHandle, segBg, tooltipBg, tooltipText,
            disabledBg, hoverBg;
    if (isDark()) {
        bg          = "#0F172A";
        card        = "#1E293B";
        border      = "#334155";
        text        = "#F1F5F9";
        textBody    = "#E2E8F0";
        textMuted   = "#94A3B8";
        inputBg     = "#0F172A";
        inputBorder = "#475569";
        headerBg    = "#334155";
        headerText  = "#CBD5E1";
        tableAlt    = "#16223A";
        grid        = "#2A3750";
        progressBg  = "#334155";
        scrollHandle = "#475569";
        segBg       = "#0F172A";
        tooltipBg   = "#0F172A";
        tooltipText = "#F1F5F9";
        disabledBg  = "#475569";
        hoverBg     = QColor(p.soft).darker(160).name(QColor::HexRgb);
    } else {
        bg          = "#F1F3F6";
        card        = "#FFFFFF";
        border      = "#E4E7EC";
        text        = "#0F172A";
        textBody    = "#1E293B";
        textMuted   = "#64748B";
        inputBg     = "#FFFFFF";
        inputBorder = "#CBD5E1";
        headerBg    = "#F1F5F9";
        headerText  = "#475569";
        tableAlt    = "#F8FAFC";
        grid        = "#EEF1F5";
        progressBg  = "#E2E8F0";
        scrollHandle = "#CBD5E1";
        segBg       = "#FFFFFF";
        tooltipBg   = "#1E293B";
        tooltipText = "#F1F5F9";
        disabledBg  = "#CBD5E1";
        hoverBg     = p.light.name(QColor::HexRgb);
    }
    
    QString downArrowUrl = isDark() ? 
        "url(:/icons/down-arrow-dark.svg)" :
        "url(:/icons/down-arrow.svg)";

    QString qss;
    QFile qssFile(":/styles/theme.qss");
    if (qssFile.open(QFile::ReadOnly | QFile::Text)) {
        qss = QLatin1String(qssFile.readAll());
        qssFile.close();
    }

    // === Thay thế token — token dài thay trước (vì @ACCENT là tiền tố) ===
    qss.replace("@ACCENT_LIGHT_STRONG",
        QColor(p.accent).lighter(120).name(QColor::HexRgb));
    qss.replace("@ACCENT_DARK",       p.dark.name(QColor::HexRgb));
    qss.replace("@ACCENT_SOFT",       p.soft.name(QColor::HexRgb));
    qss.replace("@ACCENT_LIGHT",      p.light.name(QColor::HexRgb));
    qss.replace("@ACCENT",            p.accent.name(QColor::HexRgb));
    qss.replace("@TABLE_PADDING",     tablePadding);

    // Thay token dài trước (tránh @TEXT thay nhầm @TEXT_BODY/@TEXT_MUTED...)
    qss.replace("@TEXT_BODY",     textBody);
    qss.replace("@TEXT_MUTED",    textMuted);
    qss.replace("@TOOLTIP_BG",    tooltipBg);
    qss.replace("@TOOLTIP_TEXT",  tooltipText);
    qss.replace("@INPUT_BG",      inputBg);
    qss.replace("@INPUT_BORDER",  inputBorder);
    qss.replace("@HEADER_BG",     headerBg);
    qss.replace("@HEADER_TEXT",   headerText);
    qss.replace("@TABLE_ALT",     tableAlt);
    qss.replace("@PROGRESS_BG",   progressBg);
    qss.replace("@SCROLL_HANDLE", scrollHandle);
    qss.replace("@DISABLED_BG",   disabledBg);
    qss.replace("@ROW_HOVER",     hoverBg);
    qss.replace("@DOWN_ARROW",    downArrowUrl);
    qss.replace("@SEG_BG",        segBg);
    qss.replace("@BG",            bg);
    qss.replace("@CARD",          card);
    qss.replace("@BORDER",        border);
    qss.replace("@TEXT",          text);
    qss.replace("@GRID",          grid);

    return qss;
}

void ThemeManager::applyTo(QWidget* root)
{
    if (root)
        root->setStyleSheet(buildStylesheet());

    // Fix QCalendarWidget text colors natively
    QTextCharFormat weekendFormat;
    weekendFormat.setForeground(QColor(isDark() ? "#F87171" : "#EF4444")); 
    
    QTextCharFormat weekdayFormat;
    weekdayFormat.setForeground(QColor(isDark() ? "#CBD5E1" : "#1E293B"));
    
    // Find all QCalendarWidget children of qApp
    for (QWidget *widget : qApp->allWidgets()) {
        if (QCalendarWidget *calendar = qobject_cast<QCalendarWidget*>(widget)) {
            calendar->setWeekdayTextFormat(Qt::Saturday, weekendFormat);
            calendar->setWeekdayTextFormat(Qt::Sunday, weekendFormat);
            calendar->setWeekdayTextFormat(Qt::Monday, weekdayFormat);
            calendar->setWeekdayTextFormat(Qt::Tuesday, weekdayFormat);
            calendar->setWeekdayTextFormat(Qt::Wednesday, weekdayFormat);
            calendar->setWeekdayTextFormat(Qt::Thursday, weekdayFormat);
            calendar->setWeekdayTextFormat(Qt::Friday, weekdayFormat);
        }
    }
}
