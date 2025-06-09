#include "DockerContainer.hpp"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include "trim.hpp"

static int runCommand(const std::string& cmd) {
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    int status = pclose(pipe.release());

    return WEXITSTATUS(status);
}

static std::string runCommandWithReturn(const std::string& cmd)
{
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    
    return result;
}

DockerContainer::DockerContainer(std::string id) : containerId(std::move(id)) {}

std::string DockerContainer::id() const { 
    return containerId; 
}

void DockerContainer::start() {
    std::string cmd = "docker start " + containerId + " 2>&1";
    int status = runCommand(cmd);
    if (status == 0) {
        running = true;
    } else {
        throw std::runtime_error("Failed to start container: " + containerId);
    }
}

void DockerContainer::stop() {
    std::string cmd = "docker stop " + containerId + " 2>&1";
    int status = runCommand(cmd);
    if (status == 0) {
        running = false;
    } else {
        throw std::runtime_error("Failed to stop container: " + containerId);
    }
}

void DockerContainer::remove() {
    std::string cmd = "docker rm " + containerId + " 2>&1";
    int status = runCommand(cmd);
    if (status != 0) {
        throw std::runtime_error("Failed to remove container: " + containerId);
    }
}


ContainerStatus DockerContainer::status() {
    std::string s = trim(runCommandWithReturn( "docker inspect -f '{{.State.Status}}' " + containerId));
    
    if (s == "created") return ContainerStatus::Created;
    if (s == "running") return ContainerStatus::Running;
    if (s == "paused")  return ContainerStatus::Paused;
    if (s == "exited")  return ContainerStatus::Exited;
    if (s == "dead")    return ContainerStatus::Dead;
    if (s == "removing")return ContainerStatus::Removing;
    return ContainerStatus::Unknown;
}