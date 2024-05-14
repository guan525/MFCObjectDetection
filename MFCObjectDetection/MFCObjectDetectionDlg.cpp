
// MFCObjectDetectionDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCObjectDetection.h"
#include "MFCObjectDetectionDlg.h"
#include "afxdialogex.h"

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCObjectDetectionDlg 对话框



CMFCObjectDetectionDlg::CMFCObjectDetectionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCOBJECTDETECTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCObjectDetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 通过 DDX_Control 将 CMFCPropertyGridCtrl 实例与对话框中的控件关联起来
	DDX_Control(pDX, IDC_MFCPROPERTYGRID, m_propertyGrid);
}

BEGIN_MESSAGE_MAP(CMFCObjectDetectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_SIZE()// 当窗口大小发生变化时，OnSize 函数将被调用，允许您处理布局的调整。
	ON_WM_CONTEXTMENU(&CMFCObjectDetectionDlg::OnContextMenu)// 接收到右键点击消息调用 OnContextMenu 函数

	ON_COMMAND(ID_FILE_NEW, &CMFCObjectDetectionDlg::OnFileNew)// 菜单栏——新建文件
	ON_COMMAND(ID_FILE_OPEN, &CMFCObjectDetectionDlg::OnFileOpen)// 菜单栏——打开文件
	ON_COMMAND(ID_FILE_SAVE, &CMFCObjectDetectionDlg::OnFileSave)// 菜单栏——保存文件
	ON_COMMAND(ID_FILE_SAVE_AS, &CMFCObjectDetectionDlg::OnFileSaveAs)// 菜单栏——文件另存为
	ON_COMMAND(ID_SOFTWARE_OFF, &CMFCObjectDetectionDlg::OnSoftwareOff)// 菜单栏——软件关闭
	ON_COMMAND(ID_DETECTION_GRAIN_SIZE, &CMFCObjectDetectionDlg::OnDetectionGrainSize)// 菜单栏——晶粒度检测
	ON_COMMAND(ID_DETECTION_POROSITY, &CMFCObjectDetectionDlg::OnDetectionPorosity)// 菜单栏——气孔率检测
	ON_COMMAND(ID_DETECTION_SPRING_SOLDER_JOINT, &CMFCObjectDetectionDlg::OnDetectionSpringSolderJoint)// 菜单栏——弹簧焊点检测
	ON_COMMAND(ID_HELP_DOCUMENT, &CMFCObjectDetectionDlg::OnHelpDocument)// 菜单栏——帮助文档
	ON_COMMAND(ID_ABOUT_SOFTWARE, &CMFCObjectDetectionDlg::OnAboutSoftware)// 菜单栏——关于该软件
	ON_COMMAND(ID_HIST, &CMFCObjectDetectionDlg::OnHist)// 菜单栏——灰度直方图
	ON_COMMAND(ID_THRESHOLD, &CMFCObjectDetectionDlg::OnThreshold)// 菜单栏——二值化
	ON_COMMAND(ID_EXPORT_RESULTS, &CMFCObjectDetectionDlg::OnExportResults)// 弹出菜单——导出结果

END_MESSAGE_MAP()


// CMFCObjectDetectionDlg 消息处理程序

BOOL CMFCObjectDetectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// 菜单栏
	CMenu* pMainMenu = new CMenu();
	pMainMenu->LoadMenu(IDR_MENU);//IDR_MENU为菜单ID
	SetMenu(pMainMenu);
	pMainMenu->Detach();

	// 图片控件初始化图片(NWPU)
	CWnd  *pWnd1 = GetDlgItem(IDC_PIC);//GetDlgItem用于从对话框中获取指定ID的控件指针
	pWnd1->GetClientRect(&rectPic);//调用GetClientRect方法获取控件（IDC_PIC）的客户区尺寸（不包括边框和标题），并将尺寸信息存储在CRect类型的变量rectPic中
	namedWindow("src1", WINDOW_AUTOSIZE);//创建一个名为"src1"的窗口，并设置窗口大小为自动调整（WINDOW_AUTOSIZE），以适应图像的大小。
	HWND hWndl = (HWND)cvGetWindowHandle("src1");//hWnd 表示窗口句柄,获取窗口句柄
	HWND hParent1 = ::GetParent(hWndl);//GetParent函数：一个指定子窗口的父窗口句柄
	::SetParent(hWndl, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);
	matImage = imread(GetFullPath(L"\\MFCObjectDetection\\res\\NWPU.jpg"));
	if (!matImage.empty())
	{
		resize(matImage, matImageDisp, Size(rectPic.Width(), rectPic.Height()));
		imshow("src1", matImageDisp);
	}

	// 设置表格
	HDITEM item;
	item.cxy = 130;
	item.mask = HDI_WIDTH;
	m_propertyGrid.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	m_propertyGrid.EnableHeaderCtrl(TRUE, _T("属性"), _T("数值"));//设置表头

	// 为属性网格控件添加右键菜单
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_MENU);// 加载菜单资源
	CMenu* pPopup = menu.GetSubMenu(0);// 获取第一个子菜单（上下文菜单）
	if (pPopup != nullptr)// 将菜单设置为属性网格控件的上下文菜单
	{
		m_propertyGrid.SetMenu(pPopup);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCObjectDetectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCObjectDetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCObjectDetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 获取绝对路径（OK）
String CMFCObjectDetectionDlg::GetFullPath(CString relationPath)
{
	TCHAR exePath[MAX_PATH];
	GetModuleFileName(NULL, exePath, MAX_PATH);
	// 获取目录路径
	PathRemoveFileSpec(exePath);
	PathRemoveFileSpec(exePath);
	PathRemoveFileSpec(exePath);
	// 添加res文件夹和文件名
	CString CStringFullPath = exePath;
	CStringFullPath += relationPath;
	String fullPath = CT2A(CStringFullPath.GetString());
	return	fullPath;
}

// 菜单栏——新建文件
void CMFCObjectDetectionDlg::OnFileNew()
{
	
}

// 菜单栏——打开文件（OK）
void CMFCObjectDetectionDlg::OnFileOpen()
{
	TCHAR szFilter[] = _T("All Files (*.*)|*.*|图像源文件（*.dat)|*.DAT|图片文件 (*.PNG)|*.PNG|");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	Mat background1;
	// 显示打开文件对话框   
	if (IDOK != fileDlg.DoModal())return;	//未选择文件，返回！
	// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
	CString strFilePath = fileDlg.GetPathName();
	USES_CONVERSION;
	matImage = imread(T2A(strFilePath.GetBuffer()));
	if (!matImage.empty())
	{
		resize(matImage, matImageDisp, Size(rectPic.Width(), rectPic.Height()));
		imshow("src1", matImageDisp);
	}
}

// 菜单栏——保存文件（OK）
void CMFCObjectDetectionDlg::OnFileSave()
{	
	imwrite(GetFullPath(L"\\MFCObjectDetection\\res\\result.jpg"), matImage);
}

// 菜单栏——文件另存为(OK)
void CMFCObjectDetectionDlg::OnFileSaveAs()
{
	// 创建一个CFileDialog对象，设置保存文件的类型为JPG
	CFileDialog fileDialog(FALSE, _T("jpg"), _T("result"), OFN_OVERWRITEPROMPT, _T("JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"));

	// 显示文件对话框
	if (fileDialog.DoModal() == IDOK)
	{
		// 用户选择了保存文件，获取选择的文件路径
		CString filePath = fileDialog.GetPathName();

		// 将CString转换为std::string
		std::string stdFilePath = CW2A(filePath.GetString());

		// 使用imwrite保存图像
		try
		{
			imwrite(stdFilePath, matImage);
		}
		catch (cv::Exception& e)
		{
			// 处理异常，例如显示错误消息
			AfxMessageBox(CString("Error saving image: ") + e.what());
		}
	}
}

// 菜单栏——软件关闭（OK）
void CMFCObjectDetectionDlg::OnSoftwareOff()
{
	PostQuitMessage(0);
}

//距离变换函数
void CMFCObjectDetectionDlg::distance_star(Mat& image, Mat& outimage)
{
	Mat gray, binary;
	//滤波后的二值化
	//imshow("binary", binary);

	// distance transform 距离变换
	Mat dist;
	distanceTransform(image, dist, DistanceTypes::DIST_L2, 3, CV_32F);
	normalize(dist, dist, 0, 1, NORM_MINMAX);  // 归一化函数
	//imshow("距离变换图", dist);

	// binary二值化函数
	threshold(dist, outimage, 0.17, 255, THRESH_BINARY);
	// imshow("距离变换结果图", outimage);

	return;

}

//
void CMFCObjectDetectionDlg::connected_components_stat(Mat& image) {

	Mat matDisp_image = image;
	double sum = 0;//记录总面积
	double average = 0;//记录平均面积的1/2
	int A = 0;//豌豆数量
	int B = 0;//绿豆数量
	RNG rng(123); // 创建 RNG 对象，种子为 123
	// 二值化
	Mat gray, binary;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	distance_star(binary, binary);
	Mat dist_m;
	binary.convertTo(binary, CV_8UC1);//通道转换

	//开运算、闭运算
	Mat k = getStructuringElement(MORPH_RECT, Size(13, 13), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_OPEN, k);
	//morphologyEx(binary, binary, MORPH_CLOSE, k);
	// 形态学操作 - 彩色图像，目的是去掉干扰，让结果更好
	Mat o = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary, binary, MORPH_ERODE, o);// 腐蚀，去粘连部位的干扰

	//计算连通域
	Mat labels = Mat::zeros(image.size(), CV_32S);
	Mat stats, centroids;
	int num_labels = connectedComponentsWithStats(binary, labels, stats, centroids, 8, 4);//对比新旧函数，用于过滤原始图像中轮廓分析后较小的区域，留下较大区域。

	//使用不同的颜色标记连通域
	vector<Vec3b> colors(num_labels);
	vector<Vec3b> colors1(num_labels);
	// background color
	colors[0] = Vec3b(0, 0, 0);
	colors1[0] = Vec3b(0, 0, 0);
	// object color
	for (int i = 1; i < num_labels; i++) {
		colors1[i] = Vec3b(rng.uniform(125, 125), rng.uniform(125, 125), rng.uniform(125, 125));
		colors[i] = Vec3b(rng.uniform(255, 255), rng.uniform(255, 255), rng.uniform(255, 255));
	}
	//Mat dst = Mat::zeros(image.size(), image.type());

	// render result
	Mat dst = Mat::zeros(image.size(), image.type());
	int w = image.cols;
	int h = image.rows;
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			int label = labels.at<int>(row, col);
			if (label == 0) continue;
			dst.at<Vec3b>(row, col) = colors[label];
		}
	}

	//利用统计信息标记连通域
	for (int i = 1; i < num_labels; i++) {
		Vec2d pt = centroids.at<Vec2d>(i, 0);
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);
		sum += area;
		//printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);
		//circle(dst, Point(pt[0], pt[1]), 2, Scalar(0, 0, 255), -1, 8, 0);
		//rectangle(dst, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);
	}
	average = sum / num_labels;
	for (int i = 1; i < num_labels; i++)
	{
		Vec2d pt = centroids.at<Vec2d>(i, 0);
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);

		if (area > average) {
			A = A + 1;
			//circle(dst, CvPoint2D32f(pt[0], pt[1]), 2, Scalar(0, 0, 255), -1, 8, 0);
			//rectangle(dst, Rect(x, y, width, height), Scalar(255, 0, 255), 1, 8, 0);
		}
		else
		{
			B = B + 1;
			//circle(dst, CvPoint2D32f(pt[0], pt[1]), 2, Scalar(255, 0, 0), -1, 8, 0);
			//rectangle(dst, Rect(x, y, width, height), Scalar(255, 255, 0), 1, 8, 0);
		}
	}
	//CString str;
	imwrite("D:\\Code\\VS2017_MFC_code\\MFCObjectDetection\\MFCObjectDetection\\res\\3.bmp", dst);

	// 清除属性网格中已有的数据
	m_propertyGrid.RemoveAll();

	CString propertyName[8] = { _T("图片大小"), _T("颗粒数量"), _T("颗粒总面积"), _T("颗粒面积均值"), _T("颗粒空隙数量"), _T("方差"), _T("黑影面积"), _T("其他"), };
	CString propertyValue[8] = { _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0") };//初始值
	CString propertyExplain[8] = { _T("左侧图片的大小"), _T("图片中的小颗粒个数"), _T("图片中小颗粒的总面积")
		, _T("图片中的小颗粒面积的平均值"), _T("小颗粒中间的空隙数量"), _T("方差"), _T("没有小颗粒的阴影部分面积"), _T("其他") };
	
	for (int i = 0; i < 8; i++) {
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(
			propertyName[i],
			propertyValue[i],
			propertyExplain[i]);
		m_propertyGrid.AddProperty(pProp);
	}

	// 获取图像尺寸(获取到的实际结果图片控件的大小)
	int imageWidth = matImage.cols;
	int imageHeight = matImage.rows;

	// 显示图像的长宽
	CString strImageSize;
	strImageSize.Format(_T("%d*%d"), imageWidth, imageHeight);
	propertyValue[0] = strImageSize;

	// 颗粒总数量
	CString strNumContours;
	strNumContours.Format(_T("%d"), num_labels);
	propertyValue[1] = strNumContours;

	// 颗粒总面积
	CString strTotalArea;
	strTotalArea.Format(_T("%.2f"), sum);
	propertyValue[2] = strTotalArea;

	// 颗粒面积均值
	CString strMeanContours;
	strMeanContours.Format(_T("%.2f"), average);
	propertyValue[3] = strMeanContours;
	// imshow("二分类图像", dst);
	// 更新属性网格控件
	for (int i = 0; i < 8; i++) {
		// m_propertyGrid.GetProperty(i)->SetValue(propertyValue[i]);
		CMFCPropertyGridProperty* pProp = m_propertyGrid.GetProperty(i);
		if (pProp) {
			pProp->SetValue(propertyValue[i]);
		}
	}
	//nihe(dst);椭圆拟合函数

	// 找到图像中的轮廓
	Mat gray1;
	cvtColor(dst, gray1, cv::COLOR_BGR2GRAY);
	// 定义膨胀核
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	// 应用膨胀操作
	cv::Mat dilatedImage;
	cv::dilate(gray1, gray1, kernel);

	//Mat ori = imread();
	//std::vector<cv::Scalar> colors1 = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255) };
	std::vector<std::vector<cv::Point>> contours1;
	cv::findContours(gray1, contours1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::Scalar color;
	// 绘制轮廓并使用不同颜色标记每个轮廓
	for (size_t i = 0; i < contours1.size(); i++) {
		if (i % 3 == 0)
			color = cv::Scalar(255, 0, 0);
		else if (i % 3 == 1)
			color = cv::Scalar(0, 255, 0);
		else
			color = cv::Scalar(0, 0, 255);
		cv::drawContours(matDisp_image, contours1, static_cast<int>(i), color, 2);
	}
	imwrite("D:\\Code\\VS2017_MFC_code\\MFCObjectDetection\\MFCObjectDetection\\res\\result.jpg", matDisp_image);
	if (!matDisp_image.empty())
	{
		matImage = matDisp_image;
		resize(matImage, matImageDisp, Size(rectPic.Width(), rectPic.Height()));
		imshow("src1", matImageDisp);
	}
}

//
void CMFCObjectDetectionDlg::pro(Mat& image) //处理图像，所有处理过程均通过这个函数调用处理
{
	IplImage* src = cvLoadImage("D:\\Code\\VS2017_MFC_code\\MFCObjectDetection\\MFCObjectDetection\\res\\2.bmp", 0);//原彩色图像的二值化图像，用于转化通道处理
	IplImage* dst = cvCreateImage(cvSize(src->width, src->height), 8, 3);

	// 加载图像
	Mat srcMat = imread("D:\\Code\\VS2017_MFC_code\\MFCObjectDetection\\MFCObjectDetection\\res\\2.bmp", 0);
	// 创建与原图像尺寸相同的目标图像
	Mat dstMat(srcMat.size(), CV_8UC3); // 3通道图像

	Mat img = imread("D:\\Code\\VS2017_MFC_code\\MFCObjectDetection\\MFCObjectDetection\\res\\2.bmp");

	CvMemStorage* stor = cvCreateMemStorage(0);
	CvSeq* cont = NULL;
	int numContours = cvFindContours(src, stor, &cont, sizeof(CvContour), CV_RETR_LIST);      //记录总轮廓数
	/*CString strNumContours;
	strNumContours.Format(_T("%d"), numContours);
	propertyValue[1] = strNumContours;*/
	int i = 0;
	int j = 0;
	int k = 0;
	float g = 0.0;
	int z = 0;
	float w = 0.0;
	float d = 0.0;
	float flag = 0.0;//没有用到，原本想给个标签的

	float array1[700][3] = { 0.0 };//实际上没有用到这个 这个可以扩展使用

	for (i = 0; cont; cont = cont->h_next)
	{
		int* length = new   int[numContours];        //作用是记录每个轮廓上的点数
		length[i++] = cont->total;         //记录每个轮廓上的元素个数
		//printf("cont->total:%d\n", cont->total);
		CvPoint* point = new CvPoint[cont->total];
		CvSeqReader reader;
		CvPoint pt = cvPoint(0, 0);
		CvPoint pt1 = cvPoint(0, 0);
		CvPoint pt2 = cvPoint(0, 0);
		CvPoint pt3 = cvPoint(0, 0);
		CvPoint pt4 = cvPoint(0, 0);
		cvStartReadSeq(cont, &reader);
		for (int j = 0; j < cont->total; j++)
		{
			CV_READ_SEQ_ELEM(pt, reader);
			point[j] = pt;
			//cout << pt.x << "  " << pt.y << endl;
		}
		for (j = 0; j < cont->total; j++)
		{
			int k = (j + 1) % cont->total;
			cvLine(dst, point[j], point[k], cvScalar(0, 0, 255), 1, 4, 0);
		}

		for (int j = 0; j < cont->total - 14; j++)
		{
			CV_READ_SEQ_ELEM(pt1, reader);
			CV_READ_SEQ_ELEM(pt2, reader);
			CV_READ_SEQ_ELEM(pt3, reader);
			pt1 = point[j];
			pt2 = point[j + 7];    //取步长为7
			pt3 = point[j + 14];

			if ((pt1.x != pt2.x) & (pt3.x != pt2.x))
			{
				//w = j + 8;
				//int g = int(((pt1.y-pt2.y)/(pt1.x-pt2.x))+ ((pt2.y - pt3.y) / (pt2.x - pt3.x)));
				float g = floor((pt2.x - pt1.x) * (pt3.x - pt2.x) + (pt2.y - pt1.y) * (pt3.y - pt2.y));
				if (g < 1.0)
				{
					//w = j + 8;
					flag = 1;
					//cvLine(dst, point[j], point[j+2], cvScalar(0, 255, 0), 1, 4, 0);
					circle(dstMat, pt2, 1, Scalar(0, 255, 255), -1);
					array1[z][0] = floor(pt2.x);
					array1[z][1] = floor(pt2.y);
					array1[z][2] = flag;
					z++;
				}
			}
		}
		delete point;
	}

	for (i = 0; i < 700; i++)
	{
		if (array1[i][2] == 1.0)
		{
			for (j = 1; j < 700; j++) {
				w = 35.0;
				float d = sqrt(pow((array1[i][0] - array1[j][0]), 2) +
					pow((array1[i][1] - array1[j][1]), 2));
				if ((d < w) & (d > 5.0)) {
					//w = d;
					CvPoint2D32f pt5 = cvPoint2D32f(0.0, 0.0);
					CvPoint2D32f pt6 = cvPoint2D32f(0.0, 0.0);
					//CvPoint pt6 = cvPoint(0, 0);
					pt5.x = array1[i][0];
					pt5.y = array1[i][1];
					pt6.x = array1[j][0];
					pt6.y = array1[j][1];
					//凹点分割画线
					line(img, pt5, pt6, Scalar(0, 0, 0), 2, 4, 0);
				}
			}
		}
	}
	// cvShowImage("寻找凹点图", dst);
	imshow("凹点分割后", image);
	connected_components_stat(image);
}

// 菜单栏——晶粒度检测（凹点检测）
void CMFCObjectDetectionDlg::OnDetectionGrainSize()
{
	Mat gray = onGray(matImage);
	//进行二值化处理，选择60，255.0为阈值
	threshold(gray, gray, 60, 255, cv::THRESH_BINARY);
	//进行腐蚀操作，稍微断开一些联通区域
	Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9));
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat erosion;
	erode(matImage, erosion, kernel);
	//imshow("erosion1", erosion);
	//erode(erosion, erosion, kernel2);
	//erode(erosion, erosion, kernel2);
	//imshow("erosion1", erosion);
	//waitKey(0);
	//bitwise_not(img, img, Mat());  //对二进制数据进行“非”操作

	// imshow("二值化图像", erosion);//显示二值化图像
	imwrite("D:\\Code\\VS2017_MFC_code\\MFCObjectDetection\\MFCObjectDetection\\res\\2.bmp", erosion);//保存二值化图像
	pro(matImage);//处理图像
}

