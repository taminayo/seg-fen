#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

float current_time();
void get_input();
void solve_by_seg();
void solve_by_fen();

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
        solve_by_seg();
        float t2 = current_time();
        solve_by_fen();
        float t3 = current_time();
        fprintf(stdout, "seg time taken : %.3f sec\n", t2-t1);
        fprintf(stdout, "fen time taken : %.3f sec\n", t3-t2);
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

void solve_by_seg() {
    int max_r = *max_element(INPUT.begin(), INPUT.end());

    function<void(int[], int, int, int, int, int)> update_seg = [&update_seg](int seg[], int v, int tl, int tr, int pos, int d) {
        if (pos < tl || pos > tr) return;
        if (tl == tr) seg[v] += d;
        else {
            int tm = tl + (tr-tl)/2;
            if (pos <= tm) update_seg(seg, v*2, tl, tm, pos, d);
            else update_seg(seg, v*2+1, tm+1, tr, pos, d);
            seg[v] = seg[v*2] + seg[v*2+1];
        }
    };
    function<int(int[], int, int, int, int, int)> query_seg = [&query_seg](int seg[], int v, int tl, int tr, int l, int r) {
        if (r < tl || tr < l) return 0;
        if (l <= tl && tr <= r) return seg[v];
        int tm = tl + (tr-tl)/2;
        int left = query_seg(seg, v*2, tl, tm, l, r);
        int right = query_seg(seg, v*2+1, tm+1, tr, l, r);
        return left + right;
    };

    int right_seg[4*(MAX_E+1)] = {0};
    int left_seg[4*(MAX_E+1)] = {0};
    int res = 0;
    for (const int& num : INPUT) update_seg(right_seg, 1, 0, max_r, num, 1);
    for (const int& num : INPUT) {
        int left_smaller = query_seg(left_seg, 1, 0, max_r, 0, num-1);
        int right_smaller = query_seg(right_seg, 1, 0, max_r, 0, num-1);
        int left_larger = query_seg(left_seg, 1, 0, max_r, num+1, max_r);
        int right_larger = query_seg(right_seg, 1, 0, max_r, num+1, max_r);
        res += left_smaller*right_larger + left_larger*right_smaller;
        update_seg(left_seg, 1, 0, max_r, num, 1);
        update_seg(right_seg, 1, 0, max_r, num, -1);
    }
    fprintf(stdout, "seg result : %d\n", res);
}

void solve_by_fen() {
    int max_r = *max_element(INPUT.begin(), INPUT.end());
    function<void(int[], int, int)> update_fen = [&max_r](int fen[], int id, int d) {
        while (id <= max_r) {
            fen[id] += d;
            id += id&-id;
        }
    };
    function<int(int[], int)> query_fen = [](int fen[], int id) {
        int res = 0;
        while (id > 0) {
            res += fen[id];
            id -= id&-id;
        }
        return res;
    };
    int left_fen[MAX_E+1+1] = {0};
    int right_fen[MAX_E+1+1] = {0};
    for (const int& num : INPUT) update_fen(right_fen, num, 1);
    int res = 0;
    for (const int& num : INPUT) {
        int left_smaller = query_fen(left_fen, num-1);
        int right_smaller = query_fen(right_fen, num-1);
        int left_larger = query_fen(left_fen, max_r) - query_fen(left_fen, num);
        int right_larger = query_fen(right_fen, max_r) - query_fen(right_fen, num);
        res += left_smaller*right_larger + left_larger*right_smaller;
        update_fen(left_fen, num, 1);
        update_fen(right_fen, num, -1);
    }

    fprintf(stdout, "fen result : %d\n", res);
}
