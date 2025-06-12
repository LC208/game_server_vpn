#pragma once
#include <string>
#include <vector>

enum class ContainerStatus {
    Created,
    Running,
    Paused,
    Exited,
    Dead,
    Removing,
    Unknown
};

enum class ContainerBackend {
    Docker,
};

class IContainer {
public:
    virtual ~IContainer() = default;
    virtual std::string id() const = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void remove() = 0;
    virtual ContainerStatus status() = 0;
};

class ContainerFactory {
public:
    ContainerFactory& setImage(const std::string& image);
    ContainerFactory& addVolume(const std::string& hostPath, const std::string& containerPath);
    ContainerFactory& addEnv(const std::string& key, const std::string& value);
    ContainerFactory& setBackend(ContainerBackend backend);

    std::unique_ptr<IContainer> create(const std::string& containerType);

private:
    std::string imageName = "alpine";
    ContainerBackend backend = ContainerBackend::Docker;
    std::vector<std::pair<std::string, std::string>> volumes;
    std::vector<std::pair<std::string, std::string>> environment;
};


class ContainerManager {
public:
    virtual ~ContainerManager() = default;

    virtual bool isAvailable() const = 0;

    virtual IContainer* createContainer() = 0;

    virtual void stopAll() = 0;
    virtual void removeAll() = 0;
};