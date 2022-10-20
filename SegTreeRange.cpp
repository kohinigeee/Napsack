#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>

/*
update:任意の区間にxを足す
find:任意の区間の最小値を探す
*/

#define MAXV ((1LL<<31)-1)

using namespace std;
using data = long long;

class RMQ {
  vector<data> tree;
  vector<data> lazy;
  int ln;

  void build() {
    for ( int i = ln-2; i >= 0; --i ) {
      tree[i] = min(tree[i*2+1], tree[i*2+2]);
    }
  }

  void eval( int k ) {
    if ( lazy[k] == 0 ) return;
    if ( k < ln-1 ) {
        lazy[k*2+1] += lazy[k];
        lazy[k*2+2] += lazy[k];
    }
    if ( tree[k] == MAXV ) {
        lazy[k] = 0;
        return;
    }
    tree[k] += lazy[k];
    lazy[k] = 0; 
  }

public:

  RMQ( int n ) : RMQ(vector<data> (n, MAXV)) { }

  RMQ( const vector<data>& vec ) {
    ln = 1;
    while( ln < vec.size() ) ln = ln<<1;
    tree = vector<data>(ln*2-1, MAXV);
    lazy = vector<data>(ln*2-1, 0);
    for ( int i = 0; i < vec.size(); ++i ) tree[i+ln-1] = vec[i];
    build();
  }

  void overwrite(int no, data x ) { 

    int lno = no+ln-1;
    if ( lno < ln-1 || lno >= 2*ln-1 ) return;

    vector<int> nos;

    int tmpno = lno;
    nos.push_back(tmpno);
    while( tmpno ) {
      tmpno = (tmpno-1)/2;
      nos.push_back(tmpno);
    }

    for ( int i = nos.size()-1; i >= 0; --i ) eval(nos[i]);
    tree[lno] = x;

    tmpno = lno;
    while(tmpno) {
      int p = (tmpno-1)/2;
      eval(p*2+1);
      eval(p*2+2);

      int minv = min(tree[p*2+1], tree[p*2+2]); 
      if ( tree[p] == minv ) break;
      else tree[p] = minv;
      tmpno = p;
    }
  }

// 半開区間[l, r)
  void update(int l, int r, int x, int k, int nl, int nr) {
    eval(k);
    if ( nl >= l && nr <= r ) { lazy[k] = x; eval(k); return;}
    if ( nr <= l || nl >= r ) return;

    int mid = (nl+nr)/2;
    update(l, r, x, k*2+1, nl, mid);
    update(l, r, x, k*2+2, mid, nr);
    if ( k < ln-1 ) tree[k] = min(tree[k*2+1], tree[k*2+2]); 
  }

  void update(int l, int r, int x ) { update(l, r, x, 0, 0, ln); }
  
  data find(int l, int r, int i, int nl, int nr) { //半開区間[l, r)
    eval(i);
    if ( nl >= l && nr <= r ) return tree[i];
    if ( nr <= l || nl >= r ) return MAXV;

    int mid = (nl+nr)/2;
    return min(find(l,r,i*2+1,nl,mid), find(l,r,i*2+2,mid,nr));
  }

  data find(int l, int r ) { return find(l, r, 0, 0, ln); }

  void print() {
    cout << "  no : ";
    for ( int i = 0; i < ln*2-1; ++i ) printf("%3d ", i+1);
    cout << endl;

    cout << "data : ";
    for ( auto v: tree ) {
        if ( v == MAXV ) cout << "INF ";
        else printf("%3d ", v );
    }
    cout << endl;

    cout << "lazy : ";
    for ( auto v : lazy ) printf("%3d ", v); cout << endl;
  }
};

int main()
{
  int n, q;
  cin >> n >> q;
  vector<data> v(n, 0);

  RMQ r = RMQ(v);

  while(q--) {
    int cmd, x, y;
    cin >> cmd;
    if ( cmd == 1 ) {
        cin >> x >> y;
        cout << r.find(x, y+1) << endl;
        // r.print();
    } else if ( cmd == 0 ) {
        data g;
        cin >> x >> y >> g;
        r.update(x, y+1, g);
        // r.print();
    } else if ( cmd == 2 ) {
        int i; data g;
        cin >> i >> g;
        r.overwrite(i,g);
        // r.print();
    }
  }
}
