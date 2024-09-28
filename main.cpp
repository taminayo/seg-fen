#include <iostream>
#include <numeric>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

float current_time();
void get_input();
void seg_tree();
void fen_tree();

int N;
vector<int> INPUT;
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    freopen("input.txt", "r", stdin);
//    freopen("error.txt", "w", stderr);
    freopen("output.txt", "w", stdout);

    int t;
    cin >> t;
    while (t--) {
        get_input();
        float t1 = current_time();
        seg_tree();
        float t2 = current_time();
        fen_tree();
        float t3 = current_time();
        fprintf(stdout, "time taken to construct segment tree: %.3f sec\n", t2-t1);
        fprintf(stdout, "time taken to construct fenwick tree: %.3f sec\n", t3-t2);
        cout << "\n";
    }
}

float current_time() {
    return (float)clock()/CLOCKS_PER_SEC;
}

constexpr int MAX_N = 1e3;
constexpr int MAX_E = 1e5;

void get_input() {
    cin >> N;
    INPUT.resize(N);
    for (int i = 0; i < N; ++i) cin >> INPUT[i];
}

void seg_tree() {
    int seg[4*(MAX_N+1)] = {0};
    function<void(int, int, int, int, int)> update = [&seg, &update](int v, int tl, int tr, int pos, int d) {
        if (pos < tl || pos > tr) return;
        if (tl == tr) seg[v] += d;
        else {
            int tm = tl + (tr-tl)/2;
            if (pos <= tm) update(v*2, tl, tm, pos, d);
            else update(v*2+1, tm+1, tr, pos, d);
            seg[v] = seg[v*2] + seg[v*2+1];
        }
    };
    function<int(int, int, int, int, int)> query = [&seg, &query](int v, int tl, int tr, int l, int r) {
        if (r < tl || tr < l) return 0;
        if (l <= tl && tr <= r) return seg[v];
        int tm = tl + (tr-tl)/2;
        int left = query(v*2, tl, tm, l, r);
        int right = query(v*2+1, tm+1, tr, l, r);
        return left+right;
    };
    for (int i = 0; i < N; ++i) {
        update(1, 0, N+1, i+1, INPUT[i]);
    }
}

void fen_tree() {
    int fen[MAX_N+1] = {0};
    function<void(int, int)> update = [&fen](int id, int d) {
        while (id <= N) {
            fen[id] += d;
            id += id&-id;
        }
    };
    function<int(int)> query = [&fen](int id) {
        int res = 0;
        while (id > 0) {
            res += fen[id];
            id -= id&-id;
        }
        return res;
    };
    for (int i = 0; i < N; ++i) {
        update(i+1, INPUT[i]);
    }
}
