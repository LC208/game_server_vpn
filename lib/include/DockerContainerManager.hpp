#pragma once
#include "ContainerManager.hpp"
#include <vector>
#include <memory>

class DockerContainerManager : public ContainerManager {
public:
    explicit DockerContainerManager(ContainerFactory& factory) : factory(factory) {}
    bool isAvailable() const override;
    IContainer* createContainer() override;
    void stopAll() override;
    void removeAll() override;

private:
    std::vector<std::unique_ptr<IContainer>> containers;
    ContainerFactory& factory;
};