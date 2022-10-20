#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<utility>
#include<cstdio>

using namespace std;
using t_v = long long;

class Item {
    public:

    int id;
    t_v price, cal, d;

    Item( const int id, const t_v price, const t_v cal, const t_v d ) : id(id), price(price), cal(cal), d(d) {}

    void print() { cout << "id:" << id << " price:" << price << " cal:" << cal << " d:" << d << endl; }
        
};

vector<Item> make() {
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
    vector<Item> items;
    const t_v SUMP = 13;

    items = make();

    vector<vector<pair<t_v, int>>> dp(items.size(), vector<pair<t_v, int>>(SUMP+1, make_pair(0, 0)));

    for ( t_v p = 1; p <= SUMP; ++p ) {
        t_v tmpd = p/items[0].price; //入れられる個数
        tmpd = min(tmpd, items[0].d);
        dp[0][p] = make_pair(tmpd*items[0].cal, tmpd);
    } 

    for ( int i = 1; i < items.size(); ++i ) {
        for ( t_v p = 1; p <= SUMP; ++p ) {
            for ( int j = 0; j <= items[i].d; ++j ) {
                t_v remain_p = p-items[i].price*j;
                if ( remain_p < 0 ) break;
                
                t_v sum_cal = dp[i-1][remain_p].first + items[i].cal*j;
                if ( dp[i][p].first < sum_cal ) {
                    dp[i][p] = make_pair(sum_cal, j);
                }
            }
        }
    }

    t_v ans_score = dp[items.size()-1][SUMP].first;
    vector<t_v> ans(items.size(), 0);

    t_v sump = SUMP;
    for ( int i = items.size()-1; i >= 0; --i ) {
        ans[i] = dp[i][sump].second;
        sump -= items[i].price*ans[i];
    }

    cout << "ans value = " << ans_score << endl;

    long long sum = 0;
    sump = 0;
    for ( int i = 0; i < items.size(); ++i ) {
        sum += items[i].cal*ans[i];
        sump += items[i].price*ans[i];
    }

    cout << "  "; for ( t_v p = 0; p <= SUMP; ++p ) printf("%2d ", p); cout << endl;
    for ( int i = 0; i < items.size(); ++i ) {
        cout << i << " "; for ( t_v p = 0; p <= SUMP; ++p ) printf("%2d ", dp[i][p].first); cout << endl;
    }
}