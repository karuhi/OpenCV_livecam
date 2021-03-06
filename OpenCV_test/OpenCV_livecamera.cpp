// OpenCV_test.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

// リンカーの入力を設定
// デバッグとリリースで入力するファイルが異なるのでこのようにしています。
#ifdef _DEBUG
#define CV_EXT "d.lib"
#else
#define CV_EXT ".lib"
#endif

#pragma comment(lib, "opencv_world341" CV_EXT)

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp> // videoioのヘッダーをインクルード
#include <opencv2/highgui.hpp> // highguiのヘッダーをインクルード

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <chrono>

#ifdef _OEPNMP
#include <omp.h>
#endif

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {
	int camera_id = 0; // 1WEBCam
	cv::VideoCapture cap(cv::CAP_DSHOW + camera_id); //キャプチャの準備を行う

	
	if (!cap.isOpened()) {
		return -1; // カメラが正しくオープンできているか
	}

	cap.set(cv::CAP_PROP_FPS, 30.0); // フレームレートを30.0に

	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);// 画像表示用のウィンドウを生成する
	cv::namedWindow("image_saved", cv::WINDOW_AUTOSIZE);// 画像表示用のウィンドウを生成する
	cv::namedWindow("chroma_image", cv::WINDOW_AUTOSIZE);// 画像表示用のウィンドウを生成する
	
	
	cv::Mat frame;// 画像データを格納するための変数


	cv::Mat save_frame;// 画像データを保存するための変数

	cv::Mat chroma_image;// クロマキー画像データを保存するための変数

	cv::Mat diff_frame;// 差分データを保存するための変数

	int num_el = frame.cols * frame.rows;
	int frame_len = num_el * frame.elemSize1();

	int saved = 0;
	int chromakeyed = 0;
	int diff_range = (255 / 2);
	int diff_opt = 0;
	for (;;) {
		cap >> frame; // 1フレーム分の画像データを変数frameに格納する

		if (frame.empty()) {
			break; // 画像データ取得に失敗したらループを抜ける
		}


		if (saved == 0) {
			cap >> save_frame;
		}
		int key = cv::waitKey(1);
		if (key == 113) { //qボタンが押されたとき
			break;//forループから抜ける．
		} else if (key == 115) { //sが押されたとき
			cap >> save_frame; // フレーム画像を保存する
			saved = 1;
		} else if (key == 99) { //cが押されたとき
			cap >> chroma_image; // クロマキー画像を保存する
			chromakeyed = 1;
		}

		if (chromakeyed == 0) {
			cap >> chroma_image;
			cv::imshow("chroma_image", chroma_image); // 取得した画像データをウィンドウ表示する
		} else {
			cv::absdiff(save_frame, chroma_image, diff_frame); // 差の絶対値を求める

			for (int i = 0; i < frame_len; i++) { // 画素の数だけfor

				
				uchar choroma_abs = diff_frame.data[i]; //frame.data[i]番目の情報を chroma_absに格納
				std::cout << choroma_abs << endl;
				if (choroma_abs < diff_range) { 
					diff_opt = choroma_abs;
				} else if (choroma_abs > diff_range) {
					diff_opt = 0;
				}
				diff_frame.data[i] = diff_opt;
			}
			cv::imshow("chroma_image", diff_frame); // 取得した画像データをウィンドウ表示する
		}

		cv::imshow("image", frame); // 取得した画像データをウィンドウ表示する
		cv::imshow("image_saved", save_frame); // 取得した画像データをウィンドウ表示する
		
		//if (cv::waitKey(60) >= 0) break;

	}
	return 0;
}