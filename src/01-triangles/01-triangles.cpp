//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "vgl.h"
#include "LoadShaders.h"
#include <stdio.h>
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];

const GLuint  NumVertices = 6;

static void checkGLError(const char* cmd){
    int error =glGetError();
    if( error!= GL_NO_ERROR){
        printf("%s error=%d\n",cmd,error);
    };
}

//----------------------------------------------------------------------------
//
// init
//
void
init( void )
{
    glGenVertexArrays( NumVAOs, VAOs );
    glBindVertexArray( VAOs[Triangles] );
    GLfloat  vertices[NumVertices][2] = {
        { -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
        {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
    };

    //glCreateBuffers is API of openGL4.5
    //refer to https://stackoverflow.com/questions/31841494/difference-in-glgenbuffers-and-glcreatebuffers
    /*
        创建VBO需要3个步骤：
          使用glGenBuffers()生成新缓存对象。
          使用glBindBuffer()绑定缓存对象。
          使用glBufferData()将顶点数据拷贝到缓存对象中。
    */
    glGenBuffers( NumBuffers, Buffers );
    /*glGenBuffers()创建缓存对象并且返回缓存对象的标示符。它需要2个参数：
        第一个为需要创建的缓存数量，
        第二个为用于存储单一ID或多个ID的GLuint变量或数组的地址。*/
    glBindBuffer( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
    /*
        当缓存对象创建之后，在使用缓存对象之前，我们需要将缓存对象连接到相应的缓存上。glBindBuffer()有2个参数：target与buffer。
        target告诉VBO该缓存对象将保存顶点数组数据还是索引数组数据：GL_ARRAY_BUFFER或GL_ELEMENT_ARRAY。任何顶点属性，如顶点坐标、
        纹理坐标、法线与颜色分量数组都使用GL_ARRAY_BUFFER。用于glDraw[Range]Elements()的索引数据需要使用GL_ELEMENT_ARRAY绑定。
        注意，target标志帮助VBO确定缓存对象最有效的位置，如有些系统将索引保存AGP或系统内存中，将顶点保存在显卡内存中。
        当第一次调用glBindBuffer()，VBO用0大小的内存缓存初始化该缓存，并且设置VBO的初始状态，如用途与访问属性。
    */
    //glBufferStorage is API of openGL4.5
    //glBufferStorage( GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices,  GL_STATIC_DRAW);
    /*
        当缓存初始化之后，你可以使用glBufferData()将数据拷贝到缓存对象。
        第一个参数target可以为GL_ARRAY_BUFFER或GL_ELEMENT_ARRAY。size为待传递数据字节数量。第三个参数为源数据数组指针，如data为NULL，
        则VBO仅仅预留给定数据大小的内存空间。最后一个参数usage标志位VBO的另一个性能提示，它提供缓存对象将如何使用：
        static、dynamic或stream、与read、copy或draw。
    */
    ShaderInfo  shaders[] =
    {
        { GL_VERTEX_SHADER, "media/shaders/triangles/triangles.vert" },
        { GL_FRAGMENT_SHADER, "media/shaders/triangles/triangles.frag" },
        { GL_NONE, NULL }
    };

    GLuint program = LoadShaders( shaders );
    glUseProgram( program );

    glVertexAttribPointer( vPosition, 2, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPosition );
}

//----------------------------------------------------------------------------
//
// display
//

void
display( void )
{
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glClearBufferfv(GL_COLOR, 0, black);

    glBindVertexArray( VAOs[Triangles] );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------
//
// main
//

#ifdef _WIN32
int CALLBACK WinMain(
  _In_ HINSTANCE hInstance,
  _In_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow
)
#else
int
main( int argc, char** argv )
#endif
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangles", NULL, NULL);

    glfwMakeContextCurrent(window);
    gl3wInit();

    init();

    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
