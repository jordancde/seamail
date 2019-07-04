
#include <memory>
#include <iostream>
#include <vector>

#include "./serializable.h"
#include "../account/account.h"

class LocalState: public Serializable{
    std::ostream& serialize(std::ostream&) const override;
    std::istream& deserialize(std::istream&) override;

    std::vector<std::shared_ptr<Account>> accounts;
    std::vector<std::shared_ptr<EmailProvider>> providers;

public:
    void storeAccount(std::shared_ptr<Account>);
    std::vector<std::shared_ptr<Account>> getAccounts();
    void removeAccount(Account&);

    void storeProvider(std::shared_ptr<EmailProvider>);
    std::vector<std::shared_ptr<EmailProvider>> getProviders();
    void removeProvider(EmailProvider&);

    LocalState(std::vector<std::shared_ptr<Account>> accounts, std::vector<std::shared_ptr<EmailProvider>> providers);
    LocalState(const LocalState&);
    LocalState();
    ~LocalState();
};