#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>
#include<string>
#include<fstream>
#include<sstream>
#include<utility>
#include<stdlib.h>
#include<time.h>

/*
update:任意の区間にxを足す
find:任意の区間の最小値を探す
*/

#define MINV (-(1LL<<31))

using namespace std;
using t_v = long long;
using data = pair<t_v, t_v>;
//first : 価値の総和 second:選んだ個数

data operator+(const data& p1, const data& p2 ) {
  return make_pair(p1.first+p2.first, p1.second+p2.second);
}

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
    if ( lazy[k].first == 0 && lazy[k].second == 0 ) return;
    if ( k < ln-1 ) {
        lazy[k*2+1] = lazy[k*2+1]+lazy[k];
        lazy[k*2+2] = lazy[k*2+2]+lazy[k];
    }
    if ( tree[k].first == MINV ) {
        lazy[k] = make_pair(0, 0);
        return;
    }
    tree[k] = tree[k]+lazy[k];
    lazy[k] = make_pair(0,0); 
  }

  data find(int l, int r, int i, int nl, int nr) { //半開区間[l, r)
    eval(i);
    if ( nl >= l && nr <= r ) return tree[i];
    if ( nr <= l || nl >= r ) return make_pair(MINV, 0);

    int mid = (nl+nr)/2;
    return max(find(l,r,i*2+1,nl,mid), find(l,r,i*2+2,mid,nr));
  }

// 半開区間[l, r)
  void update(int l, int r, data x, int k, int nl, int nr) {
    eval(k);
    if ( nl >= l && nr <= r ) { lazy[k] = x; eval(k); return;}
    if ( nr <= l || nl >= r ) return;

    int mid = (nl+nr)/2;
    update(l, r, x, k*2+1, nl, mid);
    update(l, r, x, k*2+2, mid, nr);
    if ( k < ln-1 ) tree[k] = max(tree[k*2+1], tree[k*2+2]); 
  }

public:

  RMQ( int n ) : RMQ(vector<data> (n, make_pair(MINV,0))) { }

  RMQ( const vector<data>& vec ) {
    ln = 1;
    while( ln < vec.size() ) ln = ln<<1;
    tree = vector<data>(ln*2-1, make_pair(MINV,0));
    lazy = vector<data>(ln*2-1, make_pair(0,0));
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

      data maxv = max(tree[p*2+1], tree[p*2+2]); 
      if ( tree[p] == maxv ) break;
      else tree[p] = maxv;
      tmpno = p;
    }
  }

  void update(int l, int r, data x ) { update(l, r, x, 0, 0, ln); }
  
  data find(int l, int r ) { return find(l, r, 0, 0, ln); }

  data getTop() { 
    eval(0);
    return tree[0];
  }

//   void print() {
//     cout << "  no : ";
//     for ( int i = 0; i < ln*2-1; ++i ) printf("%3d ", i+1);
//     cout << endl;

//     cout << "data : ";
//     for ( auto v: tree ) {
//         if ( v == MINV ) cout << "INF ";
//         else printf("%3d ", v );
//     }
//     cout << endl;

//     cout << "lazy : ";
//     for ( auto v : lazy ) printf("%3d ", v); cout << endl;
//   }
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

long long calCells( t_v sump, vector<Item>& items ) {
    long long ans = 0;
    for ( int i = 1; i < items.size(); ++i ) {
        long long i_cells = 0;
        Item it = items[i];

        long long maxp = it.price*it.d;
        long long tmpp = sump;

        if ( tmpp >= maxp ) {
            i_cells += (it.d+1)*(tmpp-maxp+1); 
            tmpp = maxp-1;
        }

        long long a = tmpp/it.price, b = tmpp%it.price;
        i_cells += it.price*(a*(a+1)/2)+(a+1)*(b+1)-1;
        ans += i_cells;
    }
    return ans;
}

int main( int argc, char** args )
{
    string fpath = "extend2_menu_hot_20170509.dat";
    t_v SUMP = 20000;

    if ( argc >= 2 ) SUMP = atol(args[1]);
    if ( argc >= 3 ) fpath = string(args[2]);

    vector<Item> items = input(fpath);
    int n = items.size();

    if ( n <= 0 ) {
        cout << "size of items is zero " << endl;
        return 0;
    }

    time_t start = clock();//計測開始

    vector<vector<data>> dp( n, vector<data>(SUMP+1, make_pair(0,0)));

    for ( t_v p = 0; p <= SUMP; ++p ) {
        t_v no = min(p/items[0].price, items[0].d);
        dp[0][p] = make_pair(no*items[0].cal, no);
    }

    for ( int i = 1; i < n; ++i ) {
        cout << "\ri = " << i;
        t_v len = SUMP/items[i].price;

        vector<t_v> cnt(items[i].price, 0);
        vector<RMQ> segs(items[i].price, RMQ(items[i].d+1));

        for (t_v p = 0; p <= SUMP; ++p ) {
            t_v segs_no = p%items[i].price;
            
            segs[segs_no].update(0, items[i].d+1, make_pair(items[i].cal, 1));
            segs[segs_no].overwrite(cnt[segs_no], make_pair(dp[i-1][p].first, 0));
            cnt[segs_no] = (cnt[segs_no]+1)%(items[i].d+1);

            dp[i][p] = segs[segs_no].getTop();
        }
    }

    t_v ansvalue = dp[n-1][SUMP].first;
    vector<t_v> ans(n);
    t_v sump = SUMP;

    for ( int i = n-1; i >= 0; --i ) {
        ans[i] = dp[i][sump].second;
        sump -= ans[i]*items[i].price;
    }

    time_t end = clock();
    double duration = double(end-start);
    long long sumCells = calCells(SUMP, items);
    double avgcelss = (double)sumCells/n;
    double averaged;

    long long sumofd = 0;
    long long enough_items = 0;

    for ( auto i : items ) {
      sumofd += i.d;
      if ( i.d*i.price >= SUMP ) {
        ++enough_items;
      }
    }

    sumofd *= SUMP; 
    averaged = double(sumofd)/items.size();



    cout << "\r[results]" << endl;
    cout << "file name\t: " << fpath << endl;
    cout << "solving\t\t: " << "DP + SegmentTree" << endl;
    cout << "SUMP\t\t: " << SUMP << endl;
    cout << "items size\t: " << n << endl;
    printf("echoug items\t: %0.2lf%\n", double(enough_items)/(items.size())*100);
    printf("average d\t: %0.2lf\n", averaged);
    printf("average celss\t: %0.2lf\n", avgcelss);
    cout << "optimized value\t: " << ansvalue << endl; 
    cout << "time\t\t: " << duration << endl;

    sump = 0;
    t_v sumc = 0;
    for ( int i = 0; i < n; ++i ) {
        sump += ans[i]*items[i].price;
        sumc += ans[i]*items[i].cal;
    }
    
    cout << endl;
    // for ( int i = 0; i < n; ++i ) {
    //     if ( ans[i] == 0 ) continue;
    //     cout << i << ": " << ans[i] << " (limit:" << items[i].d << ")" << endl;
    //     // printf("\rtest sentence");
    // }
    cout << "Sum caloly = " << sumc << endl;
    cout << "Sum price = " << sump << endl;
}
