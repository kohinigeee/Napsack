#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<utility>
#include<cstdio>

using namespace std;
using t_v = long long;
using pti = pair<t_v, int>;


template<class T>
class SegTree {
  T (*ope) ( T, T );
  T zele;
  int ln;
  vector<T> tree;

  void build() {
    for ( int i = ln-2; i >= 0; --i ) {
      tree[i] = (*ope)(tree[i*2+1], tree[i*2+2]);
    }
  }

public:

  SegTree( T (*ope1) (T,T), T zele1, const vector<T>& v) : ope(ope1), zele(zele1) {

    this->ln = 1;
    while( ln < v.size() ) this->ln = ln<<1;
    tree = vector<T>(ln*2-1, zele);
    for ( int i = 0; i < v.size(); ++i ) tree[i+ln-1] = v[i];
    if ( ln > 1 ) build();
  }

  SegTree ( T (*ope1) (T,T), T zele1, int n ) : SegTree( ope1, zele1, vector<T>(n, zele1)){
  }

  void update(int i, T v ) { // 0-origin
    i = i+ln-1;
    tree[i] = v;
    while( i > 0 ) {
      i = (i-1)/2;
      tree[i] = (*ope)(tree[i*2+1], tree[i*2+2]);
    }
  }

  void add( int i, T v ) { //+演算子必須
    update( i, tree[i+ln-1]+v);
  }

  T find(int l, int r, int i, int nl, int nr ) { //半開区間[l-r)の探索 0-origin
    if ( nl >= l && nr <= r ) return tree[i];
    if ( nl >= r || nr <= l ) return zele;

    int mid =  (nl+nr)/2;
    return (*ope)( find(l,r,i*2+1,nl,mid), find(l,r,i*2+2,mid,nr));
  }

  T find(int l, int r) { return find(l,r,0,0,ln); }

  void print() {
    cout << "ln = " << ln << endl;
    cout << "Print: ";
    for ( auto v : tree ) cout << v << " ";
    cout << endl;
  }
};


class Item {
    public:

    int id;
    t_v price, cal, d;

    Item( const int id, const t_v price, const t_v cal, const t_v d ) : id(id), price(price), cal(cal), d(d) {}

    void print() { cout << "id:" << id << " price:" << price << " cal:" << cal << " d:" << d << endl; }
        
};

vector<Item> input(string fpath ) {
    ifstream reading_file;
    vector<Item> items;

    reading_file.open(fpath, ios::in);

    if ( !reading_file ) {
        cout << "Couldn't opend " + fpath << endl;
        return items;
    }

    string reading_line_buffer;

    while(getline(reading_file, reading_line_buffer)) {

        const char delimiter = ' ';

        string separeted_string_buffer;
        istringstream line_separeter(reading_line_buffer);

        vector<t_v> values;

        while(getline(line_separeter, separeted_string_buffer, delimiter)) {
            values.push_back(stol(separeted_string_buffer));
        }

        items.push_back(Item(values[0], values[1], values[2], values[3]));
    } 

    return items; 
}

int main()
{
    string fpath = "extend_menu_hot_20170509.dat";
    vector<Item> items;
    int n;
    const t_v SUMP = 25000;

    items = input(fpath);
    n = items.size();

    vector<vector<SegTree<pti>>> segs(n, vector<SegTree<pti>>());

    
    SegTree<pti> seg( [](pti p1, pti p2) { return ( p1.first > p2.first ) ? p1 : p2; }, make_pair(t_v(0), 0), 5); 

    for ( int i = 0; i < n-1; ++i ) {
        int types = items[i].price;
        int len = SUMP/items[i].price+1;

        vector<pti> tmp(len, make_pair(0, 0));
        for ( int j = 0; j < len; ++j ) tmp[j].second = j;
        for ( int j = 0; j < types; ++j ) {
            segs[i].push_back(SegTree<pti> ( [](pti p1, pti p2) { return ( p1.first > p2.first ) ? p1 : p2; }, make_pair(-1, -1), tmp)); 
        }
    }

    for ( t_v p = 1; p <= SUMP; ++p ) {
        int d = min(items[0].d, p/items[0].price);
    }

}