// 菜单栏——气孔率检测
void CMFCObjectDetectionDlg::OnDetectionPorosity()
{

}

// 菜单栏——弹簧焊点检测
void CMFCObjectDetectionDlg::OnDetectionSpringSolderJoint()
{

}

// 菜单栏——帮助文档
void CMFCObjectDetectionDlg::OnHelpDocument()
{
	
}

// 菜单栏——关于该软件
void CMFCObjectDetectionDlg::OnAboutSoftware()
{

}

// 灰度图（OK）
Mat CMFCObjectDetectionDlg::onGray(Mat image)
{
	// 将当前输出的图像 matImage 转换为灰度图像，并将结果存储在 gray 变量中
	Mat gray;
	cvtColor(matImage, gray, COLOR_BGR2GRAY);
	return gray;
}

// 根据灰度图绘制图像的直方图（OK）
void CMFCObjectDetectionDlg::drawHist(Mat gray)
{
	// 用于存储从输入图像计算得到的直方图。
	Mat hist;
	// 表示直方图将有 256 个箱子，对应灰度图像的灰度强度级别（范围从 0 到 255）。
	int histSize = 256;
	// 像素值范围
	float range[] = { 0, 256 };
	// histRange 是指向 range 数组的指针。它作为参数传递给 calcHist 函数。
	const float* histRange = { range };
	// 使用 OpenCV 库中的 calcHist 函数计算输入图像的直方图
	// &matImage：输入图像的地址。
	// 1：图像的数量（在本例中只有一个）。
	// 0：用于计算直方图的通道索引（灰度图像通常设为 0，彩色图像通常设置为0、1、2）。
	// Mat()：用于指定图像子集的掩码（这里没有使用，因此传递了一个空矩阵）。
	// hist：输出直方图矩阵。
	// 1：直方图的维数。
	// &histSize：指向包含每个维度的箱子数量的数组的指针。
	// &histRange：指向每个维度范围数组的指针。
	calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	// 创建直方图可视化图像
	int hist_w = 2048;
	int hist_h = 1200;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	// 将直方图归一化到范围 [0, histImage.rows]
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	// 绘制直方图
	for (int i = 1; i < histSize; i++) {
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 255, 255), 2, 8, 0);

		// 在直方图上标出像素值范围（以10为单位）
		if (i % 10 == 0) {
			putText(histImage, std::to_string((i - 10) * 256 / histSize) + "-" + std::to_string(i * 256 / histSize),
				Point(bin_w * (i - 1), hist_h - 10), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 255, 255), 1);
		}
	}

	// 输出40-60的灰度图像
	// 创建与输入图像大小相同的灰度图像
	/*Mat thresholded3(gray3.size(), CV_8UC1);
	// 循环遍历图像像素，并根据像素值范围进行像素值设置
	for (int y = 0; y < gray3.rows; ++y) {
		for (int x = 0; x < gray3.cols; ++x) {
			if (gray3.at<uchar>(y, x) >= 40 && gray3.at<uchar>(y, x) <= 60) {
				thresholded3.at<uchar>(y, x) = 0; // 设置为黑色
			}
			else {
				thresholded3.at<uchar>(y, x) = 255; // 设置为白色
			}
		}
	}*/

	// 显示直方图
	imshow("灰度直方图", histImage);
}

