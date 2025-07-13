#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Command.h"


template<typename T>
class CommandFactory {
public:
    using Creator = std::function<std::unique_ptr<T>(const std::vector<std::string>&)>;

    static CommandFactory& instance() {
        static CommandFactory inst;
        return inst;
    }

    void registerType(const std::string& name, Creator creator) {
        creators_[name] = std::move(creator);
    }

    std::unique_ptr<T> create(const std::vector<std::string>& tokens) const {
        if (tokens.empty()) return nullptr;
        auto it = creators_.find(tokens[0]);
        if (it != creators_.end())
            return it->second(tokens);
        return nullptr;
    }

    std::vector<std::string> list() const {
        std::vector<std::string> keys;
        keys.reserve(creators_.size());
        for (auto& kv : creators_) keys.push_back(kv.first);
        return keys;
    }

private:
    CommandFactory() = default;
    CommandFactory(const CommandFactory&) = delete;
    void operator=(const CommandFactory&) = delete;

    std::unordered_map<std::string, Creator> creators_;
};
