#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingOpenGL2);

int user_data;

void viewerOneOff(pcl::visualization::PCLVisualizer& viewer)
{
    viewer.setBackgroundColor(1.0, 0.5, 1.0);
    pcl::PointXYZ o;
    o.x = 1.0;
    o.y = 0;
    o.z = 0;
    viewer.addSphere(o, 0.25, "sphere", 0);
    std::cout << "i only run once" << std::endl;

}

void viewerPsycho(pcl::visualization::PCLVisualizer& viewer)
{
    static unsigned count = 0;
    std::stringstream ss;
    ss << "Once per viewer loop: " << count++;
    viewer.removeShape("text", 0);
    viewer.addText(ss.str(), 200, 300, "text", 0);

    //FIXME: possible race condition here:
    user_data++;
}

int main()
{
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
    
    std::string strPCDFilePathname = "D:\\GGS\\Codes\\Mine\\QingP_Pro\\machine_vision\\QingP_MachineVision\\x64\\Release\\rabbit.pcd";
#if _DEBUG
    strPCDFilePathname = "D:\\GGS\\Codes\\Mine\\QingP_Pro\\machine_vision\\QingP_MachineVision\\x64\\Debug\\rabbit.pcd";
#endif

    pcl::io::loadPCDFile(strPCDFilePathname, *cloud);

    pcl::visualization::CloudViewer viewer("Cloud Viewer");



    //blocks until the cloud is actually rendered
    viewer.showCloud(cloud);

    //use the following functions to get access to the underlying more advanced/powerful
    //PCLVisualizer

    //This will only get called once
    viewer.runOnVisualizationThreadOnce(viewerOneOff);

    //This will get called once per visualization iteration
    viewer.runOnVisualizationThread(viewerPsycho);
    while (!viewer.wasStopped())
    {
        //you can also do cool processing here
        //FIXME: Note that this is running in a separate thread from viewerPsycho
        //and you should guard against race conditions yourself...
        user_data++;
    }
    return 0;
}





/**********************************************************************************/

//#include <iostream>
//#include <vector>
//#include <ctime>
//#include <pcl/point_cloud.h>
//#include <pcl/octree/octree.h>
//#include <boost/thread/thread.hpp>
//#include <pcl/visualization/pcl_visualizer.h>
//
//using namespace std;
//
//int main(int argc, char** argv)
//{
//	srand((unsigned int)time(NULL));
//	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//	// 创建点云数据
//	cloud->width = 1000;
//	cloud->height = 1;
//	cloud->points.resize(cloud->width * cloud->height);
//	for (size_t i = 0; i < cloud->points.size(); ++i)
//	{
//		cloud->points[i].x = 1024.0f * rand() / (RAND_MAX + 1.0f);
//		cloud->points[i].y = 1024.0f * rand() / (RAND_MAX + 1.0f);
//		cloud->points[i].z = 1024.0f * rand() / (RAND_MAX + 1.0f);
//	}
//
//	pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree(0.1);
//	octree.setInputCloud(cloud);
//	octree.addPointsFromInputCloud();
//	pcl::PointXYZ searchPoint;
//	searchPoint.x = 1024.0f * rand() / (RAND_MAX + 1.0f);
//	searchPoint.y = 1024.0f * rand() / (RAND_MAX + 1.0f);
//	searchPoint.z = 1024.0f * rand() / (RAND_MAX + 1.0f);
//
//	//半径内近邻搜索
//	vector<int>pointIdxRadiusSearch;
//	vector<float>pointRadiusSquaredDistance;
//	float radius = 256.0f * rand() / (RAND_MAX + 1.0f);
//	cout << "Neighbors within radius search at (" << searchPoint.x
//		<< " " << searchPoint.y
//		<< " " << searchPoint.z
//		<< ") with radius=" << radius << endl;
//	if (octree.radiusSearch(searchPoint, radius, pointIdxRadiusSearch, pointRadiusSquaredDistance) > 0)
//	{
//		for (size_t i = 0; i < pointIdxRadiusSearch.size(); ++i)
//			cout << "    " << cloud->points[pointIdxRadiusSearch[i]].x
//			<< " " << cloud->points[pointIdxRadiusSearch[i]].y
//			<< " " << cloud->points[pointIdxRadiusSearch[i]].z
//			<< " (squared distance: " << pointRadiusSquaredDistance[i] << ")" << endl;
//	}
//	// 初始化点云可视化对象
//	boost::shared_ptr<pcl::visualization::PCLVisualizer>viewer(new pcl::visualization::PCLVisualizer("显示点云"));
//	viewer->setBackgroundColor(0, 0, 0);  //设置背景颜色为黑色
//	// 对点云着色可视化 (red).
//	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>target_color(cloud, 255, 0, 0);
//	viewer->addPointCloud<pcl::PointXYZ>(cloud, target_color, "target cloud");
//	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "target cloud");
//
//	// 等待直到可视化窗口关闭
//	while (!viewer->wasStopped())
//	{
//		viewer->spinOnce(100);
//		boost::this_thread::sleep(boost::posix_time::microseconds(1000));
//	}
//
//	return (0);
//}