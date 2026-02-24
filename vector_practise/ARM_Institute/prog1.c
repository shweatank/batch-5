/* const char __attribute__((at(0x000000000))) src[] = "Vector embedded institute";
char __attribute__((at(0x40000004))) dest1[100];
int i;
int main() {
while(src[i]) {
dest1[i] = src[i];
i++;
}
dest1[i] = '\0';
while(1);
}
*/
