#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>
#include <chrono>

#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
//~~
#include "light.h"
#include "heightmap.h"
#include "trianglesurface.h"
#include "interactiveobject.h"

#include "meshgenerator.h"

//~~//



RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }
    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void RenderWindow::init()
{
    mLogger = Logger::getInstance();                                        //Get the instance of the utility Output logger         //Have to do this, else program will crash (or you have to put in nullptr tests...)
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));         //Connect the gameloop timer to the render function:    //This makes our render loop
    //********************** General OpenGL stuff **********************
    if (!mContext->makeCurrent(this))
    {                                     //The OpenGL context has to be set.                     //The context belongs to the instanse of this class!
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }
    if (!mInitialized)                                                      //just to make sure we don't init several times         //used in exposeEvent()
        mInitialized = true;
    initializeOpenGLFunctions();                                            //must call this to use OpenGL functions
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);        //Print render version info (what GPU is used):         //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);
    startOpenGLDebugger();  //Start the Qt OpenGL debugger      //Really helpfull when doing OpenGL     //Supported on most Windows machines - at least with NVidia GPUs    //reverts to plain glGetError() on Mac and other unsupported PCs
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
//    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

//    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mShaderProgram[0] = new Shader("../3Dprog22/plainshader.vert", "../3Dprog22/plainshader.frag");
    mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()) );
    mShaderProgram[1]= new Shader("../3Dprog22/textureshader.vert", "../3Dprog22/textureshader.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()) );
    mShaderProgram[2]= new Shader("../3Dprog22/phongshader.vert", "../3Dprog22/phongshader.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()) );

    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);

    mTexture[0] = new Texture();
    mTexture[1] = new Texture("../3Dprog22/Assets/hund.bmp");
    mTexture[2] = new Texture("../3Dprog22/Assets/doge.bmp");
    mTexture[3] = new Texture("../3Dprog22/Assets/grass.bmp");

    //Set the textures loaded to a texture unit (also called a texture slot)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTexture[2]->id());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mTexture[3]->id());

/*    mPmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "pmatrix" );      // Get the matrixUniform location from the shader
    mVmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "vmatrix" );      // This has to match the "matrix" variable name in the vertex shader
    mMmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );  */     // The uniform is used in the render() function to send the model matrix to the shader
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    mCamera.init(pMatrixUniform0, vMatrixUniform0);
//    mCamera.SetPosition({10, -10, 10});
//    mCamera.lookAt(Player->getPosition());
//    mCamera.setTarget(Player);
//    mCamera.lookAt(QVector3D{0,0,10},QVector3D{0,0,0}, QVector3D{0,1,0});
//    mCamera.translate(2, 5, 0);
//    mCamera.update();

    //~

    setupGameObject(); //Sets up every game object
    mCamera.init();
    mCamera.perspective(40, 4.0/3.0, 0.1, 500.0);
    mCamera.setTarget(Player);

    for (auto i = mObjects.begin(); i != mObjects.end(); i++)
    {
        (*i)->init();
    }

    glUseProgram(mShaderProgram[0]->getProgram());            //what shader to use


    glBindVertexArray(0);       //unbinds any VertexArray - good practice


}

