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

const char* colorStrings[] = {
    "\033[1m\033[38;5;166m",  // SUNSET
    "\033[1m\033[38;5;160m",  // CHERRY
    "\033[1m\033[38;5;0m\033[48;5;15m",  // PHANTOM
    "\033[1m\033[38;5;249m",  // TITANIUM
    "\033[1m\033[38;5;179m",  // GEAUX
    "\033[1m\033[38;5;11m",  // LIGHTNING
    "\033[1m\033[38;5;2m",  // FOREST
    "\033[1m\033[38;5;17m\033[48;5;15m",  // MIDNIGHT
    "\033[1m\033[38;5;5m",  // PASSION
    "\033[1m\033[38;5;94m",  // ROOT
    "\033[1m\033[38;5;242m",  // STORM
    "\033[1m\033[38;5;27m",  // OCEAN
    "\033[1m\033[38;5;15m"   // GLACIAL
};

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }

    while (true) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        int color_enum;
        if (read(client_fd, &color_enum, sizeof(color_enum)) < 0) {
            std::cerr << "Read failed" << std::endl;
            close(client_fd);
            continue;
        }

        std::cout << "receiving: " << color_enum << std::endl;

        const char* colorString;
        if (color_enum >= 0 && color_enum < COLOR_COUNT) {
            colorString = colorStrings[color_enum];
        } else {
            colorString = "\033[0m"; // RESET for invalid value
        }

        if (write(client_fd, colorString, strlen(colorString) + 1) < 0) {
            std::cerr << "Write failed" << std::endl;
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
