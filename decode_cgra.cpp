/*
Clement DUBOS
28.03.22
DECODE CGRA
decode_cgra.cpp
Decoder for "cgra_instructions.txt", return to txt files :
-graph.txt => connections of all PEs with minimum informations about connections
-decode.txt => decoding of every instructions with all the informations
*/

#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include "pe.h"

using namespace std ;

enum State {NO,PROLOG,KERNEL,EPILOG} ;      //Different states of the script
enum Format {P_TYPE, R_TYPE} ;              //Different instruction formats

array<Pe*,G_SIZE>current ;       //Stock the state of each active PE at each iteration
array<bool,G_SIZE>cur_act  ;      //Tells which PE is active at each iteration

void hex_to_bin (string code,unsigned int& bin_code) ;      //Convert a string of hexadecimal number to a binary number
void conv_text(unsigned int text_bin, string& text_code, size_t cnt) ;  //Convert the binary code to the instructions
void connect_graph (size_t cnt, unsigned int dir) ;     //Update the connections of the PE cnt to the PE dir
string to_op (unsigned int num, Format form) ;          //Transform a number to opcode
string to_form (unsigned int num, Format& form) ;       //Transform a boolean to the instruction format (P or R)
string to_mux (unsigned int num) ;      //Convert a number to the entry enabled by the multiplexer
string connection(Pe* p) ;        //Write the connections of the PE

int main () {

    State etat = NO ;
    string text("") ;       //used to stock each line
    string code("") ;       //used to extract the important part of the line
    string text_code("") ;      //used to store the converted instruction
    unsigned int code_bin (0b0) ;   //used to stock the binary instruction
    int i(0) ;      //counter
    int time(0) ;   //time of the execution
    size_t ctp (0) ;    //counter for prolog
    size_t ctk (0) ;    //counter for kernel
    size_t cte(0) ;     //counter for epilog
    ifstream instr ;
    ofstream decode ;
    ofstream graph ;

    instr.open("cgra_instructions.txt") ;
    decode.open("decode.txt") ;
    graph.open("graph.txt") ;
//Read each line in cgra_instructions.txt
    while(getline(instr >> ws, text)) {
        i++ ;
        switch(etat) {
            case NO :       //wait for prolog line
            if (text == "*******PROLOG*********"){
                etat = PROLOG ;
                i = 0 ; time = 0 ;
                decode << "*******PROLOG*********" << endl ;
                graph << "*******PROLOG*********" << endl ;
                continue ;
            }
            else continue ;
            break ;

            case PROLOG :
            if (text == "*******KERNEl*********") {
                etat = KERNEL ;
                i = 0 ;
                time = 0 ;
                decode << "*******KERNEl*********" << endl ;
                graph << "*******KERNEl*********" << endl ;
                continue ;
            }
            else {
                if (ctp < 100) code = text.substr(3,text.size() - 1) ;  //deals with line number with multiple characters
                else if (ctp < 1000) code = text.substr(4,text.size() - 1) ;
                else if (ctp < 10000) code = text.substr(5,text.size() - 1) ;
            }
            ctp++ ; 
            break ;

            case KERNEL :
            if (text == "*******EPILOG*********") {
                etat = EPILOG ;
                i = 0 ; time = 0 ;
                decode << "*******EPILOG*********" << endl ;
                graph << "*******EPILOG*********" << endl ;
                continue ;
            } 
            else {
                if (ctk < 100) code = text.substr(3,text.size() - 1) ;  //deals with line number with multiple characters
                else if (ctk < 1000) code = text.substr(4,text.size() - 1) ;
                else if (ctk < 10000) code = text.substr(5,text.size() - 1) ;
            }
            ctk++ ; 
            break ;

            case EPILOG :
            if (cte < 100) code = text.substr(3,text.size() - 1) ;      //deals with line number with multiple characters
            else if (cte < 1000) code = text.substr(4,text.size() - 1) ;
            else if (cte < 10000) code = text.substr(5,text.size() - 1) ;
            cte++ ; 
            break ;

            default :
            decode << "Error state" << endl ;
            break ;

        }
        hex_to_bin(code, code_bin) ;
        conv_text(code_bin, text_code, (i-1)%16) ;
        if (i%16 == 0){                 //Write information for each time iterations    
            graph << "Time : " << time << endl ;
            for (size_t j(0); j < G_SIZE ; ++j) {
                if(cur_act[j])  graph << "PE_" << j << " : " << connection(current[j]) << endl ;
            }
            graph << "-------" << endl << endl ;
            for (size_t j(0); j < G_SIZE ; ++j) {
                if(cur_act[j]) delete current[j] ;
                cur_act[j] = false ;
            }
            time++ ;
        }
        if ((i-1)%16 == 0) decode << "------" << endl ;
        decode << "PE_" <<(i-1)%16 << ": " << text_code << endl ;
        text_code = "" ;
        code_bin = 0 ;

    }

    return 0 ;
}