void RenderWindow::setupGameObject()
{
    //~~
//    mXYZ = new VisualObject("XYZ", MeshGenerator::XYZ(100.f));

    mXYZ = new VisualObject("XYZ", MeshGenerator::XYZ(500.f));
    mXYZ->setupShader(mShaderInfo[0]);
    mXYZ->mShaderInfo.TextureID = 0;
    mXYZ->setDrawMethod(EDrawMethod::Lines);
    mObjects.push_back(mXYZ);

    Player = new InteractiveObject("Player", MeshGenerator::Octahedron(1));
    Player->setupShader(mShaderInfo[0]);
    Player->mShaderInfo.TextureID = 0;
    Player->setDrawMethod(EDrawMethod::Triangles);
    mObjects.push_back (Player);

//    Plane = new VisualObject("Plane", MeshGenerator::Plane(100.f));
//    Plane->setupShader(mShaderInfo[2]);
//    Plane->mShaderInfo.TextureID = 3;
//    Plane->setDrawMethod(EDrawMethod::Triangles);
//    mObjects.push_back (Plane);

    //Oppgave 2
    HeightmapGround = new Heightmap();
    HeightmapGround->setName("Heightmap");
    HeightmapGround->setupShader(mShaderInfo[2]);
    HeightmapGround->mShaderInfo.TextureID = 3;
    HeightmapGround->setDrawMethod(EDrawMethod::Triangles);
    mObjects.push_back (HeightmapGround);

    //Oppgave 2
    mLight = new Light();
    mLight->setupShader(mShaderInfo[2]);
    mLight->mShaderInfo.TextureID = 3;
    mLight->setDrawMethod(EDrawMethod::Triangles);
    mLight->move(.1f, .1f, 5.f);
    mObjects.push_back (mLight);


}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform0 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );

    ShaderInfo tempShaderInfo;
    tempShaderInfo.MatrixUniform            = &mMatrixUniform0;
    tempShaderInfo.Shader                   = mShaderProgram[shaderIndex];
    tempShaderInfo.ProjectionMatrixUniform  = &pMatrixUniform0;
    tempShaderInfo.ViewMatrixUniform        = &vMatrixUniform0;
//    tempShaderInfo.TextureID                = shaderIndex;

    mShaderInfo[shaderIndex]                = tempShaderInfo;
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1     = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform1     = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform1     = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform1    = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "textureSampler");


    ShaderInfo tempShaderInfo;
    tempShaderInfo.MatrixUniform            = &mMatrixUniform1;
    tempShaderInfo.Shader                   = mShaderProgram[shaderIndex];
    tempShaderInfo.ProjectionMatrixUniform  = &pMatrixUniform1;
    tempShaderInfo.ViewMatrixUniform        = &vMatrixUniform1;
    tempShaderInfo.TextureUniform           = &mTextureUniform1;
//    tempShaderInfo.TextureID                = shaderIndex;


    mShaderInfo[shaderIndex]                = tempShaderInfo;
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2                 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "mMatrix" );
    vMatrixUniform2                 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "vMatrix" );
    pMatrixUniform2                 = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "pMatrix" );
    mTextureUniform2                = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "textureSampler");

    mLightColorUniform              = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightColor" );
    mObjectColorUniform             = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "objectColor" );
    mAmbientLightStrengthUniform    = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "ambientStrengt" );
    mLightPositionUniform           = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPosition" );
    mSpecularStrengthUniform        = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularStrength" );
    mSpecularExponentUniform        = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "specularExponent" );
    mLightPowerUniform              = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "lightPower" );
    mCameraPositionUniform          = glGetUniformLocation( mShaderProgram[shaderIndex]->getProgram(), "cameraPosition" );

    ShaderInfo tempShaderInfo;
    tempShaderInfo.MatrixUniform            = &mMatrixUniform2;
    tempShaderInfo.Shader                   = mShaderProgram[shaderIndex];
    tempShaderInfo.ProjectionMatrixUniform  = &pMatrixUniform2;
    tempShaderInfo.ViewMatrixUniform        = &vMatrixUniform2;
    tempShaderInfo.TextureUniform           = &mTextureUniform2;

//    tempShaderInfo.TextureID                = shaderIndex;
    mShaderInfo[shaderIndex]                = tempShaderInfo;
}

