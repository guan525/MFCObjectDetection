
// MFCObjectDetectionDlg.h: 头文件
#include <opencv2/opencv.hpp>
using namespace cv;
//

#pragma once

// CMFCObjectDetectionDlg 对话框
class CMFCObjectDetectionDlg : public CDialogEx
{
// 构造
public:
	CMFCObjectDetectionDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCOBJECTDETECTION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CRect rectPic;// 图片矩形框
	Mat matImage;// 原始大小的图片，对该图片进行修改
	Mat matImageDisp;// 只用来图片修改尺寸显示在图片控件中
	CMFCPropertyGridCtrl m_propertyGrid;// 创建 CMFCPropertyGridCtrl 实例
	CRect m_DlgRect;// 存储对话框的当前客户区尺寸
	POINT old;// 记录窗口原始坐标

	afx_msg String GetFullPath(CString relationPath);// 获取绝对路径
	afx_msg void OnFileNew();// 菜单栏——新建文件
	afx_msg void OnFileOpen();// 菜单栏——打开文件
	afx_msg void OnFileSave();// 菜单栏——保存文件
	afx_msg void OnFileSaveAs();// 菜单栏——文件另存为
	afx_msg void OnSoftwareOff();// 菜单栏——软件关闭
	afx_msg void distance_star(Mat& image, Mat& outimage); //距离变换函数
	afx_msg void connected_components_stat(Mat& image);   //带统计信息
	afx_msg void pro(Mat& image);
	afx_msg void OnDetectionGrainSize();// 菜单栏——晶粒度检测
	afx_msg void OnDetectionPorosity();// 菜单栏——气孔率检测
	afx_msg void OnDetectionSpringSolderJoint();// 菜单栏——弹簧焊点检测
	afx_msg void OnHelpDocument();// 菜单栏——帮助文档
	afx_msg void OnAboutSoftware();// 菜单栏——关于该软件
	afx_msg Mat onGray(Mat image);// 灰度图
	afx_msg void drawHist(Mat gray);// 根据灰度图绘制图像的直方图
	afx_msg void OnHist();// 菜单栏——灰度直方图
	afx_msg void OnThreshold();// 菜单栏——二值化
	afx_msg void OnExportResults();// 弹出菜单——导出结果
	afx_msg void ReSize();// 调整子控件布局

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);// 自适应大小
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);// 属性网格右键显示菜单
};
