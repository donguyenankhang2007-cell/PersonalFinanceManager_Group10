// ============================================
// File: src/ui/pages/SettingsPage.cpp
// ============================================
#include "SettingsPage.h"

#include "../theme/ThemeManager.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QFrame>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(20);

    QLabel *title = new QLabel("Cài Đặt");
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Tùy chỉnh giao diện ứng dụng");
    subtitle->setObjectName("pageSubtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(4);

    setupAppearance(layout);
    setupData(layout);
    setupAbout(layout);

    connect(&ThemeManager::instance(), &ThemeManager::avatarChanged,
            this, [this](const QString &) { syncControls(); });

    layout->addStretch();

    syncControls();
}

// ==================== HELPER: DIVIDER LINE ====================
static QFrame* createDivider()
{
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: transparent; border-bottom: 1px solid rgba(150, 150, 150, 0.2); margin: 6px 0px;");
    return line;
}

static QHBoxLayout* createSettingRow(const QString &title, const QString &desc, QLayout *controlLayout = nullptr)
{
    QHBoxLayout *row = new QHBoxLayout();
    QVBoxLayout *rowText = new QVBoxLayout();
    rowText->setSpacing(2);
    
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("settingTitle");
    QLabel *descLabel = new QLabel(desc);
    descLabel->setObjectName("settingDesc");
    
    rowText->addWidget(titleLabel);
    rowText->addWidget(descLabel);
    
    row->addLayout(rowText, 1);
    if (controlLayout) {
        row->addLayout(controlLayout);
    }
    return row;
}

static QVBoxLayout* createSectionCard(const QString &title, QVBoxLayout *parentLayout)
{
    QWidget *card = new QWidget();
    card->setObjectName("card");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 10));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 20, 24, 20);
    cardLayout->setSpacing(16);

    QLabel *sectionHeader = new QLabel(title);
    sectionHeader->setObjectName("sectionTitle");
    cardLayout->addWidget(sectionHeader);
    cardLayout->addWidget(createDivider());

    parentLayout->addWidget(card);
    return cardLayout;
}

