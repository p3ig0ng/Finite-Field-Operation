#pragma once

#include <iostream>
#include <bitset>
#include <vector>

#define RANGE 131

using namespace std;

template <size_t R>
class GF2{
public:
    GF2() = default;

    explicit GF2(unsigned int N,vector<int> f):C(N) {
        F.reset();
        for (auto i:f) {
            if (i > R) {
                cout<<"Invalid"<<endl;
                return ;
            }
            F.flip(static_cast<size_t>(i));
        }
    }

    explicit GF2(unsigned int N,const bitset<R+1>& f):C(N),F(f){}

    explicit GF2(const bitset<R>& f):C(f) // this construct is used to copy F
    {
        for (int i=0;i<R;i++) F[i] = f[i];
    }

    GF2<R>& operator=(const GF2<R>&) = default;

    bool create(const vector<int> & B) {
        C.reset();
        for (auto i:B) {
            if (i >= R)
                return false;
            C.flip(static_cast<size_t>(i));
        }
        return true;
    }

    void initial_GF2(){
        int tmp= 0;
        vector<int> B;
        while (cin>>tmp,tmp != -1) B.push_back(tmp);
        if (!create(B)) {
            cout<< "Out of Range."<<endl;
            exit(0);
        };
    }

    bool get_F_i(int i) { return F[i];}

    void print_bit(){
        for (int i=R-1;i>=0;i--) cout<<C[i];
    }

    void print_F(){
        for (int i=R;i>=0;i--) cout<<F[i];
    }

    GF2<R> operator+(const GF2<R> &a) { //addition
        GF2 result(0,F);
        result.C = C ^ a.C;
        return result;
    }

    GF2<R> multipy(const GF2<R> a) {
        bitset<R*2+1> tmp_F(0);
        for (int i=0;i<=R;i++) tmp_F[i] = F[i];

        GF2<R*2> tmp(0,tmp_F);
        for (int i=0;i<R;i++) tmp.C[i] = C[i];

        GF2<R*2> result(0,tmp_F);
        for (int i=0;i<R;i++) {
            if (a.C[i]) result = result +tmp;
            tmp.C <<= 1;
        }
        GF2<R> result2(0,F);
        to_R(result2,result);
        return result2;
    }

    GF2<R> square() {
        bitset<R*2+1> tmp_F(0);
        for (int i=0;i<=R;i++) tmp_F[i] = F[i];

        GF2<R*2> result(0,tmp_F);
        for (int i=0;i<R;i++)
            result.C[2*i] = C[i];
        GF2<R> result2(0,F);
        to_R(result2,result);
        return result2;
    }

    GF2<R> reduction(GF2<R> b) {
        GF2<R> t(0,F); t.C = C;
        if (t<b) return t;
        GF2<R> a(b);
        int l1 = get_top<R>(t.C,R-1), l2 = get_top<R>(b.C,R-1);
        while (l1 >= l2){
            t.C = t.C ^ ( a.C << (l1-l2));
            l1 = get_top<R>(t.C,l1);
        }
        return t;
    }

    GF2<R> inverse() {
        if (C.none())
            return GF2<R>(0,F);

        bitset<R+2> tmp_F(0);
        for (int i=0;i<=R;i++) tmp_F[i] = F[i];
        GF2<R+1> tmp(0,tmp_F);
        for (int i=0;i<R;i++) tmp.C[i] = C[i];
        GF2<R+1> b(0,tmp_F),c(0,tmp_F),u(tmp),v(F),FF(F); b.C.set(0);

        while (true){
            while (!u.C[0]){
                u.C >>= 1;
                if (!b.C[0]) b.C >>=1;
                else {
                    b = b+FF;
                    b.C >>=1;
                }
            }
            int l1 = get_top<R+1>(u.C,R), l2 = get_top<R+1>(v.C,R);
            if (l1==0 && u.C[0]) {GF2<R> result(0,F); for (int i=0;i<R;i++) result.C[i] = b.C[i];
                return result;}
            if (l1<l2){
                auto temp = u; u = v; v = temp;
                auto temp1 = b; b = c; c = temp1;
            }
            u = u+v; b = b+c;
        }
    }

    bitset<R> C;
private:
    bool operator<(const GF2<R> &b) {
        int a = get_top<R>(C,R-1);
        int c = get_top<R>(b.C,R-1);
        return a<c;
    }

    bool operator>(const GF2& b){
        int a = get_top<R>(C,R-1);
        int c = get_top<R>(b.C,R-1);
        return a>c;
    }

    void to_R(GF2<R>& a, GF2<2*R>& b){
        GF2<R*2> tmp;
        for (int i=0;i<=R;i++) tmp.C[i] = b.get_F_i(i);
        b = b.reduction(tmp);
        a.C.reset();
        for (int i=0;i<R;i++)
            a.C[i] = b.C[i];
    }

    template <size_t RR>
    int get_top(const bitset<RR>& a,int n){
        if (a.count() == 0) return -1;
        for (int i=n;i>=0;i--)
            if (a[i]) return i;
    }

    bitset<R+1> F;
};