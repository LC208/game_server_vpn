#pragma once
#include "ContainerManager.hpp"
#include <vector>
#include <memory>

class DockerContainerManager : public ContainerManager {
public:
    bool isAvailable() const override;
    IContainer* createContainer(const std::string& imageName) override;
    void stopAll() override;
    void removeAll() override;

private:
    std::vector<std::unique_ptr<IContainer>> containers;
};