#ifndef PE_H
#define PE_H
#define G_SIZE 16 

#include <iostream>
#include <vector>

using namespace std ;

class Pe {
    private :
    size_t node ;
    int im ;
    vector<size_t> neightbourgs ;    //Reg-Left-Right-Top-Down-DB-Im-Self
    vector<bool> connect;

    public :
    Pe(size_t node) ;

    size_t get_node () ;
    int get_im() ;
    vector<size_t> get_nbrg () ;
    vector<bool> get_connect() ;

    void set_node (size_t node) ;
    void set_connect(size_t i) ;
    void set_im(int im) ;
    void clear_connect() ;

} ;

#endif