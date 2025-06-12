#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "ContainerManager.hpp"
#include "DockerContainer.hpp"
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif


ContainerFactory& ContainerFactory::setImage(const std::string& image)
{
    this->imageName = image;
    return *this;
}

ContainerFactory& ContainerFactory::setBackend(ContainerBackend backend)
{
    this->backend = backend;
    return *this;
}


ContainerFactory& ContainerFactory::addVolume(const std::string& hostPath, const std::string& containerPath)
{   
    this->volumes.emplace_back(hostPath, containerPath);
    return *this;
}

ContainerFactory& ContainerFactory::addEnv(const std::string& key, const std::string& value)
{
    this->environment.emplace_back(key, value);
    return *this;
}

std::unique_ptr<IContainer> ContainerFactory::create(const std::string& containerType)
{
    if (backend != ContainerBackend::Docker) {
        return nullptr;
    }
    std::stringstream cmd;
    cmd << "docker create ";

    for (const auto& [host, container] : volumes) {
        cmd << "-v " << host << ":" << container << " ";
    }

    for (const auto& [key, value] : environment) {
        cmd << "-e " << key << "=" << value << " ";
    }

    cmd << imageName;

    std::array<char, 128> buffer;
    std::string containerId;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.str().c_str(), "r"), pclose);
    if (!pipe) return nullptr;

    if (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        containerId = buffer.data();
        containerId.erase(containerId.find_last_not_of(" \n\r\t") + 1);
    }

    if (containerId.empty()) return nullptr;
    return std::make_unique<DockerContainer>(containerId);
}
