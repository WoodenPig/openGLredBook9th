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
init( void )//顶点数据
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
    /*
n
    Specifies the number of buffer object names to be generated.
buffers
    Specifies an array in which the generated buffer object names are stored.
Description

        glGenBuffers returns n buffer object names in buffers. There is no guarantee that the names form a contiguous set 
      of integers; however, it is guaranteed that none of the returned names was in use immediately before the call to 
      glGenBuffers.

        Buffer object names returned by a call to glGenBuffers are not returned by subsequent calls, unless they are first 
      deleted with glDeleteBuffers.No buffer objects are associated with the returned buffer object names until they are 
      first bound by calling glBindBuffer.
   */
    glBindBuffer( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
    /*
        void glBindBuffer(GLenum target, GLuint buffer);

target
    Specifies the target buffer object. The symbolic constant must be GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 
    GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 
    GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER, 
    GL_TRANSFORM_FEEDBACK_BUFFER, or GL_UNIFORM_BUFFER.
buffer
    Specifies the name of a buffer object.
Description
glBindBuffer binds a buffer object to the specified buffer binding point. Calling glBindBuffer with target​ set to one of 
the accepted symbolic constants and buffer set to the name of a buffer object binds that buffer object name to the target. 
If no buffer object with name buffer exists, one is created with that name. When a buffer object is bound to a target, the 
previous binding for that target is automatically broken.
    */
    //glBufferStorage is API of openGL4.5
    //glBufferStorage( GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices,  GL_STATIC_DRAW);
    /*
        void glBufferData(GLenum target​, GLsizeiptr size​, const GLvoid * data​, GLenum usage​);

target
    Specifies the target buffer object. The symbolic constant must be GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 
    GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 
    GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER, or GL_UNIFORM_BUFFER.

size
    Specifies the size in bytes of the buffer object's new data store.
data
    Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
usage
    Specifies the expected usage pattern of the data store. The symbolic constant must be GL_STREAM_DRAW, GL_STREAM_READ, 
    GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.

Description

glBufferData creates a new data store for the buffer object currently bound to target​. Any pre-existing data store is deleted. 
The new data store is created with the specified size​ in bytes and usage​. If data​ is not NULL, the data store is initialized 
with data from this pointer. In its initial state, the new data store is not mapped, it has a NULL mapped pointer, and its 
mapped access is GL_READ_WRITE.
    */
    
    //*********************************************顶点着色器*****************************************************
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
    /*
        
array
    Specifies the name of the vertex array to bind.

Description

        glBindVertexArray binds the vertex array object with name array​. array​ is the name of a vertex array object 
    previously returned from a call to glGenVertexArrays, or zero to break the existing vertex array object binding.
    If no vertex array object with name array​ exists, one is created when array​ is first bound. If the bind is successful 
    no change is made to the state of the vertex array object, and any previous vertex array object binding is broken.
Errors
    GL_INVALID_OPERATION is generated if array​ is not zero or the name of a vertex array object previously returned from 
    a call to glGenVertexArrays.
   */
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    /*      
mode
    Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, 
    GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY,
    GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
first
    Specifies the starting index in the enabled arrays.
count
    Specifies the number of indices to be rendered. 
    */
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
