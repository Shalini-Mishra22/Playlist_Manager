#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fstream>


using namespace std;

class SongNode {
public:
    char song[100];
    SongNode* next;
    SongNode* prev;

    SongNode(const char* songName = "") {
        strcpy(song, songName);
        next = nullptr;
        prev = nullptr;
    }
};

class Playlist {
private:
    SongNode* head;
    SongNode* top;
    SongNode* top1;

    void toFile(const char* song) {
        fstream file;
        file.open("playlist.txt", ios::out | ios::app);
        file << song << endl;
        file.close();
    }

    void deleteFromFile(const char* song) {
        fstream file, tempFile;
        string line;
        bool found = false;

        file.open("playlist.txt", ios::in);
        tempFile.open("temp.txt", ios::out);

        while (getline(file, line)) {
            if (line != song) {
                tempFile << line << endl;
            } else {
                found = true;
            }
        }

        file.close();
        tempFile.close();

        remove("playlist.txt");
        rename("temp.txt", "playlist.txt");

        if (!found) {
            cout << "There is no song with name you entered." << endl;
        } else {
            cout << "Song has been deleted." << endl;
        }
    }

public:
    Playlist() {
        head = nullptr;
        top = nullptr;
        top1 = nullptr;
    }

    void addSong() {
        if (head == nullptr) {
            cout << "Playlist is not initialized." << endl;
            return;
        }

        char song[100];
        cout << "\nEnter Song name: ";
        cin >> song;

        SongNode* newNode = new SongNode(song);
        toFile(song);

        SongNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }

