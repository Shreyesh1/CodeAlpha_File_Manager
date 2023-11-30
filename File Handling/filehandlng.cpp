#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void displayMenu() {
    std::cout << "Command-line File Manager\n";
    std::cout << "1. List files in current directory\n";
    std::cout << "2. Navigate to a directory\n";
    std::cout << "3. Create a directory\n";
    std::cout << "4. Copy a file\n";
    std::cout << "5. Move a file\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

void listFilesInDirectory(const fs::path& directoryPath) {
    std::cout << "Files in " << directoryPath << ":\n";
    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            std::cout << entry.path().filename() << "\n";
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

bool isDirectory(const fs::path& path) {
    return fs::is_directory(path);
}

bool isValidChoice(int choice) {
    return (choice >= 1 && choice <= 6);
}

int main() {
    fs::path currentPath = fs::current_path();

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        if (!isValidChoice(choice)) {
            std::cout << "Invalid choice! Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                listFilesInDirectory(currentPath);
                break;
            }
            case 2: {
                std::string newPath;
                std::cout << "Enter directory path to navigate: ";
                std::cin.ignore(); // Clear input buffer
                std::getline(std::cin, newPath);
                fs::path absolutePath = fs::absolute(newPath);
                if (!isDirectory(absolutePath)) {
                    std::cout << "Not a valid directory path!\n";
                } else {
                    currentPath = absolutePath;
                    std::cout << "Successfully navigated to: " << currentPath << "\n";
                }
                break;
            }
            case 3: {
                std::string newDirName;
                std::cout << "Enter directory name to create: ";
                std::cin.ignore();
                std::getline(std::cin, newDirName);
                try {
                    fs::create_directory(currentPath / newDirName);
                    std::cout << "Directory created successfully.\n";
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error creating directory: " << e.what() << std::endl;
                }
                break;
            }
            case 4: {
                std::string source, destination;
                std::cout << "Enter source file path: ";
                std::cin >> source;
                std::cout << "Enter destination file path: ";
                std::cin >> destination;
                try {
                    fs::copy(currentPath / source, currentPath / destination);
                    std::cout << "File copied successfully.\n";
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error copying file: " << e.what() << std::endl;
                }
                break;
            }
            case 5: {
                std::string source, destination;
                std::cout << "Enter source file path: ";
                std::cin >> source;
                std::cout << "Enter destination file path: ";
                std::cin >> destination;
                try {
                    fs::rename(currentPath / source, currentPath / destination);
                    std::cout << "File moved successfully.\n";
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error moving file: " << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                std::cout << "Exiting...\n";
                break;
            }
            default: {
                std::cout << "Invalid choice! Please try again.\n";
                break;
            }
        }
    } while (choice != 6);

    return 0;
}
