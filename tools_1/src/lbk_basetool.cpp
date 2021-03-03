#include"lbk_basetool.h"
/*获取时间毫秒数*/
double const lbk_get_time_ms() {
	double t;
#if defined _WIN32
	t = GetTickCount64(); //返回时间 ms
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t = tv.tv_sec * 1000. + tv.tv_usec / 1000.;
#endif
	return t;
}
