#include <stdio.h>
int search(int l, int r, int target, int a[]) {
    int mid = (l + r) / 2;
  //  printf("mid=%d and a[mid]=%d\n",mid,a[mid]);
    if(a[mid] == target) {
        return mid;
    }
    else if(a[mid] < target) {
        return search(mid + 1, r, target, a);
    }
    else {
        return search(0, mid - 1, target, a);
    }
}
void solve() {
    int a[] = {1, 2, 3, 4, 5};
    printf("%d", search(0, 5, 3, a));
}
int main() {
    solve();
	return 0;
}