void RenderWindow::render()
{
    mTimeStart.restart();                                   //restart FPS clock
    mContext->makeCurrent(this);                            //must be called every frame (every time mContext->swapBuffers is called)
    initializeOpenGLFunctions();                            //must call this every frame it seems...
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //clear the screen for each redraw
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //~~ Tick
    auto time1 = std::chrono::system_clock::now();
    static auto time2 = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration = time1 - time2;
    time2 = std::chrono::system_clock::now();
    Tick(duration.count() / 1000);

//    glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),mLight->mMatrix.column(3).z());
//    glUniform3f(mCameraPositionUniform, mCamera.GetPosition().x(), mCamera.GetPosition().y(), mCamera.GetPosition().z());
//    glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
//    glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrenght);



    //~~ Draw
    for (auto i = mObjects.begin(); i != mObjects.end(); i++)
    {
        if ((*i)->mShaderInfo.TextureUniform != nullptr)
        {
            glUniform1i(*(*i)->mShaderInfo.TextureUniform, (*i)->mShaderInfo.TextureID);
        }
        if((*i)->mShaderInfo.Shader == mShaderInfo[0].Shader)
        {
            std::cout << (*i)->getName() << "'s Shader ID = 0" << std::endl;

            glUseProgram((*i)->mShaderInfo.Shader->getProgram());

            glUniformMatrix4fv(*(*i)->mShaderInfo.MatrixUniform, 1, GL_FALSE, (*i)->mMatrix.constData());
//                glUseProgram((*i)->mShaderInfo.Shader->getProgram());

            mCamera.update(*(*i)->mShaderInfo.ProjectionMatrixUniform, *(*i)->mShaderInfo.ViewMatrixUniform);
            (*i)->draw();
        }
        else if((*i)->mShaderInfo.Shader == mShaderInfo[1].Shader)
        {
            std::cout << (*i)->getName() << "'s Shader ID = 1" << std::endl;

            glUseProgram((*i)->mShaderInfo.Shader->getProgram());

            glUniformMatrix4fv(*(*i)->mShaderInfo.MatrixUniform, 1, GL_FALSE, (*i)->mMatrix.constData());
//                glUseProgram((*i)->mShaderInfo.Shader->getProgram());

            glUniform1i(*(*i)->mShaderInfo.TextureUniform, (*i)->mShaderInfo.TextureID);

//                glActiveTexture(GL_TEXTURE1);

            mCamera.update(*(*i)->mShaderInfo.ProjectionMatrixUniform, *(*i)->mShaderInfo.ViewMatrixUniform);
            (*i)->draw();
        }
        else if((*i)->mShaderInfo.Shader == mShaderInfo[2].Shader)
        {

            std::cout << (*i)->getName() << "'s Shader ID = 2" << std::endl;

            glUseProgram((*i)->mShaderInfo.Shader->getProgram());
            std::cout << (*i)->getName() << "| 001" << std::endl;

            mCamera.update(*(*i)->mShaderInfo.ProjectionMatrixUniform, *(*i)->mShaderInfo.ViewMatrixUniform);
            std::cout << (*i)->getName() << "| 002" << std::endl;

            glUniformMatrix4fv(*(*i)->mShaderInfo.MatrixUniform, 1, GL_FALSE, (*i)->mMatrix.constData());
            std::cout << (*i)->getName() << "| 003" << std::endl;

            std::cout << (*i)->getName() << "| 004" << std::endl;


            glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),mLight->mMatrix.column(3).z());
            std::cout << (*i)->getName() << "| 005" << std::endl;
            glUniform3f(mCameraPositionUniform, mCamera.GetPosition().x(), mCamera.GetPosition().y(), mCamera.GetPosition().z());
            std::cout << (*i)->getName() << "| 006" << std::endl;
            glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
            std::cout << (*i)->getName() << "| 007" << std::endl;
            glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrenght);
            std::cout << (*i)->getName() << "| 008" << std::endl;


            (*i)->draw();
            std::cout << (*i)->getName() << "| 009" << std::endl;

        }
        else
        {
            std::cout << "Shader ID invalid" << std::endl;
        }

    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    calculateFramerate();                                   // Calculate framerate
//    checkForGLerrors();                                     //  using our expanded OpenGL debugger to check if everything is OK.
    mContext->swapBuffers(this);                            //Qt require us to call this swapBuffers() -function. | swapInterval is 1 by default which means that swapBuffers() will (hopefully) block and wait for vsync.
}