        current->next = newNode;
        newNode->prev = current;
    }

    void addSongFromFile(const string& song) {
        SongNode* newNode = new SongNode(song.c_str());

        SongNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }

        current->next = newNode;
        newNode->prev = current;
    }

    void deleteLastSong() {
        if (head == nullptr || head->next == nullptr) {
            cout << "No songs to delete." << endl;
            return;
        }

        SongNode* current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }

        SongNode* toDelete = current->next;
        current->next = nullptr;
        delete toDelete;

        cout << "Deleted" << endl;
    }

    void printPlaylist() {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }

        cout << "\nPlaylist Name: " << head->song << endl;
        SongNode* current = head->next;

        while (current != nullptr) {
            cout << current->song << endl;
            current = current->next;
        }
    }

    void countSongs() {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }

        int count = 0;
        SongNode* current = head->next;

        while (current != nullptr) {
            count++;
            current = current->next;
        }

        cout << "\nTotal songs: " << count << endl;
    }

    void searchSong() {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }

        char song[100];
        cout << "\nEnter song to be searched: ";
        cin >> song;

        SongNode* current = head->next;
        bool found = false;

        while (current != nullptr) {
            if (strcmp(current->song, song) == 0) {
                cout << "\nSong Found" << endl;
                found = true;
                break;
            }
            current = current->next;
        }

        if (!found) {
            cout << "\nSong Not found" << endl;
        }
    }

    void playSong() {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }

        printPlaylist();
        char song[100];
        cout << "\nChoose song you wish to play: ";
        cin >> song;

        SongNode* current = head->next;
        bool found = false;

        while (current != nullptr) {
            if (strcmp(current->song, song) == 0) {
                cout << "\nNow Playing: " << song << endl;
                found = true;
                push(song);
                break;
            }
            current = current->next;
        }

        if (!found) {
            cout << "\nSong Not found" << endl;
        }
    }

    void push(const char* song) {
        if (top == nullptr) {
            top = new SongNode(song);
        } else {
            if (strcmp(top->song, song) != 0) {
                SongNode* newNode = new SongNode(song);
                newNode->next = top;
                top = newNode;
            }
        }
    }

    void displayRecentlyPlayed() {
        if (top == nullptr) {
            cout << "\nNo recently played tracks." << endl;
            return;
        }

        cout << "\nRecently played tracks:" << endl;
        SongNode* current = top;

        while (current != nullptr) {
            cout << current->song << endl;
            current = current->next;
        }
    }

    void displayLastPlayed() {
        if (top == nullptr) {
            cout << "\nNo last played tracks." << endl;
            return;
        }

        cout << "\nLast Played Song: " << top->song << endl;
    }

    void sortPlaylist() {
        if (head == nullptr || head->next == nullptr) {
            cout << "Playlist is empty or has only one song." << endl;
            return;
        }

        bool swapped;
        SongNode* current;
        SongNode* lastSorted = nullptr;

        do {
            swapped = false;
            current = head->next;

            while (current->next != lastSorted) {
                if (strcmp(current->song, current->next->song) > 0) {
                    swap(current->song, current->next->song);
                    swapped = true;
                }
                current = current->next;
            }
            lastSorted = current;
        } while (swapped);
    }

    void addPlaylistFromFile() {
        fstream file;
        string line;

        file.open("playlist.txt", ios::in);
        while (getline(file, line)) {
            addSongFromFile(line);
        }

        cout << "Playlist Added" << endl;
        file.close();
    }

    void deleteSongMenu() {
        int choice;
        cout << "Which type of delete do you want?\n1. By Search\n2. By Position" << endl;
        cin >> choice;

        if (choice == 1) {
            deleteSongBySearch();
        } else if (choice == 2) {
            int pos;
            cout << "\nEnter the position of the song: ";
            cin >> pos;
            deleteSongByPosition(pos);
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    void deleteSongBySearch() {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }

        printPlaylist();
        char song[100];
        cout << "\nChoose song you wish to delete: ";
        cin >> song;

        SongNode* current = head->next;
        SongNode* prev = head;
        bool found = false;

        while (current != nullptr) {
            if (strcmp(current->song, song) == 0) {
                prev->next = current->next;
                if (current->next != nullptr) {
                    current->next->prev = prev;
                }
                deleteFromFile(current->song);
                delete current;
                found = true;
                cout << "Song Deleted" << endl;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found) {
            cout << "\nSong Not found" << endl;
        }
    }

    void deleteSongByPosition(int pos) {
        if (head == nullptr || head->next == nullptr) {
            cout << "Playlist is empty or has only one song." << endl;
            return;
        }

        if (pos < 1) {
            cout << "Invalid position." << endl;
            return;
        }

        SongNode* current = head->next;
        SongNode* prev = head;

        for (int i = 1; current != nullptr && i < pos; ++i) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Position out of range." << endl;
            return;
        }

        prev->next = current->next;
        if (current->next != nullptr) {
            current->next->prev = prev;
        }
        deleteFromFile(current->song);
        delete current;
        cout << "Song Deleted" << endl;
    }

    void initialize(const char* playlistName) {
        if (head == nullptr) {
            head = new SongNode(playlistName);
        } else {
            cout << "Playlist is already initialized." << endl;
        }
    }
};

int main() {
    int choice;
    char playlistName[100];
    Playlist playlist;

    cout << "\t\t\t WELCOME" << endl;
    cout << "\n*Please use '_' for space." << endl;
    cout << "\nEnter your playlist name: ";
    cin.getline(playlistName, 100);

    playlist.initialize(playlistName);

    do {
        cout << "\n1. Add New Song\n2. Delete Song\n3. Display Entered Playlist\n4. Total Songs\n5. Search Song\n6. Play Song\n7. Recently Played List\n8. Last Played\n9. Sorted Playlist\n10. Add From File\n11. Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                playlist.addSong();
                break;
            case 2:
                playlist.deleteSongMenu();
                break;
            case 3:
                playlist.printPlaylist();
                break;
            case 4:
                playlist.countSongs();
                break;
            case 5:
                playlist.searchSong();
                break;
            case 6:
                playlist.playSong();
                break;
            case 7:
                playlist.displayRecentlyPlayed();
                break;
            case 8:
                playlist.displayLastPlayed();
                break;
            case 9:
                playlist.sortPlaylist();
                playlist.printPlaylist();
                break;
            case 10:
                playlist.addPlaylistFromFile();
                break;
            case 11:
                exit(0);
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 11);

    return 0;
}