// 菜单栏——灰度直方图（OK）
void CMFCObjectDetectionDlg::OnHist()
{
	Mat gray = onGray(matImage);
	drawHist(gray);
}

// 菜单栏——二值化(OK)
void CMFCObjectDetectionDlg::OnThreshold()
{
	Mat gray = onGray(matImage);
	Mat thresholded;
	// 大于 100 的设置为 255（白），小于等于 50 的设置为 0（黑）
	threshold(gray, thresholded, 100, 255, THRESH_BINARY);
	imshow("二值化", thresholded);
}

// 属性网格右键显示菜单(OK)
void CMFCObjectDetectionDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd->GetDlgCtrlID() == IDC_MFCPROPERTYGRID) // 确保右键点击的是属性网格控件
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP_MENU)); // 加载菜单资源
		CMenu* pPopupMenu = menu.GetSubMenu(0);
		if (pPopupMenu != nullptr)
		{
			// 可以在这里根据需要启用或禁用菜单项
			pPopupMenu->EnableMenuItem(ID_EXPORT_RESULTS, MF_BYCOMMAND | MF_ENABLED);

			// 在鼠标点击处显示菜单
			pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			pPopupMenu->Detach(); // 断开菜单与原始菜单的连接
		}
		menu.DestroyMenu(); // 清理菜单资源
	}
	else
	{
		CDialogEx::OnContextMenu(pWnd, point); // 其他控件的右键处理
	}
}

