#include "repository.hpp"

#include "domain_errors.hpp"

#include <fstream>
#include <utility>

json_repository::json_repository(std::filesystem::path path)
    : path_(std::move(path)) {}

void json_repository::save(const app_state& state) {
    std::ofstream output{path_};
    if (!output) {
        throw repository_error("could not open file for writing: " + path_.string());
    }

    const nlohmann::json doc = state;
    output << doc.dump(2);
}

app_state json_repository::load() const {
    std::ifstream input{path_};
    if (!input) {
        throw repository_error("could not open file for reading: " + path_.string());
    }

    nlohmann::json doc;
    input >> doc;
    return doc.get<app_state>();
}

void memory_repository::save(const app_state& state) {
    state_ = state;
}

app_state memory_repository::load() const {
    if (!state_.has_value()) {
        throw repository_error("memory repository is empty");
    }
    return *state_;
}
