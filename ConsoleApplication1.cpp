// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>
#include <direct.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int interval;
float proportion;

void imgResize(string path ,string write , int size[1]) {
	//string path = "C:\\Users\\10985\\source\\repos\\CVDemo01\\test03_resize\\10.png";
	Mat images;
	vector<String> filename;
	glob(path, filename, false);
	size_t count = filename.size();
	for (int i = 0; i < count; i++)
	{
		images = imread(filename[i]);
		if (!images.data) {
			cout << "Image reading error !" << endl;
			continue;
		}

		// 最小分辨率（100x70像素）
		//Size dsize = Size(100, 70);
		Size dsize = Size(size[0], size[1]);//分辨率
		Mat shrink;
		resize(images, shrink, dsize, 0, 0, INTER_AREA);

		//保存 
		stringstream str;
		str << write;//<< (i + 1)<< ".png";
		imwrite(str.str(), shrink);
		//std::cout << "\r" << write << (i + 1) * 10 << ".png";
	}
}

void filewrite(string name,string str) {
	ofstream file;
	file.open(name);
	file << str;
	file.close();
}

//#define interval 1  //修改这里以改变多久保存一帧
int main()
{
	
	::_mkdir("./video");
	::_mkdir("./particles");
	::_mkdir("./textures");
	::_mkdir("./textures/particle");
	string route;
	tag:
	cout << "文件名：";
	cin >> route;
	vector<int> arr;
	route = "./video/" + route;
	VideoCapture capture(route);
	if (!capture.isOpened()) {
		system("cls");
		cout << "打开文件时出现问题，请检查文件名" << endl;
		goto tag;
	}
	int imgIndex = 0;
	int frames = 0;
	Mat frame;
	capture >> frame;
	int height = frame.rows;//row表示行，rows表示行的总数，即图像的高
	int width = frame.cols;
	system("cls");
	std::cout << "保存间隔：";
	cin >> interval;
	cout << "压缩率：";
	cin >> proportion;
	proportion = proportion/100;
	while (!frame.empty()) {
		string output = "./textures/particle/" + to_string(imgIndex) + ".jpg";
		std::cout << "\r保存" << output;
		if (imgIndex % interval == 0) {
			//第一个参数是写入的文件名，第二个参数是Mat类型的图像数据。
			imwrite(output, frame);
			frames++;
			arr.push_back(imgIndex);
		}

		imgIndex++;
		capture >> frame;
	}
	std::cout << "\n保存完毕\n";
	int num[2] = {(width * proportion),(height * proportion)};//大小
	for (int i = 0; i < (frames); i++) {
		string imgout = "./textures/particle/" + to_string(arr[i]) + ".jpg";
		string out = "./textures/particle/" + to_string(arr[i])+ ".png";
		//Mat img = imread(imgout);
		std::cout << "\r压缩" << imgout << "中...";
		imgResize(imgout, out, num);
	}
	cout << "\n压缩完毕" << endl;
	for (int i = 0; i < (frames); i++) {
		string str="./textures/particle/" + to_string(arr[i]) + ".jpg";
		const char* imgout = str.data();
		cout << "\r正在清理...";
		remove(imgout);
	}

	for (int i = 0; i <= (frames - 1); i++) {
		string str = "./textures/particle/" + to_string(arr[i]) + ".png";
		string newstr = "./textures/particle/" + to_string(i) + ".png";
		string filename = "./particles/" + to_string(i) + ".json";
		const char* imgout = str.data();
		const char* imgoutnew = newstr.data();
		rename(imgout, imgoutnew);
		filewrite(filename, "{\"format_version\": \"1.10.0\",\"particle_effect\": {\"description\": {\"identifier\": \"video:p" + to_string(i) + "\",\"basic_render_parameters\": {\"material\": \"particles_alpha\",\"texture\": \"textures/particle/" + to_string(i) + "\"}},\"components\": {\"minecraft:emitter_rate_instant\": {\"num_particles\": 1},\"minecraft:emitter_lifetime_looping\": {\"active_time\": 1},\"minecraft:emitter_lifetime_expression\": {},\"minecraft:particle_initial_spin\": {},\"minecraft:particle_lifetime_expression\": {\"max_lifetime\": " + to_string(0.075) + "},\"minecraft:particle_appearance_billboard\": {\"size\": [" + to_string((float)num[0]/400) + "," + to_string((float)num[1]/400) + "],\"facing_camera_mode\": \"rotate_xyz\",\"uv\": {}}}}}");
			
	}
	cout << "\n完成" << endl;
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