// 右键菜单——导出结果(txt中文有乱码)
void CMFCObjectDetectionDlg::OnExportResults()
{
	// 导出图片
	CWnd* pWndPic = GetDlgItem(IDC_PIC);
	CRect rectPic;
	pWndPic->GetWindowRect(&rectPic); // 获取图片控件的屏幕坐标
	ScreenToClient(&rectPic); // 转换为客户区坐标

	// 确保matImage有效且已初始化
	if (!matImage.empty()) {
		String imagePath = GetFullPath(L"\\MFCObjectDetection\\res\\export\\export_image.jpg");
		imwrite(imagePath, matImage);// 保存图片
	}

	CString strProperties;
	int itemCount = m_propertyGrid.GetPropertyCount();// 获取属性项数量
	for (int i = 0; i < itemCount; i++) {
		CMFCPropertyGridProperty* pItem = m_propertyGrid.GetProperty(i);
		if (pItem != nullptr) {
			CString propName = pItem->GetName(); // 属性名称
			CString propValue = pItem->GetValue(); // 属性值
			strProperties.AppendFormat(_T("%s: %s\r\n"), propName, propValue); // 格式化字符串
		}
	}

	// 调用GetFullPath获取cv::String类型的绝对路径
	String TextFilePath = GetFullPath(L"\\MFCObjectDetection\\res\\export\\export_text.txt");

	// 使用CA2W将 string 转换为 CString
	CString CTextFilePath = CA2W(TextFilePath.c_str());

	// 然后使用CString类型的路径进行文件操作
	try
	{
		CFile file(CTextFilePath, CFile::modeCreate | CFile::modeWrite);
		file.Write(strProperties, strProperties.GetLength());
		file.Close();

		// 显示成功消息
		CString messageText = _T("导出完成！");
		CString messageCaption = _T("提示");
		MessageBox(messageText, messageCaption, MB_OK | MB_ICONINFORMATION);
	}
	catch (CFileException* e)
	{
		// 文件操作失败，处理异常
		e->ReportError();
		AfxMessageBox(_T("文件写入时发生错误！"), MB_ICONERROR);
		e->Delete();
	}
}

