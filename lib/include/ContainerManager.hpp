#pragma once
#include <string>
#include <vector>

class IContainer {
public:
    virtual ~IContainer() = default;
    virtual std::string id() const = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void remove() = 0;
};

class ContainerManager {
public:
    virtual ~ContainerManager() = default;

    virtual bool isAvailable() const = 0;

    virtual IContainer* createContainer(const std::string& imageName) = 0;

    virtual void stopAll() = 0;
    virtual void removeAll() = 0;
};