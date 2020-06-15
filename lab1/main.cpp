#include<iostream>
#include<cmath>
#include<set>
#include<algorithm>
#include<cstdio>


using namespace std;

double f(double x) {
    return -(x*x) - 0.5;
}


int main() {
    freopen("result3.out", "w", stdout);

    int n, m;
    double x0, x1;
    cin >> m >> n >> x0 >> x1;

    char a[n+1][m];

    double mx = f(x0);
    double mn = f(x0);
    double zeroPos = 0;

    for (int i = 0; i < m; ++i) {
        auto val = f(x0 + (x1 - x0) / m * i);
        mx = max(mx, val);
        mn = min(mn, val);
    }


    double zero_y;
    double step_y;

    if (mn < 0 && mx < 0) {
        step_y = abs(mn / n);
        zero_y = 0;
    }
    if (mn > 0 && mx > 0) {
        step_y = mx / n;
        zero_y = n;
    }
    if (mx * mn < 0) {
        step_y = (mx - mn) / n;
        zero_y = round(mx / step_y);
    }
    if (mx == 0) {
        zero_y = 0;
        step_y = (mx - mn) / n;
    }
    if (mn == 0) {
        zero_y = n;
        step_y = (mx - mn) / n;
    }


    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < m; j++) {
            double current_f = f(x0 + (x1 - x0) / m * j);
            if (i == zero_y) {
                a[i][j] = '-';
            }
            if (i > zero_y) {
                if (((current_f < mx - step_y * i) && (mx >= 0)) ||
                    ((current_f < 0 - step_y * i) && (mx < 0))) {
                    a[i][j] = '#';
                } else {
                    a[i][j] = ' ';
                }
            }
            if (i < zero_y) {
                if (((current_f < mx - step_y * i) && (mx >= 0)) ||
                    ((current_f < 0 - step_y * i) && (mx < 0))) {
                    a[i][j] = ' ';
                } else {
                    a[i][j] = '#';
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << a[i][j];
        }
        cout << endl;
    }


    return 0;
}