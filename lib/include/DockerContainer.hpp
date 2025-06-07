#pragma once
#include "ContainerManager.hpp"

class DockerContainer : public IContainer {
public:
    DockerContainer(std::string id);
    std::string id() const override;
    void start() override;
    void stop() override;
    void remove() override;

private:
    std::string containerId;
    bool running = false;
};