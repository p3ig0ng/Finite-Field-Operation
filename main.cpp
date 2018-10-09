#include "method.hpp"

int main() {
    vector<int> A,F;

    cout<<"Please input --f-- which means the irreducible polynomial of the finite field."<<endl;
    cout<<"the bit of f with -1 as the end"<<endl;
    int tmp= 0;
    while (cin>>tmp,tmp != -1) F.push_back(tmp);

    cout<<"Please input the decimal number: ";
    int num,count = 0;
    cin>>num;
    while (num>0){
        if (num & 0x1) A.push_back(count);
        count++;
        num >>= 1;
    }
    GF2<RANGE> a(0,F),b(0,F);
    a.create(A);

    cout<<"Please input the bit of --b-- with -1 as the end."<<endl;
    b.initial_GF2();

    cout<<"a:"<<endl; a.print_bit(); cout<<endl;
    cout<<"b:"<<endl; b.print_bit(); cout<<endl;
    cout<<"f:"<<endl; a.print_F(); cout<<endl;
    cout<<"a+b:"<<endl; (a+b).print_bit(); cout<<endl<<'\n';
    cout<<"a*b:"<<endl; a.multipy(b).print_bit(); cout<<endl<<'\n';
    cout<<"a%b:"<<endl; a.reduction(b).print_bit(); cout<<endl<<'\n';
    cout<<"square a :"<<endl; a.square().print_bit(); cout<<endl<<'\n';

    cout<<"inverse of a mod f:\n";
    GF2<RANGE> g = a.inverse(); g.print_bit(); cout<<endl;

    cout<<"Check inverse : a^(-1) * a (mod f)"<<endl;
    g.multipy(a).print_bit();

    return 0;
}

