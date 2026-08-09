#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

#include "serialization.hpp"

#include <filesystem>
#include <optional>

class state_repository {
public:
    virtual void save(const app_state& state) = 0;
    virtual app_state load() const = 0;
    virtual ~state_repository() = default;
};

class json_repository : public state_repository {
private:
    std::filesystem::path path_;

public:
    explicit json_repository(std::filesystem::path path);

    void save(const app_state& state) override;
    app_state load() const override;
};

class memory_repository : public state_repository {
private:
    std::optional<app_state> state_;

public:
    void save(const app_state& state) override;
    app_state load() const override;
};

#endif
