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

#include "raindrop.h"
#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
//~~
#include "trophy.h"
#include "vertex.h"
#include "sun.h"
#include "light.h"
#include "heightmap.h"
#include "trianglesurface.h"
#include "interactiveobject.h"

#include "meshgenerator.h"
//~~
//#include "soundmanager.h"
//#include "soundsource.h"

#include <chrono>
#include <thread>

#include "regulartriangulation.h"

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Oppgave 5 // Primært kamera brukes til Play mode
    setupGameObject();
//    mCamera.init();
//    mCamera.perspective(40, 4.0/3.0, 0.1, 1000.0);
//    mCamera.setTarget(mPlayer);
//    mCamera.SetPosition(mPlayer->getPosition() + QVector3D(0.f, 0.f, 50.f));

    // Oppgave 5 // Sekundært kamera brukes til Editor Mode,
    mCamera2.init();
    mCamera2.perspective(40, 4.0/3.0, 0.1, 1000.0);
    mCamera2.setIsEditorCamera(false);
    mCamera2.setTarget(mEditorModeTarget);
    mCamera2.SetPosition(mEditorModeTarget->getPosition() + QVector3D(0.f, 0.f, 60.f));

    mCurrentCamera = &mCamera2;




    Triangulation = new RegularTriangulation("Arnes4.txt");

//    mRegularSurface->setPosition(mSceneOrigo);
    Triangulation->setupShader(mShaderInfo[2]);
    Triangulation->mShaderInfo.TextureID = 3;
