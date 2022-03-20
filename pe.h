#ifndef PE_H
#define PE_H
#define G_SIZE 16 

#include <iostream>
#include <vector>

using namespace std ;

class Pe {
    private :
    size_t uid ;
    size_t node ;
    vector<size_t> voisins ;    //Reg-Left-Right-Top-Down-DB-Im-Self
    vector<bool> connect;

    public :
    Pe(size_t node) ;

    size_t get_uid () ;
    size_t get_node () ;
    vector<size_t> get_voisins () ;
    vector<bool> get_connect() ;

    void set_node (size_t node) ;
    void set_connect(size_t i) ;
    void clear_connect() ;

} ;

#endif