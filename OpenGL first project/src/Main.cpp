#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>

#include "DrawBoard.h"

#include "vertexbuffer.h"
#include "IndexBuffer.h"
#include "shader.h"



int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "error!" << std::endl;
    }
    
    float width = 0.5f;
    float height = 0.1f;
    float Xoffset = 0.0f;
    float Yoffset = 0.0f;

    float speed = 1.0f / 60.0f;


    float pos[8] =
    {
       -width + Xoffset,-height + Yoffset,
        width + Yoffset,height + Yoffset,
       -width + Xoffset,height + Yoffset,
        width + Xoffset,-height + Yoffset,

    };

    unsigned int indexs[6] =
    {
        0,1,2,
        0,1,3
    };


    // gen vertex array 
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //setup shader
    unsigned int shader = CreateShader();
    glUseProgram(shader);


    //setup uniform
    int location = glGetUniformLocation(shader, "u_color");
    _ASSERT(location != -1);

    //gen vertex buffer
    VertexBuffer vb(pos, 4 * 2 * sizeof(float));


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    //create and save index buffer on gpu
    IndexBuffer ib(indexs, 6,1);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    float currenttime = 0.0f;
    float prevtime = 0.0f;
    float timediff;
    unsigned int counter = 0;
    float currentfps = 0;


    Board test(10,10);
    float testarray[242] = {};
    test.genBoardVertexBuffer(10,10,testarray);
    
    for (int i = 0; i < 2*121; i++) {
        std::cout << i <<":" << testarray[i] << std::endl;
    }
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        float pos[8] = {
           -width + Xoffset,-height + Yoffset,
            width + Xoffset,height + Yoffset,
           -width + Xoffset,height + Yoffset,
            width + Xoffset,-height + Yoffset,
        };

        glUseProgram(shader);

        glUniform4f(location, 0.8f, 0.3f, 0.02f, 1.0f);

        glBindVertexArray(vao);
        
        vb.AddData(pos,4 * 2 * sizeof(float));
        vb.UnBind();
        ib.Bind();


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        //move object
        if (width + Xoffset > 1.0f)
            speed = -1.0f / currentfps;
        else if (width + Xoffset < 0.0f)
            speed = 1.0f / currentfps;

        Xoffset += speed;


        //fps
        currenttime = glfwGetTime();
        timediff = currenttime - prevtime;
        counter++;
        if (timediff >= 1 / 30.0f) {
            //std::cout << counter / timediff << std::endl;
            currentfps = counter / timediff;
            prevtime = currenttime;
            counter = 0;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}