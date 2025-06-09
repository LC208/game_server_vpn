#include <catch2/catch_test_macros.hpp>
#include "ContainerManager.hpp"
#include "DockerContainer.hpp"
#include "DockerContainerManager.hpp"
#include <cstdio>
#include <iostream>

std::string createTestContainer(const std::string& image = "alpine") {
    std::string cmd = "docker create " + image;
    std::array<char, 256> buffer;
    std::string result;

    FILE* pipe = popen(cmd.c_str(), "r");
    REQUIRE(pipe != nullptr);

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);

    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
}

TEST_CASE("DockerContainer: preserves ID", "[docker][container]") {
    std::string id = createTestContainer();
    DockerContainer container(id);

    REQUIRE(container.id() == id);
    container.remove();
}

TEST_CASE("DockerContainer: start and stop", "[docker][container]") {
    std::string id = createTestContainer();
    DockerContainer container(id);

    container.start();
    std::cout << int(container.status()) << std::endl;


    REQUIRE((container.status() == ContainerStatus::Running || container.status() == ContainerStatus::Exited));

    container.stop();

    REQUIRE(container.status() != ContainerStatus::Running);

    container.remove();
}

TEST_CASE("DockerContainer: remove", "[docker][container]") {
    std::string id = createTestContainer();
    DockerContainer container(id);
    container.remove();

    std::string checkCmd = "docker ps -a -q --filter id=" + id;
    FILE* pipe = popen(checkCmd.c_str(), "r");
    REQUIRE(pipe != nullptr);

    char result[64] = {0};
    fgets(result, sizeof(result), pipe);
    pclose(pipe);

    REQUIRE(std::string(result).find_first_not_of(" \n\r\t") == std::string::npos);
}


TEST_CASE("DockerContainerManager: availability", "[docker][manager]") {
    DockerContainerManager manager;
    REQUIRE(manager.isAvailable());
}

TEST_CASE("DockerContainerManager: create container", "[docker][manager]") {
    DockerContainerManager manager;
    IContainer* container = manager.createContainer("alpine");
    REQUIRE(container != nullptr);
    REQUIRE_FALSE(container->id().empty());

    manager.removeAll();
}

TEST_CASE("DockerContainerManager: manage multiple containers", "[docker][manager]") {
    DockerContainerManager manager;
    IContainer* container1 = manager.createContainer("alpine");
    IContainer* container2 = manager.createContainer("alpine");

    REQUIRE(container1);
    REQUIRE(container2);

    manager.stopAll();
    manager.removeAll();
}