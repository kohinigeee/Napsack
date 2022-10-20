#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>
#include<string>
#include<fstream>
#include<sstream>

/*
update:任意の区間にxを足す
find:任意の区間の最小値を探す
*/

#define MINV (-(1LL<<31))

using namespace std;
using data = long long;
using t_v = long long;

class RMQ {
  vector<data> tree;
  vector<data> lazy;
  int ln;

  void build() {
    for ( int i = ln-2; i >= 0; --i ) {
      tree[i] = max(tree[i*2+1], tree[i*2+2]);
    }
  }

  void eval( int k ) {
    if ( lazy[k] == 0 ) return;
    if ( k < ln-1 ) {
        lazy[k*2+1] += lazy[k];
        lazy[k*2+2] += lazy[k];
    }
    if ( tree[k] == MINV ) {
        lazy[k] = 0;
        return;
    }
    tree[k] += lazy[k];
    lazy[k] = 0; 
  }

  data find(int l, int r, int i, int nl, int nr) { //半開区間[l, r)
    eval(i);
    if ( nl >= l && nr <= r ) return tree[i];
    if ( nr <= l || nl >= r ) return MINV;

    int mid = (nl+nr)/2;
    return max(find(l,r,i*2+1,nl,mid), find(l,r,i*2+2,mid,nr));
  }

// 半開区間[l, r)
  void update(int l, int r, int x, int k, int nl, int nr) {
    eval(k);
    if ( nl >= l && nr <= r ) { lazy[k] = x; eval(k); return;}
    if ( nr <= l || nl >= r ) return;

    int mid = (nl+nr)/2;
    update(l, r, x, k*2+1, nl, mid);
    update(l, r, x, k*2+2, mid, nr);
    if ( k < ln-1 ) tree[k] = max(tree[k*2+1], tree[k*2+2]); 
  }

public:

  RMQ( int n ) : RMQ(vector<data> (n, MINV)) { }

  RMQ( const vector<data>& vec ) {
    ln = 1;
    while( ln < vec.size() ) ln = ln<<1;
    tree = vector<data>(ln*2-1, MINV);
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

      int maxv = max(tree[p*2+1], tree[p*2+2]); 
      if ( tree[p] == maxv ) break;
      else tree[p] = maxv;
      tmpno = p;
    }
  }

  void update(int l, int r, int x ) { update(l, r, x, 0, 0, ln); }
  
  data find(int l, int r ) { return find(l, r, 0, 0, ln); }

  data getTop() { 
    eval(0);
    return tree[0];
  }

  void print() {
    cout << "  no : ";
    for ( int i = 0; i < ln*2-1; ++i ) printf("%3d ", i+1);
    cout << endl;

    cout << "data : ";
    for ( auto v: tree ) {
        if ( v == MINV ) cout << "INF ";
        else printf("%3d ", v );
    }
    cout << endl;

    cout << "lazy : ";
    for ( auto v : lazy ) printf("%3d ", v); cout << endl;
  }
};



class Item {
    public:

    int id;
    t_v price, cal, d;

    Item( const int id, const t_v price, const t_v cal, const t_v d ) : id(id), price(price), cal(cal), d(d) {}

    void print() { cout << "id:" << id << " price:" << price << " cal:" << cal << " d:" << d << endl; }
        
};

vector<Item> input() {
    vector<Item> items;
    
    items.push_back(Item(1, 4, 4, 3));
    items.push_back(Item(2, 3, 5, 3));
    items.push_back(Item(3, 3, 6, 3));
    items.push_back(Item(4, 3, 7, 3));
    items.push_back(Item(5, 2, 8, 3));

    return items;
}

int main()
{
    string fpath = "extend_menu_hot_20170509.dat";
    const t_v SUMP = 13;

    vector<Item> items = input();
    int n = items.size();

    if ( n <= 0 ) {
        cout << "size of items is zero " << endl;
        return 0;
    }

    vector<vector<t_v>> dp( n, vector<t_v>(SUMP+1, 0));

    for ( t_v p = 0; p <= SUMP; ++p ) {
        t_v no = min(p/items[0].price, items[0].d);
        dp[0][p] = no*items[0].cal;
    }

    for ( int i = 1; i < n; ++i ) {
        t_v len = SUMP/items[i].price;

        vector<t_v> cnt(items[i].price, 0);
        vector<RMQ> segs(items[i].price, RMQ(items[i].d+1));

        for (t_v p = 0; p <= SUMP; ++p ) {
            t_v segs_no = p%items[i].price;
            
            segs[segs_no].update(0, items[i].d+1, items[i].cal);
            segs[segs_no].overwrite(cnt[segs_no], dp[i-1][p]);
            cnt[segs_no] = (cnt[segs_no]+1)%(items[i].d+1);

            dp[i][p] = segs[segs_no].getTop();
        }
    }
    cout << "optimizes ans = " << dp[n-1][SUMP] << endl;

   
    for ( auto vs : dp ) {
        for ( auto v : vs ) printf("%2d ", v);
        cout << endl;
    }
}