// 根据对话框大小变化，等比缩放所有子控件（OK）
void CMFCObjectDetectionDlg::ReSize()
{
	// 获取现在对话框的大小
	CRect rectNow;
	GetClientRect(&rectNow);     // 取客户区大小
	POINT Newp = { rectNow.right - rectNow.left, rectNow.bottom - rectNow.top };

	// 计算缩放比例
	float fsp[2];
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;

	// 遍历所有子控件，调整它们的大小和位置
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild)
	{
		CRect Rect;
		int woc = ::GetDlgCtrlID(hwndChild); // 取得ID
		GetDlgItem(woc)->GetWindowRect(Rect); // 获取控件的窗口矩形
		ScreenToClient(Rect); // 将控件的矩形转换为对话框的客户区坐标
		CPoint OldTLPoint = Rect.TopLeft(); // 控件原始左上角坐标
		CPoint TLPoint = { long(OldTLPoint.x * fsp[0]), long(OldTLPoint.y * fsp[1]) };
		CPoint OldBRPoint = Rect.BottomRight(); // 控件原始右下角坐标
		CPoint BRPoint = { long(OldBRPoint.x * fsp[0]), long(OldBRPoint.y * fsp[1]) };
		Rect.SetRect(TLPoint, BRPoint); // 设置新的控件矩形
		GetDlgItem(woc)->MoveWindow(Rect, TRUE); // 移动控件
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT); // 获取下一个子控件
	}
	old = Newp;// 更新原始坐标
}

