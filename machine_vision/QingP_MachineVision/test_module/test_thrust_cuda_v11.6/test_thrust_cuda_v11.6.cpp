// test_thrust_cuda_v11.6.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/sort.h>
#include <thrust/copy.h>
#include <thrust/random.h>

int main() {
	// Generate 32M random numbers serially.
	thrust::default_random_engine rng(1337);
	thrust::uniform_int_distribution<int> dist;
	thrust::host_vector<int> h_vec(32 << 20);
	thrust::generate(h_vec.begin(), h_vec.end(), [&] { return dist(rng); });

	// Transfer data to the device.
	thrust::device_vector<int> d_vec = h_vec;

	// Sort data on the device.
	thrust::sort(d_vec.begin(), d_vec.end());

	// Transfer data back to host.
	thrust::copy(d_vec.begin(), d_vec.end(), h_vec.begin());

	system("pause");

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
