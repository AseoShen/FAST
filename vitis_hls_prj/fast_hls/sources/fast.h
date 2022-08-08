
typedef int DTYPE;

#define RADIUS 3

void fast_accel(DTYPE* img_in, int threshold, DTYPE* img_out, int rows, int cols);

int check_pre(DTYPE* img_in, int threshold, int x, int y, int rows, int cols);

int check_for(DTYPE* img_in, int threshold, int x, int y, int rows, int cols);

int check_thres(DTYPE center, DTYPE point, int threshold);