// 处理对话框尺寸变化事件，调用 ReSize 函数（OK）
void CMFCObjectDetectionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)
		ReSize();
	// TODO: 在此处添加消息处理程序代码
	CDialog::OnSize(nType, cx, cy);

	if (0 == m_DlgRect.left && 0 == m_DlgRect.right &&
		0 == m_DlgRect.top && 0 == m_DlgRect.bottom)
	{//第一次启动对话框时的大小变化不做处理
	}
	else
	{
		if (0 == cx && 0 == cy)
		{//如果是按下了最小化，则触发条件，这时不保存对话框数据
			return;
		}

		// 重绘图像
		if (!matImage.empty())
		{
			CWnd  *pWnd1 = GetDlgItem(IDC_PIC);//GetDlgItem用于从对话框中获取指定ID的控件指针
			pWnd1->GetClientRect(&rectPic);//调用GetClientRect方法获取控件（IDC_PIC）的客户区尺寸（不包括边框和标题），并将尺寸信息存储在CRect类型的变量rectPic中
			resize(matImage, matImageDisp, Size(rectPic.Width(), rectPic.Height()));
			imshow("src1", matImageDisp);//重绘图像
		}
	}
	GetClientRect(&m_DlgRect); //save size of current dialog
	Invalidate();//更新窗口
}
