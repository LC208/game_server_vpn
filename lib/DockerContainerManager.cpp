#include "DockerContainerManager.hpp"
#include <memory>
#include <cstdio>
#include <string>
#include "ContainerManager.hpp"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

IContainer* DockerContainerManager::createContainer() {
    auto container = factory.create("docker");
    if (!container) return nullptr;
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