void hex_to_bin (string code,unsigned int& bin_code) {
    if (code.size() == 7) code.insert(0,"0") ;
    std::istringstream converter(code);
    converter >> std::hex >> bin_code;
}

void conv_text(unsigned int text_bin, string& text_code, size_t cnt) {
    unsigned int temp(0) ;
    unsigned int mask = 0x0 ;
    Format form = R_TYPE ;
//extract each information from binary code
    for (int i(0); i < 11 ; ++i ) { 
        switch(i) {
        case 0:
            mask = 0b1 ;
            mask <<= 27 ;
            temp = text_bin & mask ;
            temp >>= 27 ;
            text_code += " | Format : " + to_form(temp,form) ;
            break ;

        case 1 :
            mask = 0xf ;
            mask <<= 28 ;
            temp = text_bin & mask ;
            temp >>= 28 ;
            text_code += " | OpCode : " + to_op(temp,form) ;
            if (to_op(temp,form) == "NOP") {
                return ;
            }
            else{
                cur_act[cnt] = true ;
                current[cnt] = new Pe(cnt) ;
            }
            break ;

        case 2 :
            mask = 0b111 ;
            mask <<= 24 ;
            temp = text_bin & mask ;
            temp >>= 24 ;
            text_code += " | Lmux : " + to_mux(temp) ;
            connect_graph(cnt, temp) ;
            break ;
        case 3 :
            mask = 0b111 ;
            mask <<= 21 ;
            temp = text_bin & mask ;
            temp >>= 21 ;
            text_code += " | Rmux : " + to_mux(temp) ;
            connect_graph(cnt, temp) ;
            break ;
        case 4 :
            mask = 0b11 ;
            mask <<= 19 ;
            temp = text_bin & mask ;
            temp >>= 19 ;
            text_code += " | R1 : " + to_string(temp) ;
            break ;
        case 5 :
            mask = 0b11 ;
            mask <<= 17 ;
            temp = text_bin & mask ;
            temp >>= 17 ;
            text_code += " | R2 : " + to_string(temp) ;
            break ;
        case 6 :
            mask = 0b11 ;
            mask <<= 15 ;
            temp = text_bin & mask ;
            temp >>= 15 ;
            if (form == R_TYPE) text_code += " | RW : " + to_string(temp) ;
            else if (form == P_TYPE) text_code += " | RP : " + to_string(temp) ;
            break ;
        case 7 :
            if (form == R_TYPE) {
                mask = 0b1 ;
                mask <<= 14 ;
                temp = text_bin & mask ;
                temp >>= 14 ;
                text_code += " | WE : " + to_string(temp) ;
                break ;
            } else if (form == P_TYPE) {
                mask = 0b111 ;
                mask <<= 12 ;
                temp = text_bin & mask ;
                temp >>= 12 ;
                text_code += " | Pmux : " + to_mux(temp) ;
                break ;
            }
        case 8 :
            if (form == R_TYPE) {
                mask = 0b1 ;
                mask <<= 13 ;
                temp = text_bin & mask ;
                temp >>= 13 ;
                text_code += " | AB : " + to_string(temp) ;
                break ;
            } else if (form == P_TYPE){
                mask = 0xfff ;
            temp = text_bin & mask ;
            text_code += " | Im : " + to_string(temp) ;
            if(cur_act[cnt]) current[cnt]->set_im(temp) ;
            break ;
            }
        case 9 :
            if (form == R_TYPE) {
                mask = 0b1 ;
                mask <<= 12 ;
                temp = text_bin & mask ;
                temp >>= 12 ;
                text_code += " | DB : " + to_string(temp) ;
                break ;
            } else break ;
        case 10 :
            if (form == R_TYPE) {
                mask = 0xfff ;
                temp = text_bin & mask ;
                text_code += " | Im : " + to_string(temp) ;
                if(cur_act[cnt]) current[cnt]->set_im(temp) ;
                break ;
            } else break ;
        default :
        text_code += "Error bin" ;
        exit(0) ;
        }
    }
}

