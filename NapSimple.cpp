#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<utility>
#include<cstdio>
#include<time.h>

using namespace std;
using t_v = long long;

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

int main(int argc, char** args)
{
    string fpath = "extend2_menu_hot_20170509.dat";
    t_v SUMP = 20000;
    long long sumCells = 0;

    if ( argc >= 2 ) SUMP = atol(args[1]);
    if ( argc >= 3 ) fpath = string(args[2]);

    vector<Item> items;

    items = input(fpath);   

    time_t strat = clock();
    
    vector<vector<pair<t_v, int>>> dp(items.size(), vector<pair<t_v, int>>(SUMP+1, make_pair(0, 0)));

    for ( t_v p = 1; p <= SUMP; ++p ) {
        t_v tmpd = p/items[0].price; //入れられる個数
        tmpd = min(tmpd, items[0].d);
        dp[0][p] = make_pair(tmpd*items[0].cal, tmpd);
    } 

    for ( int i = 1; i < items.size(); ++i ) {
        cout << "\ri = " << i << flush; 
        for ( t_v p = 1; p <= SUMP; ++p ) {
            for ( int j = 0; j <= items[i].d; ++j ) {
                t_v remain_p = p-items[i].price*j;
                if ( remain_p < 0 ) break;
                
                t_v sum_cal = dp[i-1][remain_p].first + items[i].cal*j;
                ++sumCells;
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

    time_t end = clock();
    double duration = double(end-strat);
    double avgcells = double(sumCells)/items.size();
    double averaged;

    long long sumofd = 0;
    long long enough_items=0;

    for ( auto i : items ) {
        sumofd += i.d;
        enough_items += (i.d*i.price >= SUMP ) ? 1 : 0;
    }
    sumofd *= SUMP; 
    averaged = double(sumofd)/items.size();


    cout << "\r[results]" << endl;
    cout << "file name\t: " << fpath << endl;
    cout << "solving\t\t: " << "Simple DP" << endl;
    cout << "SUMP\t\t: " << SUMP << endl;
    cout << "items size\t: " << items.size() << endl;
    printf("echoug items\t: %0.2lf%\n", double(enough_items)/(items.size())*100);
    printf("averages d\t: %0.2lf\n", averaged);
    printf("averages cells\t: %0.2lf\n", avgcells);
    cout << "optimized value\t: " << ans_score << endl; 
    cout << "time\t\t: " << duration << endl;

    long long sum = 0;
    sump = 0;
    for ( int i = 0; i < items.size(); ++i ) {
        sum += items[i].cal*ans[i];
        sump += items[i].price*ans[i];
    }
  
    cout << endl;
    // for ( int i = 0; i < items.size(); ++i ) {
        
    //     if ( ans[i] == 0 ) continue;
    //     cout << i << ": " << ans[i] << " (limit:" << items[i].d << ")" << endl;
    // }
}