// ==================== APPEARANCE ====================
void SettingsPage::setupAppearance(QVBoxLayout *layout)
{
    QVBoxLayout *cardLayout = createSectionCard("Giao Diện & Tùy Chỉnh", layout);

    // --- Row 1: Theme mode ---
    QHBoxLayout *modeBtns = new QHBoxLayout();
    modeBtns->setSpacing(8);

    QPushButton *btnLight = new QPushButton("Sáng");
    btnLight->setObjectName("segBtn");
    btnLight->setCheckable(true);
    btnLight->setCursor(Qt::PointingHandCursor);

    QPushButton *btnDark = new QPushButton("Tối");
    btnDark->setObjectName("segBtn");
    btnDark->setCheckable(true);
    btnDark->setCursor(Qt::PointingHandCursor);

    m_darkGroup = new QButtonGroup(this);
    m_darkGroup->setExclusive(true);
    m_darkGroup->addButton(btnLight, 0);
    m_darkGroup->addButton(btnDark, 1);

    connect(btnLight, &QPushButton::clicked, this, []() {
        ThemeManager::instance().setDarkMode(false);
    });
    connect(btnDark, &QPushButton::clicked, this, []() {
        ThemeManager::instance().setDarkMode(true);
    });

    modeBtns->addWidget(btnLight);
    modeBtns->addWidget(btnDark);

    cardLayout->addLayout(createSettingRow("Chế độ Giao diện", "Chọn tông màu sáng hoặc tối cho ứng dụng", modeBtns));
    cardLayout->addWidget(createDivider());

    // --- Row 2: Accent color ---
    QHBoxLayout *swatchRow = new QHBoxLayout();
    swatchRow->setSpacing(10);

    const QVector<AccentPreset> &presets = ThemeManager::instance().presets();
    for (int i = 0; i < presets.size(); ++i) {
        QPushButton *swatch = new QPushButton();
        swatch->setObjectName("swatchBtn");
        swatch->setFixedSize(30, 30);
        swatch->setCursor(Qt::PointingHandCursor);
        swatch->setCheckable(true);
        swatch->setToolTip(presets.at(i).name);
        swatch->setStyleSheet(
            QString("background-color: %1;")
                .arg(presets.at(i).accent.name(QColor::HexRgb)));

        const int index = i;
        connect(swatch, &QPushButton::clicked, this, [index]() {
            ThemeManager::instance().setAccentIndex(index);
        });

        m_swatches.append(swatch);
        swatchRow->addWidget(swatch);
    }

    cardLayout->addLayout(createSettingRow("Màu Chủ Đạo", "Màu nhấn chính cho các trạng thái hoạt động và thanh bên", swatchRow));
    cardLayout->addWidget(createDivider());

    // --- Row 3: Sidebar Avatar ---
    QHBoxLayout *avatarRow = new QHBoxLayout();
    avatarRow->setSpacing(8);

    const QVector<QPair<QString, QString>> avatars = {
        { ":/icons/user.svg",        "Người dùng mặc định" },
        { ":/icons/shield-cat.svg",  "Mèo Khiên 🐱" },
        { ":/icons/dog.svg",         "Chó 🐶" },
        { ":/icons/cow.svg",         "Bò 🐮" },
        { ":/icons/dragon.svg",      "Rồng 🐉" },
        { ":/icons/otter.svg",       "Rái cá 🦦" },
        { ":/icons/hippo.svg",       "Hà mã 🦛" },
        { ":/icons/fish.svg",        "Cá 🐟" },
        { ":/icons/fish-fins.svg",   "Cá vàng 🐠" },
        { ":/icons/crow.svg",        "Quạ 🐦" },
        { ":/icons/shrimp.svg",      "Tôm 🦐" },
        { ":/icons/worm.svg",        "Sâu 🐛" }
    };

    for (const auto &item : avatars) {
        QPushButton *btn = new QPushButton();
        btn->setFixedSize(34, 34);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setCheckable(true);
        btn->setIcon(QIcon(item.first));
        btn->setIconSize(QSize(18, 18));
        btn->setToolTip(item.second);
        btn->setProperty("avatarPath", item.first);
        btn->setObjectName("accentButton");

        QString path = item.first;
        connect(btn, &QPushButton::clicked, this, [path]() {
            ThemeManager::instance().setAvatarPath(path);
        });

        m_avatarButtons.append(btn);
        avatarRow->addWidget(btn);
    }

    cardLayout->addLayout(createSettingRow("Ảnh đại diện Thanh bên", "Chọn biểu tượng hồ sơ hoặc linh vật cho thanh bên", avatarRow));
    cardLayout->addWidget(createDivider());

    // --- Row 4: Table Density ---
    QHBoxLayout *densityRow = new QHBoxLayout();
    densityRow->setSpacing(8);

    QPushButton *btnComfortable = new QPushButton("Thoải mái");
    btnComfortable->setObjectName("segBtn");
    btnComfortable->setCheckable(true);
    btnComfortable->setCursor(Qt::PointingHandCursor);

    QPushButton *btnCompact = new QPushButton("Thu gọn");
    btnCompact->setObjectName("segBtn");
    btnCompact->setCheckable(true);
    btnCompact->setCursor(Qt::PointingHandCursor);

    m_densityGroup = new QButtonGroup(this);
    m_densityGroup->setExclusive(true);
    m_densityGroup->addButton(btnComfortable, 0);
    m_densityGroup->addButton(btnCompact, 1);

    connect(btnComfortable, &QPushButton::clicked, this, []() {
        ThemeManager::instance().setDensity(TableDensity::Comfortable);
    });
    connect(btnCompact, &QPushButton::clicked, this, []() {
        ThemeManager::instance().setDensity(TableDensity::Compact);
    });

    densityRow->addWidget(btnComfortable);
    densityRow->addWidget(btnCompact);

    cardLayout->addLayout(createSettingRow("Mật độ Bảng", "Điều chỉnh khoảng cách các hàng trong bảng dữ liệu", densityRow));
    cardLayout->addWidget(createDivider());

    // --- Row 5: Dashboard Charts ---
    m_chartsCheck = new QCheckBox("Bật");
    m_chartsCheck->setObjectName("settingTitle");
    m_chartsCheck->setCursor(Qt::PointingHandCursor);
    connect(m_chartsCheck, &QCheckBox::toggled, this, [](bool checked) {
        ThemeManager::instance().setChartsEnabled(checked);
    });

    QHBoxLayout *chartsRow = new QHBoxLayout();
    chartsRow->addWidget(m_chartsCheck);
    chartsRow->addStretch();
    
    cardLayout->addLayout(createSettingRow("Biểu đồ Phân tích Dashboard", "Hiển thị biểu đồ tổng quan (Thu Nhập vs Chi Tiêu, Chi tiêu theo danh mục)", chartsRow));
}

// ==================== DATA ====================
void SettingsPage::setupData(QVBoxLayout *layout)
{
    QVBoxLayout *cardLayout = createSectionCard("Quản lý Dữ liệu & Sao lưu", layout);

    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->setSpacing(10);

    QPushButton *btnBackup = new QPushButton("Sao lưu Dữ liệu...");
    btnBackup->setObjectName("secondaryBtn");
    btnBackup->setCursor(Qt::PointingHandCursor);

    QPushButton *btnRestore = new QPushButton("Khôi phục Dữ liệu...");
    btnRestore->setObjectName("secondaryBtn");
    btnRestore->setCursor(Qt::PointingHandCursor);

    connect(btnBackup, &QPushButton::clicked, this, &SettingsPage::backupDatabase);
    connect(btnRestore, &QPushButton::clicked, this, &SettingsPage::restoreDatabase);

    btnRow->addWidget(btnBackup);
    btnRow->addWidget(btnRestore);

    cardLayout->addLayout(createSettingRow("Lưu trữ Cơ sở dữ liệu", "Mọi giao dịch và cài đặt được lưu trong file SQLite (finance.db)", btnRow));
}

