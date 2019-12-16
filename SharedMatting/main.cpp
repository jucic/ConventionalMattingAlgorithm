/*本程序用于提取图像的前景轮廓。本项目组的工作是将原先的OpenCV 1.X代码修改为OpenCV 2.X代码。
 * 代码在WIN7 + Qt5.2.0 + OpenCV2.4.8平台上测试，成功运行。
 * Tips: input文件夹、trimap文件夹与result文件夹放在程序运行的当前目录下，即可正常运行，产生结果。
 * 2014年1月7日
*/
#include "stdafx.h"
#include "sharedmatting.h"
#include <string>

using namespace std;

int main()
{
    /*char fileAddr[64] = {0};

    for (int n = 1; n < 28; ++n) {
        SharedMatting sm;

        sprintf(fileAddr, "input/GT%d%d.png", n/10, n%10);
        sm.loadImage(fileAddr);

        sprintf(fileAddr, "trimap/trimap1/GT%d%d.png", n/10, n%10);
        sm.loadTrimap(fileAddr);

        sm.solveAlpha();

        sprintf(fileAddr, "result/GT%d%d.png", n/10, n%10);
        sm.save(fileAddr);
    }*/


	SharedMatting sm;

	sm.loadImage("./data/image.png");

	sm.loadTrimap("./data/trimap.png");


	const auto T1 = std::chrono::system_clock::now();

	sm.solveAlpha();

	const auto T2 = std::chrono::system_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - T1).count();
	std::cout << "SharedMatting time: " << duration << "[ms]" << std::endl << std::endl;

	sm.save("./data/res.png");
	system("pause");
    return 0;
}
