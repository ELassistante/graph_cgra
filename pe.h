#ifndef PE_H
#define PE_H
#define G_SIZE 16 

#include <iostream>
#include <vector>
#include <array>

using namespace std ;

class Pe {
    private :
    size_t node ;
    int im ;
    array<size_t,8> neightbourgs ;      //Reg-Left-Right-Top-Down-DB-Im-Self
    array<bool,8> connect;              //Active connections

    public :
    Pe(size_t node) ;                   //Initialize the neightbours

    size_t get_node () ;
    int get_im() ;
    array<size_t,8> get_nbrg () ;
    array<bool,8> get_connect() ;

    void set_node (size_t node) ;
    void set_connect(size_t i) ;
    void set_im(int im) ;
    void clear_connect() ;

} ;

#endif