void SettingsPage::backupDatabase()
{
    QString dbPath =
        AppContext::instance().database().database().databaseName();
    if (dbPath.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy file cơ sở dữ liệu!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this, "Sao lưu Dữ liệu", "finance_backup.db", "File cơ sở dữ liệu (*.db)");
    if (fileName.isEmpty())
        return;

    if (QFile::exists(fileName))
        QFile::remove(fileName);

    if (QFile::copy(dbPath, fileName)) {
        QMessageBox::information(this, "Sao lưu",
                                 QString("Đã lưu sao lưu thành công tại:\n%1").arg(fileName));
    } else {
        QMessageBox::warning(this, "Lỗi", "Sao lưu thất bại!");
    }
}

void SettingsPage::restoreDatabase()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Khôi phục Dữ liệu", "", "File cơ sở dữ liệu (*.db)");
    if (fileName.isEmpty())
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        "Khôi phục sẽ THAY THẾ toàn bộ dữ liệu hiện tại bằng file sao lưu.\n"
        "Bạn có chắc chắn muốn tiếp tục không?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
        return;

    DatabaseManager &dbm = AppContext::instance().database();
    QString dbPath = dbm.database().databaseName();

    dbm.closeDatabase();

    bool ok = false;
    if (QFile::remove(dbPath) && QFile::copy(fileName, dbPath)) {
        ok = dbm.openDatabase(dbPath) && dbm.initializeDatabase();
    }

    if (ok) {
        QMessageBox::information(
            this, "Khôi phục",
            "Khôi phục cơ sở dữ liệu thành công.\n"
            "Tất cả các trang sẽ tự động cập nhật khi bạn truy cập.");
    } else {
        QMessageBox::warning(this, "Lỗi", "Khôi phục thất bại!");
    }
}

// ==================== ABOUT ====================
void SettingsPage::setupAbout(QVBoxLayout *layout)
{
    QWidget *card = new QWidget();
    card->setObjectName("card");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 10));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(24, 20, 24, 20);
    cardLayout->setSpacing(20);

    // App Badge Icon
    QLabel *iconBadge = new QLabel("₫");
    iconBadge->setFixedSize(54, 54);
    iconBadge->setAlignment(Qt::AlignCenter);
    iconBadge->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #10B981, stop:1 #059669);"
        "color: white; font-size: 28px; font-weight: 800; border-radius: 16px;");

    QVBoxLayout *infoBox = new QVBoxLayout();
    infoBox->setSpacing(4);

    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(10);
    QLabel *appName = new QLabel("Quản lý Tài chính Cá nhân");
    appName->setObjectName("sectionTitle");

    QLabel *versionBadge = new QLabel("v1.0.0");
    versionBadge->setObjectName("badge");

    titleRow->addWidget(appName);
    titleRow->addWidget(versionBadge);
    titleRow->addStretch();

    QLabel *teamInfo = new QLabel("Nhóm 10 — Lập trình Hướng Đối Tượng (OOP)");
    teamInfo->setObjectName("footerTitle");

    QLabel *techStack = new QLabel("Phát triển với C++17 · Qt 6 Widgets · SQLite Database");
    techStack->setObjectName("footerText");

    QLabel *members = new QLabel("Thành viên nhóm: An Khang · Hữu Lam · Minh Hạo · Việt Tường · Gia Hưng");
    members->setObjectName("footerText");

    infoBox->addLayout(titleRow);
    infoBox->addWidget(teamInfo);
    infoBox->addWidget(techStack);
    infoBox->addWidget(members);

    cardLayout->addWidget(iconBadge);
    cardLayout->addLayout(infoBox, 1);

    layout->addWidget(card);
}

// ==================== SYNC ====================
void SettingsPage::syncControls()
{
    ThemeManager &theme = ThemeManager::instance();

    for (int i = 0; i < m_swatches.size(); ++i) {
        m_swatches.at(i)->setChecked(i == theme.accentIndex());
    }

    QString currentAvatar = theme.avatarPath();
    for (QPushButton *btn : m_avatarButtons) {
        btn->setChecked(btn->property("avatarPath").toString() == currentAvatar);
    }

    QAbstractButton *densityBtn = m_densityGroup->button(
        theme.density() == TableDensity::Comfortable ? 0 : 1);
    if (densityBtn)
        densityBtn->setChecked(true);

    QAbstractButton *darkBtn = m_darkGroup->button(theme.darkMode() ? 1 : 0);
    if (darkBtn)
        darkBtn->setChecked(true);

    m_chartsCheck->setChecked(theme.chartsEnabled());
}