void RenderWindow::exposeEvent(QExposeEvent *)
{
    //This function is called from Qt when window is exposed (shown)
    // and when it is resized
    //exposeEvent is a overridden function from QWindow that we inherit from
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop == the render()-function
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}
void RenderWindow::calculateFramerate()
{
    //The way this function is set up is that we start the clock before doing the draw call,
    // and check the time right after it is finished (done in the render function)
    //This will approximate what framerate we COULD have.
    //The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};

    if (mMainWindow)
    {
        ++frameCount;
        if (frameCount > 30)
        {
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;
        }
    }
}
void RenderWindow::checkForGLerrors()
{
    //Uses QOpenGLDebugLogger if this is present
    //Reverts to glGetError() if not
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}
void RenderWindow::startOpenGLDebugger()
{
    //Tries to start the extended OpenGL debugger that comes with Qt
    //Usually works on Windows machines, but not on Mac...
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    mCurrentInputs[event->button()] = true;
//    if (event->button() == Qt::LeftButton) {
//        mCamera.setStatus(true);
//    }
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mCurrentInputs[event->button()] = false;
//    if (event->button() == Qt::LeftButton) {
//        mCamera.setStatus(false);
//    }
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    static float previousX = event->position().x();
    static float previousY = event->position().y();

    float newX = event->position().x();
    float newY = event->position().y();

    float diffX = previousX - newX;
    float diffY = previousY - newY;

    if (mCurrentInputs[Qt::LeftButton]) {
        mCamera.FollowMouseMovements(diffX, diffY);
    }

    previousX = newX;
    previousY = newY;
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    mCurrentInputs[event->key()] = true;



    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program


    }
    if (event->key() == Qt::Key_T)
    {
//        std::unordered_map<int, bool>::iterator inputIterator;

//        for (inputIterator = mCurrentInputs.begin(); inputIterator != mCurrentInputs.end(); inputIterator++)
//        {
//            std::cout << (char)(inputIterator->first) << " : " << inputIterator->second << std::endl;
//        }

//        for (auto const& [key, val] : mCurrentInputs)
//        {
//            std::cout << key << " : " << val << std::endl;
//        }
    }


}
void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
//    mCurrentInputs.erase(event->key());
    mCurrentInputs[event->key()] = false;

//    std::unordered_map<int, bool>::iterator inputIterator;

//    for (inputIterator = mCurrentInputs.begin(); inputIterator != mCurrentInputs.end(); inputIterator++)
//    {
//        std::cout << inputIterator->first << " : " << inputIterator->second << std::endl;
//    }

//    for (auto const& [key, val] : mCurrentInputs)
//    {
//        std::cout << key << " : " << val << std::endl;
//    }
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
   mCamera.Zoom(event->angleDelta().y());

}
void RenderWindow::Tick(float deltaTime)
{
    mCamera.Tick(deltaTime);


    if(Player && mLight)
    {
        mLight->mMatrix.translate({0.1f, 0, 0});
        mLight->mMatrix.rotate(1, {0, 0, 1});
        mLight->mMatrix.setToIdentity();

        QVector3D playerPos = Player->getPosition();
        playerPos.setZ(playerPos.z()+10.f);
        mLight->mMatrix.translate(playerPos);
    }


    if (Player)
    {
        if (mCurrentInputs[Qt::Key_W])
        {
            Player->MoveForward(1);
        }
        if (mCurrentInputs[Qt::Key_S])
        {
            Player->MoveForward(-1);
        }
        if (mCurrentInputs[Qt::Key_D])
        {
            Player->MoveRight(-1);
        }
        if (mCurrentInputs[Qt::Key_A])
        {
            Player->MoveRight(1);
        }
        if (mCurrentInputs[Qt::Key_E])
        {
            Player->move(0.f, 0.f, 1.f);
        }
        if (mCurrentInputs[Qt::Key_Q])
        {
            Player->move(0.f, 0.f, -1.f);
        }
    }
//    mCamera.translate(1.f, 0, 0);

//    static float deletemeTest = 0.0f;
//    deletemeTest -= .01f;

//    mCamera.SetPosition(QVector3D{deletemeTest,deletemeTest, deletemeTest});

//    mCamera.translate(0,0,deletemeTest);
}