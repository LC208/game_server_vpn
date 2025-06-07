#include "DockerContainerManager.hpp"
#include "DockerContainer.hpp"
#include <memory>
#include <cstdio>
#include <array>
#include <string>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

IContainer* DockerContainerManager::createContainer(const std::string& imageName) {
    std::string cmd = "docker create " + imageName;

    std::array<char, 128> buffer;
    std::string containerId;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return nullptr;

    if (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        containerId = buffer.data();
        containerId.erase(containerId.find_last_not_of(" \n\r\t") + 1);
    }
    pclose(pipe);

    if (containerId.empty()) return nullptr;

    auto container = std::make_unique<DockerContainer>(containerId);
    IContainer* rawPtr = container.get();
    containers.push_back(std::move(container));

    return rawPtr;
}

bool DockerContainerManager::isAvailable() const {
    const char* cmd = "docker --version";

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        return false; 
    }

    char buffer[128];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result.find("Docker version") != std::string::npos;
}

void DockerContainerManager::stopAll() {
    for (auto& c : containers) c->stop();
}

void DockerContainerManager::removeAll() {
    for (auto& c : containers) c->remove();
    containers.clear();
}