string to_op (unsigned int num, Format form) {
    string op ;
    if (form == R_TYPE) {
        switch(num) {
            case 0 :
            op = "Add" ;
            break ;

            case 1 :
            op = "Sub" ;
            break ;

            case 2 :
            op = "Mult" ;
            break ;
            
            case 3 :
            op = "AND" ;
            break ;
            
            case 4 :
            op = "OR" ;
            break ;
            
            case 5:
            op = "XOR" ;
            break ;
            
            case 6 :
            op = "cgraASR" ;
            break ;
            
            case 7 :
            op = "NOP" ;
            break ;
            
            case 8 :
            op = "cgraASL" ;
            break ;
            
            case 9 :
            op = "Div" ;
            break ;
            
            case 10 :
            op = "Rem" ;
            break ;
            
            case 11 :
            op = "LSHR" ;
            break ;
            
            case 12 :
            op = "EQ" ;
            break ;
            
            case 13 :
            op = "NEQ" ;
            break ;
            
            case 14 :
            op = "GT" ;
            break ;
            
            case 15 :
            op = "LT" ;
            break ;    

            default : 
            op = "Error R_type" ;
            break ;
        }
    } else if (form == P_TYPE) {
        switch(num) {
            case 0 :
            op = "SetConfigBoundary" ;
            break ;

            case 1 :
            op = "LDI" ;
            break ;

            case 2 :
            op = "LDMI" ;
            break ;
            
            case 3 :
            op = "LDUI" ;
            break ;
            
            case 4 :
            op = "sel" ;
            break ;
            
            case 5:
            op = "loopexit" ;
            break ;
            
            case 6 :
            op = "adress_gen" ;
            break ;
            
            case 7 :
            op = "NOP" ;
            break ;
            
            case 8 :
            op = "signExtend" ;
            break ;

            default :
            op = "Error P_Type" ;
            break ;
        }
    }

    return op ;
}

string to_form(unsigned int num, Format& form) {
    if (num == 0b1){
        form = P_TYPE; 
        return "P-type" ;
    }
    else if (num == 0b0) {
        form = R_TYPE ;
        return "R-type" ;
    }
}

string to_mux (unsigned int num) {
    string mux ;
    switch(num) {
        case 0 :
        mux = "Reg" ;
        break ;

        case 1 :
        mux = "Left" ;
        break ;

        case 2 :
        mux = "Right" ;
        break ;
        
        case 3 :
        mux = "Upper" ;
        break ;
        
        case 4 :
        mux = "Bottom" ;
        break ;
        
        case 5:
        mux = "DB" ;
        break ;
        
        case 6 :
        mux = "Im" ;
        break ;
        
        case 7 :
        mux = "Self" ;
        break ;

        default :
        mux = "Error mux" ;
        break ;
    }
    return mux ;
}

void connect_graph (size_t cnt, unsigned int dir)  {
    if(cur_act[cnt])current[cnt]->set_connect(dir) ;
}

string connection(Pe* p) {
    string txt ("");
    for (size_t i(0); i < p->get_nbrg().size() ; ++i) {
        if (p->get_connect()[i]) {
            switch (i) {
                case 0 :
                txt += " Reg |" ;
                break ;

                case 1 :
                case 2 :
                case 3 :
                case 4 :
                txt += " PE_" + to_string(p->get_nbrg()[i]) + " |"  ;
                break ;

                case 5 :
                txt += " DB |" ;
                break ;

                case 6 :
                txt += " Im = " + to_string(p->get_im()) + " |";
                break ;

                case 7 :
                txt += " Self |" ;
                break ;

                default : 
                txt += " Error connections " ;
                break ;
            }
        }
    }
    return txt ;
}
