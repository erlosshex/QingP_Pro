// test_opengl_environment.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static int iWidth = 800;
static int iHeight = 600;

static char carrWindowName[] = "opengl window";

void framebufferSizeCallback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);
void renderOrder();

int main()
{
    std::cout << "Hello World!\n";

    std::cout << "opengl programing start ...... " << "\n";

    //函数库的初始化
    glfwInit();

    //设定opengl版本4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef _apple_
    //设定opengl为核心模式
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //创建窗口对象
    GLFWwindow* pwindow = glfwCreateWindow(iWidth, iHeight, carrWindowName, NULL, NULL);

    if (pwindow == NULL)
    {
        std::cout << "opengl window start fail!\n";
        std::cout << "opengl programing will terminate ......\n";
        
        //终止opengl程序
        glfwTerminate(); 
        return -1;
    }

    //设定当前上下文
    glfwMakeContextCurrent(pwindow);

    //注册窗口大小改变回调
    glfwSetFramebufferSizeCallback(pwindow, framebufferSizeCallback);

    //初始化gladLoader
    if (false == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "can't get glfw pointer address!\n";
        std::cout << "opengl programing will terminate ......\n";

        glfwTerminate();
        return -1;
    }

    //执行到这里就是默认完成了所有初始化
    //glfw没有执行结束的时候就一直是死循环
    while (false == glfwWindowShouldClose(pwindow))
    {
        processInput(pwindow);
        renderOrder();

        //缓冲区交互
        //关于双缓存的补充：
        //如果使用单缓存的渲染模式，那么游戏画面就会出现闪烁现象，原因是图形渲染出一帧的画面时，每一个像素点不是同时渲染出来，
        //而是遵循流水线，由左往右，由上往下来进行渲染的，所以这个时候采用双缓存，当前缓存显示完，直接把后缓存的画面交换上去，
        //并且使用glfwSwapBuffers()函数进行交换
        glfwSwapBuffers(pwindow);

        //执行被触发的事件
        glfwPollEvents();
    }

    return 0;
}

/// <summary>
/// 回调函数。窗口大小改变时。
/// </summary>
void framebufferSizeCallback(GLFWwindow* pwindow, int iWidth, int iHeight)
{
    //这是控制视口的函数。
    //前面两个参数是指视口的左下角的坐标偏移位置。（opengl的视口窗口默认左下角为原点(0,0)
    //后面两个参数是指视口的宽度和高度
    glViewport(0, 0, iWidth, iHeight);

    std::cout << "opengl window resolution change! Width : " << iWidth << ", Height : " << iHeight << std::endl;
}

/// <summary>
/// 处理输入函数
/// </summary>
void processInput(GLFWwindow* pwindow)
{
    if (glfwGetKey(pwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pwindow, true);
    }
}

/// <summary>
/// 渲染指令
/// </summary>
void renderOrder()
{
    //深绿色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //关于glfwClear函数，这是一个清屏的函数，当前是去掉颜色缓存区
    //如果传入参数GL_DEPCH_BUFFER_BIT（深度缓存区）和GL_STENCIL_BUFFER_BIT（模板缓存区）就是清掉对应的缓存区
    glClear(GL_COLOR_BUFFER_BIT);
}

