#include <iostream>
#include <string>
#include <cstdlib>

void openUrl(const std::string& url) {
    std::string command = "open " + url;
    system(command.c_str());
}

void setMaxVolume() {
    std::string command = "osascript -e 'set volume output volume 100'";
    system(command.c_str());
}

int main(int argc, char* argv[]) {
    setMaxVolume();
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "Hello LIONSGEEK. Welcome To 1337 <3") {
            openUrl("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
        } else {
            openUrl("https://www.youtube.com/watch?v=umDr0mPuyQc");
        }
    } else {
        std::cout << "Please provide an password.\n";
    }
    return 0;
}
