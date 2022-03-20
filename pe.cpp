#include "pe.h"

using namespace std ;

Pe::Pe(size_t node) : node(node) {
    for (int i(0); i < 8 ; ++i) {
        voisins.push_back(0); 
        connect.push_back(false);
    }
    switch(node) {
        case 0 :
        this->voisins[0] = 3 ; this->voisins[1] = 1 ; this->voisins[2] = 12 ; this->voisins[3] = 4 ;
        break ;

        case 1 :
        this->voisins[0] = 0 ; this->voisins[1] = 2 ; this->voisins[2] = 13 ; this->voisins[3] = 5 ;
        break ;

        case 2 :
        this->voisins[0] = 1 ; this->voisins[1] = 3 ; this->voisins[2] = 14 ; this->voisins[3] = 6 ;
        break ;

        case 3 :
        this->voisins[0] = 2 ; this->voisins[1] = 0 ; this->voisins[2] = 15 ; this->voisins[3] = 7 ;
        break ;

        case 4 :
        this->voisins[0] = 7 ; this->voisins[1] = 5 ; this->voisins[2] = 0 ; this->voisins[3] = 8 ;
        break ;

        case 5 :
        this->voisins[0] = 4 ; this->voisins[1] = 6 ; this->voisins[2] = 1 ; this->voisins[3] = 9 ;
        break ;

        case 6 :
        this->voisins[0] = 5 ; this->voisins[1] = 7 ; this->voisins[2] = 2 ; this->voisins[3] = 10 ;
        break ;

        case 7 :
        this->voisins[0] = 6 ; this->voisins[1] = 4 ; this->voisins[2] = 3 ; this->voisins[3] = 11 ;
        break ;

        case 8 :
        this->voisins[0] = 11 ; this->voisins[1] = 9 ; this->voisins[2] = 4 ; this->voisins[3] = 12 ;
        break ;

        case 9 :
        this->voisins[0] = 8 ; this->voisins[1] = 10 ; this->voisins[2] = 5 ; this->voisins[3] = 13 ;
        break ;

        case 10 :
        this->voisins[0] = 9 ; this->voisins[1] = 11 ; this->voisins[2] = 6 ; this->voisins[3] = 14 ;
        break ;

        case 11 :
        this->voisins[0] = 10 ; this->voisins[1] = 8 ; this->voisins[2] = 7 ; this->voisins[3] = 15 ;
        break ;

        case 12 :
        this->voisins[0] = 15 ; this->voisins[1] = 13 ; this->voisins[2] = 8 ; this->voisins[3] = 0 ;
        break ;

        case 13 :
        this->voisins[0] = 12 ; this->voisins[1] = 14 ; this->voisins[2] = 9 ; this->voisins[3] = 1 ;
        break ;

        case 14 :
        this->voisins[0] = 13 ; this->voisins[1] = 15 ; this->voisins[2] = 10 ; this->voisins[3] = 2 ;
        break ;

        case 15 :
        this->voisins[0] = 14 ; this->voisins[1] = 12 ; this->voisins[2] = 11 ; this->voisins[3] = 3 ;
        break ;

        default :
        exit(0) ;
    }
}

size_t Pe::get_uid () {
    return this->uid ;
}
size_t Pe::get_node () {
    return this->node ;
}
vector<size_t> Pe::get_voisins () {
    return this->voisins ;
}
vector<bool> Pe::get_connect() {
    return this->connect ;
}

void Pe::set_node (size_t node) {
    this->node = node ;
}
void Pe::set_connect(size_t i) {
    this->connect[i] = true ;
}
void Pe::clear_connect() {
    for(int i(0); i < this->connect.size(); ++i) {
        this->connect[i] = false ;
    }
}