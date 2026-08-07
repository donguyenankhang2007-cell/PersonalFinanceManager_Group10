#ifndef STYLEUTILS_H
#define STYLEUTILS_H

#include <QString>
#include <QColor>
#include <QList>

class StyleUtils
{
public:
    static QString getBudgetProgressStyle(double percent)
    {
        QString barBaseStyle = "QProgressBar { background-color: #F1F5F9; border: none; border-radius: 6px; } ";
        
        if (percent >= 150) {
            return barBaseStyle + "QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #991B1B, stop:1 #7F1D1D); border-radius: 6px; }";
        } else if (percent >= 100) {
            return barBaseStyle + "QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #F43F5E, stop:1 #E11D48); border-radius: 6px; }";
        } else if (percent >= 80) {
            return barBaseStyle + "QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FBBF24, stop:1 #F59E0B); border-radius: 6px; }";
        } else {
            return barBaseStyle + "QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #60A5FA, stop:1 #3B82F6); border-radius: 6px; }";
        }
    }

    static QString getBudgetTextColor(double percent)
    {
        if (percent >= 250) return "#991B1B";
        if (percent >= 150) return "#E11D48";
        if (percent >= 100) return "#EF4444";
        if (percent >= 80)  return "#D97706";
        return "#10B981";
    }

    static QString getBudgetIcon(double percent)
    {
        if (percent >= 150) return "⚠️ ";
        return "";
    }

    static QColor getCategoryColor(int categoryId)
    {
        static const QList<QColor> palette = {
            QColor("#3B82F6"), // Blue
            QColor("#10B981"), // Emerald
            QColor("#F59E0B"), // Amber
            QColor("#EF4444"), // Red
            QColor("#8B5CF6"), // Violet
            QColor("#EC4899"), // Pink
            QColor("#06B6D4"), // Cyan
            QColor("#84CC16"), // Lime
            QColor("#F97316"), // Orange
            QColor("#14B8A6"), // Teal
            QColor("#6366F1"), // Indigo
            QColor("#A855F7"), // Purple
            QColor("#D946EF"), // Fuchsia
            QColor("#F43F5E"), // Rose
            QColor("#0EA5E9"), // Light Blue
            QColor("#22C55E"), // Green
            QColor("#EAB308"), // Yellow
            QColor("#64748B"), // Slate
            QColor("#737373"), // Neutral
            QColor("#111827")  // Gray 900
        };
        
        if (categoryId < 0) return QColor("#94A3B8");
        return palette.at(categoryId % palette.size());
    }
};

#endif // STYLEUTILS_H
