#include "AccountService.h"

AccountService::AccountService(QObject* parent) : QObject(parent)
{
}

bool AccountService::addAccount(const Account& account, QString* errorMessage)
{
    if (account.getName().isEmpty()) {
        if (errorMessage) *errorMessage = "Tên tài khoản không được để trống!";
        return false;
    }
    if (m_accountRepo.addAccount(account)) {
        emit accountAdded(account);
        return true;
    }
    if (errorMessage) *errorMessage = "Lỗi cơ sở dữ liệu khi thêm tài khoản!";
    return false;
}

bool AccountService::updateAccount(const Account& account, QString* errorMessage)
{
    if (account.getName().isEmpty()) {
        if (errorMessage) *errorMessage = "Tên tài khoản không được để trống!";
        return false;
    }
    if (m_accountRepo.updateAccount(account)) {
        emit accountUpdated(account);
        return true;
    }
    if (errorMessage) *errorMessage = "Lỗi cơ sở dữ liệu khi cập nhật tài khoản!";
    return false;
}

bool AccountService::removeAccount(int id, QString* errorMessage)
{
    if (m_accountRepo.deleteAccount(id)) {
        emit accountRemoved(id);
        return true;
    }
    if (errorMessage) *errorMessage = "Lỗi cơ sở dữ liệu khi xóa tài khoản! Có thể tài khoản đang có giao dịch.";
    return false;
}

QVector<Account> AccountService::getAllAccounts()
{
    return m_accountRepo.getAllAccounts();
}

Account AccountService::getAccountById(int id)
{
    return m_accountRepo.getAccountById(id);
}
