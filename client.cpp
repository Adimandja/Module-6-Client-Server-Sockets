#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

enum Color {
    SUNSET,
    CHERRY,
    PHANTOM,
    TITANIUM,
    GEAUX,
    LIGHTNING,
    FOREST,
    MIDNIGHT,
    PASSION,
    ROOT,
    STORM,
    OCEAN,
    GLACIAL,
    COLOR_COUNT
};

const char* colorNames[] = {
    "Sunset Orange",  // SUNSET
    "Cherry Bomb",    // CHERRY
    "Phantom Black",  // PHANTOM
    "Titanium Silver",// TITANIUM
    "Geaux Gold",     // GEAUX
    "Lightning Yellow", // LIGHTNING
    "Forest Green",   // FOREST
    "Midnight Blue",  // MIDNIGHT
    "Passion Purple", // PASSION
    "Root Beer",      // ROOT
    "Storm Surge",    // STORM
    "Ocean's Rip",    // OCEAN
    "Glacial White"   // GLACIAL
};

int main() {
    int client_fd;
    struct sockaddr_in server_addr;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(client_fd);
        return 1;
    }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return 1;
    }

    int color_enum;
    std::cout << "Select a color:\n";
    for (int i = 0; i < COLOR_COUNT; ++i) {
        std::cout << i << ": " << colorNames[i] << "\n";
    }
    std::cin >> color_enum;

    if (color_enum < 0 || color_enum >= COLOR_COUNT) {
        std::cerr << "Invalid color selection" << std::endl;
        close(client_fd);
        return 1;
    }

    if (write(client_fd, &color_enum, sizeof(color_enum)) < 0) {
        std::cerr << "Write failed" << std::endl;
        close(client_fd);
        return 1;
    }

    char buffer[32];
    if (read(client_fd, buffer, sizeof(buffer)) < 0) {
        std::cerr << "Read failed" << std::endl;
        close(client_fd);
        return 1;
    }

    std::cout << "Server Response: " << buffer << colorNames[color_enum] << "\033[0m" << std::endl;

    close(client_fd);
    return 0;
}