//    mRegularSurface->setDrawMethod(EDrawMethod::Triangles);

    mObjects.push_back(Triangulation);



    for (auto i = mObjects.begin(); i != mObjects.end(); i++)
    {
        (*i)->init();
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    mLaserSound = SoundManager::getInstance()->createSource
//            ("Laser", QVector3D(0.0f, 0.0f, 0.0f),"../3DProg22/Assets/laser.wav", false, 0.7f);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////












    glUseProgram(mShaderProgram[0]->getProgram());
    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

void RenderWindow::setupGameObject()
{
    // Hvert object instansieres her med en ShaderInfo "pakke" som siden..
    // blir brukt til å hente ut relevant informasjon for hvordan objektet skal rendres.
    //
    // ShaderInfo pakkene blir laget i setupShader()-funksjonene over.
    //
    // ShaderInfo struct ligger i VisualObject

    // Oppgave 5 // Eksempel på editorOnly object som bare rendres i editor mode.

//    mRegularSurface = new RegularSurface("MapSurface", MeshGenerator::TriangleSurfaceReadTxt("../3Dprog22/txt_files/test.txt"));

//    mRegularSurface->mMatrix.translate(-635757, -6667498, -133);
//    mRegularSurface->setPosition(mSceneOrigo);
//    mRegularSurface->setupShader(mShaderInfo[0]);
//    mRegularSurface->mShaderInfo.TextureID = 0;
//    mRegularSurface->setDrawMethod(EDrawMethod::Points);
//    mObjects.push_back(mRegularSurface);


//    mXYZ = new VisualObject("XYZ", MeshGenerator::XYZ(500.f));
//    mXYZ->setupShader(mShaderInfo[0]);
//    mXYZ->mShaderInfo.TextureID = 0;
//    mXYZ->setDrawMethod(EDrawMethod::Lines);
//    mXYZ->bEditorOnlyRender = true;
//    mXYZ->setPosition(mSceneOrigo);
//    mObjects.push_back(mXYZ);

    // Oppgave 4
//    mPlayer = new InteractiveObject("Player", MeshGenerator::OBJFile("../3Dprog22/Assets/monkey.obj"));
//    mPlayer->setupShader(mShaderInfo[2]);
//    mPlayer->setPosition(250.f, 250.f, 10.f);
//    mPlayer->mShaderInfo.TextureID = 2;
//    mPlayer->setDrawMethod(EDrawMethod::Triangles);
//    mPlayer->setScale(3.f);
//    mPlayer->bShouldBeRendered = true;
//    mObjects.push_back (mPlayer);



    // Oppgave 5 & 6
    mEditorModeTarget = new InteractiveObject("EditorModeTarget", MeshGenerator::OBJFile("../3Dprog22/Assets/camera.obj"));
    mEditorModeTarget->setupShader(mShaderInfo[2]);
    mEditorModeTarget->setPosition(250.f, 250.f, 10.f);
    mEditorModeTarget->mShaderInfo.TextureID = 2;
    mEditorModeTarget->setDrawMethod(EDrawMethod::Triangles);
    mEditorModeTarget->bEditorOnlyRender = true;
    mEditorModeTarget->bShouldBeRendered = false;

    mObjects.push_back (mEditorModeTarget);

    // Oppgave 2 // Følger oppgave-tall logikk der oppgave 1 er å lage et tomt repo.
    HeightmapGround = new Heightmap("../3Dprog22/Assets/EksamenHeightmap.bmp", 40, 1.f, 1);
    HeightmapGround->setName("Heightmap");
    HeightmapGround->setupShader(mShaderInfo[2]);
    HeightmapGround->mShaderInfo.TextureID = 3;
    HeightmapGround->setDrawMethod(EDrawMethod::Triangles);

    HeightmapGround->bShouldBeRendered = false;

    mObjects.push_back (HeightmapGround);

    // Oppgave 3
    mSun = new Sun();
    mSun->setVertices(MeshGenerator::Octahedron(1));
    mSun->setName("Sun");
    mSun->setMonoColor(QVector3D(1.f, 1.f, 0.f));
    mSun->setupShader(mShaderInfo[0]);
    mSun->mShaderInfo.TextureID = 0;
    mSun->setDrawMethod(EDrawMethod::Triangles);
    mSun->mLightStrenght = 50000.f;
//    mSun->bShouldBeRendered = false;
    mObjects.push_back (mSun);

    //Oppgave 7
//    mBezierCurve = new VisualObject("BezierCurve", MeshGenerator::FourPointBezierCurve(Vertex(10,10,10,1,0,1),
//                                                                                       Vertex(30,20,10,1,0,1),
//                                                                                       Vertex(40,10,10,1,0,1),
//                                                                                       Vertex(50,30,10,1,0,1)));
//    mBezierCurve->setupShader(mShaderInfo[0]);
//    mBezierCurve->mShaderInfo.TextureID = 0;
//    mBezierCurve->setDrawMethod(EDrawMethod::Lines);
//    mBezierCurve->setPosition(QVector3D(250.f, 250.f, 20.f));
//    mObjects.push_back(mBezierCurve);

    //Oppgave 7
    mNPC1 = new NPC("NPC1", MeshGenerator::Cube());
    mNPC1->setupShader(mShaderInfo[0]);
    mNPC1->mShaderInfo.TextureID = 0;
    mNPC1->setPatrolPathObject(mBezierCurve);
    mObjects.push_back(mNPC1);

    //Oppgave 8
//    bool bRedTeam = true;
//    for (int i = 0; i < 2; i++)
//    {
//        for (int i = 0; i < 10; i++)
//        {
//            mTrophy = new Trophy(bRedTeam);
//            mTrophy->setupShader(mShaderInfo[0]);
//            mTrophy->mShaderInfo.TextureID = 0;
//            std::string name = "trophy" + std::to_string(i);
//            mTrophy->setName(name);
//            float randX = (rand() % 100) + mSceneOrigo.x();
//            float randY = (rand() % 100) + mSceneOrigo.y();
//            mTrophy->move(randX, randY, 5.f);
//            mObjects.push_back(mTrophy);
//            mTrophies.push_back(mTrophy);
//        }
//        bRedTeam = false;
//    }


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

    //~~ Draw // Har satt opp her en loop som sjekker hvilken shader hvert object er spesifisert at skal bruke
    for (auto i = mObjects.begin(); i != mObjects.end(); i++)
    {
        if((*i)->bShouldBeRendered == true)
        {
            if((*i)->mShaderInfo.Shader == mShaderInfo[0].Shader)
            {
                glUseProgram((*i)->mShaderInfo.Shader->getProgram());
                glUniformMatrix4fv(*(*i)->mShaderInfo.MatrixUniform, 1, GL_FALSE, (*i)->mMatrix.constData());
                mCurrentCamera->update(*(*i)->mShaderInfo.ProjectionMatrixUniform, *(*i)->mShaderInfo.ViewMatrixUniform);
                (*i)->draw();
            }
            else if((*i)->mShaderInfo.Shader == mShaderInfo[1].Shader)
            {
                glUseProgram((*i)->mShaderInfo.Shader->getProgram());
                glUniformMatrix4fv(*(*i)->mShaderInfo.MatrixUniform, 1, GL_FALSE, (*i)->mMatrix.constData());
                glUniform1i(*(*i)->mShaderInfo.TextureUniform, (*i)->mShaderInfo.TextureID);
                mCurrentCamera->update(*(*i)->mShaderInfo.ProjectionMatrixUniform, *(*i)->mShaderInfo.ViewMatrixUniform);
                (*i)->draw();
            }
            else if((*i)->mShaderInfo.Shader == mShaderInfo[2].Shader)
            {
                glUseProgram((*i)->mShaderInfo.Shader->getProgram());
                glUniformMatrix4fv(*(*i)->mShaderInfo.MatrixUniform, 1, GL_FALSE, (*i)->mMatrix.constData());
                glUniform1i(*(*i)->mShaderInfo.TextureUniform, (*i)->mShaderInfo.TextureID);
                glUniform3f(mLightPositionUniform, mSun->mMatrix.column(3).x(), mSun->mMatrix.column(3).y(),mSun->mMatrix.column(3).z());
                glUniform3f(mCameraPositionUniform, mCamera.GetPosition().x(), mCamera.GetPosition().y(), mCamera.GetPosition().z());
                glUniform3f(mLightColorUniform, mSun->mLightColor.x(), mSun->mLightColor.y(), mSun->mLightColor.z());
                glUniform1f(mSpecularStrengthUniform, mSun->mSpecularStrenght);

                mCurrentCamera->update(*(*i)->mShaderInfo.ProjectionMatrixUniform, *(*i)->mShaderInfo.ViewMatrixUniform);
                (*i)->draw();
            }
            else
            {
                std::cout << "Shader ID invalid" << std::endl;
            }
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
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mCurrentInputs[event->button()] = false;
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
        mCurrentCamera->FollowMouseMovements(diffX, diffY);
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
        std::cout << "RenderWindow::keyPressEvent() | Key_T" << std::endl;
//        mLaserSound->play();


        float randX = (rand() % 200) + 125.0f;
        float randY = (rand() % 200) + 100.0f;
        float randZ = (rand() % 30) + 60.0f;

        Raindrop* NewRaindrop = new Raindrop(QVector3D(randX, randY, randZ), 1, Triangulation, 1);

//        NewRaindrop->setVertices(MeshGenerator::Octahedron(8));
        NewRaindrop->init();
        NewRaindrop->setupShader(mShaderInfo[0]);
        NewRaindrop->mShaderInfo.TextureID = 0;
//        NewBall->move(0,0,50);

//        NewRaindrop->setPosition(250.f, 200.f, 40.f);


//        NewBall->
        Raindrops.push_back(NewRaindrop);
        mObjects.push_back(NewRaindrop);


    }
    if (event->key() == Qt::Key_Y)
    {
//        std::cout << "Key_Y" << std::endl;
        toggleEditorMode();
    }
}
void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    mCurrentInputs[event->key()] = false;
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
   mCurrentCamera->Zoom(event->angleDelta().y());
}

void RenderWindow::Tick(float deltaTime)
{
    mCurrentCamera->Tick(deltaTime);

    for (auto i = Raindrops.begin(); i != Raindrops.end(); i++)
    {
        (*i)->TickRaindrop(deltaTime);
    }

//    SoundManager::getInstance()->updateListener(QVector3D(0.f, 0.f, 0.f),{0,0,0}, {1,0,0}, {0,0,1});

//    mLaserSound->setPosition(QVector3D(0.f,0.f,0.f));
//    mLaserSound->play();


    // Oppgave 3
    if (mSun)
    {
//        mSun->progressOrbit();
    }
    // Oppgave 7
    if (mNPC1)
    {
        mNPC1->patrol();
    }

    // Oppgave 2
    if(mPlayer)
    {
        QVector3D playerPos = mPlayer->getPosition();
        mPlayer->setHeight(3+HeightmapGround->GetHeight(playerPos.x(), playerPos.y()));
    }

    if (mPlayer && mCurrentCamera == &mCamera)
    {
        if (mCurrentInputs[Qt::Key_W])
        {
            mPlayer->MoveForwardLocal(1.f);
        }
        if (mCurrentInputs[Qt::Key_S])
        {
            mPlayer->MoveForwardLocal(-1.f);
        }
        if (mCurrentInputs[Qt::Key_D])
        {
            mPlayer->RotateRight(.4f);
        }
        if (mCurrentInputs[Qt::Key_A])
        {
            mPlayer->RotateRight(-.4f);
        }
    }
    else if (mEditorModeTarget && mCurrentCamera == &mCamera2)
    {
        if (mCurrentInputs[Qt::Key_W])
        {
            mEditorModeTarget->move(1.f, 0.f, 0.f);
        }
        if (mCurrentInputs[Qt::Key_S])
        {
            mEditorModeTarget->move(-1.f, 0.f, 0.f);
        }
        if (mCurrentInputs[Qt::Key_D])
        {
            mEditorModeTarget->move(0.f, -1.f, 0.f);
        }
        if (mCurrentInputs[Qt::Key_A])
        {
            mEditorModeTarget->move(0.f, 1.f, 0.f);
        }
        if (mCurrentInputs[Qt::Key_E])
        {
            mEditorModeTarget->move(0.f, 0.f, 1.f);
        }
        if (mCurrentInputs[Qt::Key_Q])
        {
            mEditorModeTarget->move(0.f, 0.f, -1.f);
        }
    }

    // Oppgave 8 // Trofeer som blir plukket opp blir satt til å ikke rendre, og win condition checkes også.
    static float secondCounter;
    secondCounter += .2f;
    if (secondCounter > 1.f)
    {
        secondCounter = 0;

        if(mPlayer)
        {
            QVector3D tempPlayerLoc = mPlayer->getPosition();
            for(int i = 0; i < mTrophies.size(); i++)
            {
                if(mTrophies.at(i)->getIsRed() && mTrophies.at(i)->getIsTaken() == false)
                {
                    QVector3D tempTrophyLoc = mTrophies.at(i)->getPosition();
                    if (
                            (tempPlayerLoc.x() - tempTrophyLoc.x() < 3.f && (tempPlayerLoc.x() - tempTrophyLoc.x() > -3.f))
                         && (tempPlayerLoc.y() - tempTrophyLoc.y() < 3.f && (tempPlayerLoc.y() - tempTrophyLoc.y() > -3.f))
                        )
                    {
                        mTrophies.at(i)->bShouldBeRendered = false;
                        mTrophies.at(i)->setIsTaken(true);
                        mNumberOfTrophiesGathered++;
                        if(mNumberOfTrophiesGathered > 9)
                        {
                            std::cout << "YOU WIN!!!" << std::endl;
                            mLogger->logText("YOU WIN!!!");
                        }
                    }
                }
            }
        }
    }
}

void RenderWindow::toggleEditorMode()
{
    bIsEditorModeEnabled = !bIsEditorModeEnabled;
    for (auto i = mObjects.begin(); i != mObjects.end(); i++)
    {
        (*i)->toggleEditorMode();
    }

    if(mCurrentCamera == &mCamera)
    {
        mCurrentCamera = &mCamera2;
    }
    else if (mCurrentCamera == &mCamera2)
    {
        mCurrentCamera = &mCamera;
    }

//    mLaserSound->play();


}
