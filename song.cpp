#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include "songs.h"
using namespace std;

enum Action{
    PREVIOUS,
    NEXT,
    QUIT
};

struct Node {
    struct Node *previous_song;    
    struct Song *song;
    struct Node *next_song;    
};

struct Node * generate_playlist(std::vector<struct Song> *songs, int n_songs=30);
struct Node * push_song(struct Node *previous, struct Song *s);
int get_user_action();

int main() {
    std::string filename = "metadata.csv";
    std::vector<struct Song> songs = read_songs(filename);
    cout<<"Loaded "<<songs.size()<<" songs"<<endl;

    int n_songs;
    cout<<"How many songs you want to listen? "; cin>>n_songs;
    struct Node *head = generate_playlist(&songs, n_songs);
    cout<<"\033[2J\033[1;1H"; // clear screen

    struct Node *current_song = head;
    while (true) {
        cout<<"Playing: "<<endl;
        current_song->song->long_info();
        int action = get_user_action();
        if (action == QUIT) {
            break;
        } else if (action == PREVIOUS && current_song->previous_song != NULL) {
            current_song = current_song->previous_song;
        } else if (action == NEXT && current_song->next_song != NULL) {
            current_song = current_song->next_song;
        } else if (action == NEXT && current_song->next_song == NULL) {
            break;
        }
        cout << "\033[2J\033[1;1H"; // clear screen
    }
    cout<<"All songs played\n";
    return 1;
}

struct Node * generate_playlist(std::vector<struct Song> *songs, int n_songs) {
    struct Node *head=NULL;
    struct Node *current_node=NULL;
    srand(time(NULL));
    for (int i=0; i<n_songs; i++) {
        int index = rand() % songs->size();
        current_node = push_song(current_node, &songs->at(index));
        if (i==0) 
            head = current_node;
    }
    return head;
}

struct Node * push_song(struct Node *previous, struct Song *s) {
    struct Node *new_song = new struct Node();
    new_song->previous_song = previous;
    new_song->song = s;
    new_song->next_song = NULL;
    if (previous)
        previous->next_song = new_song;
    return new_song;
}

int get_user_action() {
    char input;
    cout<<"Press (q) to quit: "<<endl;
    cout<<"Press <- (p) or (n)-> to change song: "<<endl;
    
    cin>>input;

    int action;
    if (input == 'n')
        action = NEXT;
    else if (input == 'p')
        action = PREVIOUS;
    else if (input == 'q')
        action = QUIT;
    return action;
}
