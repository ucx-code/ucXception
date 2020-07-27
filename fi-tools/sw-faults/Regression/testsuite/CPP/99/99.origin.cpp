
// Projeto CG
//Diogo Falcao
//Joao Pedro
//Pedro Reis

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <string.h>
#include "RgbImage.h"
#include <sstream>
#include <string>
#include <iostream>
#include <vector>  //for std::vector
#include "materiais.h"

#define AZUL 0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define VERDE 0.0, 1.0, 0.0, 1.0
#define BRANCO 1.0, 1.0, 1.0, 1.0
#define PRETO 0.0, 0.0, 0.0, 1.0

#define NUM_PARTICLES    1000          /* Number of particles  */
#define NUM_DEBRIS       70            /* Number of debris     */

#define NUM_PARTICLES_CHUVA 10000

void desenhaPontosMenu(char *string);
void iniciarPrograma();
struct enemy
{
  float position[3];
  int   show; //0-no ecra    1-destruido
  int   fuel;
};
std::vector<enemy> enemysList;

/****
*****variaveis sitema de particulas
*****/
struct particleData
{
  float   position[3];
  float   speed[3];
  float   color[3];
};
typedef struct particleData    particleData;

struct debrisData
{
  float   position[3];
  float   speed[3];
  float   orientation[3];        /* Rotation angles around x, y, and z axes */
  float   orientationSpeed[3];
  float   color[3];
  float   scale[3];
};
typedef struct debrisData    debrisData;

particleData    particles[NUM_PARTICLES];
debrisData      debris[NUM_DEBRIS];
//int             fuel = 0; /* "fuel" of the explosion */


/****
*****variaveis som
*****/
ALuint  Buffer;
ALuint  BufferEx;
ALuint  Source;// Sources are points of emitting sound.
ALuint  SourceEx;// Sources are points of emitting sound.
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };// Position of the source sound.
ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };// Velocity of the source sound.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };// Position of the Listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };// Velocity of the Listener.
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };// Orientation of the Listener. (first 3 elements are "at", second 3 are "up"), Also note that these should be units of '1'.
ALfloat SourcePosEx[] = { 0.0, 0.0, 0.0 };// Position of the source sound.
ALfloat SourceVelEx[] = { 0.0, 0.0, 0.0 };// Velocity of the source sound.
ALfloat ListenerPosEx[] = { 0.0, 0.0, 0.0 };// Position of the Listener.
ALfloat ListenerVelEx[] = { 0.0, 0.0, 0.0 };// Velocity of the Listener.
ALfloat ListenerOriEx[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };// Orientation of the Listener. (first 3 elements are "at", second 3 are "up"), Also note that these should be units of '1'.


/****
*****variaveis opengl
*****/
GLint largura = 500, altura = 500;

GLuint      texture[15];
GLuint      skybox[6];
RgbImage    imag;
RgbImage    imagS;

GLfloat PI = 3.14159f;
GLfloat rVisao = 1500, aVisao = 0.5f * PI, incVisao = 20.0f;

float pos_seta_y = 4.3;
float pos_seta_som_y = -0.4;
float pos_seta_robot_y = -4.1;

int som = 1; // 0 - som off;   1 - som on
int op_wc =0;
int op_tec = 0;
int op_competi = 0;
int op_espacial = 0;
int inicio = 1;
int opcoes = 0;
int pontos = 0;
int instrucoes = 0;
int acerca = 0;
int cenario = 1; // 1 - Dia; 2 - Chuva; 3 - Nevoeiro
int iniciarjogo = 0; //1-jogo iniciado     0-nao iniciado
int pausegame = 1; //1-jogo a correr     -1-jogo pausado
int electroDest = 1;
int op_robot = 1;  // 1-zinco; 2-bronze; 3-mercurio

GLint   anguloCube = 0;
GLfloat move_cha = 0.0;
GLint   angB = -45;
GLint   angR = 0;
GLint   msecDelay=1;
GLint   msecDelayRobot=2;
GLint   enemyDelay = 2000; //2sec
GLint   enemyCreateDelay = 50; //2sec
GLfloat pastX = 0.0;
GLfloat pastZ = 0.0;
GLint   delay_cube = 100;
GLint   delay_cha = 2;
GLfloat torusin = -10.0;
GLfloat torusinDir = -1;
GLfloat torusout = -12.0;
GLfloat torusoutDir = 1;
GLfloat robotZ = 0.0;
GLfloat robotX = 0.0;
GLfloat robotY = 0.0;
GLfloat lastRobotX = 0.0;
GLfloat lastRobotZ = 0.0;
GLfloat avancoMira = 30.0;
GLfloat robotYDir = 1;
GLfloat cha_dir = 1.0;
GLfloat luzGlobalCor[4]={1.0,1.0,1.0,1.0};
GLfloat matAmbiente[] = {1.0,1.0,1.0,1.0};
GLfloat matDifusa[]   = {1.0,1.0,1.0,1.0};
GLfloat matEspecular[]= {1.0, 1.0, 1.0, 1.0};
GLint   especMaterial = 20;
GLint   disparo = 0;

int   health = 100;
int dead=0;
int score=0;
char *textoPontos;

GLfloat obsPini[] = {1.0f, 1.0f, 20.0f };
GLfloat obsPfin[] = {obsPini[0] - rVisao * (GLfloat) cos(aVisao), obsPini[1], obsPini[2] - rVisao * (GLfloat) sin(aVisao) };
GLint   CAMERA = -1;


GLfloat Pos1[]= {-49.0f, 10.0f,  -15.0f, 1.0f };
GLfloat Pos2[]= { -49.0f, 10.0f,  15.0f, 1.0f };
bool    Focos[] = {1,1};        //.. Dois Focos ligados ou desligados
GLfloat anguloFoco= 30.0;       //.. angulo inicial do foco
GLfloat anguloINC = 3.0;        //.. incremento
GLfloat anguloMIN = 3.0;        //.. minimo
GLfloat anguloMAX = 70.0;       //.. maximo






/****** CENAS CHUVA e nevoeiro******/
GLfloat     fogColor[]              =   {0.2, 0.2, 0.2, 1.0};
void initNevoeiro(void);
void desenhaChuva();
float slowdown = 1.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

typedef struct {
    // Life
    bool alive; // is the particle alive?
    float life; // particle lifespan
    float fade; // decay
    // color
    float red;
    float green;
    float blue;
    // Position/direction
    float xpos;
    float ypos;
    float zpos;
    // Velocity/Direction, only goes down in y dir
    float vel;
    // Gravity
    float gravity;
}particles2;

// Paticle System
particles2 par_sys[NUM_PARTICLES_CHUVA];


//funcao de limpeza para reiniciar o jogo
void clear(){
    iniciarjogo=0;
    inicio=1;
    enemysList.clear();
    robotX=0;
    robotZ=0;
    robotY=0;
    angR=0;
    health=100;
    score=0;
    dead=0;
    avancoMira=30.0;
    torusin = -10.0;
    torusinDir = -1;
    torusout = -12.0;
    torusoutDir = 1;

    rVisao = 1500;
    aVisao = 0.5f * PI;
    incVisao = 20.0f;
    angB = -45;
    pastX = 0.0;
    pastZ = 0.0;
    lastRobotX = 0.0;
    lastRobotZ = 0.0;
    robotYDir = 1;
    disparo = 0;
    CAMERA = -1;

    iniciarPrograma();

}

void iniciaParticulasChuva(int i) {
        par_sys[i].alive = true;
        par_sys[i].life = 3.0;
        par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;

        par_sys[i].xpos = (float) (rand() % 50) - 10;
        par_sys[i].ypos = 50.0;
        par_sys[i].zpos = (float) (rand() % 50 ) - 10;

        par_sys[i].red = 0.5;
        par_sys[i].green = 0.5;
        par_sys[i].blue = 1.0;

        par_sys[i].vel = velocity;
        par_sys[i].gravity = -0.8;//-0.8;

}



void init( ) {
    int x, z;

    //glShadeModel(GL_SMOOTH);
    //if(cenario==3)
    glEnable(GL_FOG);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

    // Ground Verticies
    // Ground Colors
    for (z = 0; z < 21; z++) {
        for (x = 0; x < 21; x++) {
            ground_points[x][z][0] = x - 10.0;
            ground_points[x][z][1] = accum;
            ground_points[x][z][2] = z - 10.0;

            ground_colors[z][x][0] = r; // red value
            ground_colors[z][x][1] = g; // green value
            ground_colors[z][x][2] = b; // blue value
            ground_colors[z][x][3] = 0.0; // acummulation factor
        }
    }


    // Initialize particles
    for (loop = 0; loop < NUM_PARTICLES_CHUVA; loop++) {
        iniciaParticulasChuva(loop);
    }

}




void desenhaChuva() {
    int loop;
    float x, y, z;
    for (loop = 0; loop < NUM_PARTICLES_CHUVA; loop=loop+2) {
        //printf("Entra %d\n",loop);
        if (par_sys[loop].alive == true) {
            //printf("Entra %d\n",loop);
            x = par_sys[loop].xpos;
            y = par_sys[loop].ypos;
            z = par_sys[loop].zpos + zoom;

            // Draw particles


            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cyanPlasticAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cyanPlasticDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  cyanPlasticSpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS,  cyanPlasticCoef);

            //glColor3f(0.5, 0.5, 1.0);
            glBegin(GL_LINES);
                glVertex3f(x, y, z);
                glVertex3f(x, y+0.5, z);
            glEnd();

            // Update values
            //Move
            // Adjust slowdown for speed!
            par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
            par_sys[loop].vel += par_sys[loop].gravity;
            // Decay
            par_sys[loop].life -= par_sys[loop].fade;

            if (par_sys[loop].ypos <= -10) {
                par_sys[loop].life = -1.0;
            }
            //Revive
            if (par_sys[loop].life < 0.0) {
                iniciaParticulasChuva(loop);
            }
        }
    }
}

void initNevoeiro(void){
     glFogfv(GL_FOG_COLOR, fogColor);
     glFogi (GL_FOG_MODE, GL_EXP);          //Temos que usar este modo de forma a controlarmosa densidade (dando o efeito do fumo a aumentar enquanto o cigarro queima)
    glFogf(GL_FOG_START, 0);                //Distancia a que se começa a ver o nevoeiro.
    glFogf(GL_FOG_END, 50);             //Distancia ate ao fim do nevoeiro.
     glHint(GL_FOG_HINT, GL_NICEST);            //Define cor do nevoeiro.
     glFogf (GL_FOG_DENSITY, 0.016);        //Definimos a densidade, em GL_LINEAR nao se aplica.

                            //Ativa nevoeiro.

        //Fog
    glEnable(GL_FOG);


}

/* ********************************
 * FUNCOES OPENAL REFERENTES AO AUDIO DO JOGO
 * ****************************** */

ALboolean LoadALData()
{

    int i;
    for(i=0;i<2;i++){

        // Variables to load into.
        ALenum      format;
        ALsizei     size;
        ALvoid*     data;
        ALsizei     freq;
        ALboolean   loop;



        if(i==0){
            // Load wav data into a buffer.
            alGenBuffers(1, &Buffer);

            if(alGetError() != AL_NO_ERROR)
                return AL_FALSE;
            alutLoadWAVFile((ALbyte*)"wav/laserGun.wav", &format, &data, &size, &freq, &loop);

            // Bind the buffer with the source.
            alBufferData(Buffer, format, data, size, freq);
            alutUnloadWAV(format, data, size, freq);

            alGenSources(1, &Source);

            if(alGetError() != AL_NO_ERROR)
                return AL_FALSE;

            alSourcei (Source, AL_BUFFER,   Buffer   );
            alSourcef (Source, AL_PITCH,    1.0      );
            alSourcef (Source, AL_GAIN,     1.0      );
            alSourcefv(Source, AL_POSITION, SourcePos);
            alSourcefv(Source, AL_VELOCITY, SourceVel);
            alSourcei (Source, AL_LOOPING,  loop     );
        }
        else{
            // Load wav data into a buffer.
            alGenBuffers(1, &BufferEx);

            if(alGetError() != AL_NO_ERROR)
                return AL_FALSE;

            alutLoadWAVFile((ALbyte*)"wav/explosion.wav", &format, &data, &size, &freq, &loop);

            alBufferData(BufferEx, format, data, size, freq);
            alutUnloadWAV(format, data, size, freq);

            alGenSources(1, &SourceEx);

            if(alGetError() != AL_NO_ERROR)
                return AL_FALSE;

            alSourcei (SourceEx, AL_BUFFER,   BufferEx   );
            alSourcef (SourceEx, AL_PITCH,    1.0      );
            alSourcef (SourceEx, AL_GAIN,     1.0      );
            alSourcefv(SourceEx, AL_POSITION, SourcePosEx);
            alSourcefv(SourceEx, AL_VELOCITY, SourceVelEx);
            alSourcei (SourceEx, AL_LOOPING,  loop     );
        }





        // Do another error check and return.
        if(alGetError() == AL_NO_ERROR)
            return AL_TRUE;
    }
    return AL_FALSE;
}

void SetListenerValues()
{
    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
    alListenerfv(AL_POSITION,    ListenerPosEx);
    alListenerfv(AL_VELOCITY,    ListenerVelEx);
    alListenerfv(AL_ORIENTATION, ListenerOriEx);
}

void KillALData()
{
    alDeleteBuffers(1, &Buffer);
    alDeleteSources(1, &Source);
    alDeleteBuffers(1, &BufferEx);
    alDeleteSources(1, &SourceEx);
    alutExit();
}

// atualiza o vector com as posicoes de observacao
void atualizarVisao()
{
    obsPfin[0] = obsPini[0] + rVisao * cos(aVisao);
    obsPfin[2] = obsPini[2] - rVisao * sin(aVisao);
    glutPostRedisplay();
}


/* ********************************
 * FUNCOES DE APLICACAO DE TEXTURAS
 * ****************************** */

void iniciarTexturas()
{
    int i;

    for(i = 0; i < 14; i++)
    {
        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);

        if(i == 0)
            imag.LoadBmpFile("tex/chao.bmp");
        else if(i == 1)
            imag.LoadBmpFile("tex/fundo.bmp");
        else if(i == 2)
            imag.LoadBmpFile("tex/start.bmp");
        else if(i == 3)
            imag.LoadBmpFile("tex/options.bmp");
        else if(i == 4)
            imag.LoadBmpFile("tex/scores.bmp");
        else if(i == 5)
            imag.LoadBmpFile("tex/instructions.bmp");
        else if(i == 6)
            imag.LoadBmpFile("tex/about.bmp");
        else if (i==7)
            imag.LoadBmpFile("tex/opcoes.bmp");
        else if (i==8)
            imag.LoadBmpFile("tex/check.bmp");
        else if (i==9)
            imag.LoadBmpFile("tex/autores.bmp");
        else if (i==10)
            imag.LoadBmpFile("tex/pontuacao.bmp");
        else if (i==11)
            imag.LoadBmpFile("tex/instrucoes.bmp");
        else if (i==12)
            imag.LoadBmpFile("tex/lagrima.bmp");
        else if (i==13)
            imag.LoadBmpFile("tex/electro.bmp");


        if(i==12)
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        else
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imag.GetNumCols(), imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, imag.ImageData() );
    }
}


void iniciarSkyBox()
{
    int i;

    for(i = 0; i < 6; i++)
    {
        glGenTextures(1, &skybox[i]);
        glBindTexture(GL_TEXTURE_2D, skybox[i]);

        //NORMAL
        if(cenario==1){
            if(i == 0)//chao
                imagS.LoadBmpFile("tex/jf_nuke/nuke_dn.bmp");
            else if(i == 1)//teto
                imagS.LoadBmpFile("tex/jf_nuke/nuke_up.bmp");
            else if(i == 2)//back
                imagS.LoadBmpFile("tex/jf_nuke/nuke_bk.bmp");
            else if(i == 3)//left
                imagS.LoadBmpFile("tex/jf_nuke/nuke_lf.bmp");
            else if(i == 4)//front
                imagS.LoadBmpFile("tex/jf_nuke/nuke_ft.bmp");
            else if(i == 5)//right
                imagS.LoadBmpFile("tex/jf_nuke/nuke_rt.bmp");
        }else if(cenario==2){
              //para chuva
            if(i == 0)//chao
                imagS.LoadBmpFile("tex/elbrus/elbrus_back.bmp");
            else if(i == 1)//teto
                imagS.LoadBmpFile("tex/elbrus/elbrus_top.bmp");
            else if(i == 2)//back
                imagS.LoadBmpFile("tex/elbrus/elbrus_back.bmp");
            else if(i == 3)//left
                imagS.LoadBmpFile("tex/elbrus/elbrus_left.bmp");
            else if(i == 4)//front
                imagS.LoadBmpFile("tex/elbrus/elbrus_front.bmp");
            else if(i == 5)//right
                imagS.LoadBmpFile("tex/elbrus/elbrus_right.bmp");

        }else if(cenario==3){
            // para nevoeiro

            if(i == 0)//chao
                imagS.LoadBmpFile("tex/mp_nightwalk1/nightwalker-ftd_dn.bmp");
            else if(i == 1)//teto
                imagS.LoadBmpFile("tex/mp_nightwalk1/nightwalker-ftd_up.bmp");
            else if(i == 2)//back
                imagS.LoadBmpFile("tex/mp_nightwalk1/nightwalker-ftd_ft.bmp");
            else if(i == 3)//left
                imagS.LoadBmpFile("tex/mp_nightwalk1/nightwalker-ftd_lf.bmp");
            else if(i == 4)//front
                imagS.LoadBmpFile("tex/mp_nightwalk1/nightwalker-ftd_bk.bmp");
            else if(i == 5)//right
                imagS.LoadBmpFile("tex/mp_nightwalk1/nightwalker-ftd_rt.bmp");
        }


        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagS.GetNumCols(), imagS.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, imagS.ImageData() );
    }
}

void desenhaEletricidade(int um, int dois){
    if(dois){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[13]);
        glPushMatrix();
        glBegin(GL_QUADS);


        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-49.0,10.0, -15.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-49.0,10.0, -25.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-49.0,-10.0, -25.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-49.0,-10.0, -15.0);

        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }

    if(um){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[13]);
        glPushMatrix();
        glBegin(GL_QUADS);


        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-49.0,10.0, 25.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-49.0,10.0, 15.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-49.0,-10.0, 15.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-49.0,-10.0, 25.0);

        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
}


void func_instrucoes() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-10.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(10.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-10.0f, -10.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}

void func_about() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-10.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(10.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-10.0f, -10.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}

void func_pontos() {

    //ler pontos
    FILE *fp1;
    char old_pontos[20];
    fp1 = fopen("F_pontos.txt","r");
    fgets(old_pontos,20,fp1);
    fclose(fp1);

    desenhaPontosMenu(old_pontos);

    // carregar texturas
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-10.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(10.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-10.0f, -10.0f);


    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}

void func_opcoes() {

    //seta cenario
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glPushMatrix();

    glTranslatef(-2.3,pos_seta_y,0);

    glBegin(GL_QUADS);


    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //seta som
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glPushMatrix();

    glTranslatef(-6.3,pos_seta_som_y,0);

    glBegin(GL_QUADS);


    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //seta opcoes robot
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glPushMatrix();

    glTranslatef(-6.7,pos_seta_robot_y,0);

    glBegin(GL_QUADS);


    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.5f, -0.5f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //fundo do menu opcoes
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-10.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(10.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-10.0f, -10.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void menu()
{
    // start
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, 5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.0f, 5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.0f, 3.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, 3.0f);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    // options
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, 0.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // scores
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.0f, -3.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, -3.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //instructionbs
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.0f, -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.0f, -6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, -6.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // about
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, -7.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.0f, -7.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.0f, -9.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, -9.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //fundo do menu
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-10.0f, 10.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(10.0f, 10.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(10.0f, -10.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-10.0f, -10.0f);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


}

void desenhaLagrima(){
    //parede com menino da lagrima
    /*float deltax = -49.0;//largura em z e altura em y
    float dim = 1.0;

    float   i,j;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[12]);
    glPushMatrix();
        //glRotatef(90,1,0,0);
        glBegin(GL_QUADS);
            glNormal3f(1.0,0.0,0.0);
            for(i=0;i<dim;i++){
                for(j=0;j<dim;j++){

                    glTexCoord2f(i/dim,j/dim);          glVertex3f(deltax,  ((j)/dim)       *(-10.0)    ,((i)/dim)      *15.0);//00
                    glTexCoord2f((i+1)/dim,j/dim);      glVertex3f(deltax,  ((j)/dim)       *(-10.0)       ,((i+1)/dim)      *-15.0);//01
                    glTexCoord2f((i+1)/dim,(j+1)/dim);  glVertex3f(deltax,  ((j+1)/dim)     *30.0       ,((i+1)/dim)    *(-15.0));//11
                    glTexCoord2f(i/dim,(j+1)/dim);      glVertex3f(deltax,  ((j+1)/dim)     *(30.0)    ,((i)/dim)    *(15.0));//10

                }
            }
        glEnd();
    glPopMatrix();*/


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glPushMatrix();
    glNormal3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-49.0,30.0, -15.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-49.0,30.0, 15.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-49.0,-10.0, 15.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-49.0,-10.0, -15.0);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}




void desenhaSkyBox(){

    //chao
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glPushMatrix();
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0,-15.0,-50);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0,-15.0,50);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0,-15.0,50);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0,-15.0,-50);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //teto
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glPushMatrix();
    glNormal3f(0.0f, -1.0f, 0.0f);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 1.0f); glVertex3f(50.1,29.9,-50.1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(50.1,29.9,50.1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.1,29.9,50.1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.1,29.9,-50.1);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //back
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glPushMatrix();
    glNormal3f(0.0f, 0.0f, -1.0f);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0,30.0,50);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.1,30.0,50);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.1,-15.0,50);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0,-15.0,50);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glPushMatrix();
    glNormal3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0,30.0,50.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-50.0,30.0,-50.1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-50.0,-15.0,-50.1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0,-15.0,50.0);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //front
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glPushMatrix();
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 1.0f); glVertex3f(-50.0,30.0,-50.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(50.1,30.0,-50.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(50.1,-15.0,-50.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0,-15.0,-50.0);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glPushMatrix();
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);


    glTexCoord2f(0.0f, 1.0f); glVertex3f(50.0,30.0,-50.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(50.0,30.0,50.1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(50.0,-15.0,50.1);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0,-15.0,-50.0);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


/* ********************************
 * FUNCOES REFERENTES AO MOTOR DE PARTICULAS (EXPLOSAO)
 * ****************************** */

void newSpeed (float dest[3]){
    float    x;
    float    y;
    float    z;
    float    len;

    x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
    y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
    z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

  /*Normalizing the speed vectors gives a "fireball" effect*/
  //if (wantNormalize){
        len = sqrt (x * x + y * y + z * z);

        if (len){
            x = x / len;
            y = y / len;
            z = z / len;
        }
    //}

    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
}


void newExplosion (void)
{
    //puts("BOOOOOM HEADSHOT!!");
    int    i;

    for (i = 0; i < NUM_PARTICLES; i++){
        particles[i].position[0] = 0.0;
        particles[i].position[1] = 0.0;
        particles[i].position[2] = 0.0;

        particles[i].color[0] = 1.0;
        particles[i].color[1] = 1.0;
        particles[i].color[2] = 0.5;

        newSpeed (particles[i].speed);
    }

    for (i = 0; i < NUM_DEBRIS; i++){
        debris[i].position[0] = 0.0;
        debris[i].position[1] = 0.0;
        debris[i].position[2] = 0.0;

        debris[i].orientation[0] = 0.0;
        debris[i].orientation[1] = 0.0;
        debris[i].orientation[2] = 0.0;

        debris[i].color[0] = 0.7;
        debris[i].color[1] = 0.7;
        debris[i].color[2] = 0.7;

        debris[i].scale[0] = (2.0 *
            ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
        debris[i].scale[1] = (2.0 *
            ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
        debris[i].scale[2] = (2.0 *
            ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

        newSpeed (debris[i].speed);
        newSpeed (debris[i].orientationSpeed);
    }

    //fuel = 1;
}



void pufpuf(){
    int    i;

    /*if (fuel == 0 && cuboDest==0)
    {
        //glEnable (GL_LIGHTING);
        //glDisable (GL_LIGHT0);
        glEnable (GL_DEPTH_TEST);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, goldAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goldDif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, goldSpec);
        glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, goldCoef);
        glPushMatrix();
            glTranslatef(25.0,0,-25.0);
            glutSolidCube (10.0);
        glPopMatrix();
    }*/


    int j;
    for(j=0;j<enemysList.size();j++){
      if (enemysList[j].fuel > 0 && enemysList[j].show==1)
        {
    //if (fuel > 0 /*&& cuboDest<2*/){
        //cuboDest=2;

        glPushMatrix ();
            glTranslatef(enemysList[j].position[0],enemysList[j].position[1],enemysList[j].position[2]);

            glDisable (GL_LIGHTING);
            glDisable (GL_DEPTH_TEST);

            glBegin (GL_POINTS);
                for (i = 0; i < NUM_PARTICLES; i++){
                    glColor3fv (particles[i].color);
                    glVertex3fv (particles[i].position);
                }
            glEnd ();
        glPopMatrix ();

        glEnable (GL_LIGHTING);
        //glEnable (GL_LIGHT0);
        glEnable (GL_DEPTH_TEST);

        glNormal3f (0.0, 0.0, 1.0);

        for (i = 0; i < NUM_DEBRIS; i++){
            glColor3fv (debris[i].color);

            glPushMatrix ();
                glTranslatef(enemysList[j].position[0],enemysList[j].position[1],enemysList[j].position[2]);
                glTranslatef (debris[i].position[0], debris[i].position[1], debris[i].position[2]);

                glRotatef (debris[i].orientation[0], 1.0, 0.0, 0.0);
                glRotatef (debris[i].orientation[1], 0.0, 1.0, 0.0);
                glRotatef (debris[i].orientation[2], 0.0, 0.0, 1.0);

                glScalef (debris[i].scale[0], debris[i].scale[1], debris[i].scale[2]);

                glBegin (GL_TRIANGLES);
                    glVertex3f (0.0, 0.5, 0.0);
                    glVertex3f (-0.25, 0.0, 0.0);
                    glVertex3f (0.25, 0.0, 0.0);
                glEnd ();
            glPopMatrix ();
        }
    }
}
}

/* ********************************
 * FUNCOES DE COLISOES E LIMITES
 * ****************************** */

 void checkLimit(){

    if(robotZ<=-50)
        robotZ=-50;
    else if(robotZ>=50)
        robotZ=50;

    if(robotX>=50)
        robotX=50;
    else if(robotX<=-50)
        robotX=-50;


    GLfloat colx = (robotX)-avancoMira*sin(angR*PI/180);
    GLfloat colz = (robotZ)-avancoMira*cos(angR*PI/180);
    GLfloat coly = (robotY+1.5);

    if(colx<-50.0 || colx>50.0 || colz<-50.0 || colz>50.0)
        avancoMira--;
    /*else if((colx > 20.0 && colx < 30.0) && (colz < -20.0 && colz > -30.0) && (coly > -5.0 && coly < 5)){
        if(cuboDest == 0){
            puts("colisao da mira com cubo");
            avancoMira--;
        }
    }*/
    else if (avancoMira<30.0){
        avancoMira++;
        //puts("< 50");
    }

}

//x,altura,z
//colisao com a mira
int colisoes(GLfloat x, GLfloat olhoy, GLfloat altura, GLfloat z, GLfloat olhox, GLfloat olhoz){
    //25,0,-25

    int i;
    int aux=0;
    int indEl=-1; //indice a eliminar
    GLfloat colx = (olhox+robotX)-altura*sin(angR*PI/180);
    GLfloat colz = (olhoz+robotZ)-altura*cos(angR*PI/180);
    GLfloat coly = (olhoy+robotY);

    //printf("x= %f      z= %f\n", colx,colz);

    //colisao disparo com cubo
    /*if((colx > 20.0 && colx < 30.0) && (colz < -20.0 && colz > -30.0) && (coly > -5.0 && coly < 5)){
        cuboDest=1;
        //puts("colisao");
        return 1;
    }*/

    for (i=0;i<enemysList.size();i++){
        if((colx > enemysList[i].position[0]-3 && colx < enemysList[i].position[0]+3) && (colz < enemysList[i].position[2]+3 && colz > enemysList[i].position[2]-3) && (enemysList[i].position[1]-3 < 1.5) && enemysList[i].show==0){
            puts("tiro no inimigo");
            score+=10;
            enemysList[i].show=1;
            aux=1;
            break;
        }

        if(enemysList[i].show==1 && enemysList[i].fuel==0)
            indEl=i;
    }

    //elimina da lista caso o efeito da explosao já tenha acabado
    if(indEl!=-1)
        enemysList.erase(enemysList.begin()+indEl);

    //se acertar no inimigo, explode
    if(aux==1){
        newExplosion();
        //enemysList.erase(enemysList.begin()+i);
        return 1;
    }

    //tiro colide com caixas de eletricidade
    if(glIsEnabled(GL_LIGHT1))
    if(colx<=-49 && colz>15 && colz<25){
        printf("caixa 1\n");

        score+=50;
        glDisable(GL_LIGHT1);
        enemy box;
        box.position[0]=-49;
        box.position[1]=0;
        box.position[2]=20;
        box.fuel=50;
        box.show=1;
        enemysList.push_back(box);
        newExplosion ();
        return 2;
    }
    if(glIsEnabled(GL_LIGHT2))
    if(colx<=-49 && colz>-25 && colz<-15){
        printf("caixa 2\n");


        score+=50;
        glDisable(GL_LIGHT2);
        enemy box;
        box.position[0]=-49;
        box.position[1]=0;
        box.position[2]=-20;
        box.fuel=50;
        box.show=1;
        enemysList.push_back(box);
        newExplosion ();
        return 2;
    }

    //tiro fora do limite
    if(colx >= 50 || colx <= -50 || colz >= 50 || colz <= -50){
        //puts("fora do limite");
        return 2;
    }

    //continuar
    return 0;
}

void colisoesInimigos(){

    int i;
    int aux = 0;
    if(pausegame==1){
        for (i=0;i<enemysList.size();i++){
            if((robotX > enemysList[i].position[0]-3 && robotX < enemysList[i].position[0]+3) && (robotZ < enemysList[i].position[2]+3 && robotZ > enemysList[i].position[2]-3) && (enemysList[i].position[1]-3 < 2) && enemysList[i].show==0){
                puts("colisao com o boneco");
                aux=1;
                if(health>0)
                    health--;
                robotZ=lastRobotZ;
                robotX=lastRobotX;
                if (health==0 && dead==0){
                    puts("morrest");
                    dead=1;
                    enemy hero;
                    hero.position[0]=robotX;
                    hero.position[1]=robotY;
                    hero.position[2]=robotZ;
                    hero.fuel=50;
                    hero.show=1;
                    enemysList.push_back(hero);
                    newExplosion ();
                }
                break;
            }

        }

        if(aux==0){
            lastRobotZ=robotZ;
            lastRobotX=robotX;
        }
    }
}


/* ********************************
 * FUNCOES DE DESENHO DE OBJETOS E TEXTO
 * ****************************** */

void desenhaPontosMenu(char *string)
{

    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1,1,1);
    glRasterPos2f(-7.5,3.7);

    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);

   glDisable(GL_COLOR_MATERIAL);


}


void desenhaTexto(char *string)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rubyAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rubyDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rubySpec);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rubyCoef);

    glRasterPos2f(2,9);

    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);

}

void desenhaMira(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rubyAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rubyDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rubySpec);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rubyCoef);

    //X
    glLineWidth(2);
    glPushMatrix();
        glBegin(GL_LINES);
            glVertex3f(0, -1.5, -avancoMira);
            glVertex3f(0, 1.5, -avancoMira);
        glEnd();

        glBegin(GL_LINES);
            glVertex3f(1.5, 0, -avancoMira);
            glVertex3f(-1.5, 0, -avancoMira);
        glEnd();
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whiteRubberAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteRubberDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteRubberSpec);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, whiteRubberCoef);

    //O
    glPushMatrix();
            glTranslatef(0,0,-avancoMira);
            glutSolidTorus(0.2,2,32,32);//(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings)
    glPopMatrix();
}

void desenhaBombas(){
    int i;

    for(i=0;i<enemysList.size();i++){
        if(enemysList[i].show==0){

            glPushMatrix();
                //glTranslatef(0,0,0);

                //glTranslatef(enemysList[i].position[0],enemysList[i].position[1],enemysList[i].position[2]);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackRubberAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackRubberDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackRubberSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, blackRubberCoef);

                //rotacao das bombas
                glTranslatef(enemysList[i].position[0],enemysList[i].position[1],enemysList[i].position[2]);
                glRotatef(anguloCube,1,0,0);
                glTranslatef(-enemysList[i].position[0],-enemysList[i].position[1],-enemysList[i].position[2]);

                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1],enemysList[i].position[2]);
                    glutSolidSphere(3,32,32);
                glPopMatrix();


                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whiteRubberAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whiteRubberDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteRubberSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, whiteRubberCoef);
                //+z
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1],enemysList[i].position[2]+2.9);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //-z
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1],enemysList[i].position[2]-2.9);
                    glRotatef(180,1,0,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();

                //+x
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0]+2.9,enemysList[i].position[1],enemysList[i].position[2]);
                    glRotatef(90,0,1,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //-x
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0]-2.9,enemysList[i].position[1],enemysList[i].position[2]);
                    glRotatef(-90,0,1,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //+y
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1]+2.9,enemysList[i].position[2]);
                    glRotatef(-90,1,0,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //-y
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1]-2.9,enemysList[i].position[2]);
                    glRotatef(90,1,0,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();

                //+y+z
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1]+2,enemysList[i].position[2]+2);
                    glRotatef(-45,1,0,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //-y+z
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1]-2,enemysList[i].position[2]+2);
                    glRotatef(45,1,0,0);
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //+y-z
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1]+2,enemysList[i].position[2]-2);
                    glRotatef(-135,1,0,0);//-90-45
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
                //-y-z
                glPushMatrix();
                    glTranslatef(enemysList[i].position[0],enemysList[i].position[1]-2,enemysList[i].position[2]-2);
                    glRotatef(135,1,0,0);//90+45
                    glutSolidCone(0.75, 1, 32, 32);//base,height,slices,stacks
                glPopMatrix();
            glPopMatrix();
        }
    }
}

void desenhaRobot(){
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);



    glPushMatrix();
        glTranslatef(robotX,robotY,robotZ);
        glRotatef(angR,0.0,1.0,0.0);
        /*braço direito*/
        //glColor4f(AZUL);
        if (op_robot == 1){ //zinco
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silverAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silverDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, silverSpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, silverCoef);
        }
        else if (op_robot == 2){ //bronze
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bronzeAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bronzeDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bronzeSpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, bronzeCoef);

        }
        else if (op_robot == 3){ //mercurio
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlasticAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlasticDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlasticSpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticCoef);

        }
        glPushMatrix();
            glTranslatef(3.5,-3.0,-1.5);
            glRotatef(angB,1.0,0.0,0.0);
            glTranslatef(0.0,0.0,-2.0);
            glScalef(1.0,1.0,3.0);
            glutSolidCube(1.5);
        glPopMatrix();


        /*braço esquerdo*/
        //glColor4f(AZUL);
        glPushMatrix();
            glTranslatef(-3.5,-3.0,-1.5);
            glRotatef(angB,1.0,0.0,0.0);
            glTranslatef(0.0,0.0,-2.0);
            glScalef(1.0,1.0,3.0);
            glutSolidCube(1.5);
        glPopMatrix();

        /*corpo*/
        //glColor4f(VERMELHO);
        if (op_robot == 1){ //zinco
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chromeAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chromeDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chromeSpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, chromeCoef);
        }
        else if (op_robot == 2){ //bronze
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bronzeAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bronzeDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bronzeSpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, bronzeCoef);

        }
        else if (op_robot == 3){ //mercurio
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rubyAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rubyDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rubySpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rubyCoef);

        }
        glPushMatrix();
            //glTranslatef(0.0,0.0,0.0);
            glRotatef(90.0,1.0,0.0,0.0);
            GLUquadricObj *quadratic;
            quadratic = gluNewQuadric();
            gluCylinder(quadratic,3.0,3.0,10.0,32,32);//raio base,raio topo,altura,subdivisoes voltaz,subdivisoes ao longo z
        glPopMatrix();

        glPushMatrix();
            //glRotatef(angC,0.0,1.0,0.0);
            /*cabeca*/
            //glColor4f(BRANCO);
            if (op_robot == 1){ //zinco
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, chromeAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chromeDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chromeSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, chromeCoef);
            }
            else if (op_robot == 2){ //bronze
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whitePlasticAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whitePlasticDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whitePlasticSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, whitePlasticCoef);
            }
            else if (op_robot == 3){ //mercurio
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowRubberAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowRubberDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowRubberSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, yellowRubberCoef);

            }
            glPushMatrix();
                glTranslatef(0.0,1.0,0.0);
                glutSolidSphere(2.5,32,32);
            glPopMatrix();

            /*olhos*/
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rubyAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rubyDif);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rubySpec);
            glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rubyCoef);
            glPushMatrix();
                glTranslatef(0.75,1.5,-2.2);
                glutSolidSphere(0.3,32,32);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-0.75,1.5,-2.2);
                glutSolidSphere(0.3,32,32);
            glPopMatrix();

            /*mira*/
            glPushMatrix();

                glTranslatef(0,1.5,0);
                desenhaMira();
            glPopMatrix();

            checkLimit();

            if(disparo==1){

                //glEnable(GL_LIGHTING);
                if (op_robot == 1) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowRubberAmb);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowRubberDif);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellowRubberSpec);
                    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, yellowRubberCoef);
                }
                else if (op_robot == 2) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, whitePlasticAmb);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, whitePlasticDif);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whitePlasticSpec);
                    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, whitePlasticCoef);
                }
                else if (op_robot == 3) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greenRubberAmb);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greenRubberDif);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greenRubberSpec);
                    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, greenRubberCoef);
                }

                if(som==1)
                    alSourcePlay(Source);

                for(int i = 1;i<=25;i++){
                    glPushMatrix();
                        glTranslatef(0.75,1.5,-2.2);
                        glRotatef(180.0,1.0,0.0,0.0);
                        GLUquadricObj *tirodir;
                        tirodir = gluNewQuadric();
                        gluCylinder(tirodir,0.1,0.1,5.0*i,32,32);//raio base,raio topo,altura,subdivisoes voltaz,subdivisoes ao longo z
                    glPopMatrix();

                    glPushMatrix();
                        glTranslatef(-0.75,1.5,-2.2);
                        glRotatef(180.0,1.0,0.0,0.0);
                        GLUquadricObj *tiroesq;
                        tiroesq = gluNewQuadric();
                        gluCylinder(tiroesq,0.1,0.1,5.0*i,32,32);//raio base,raio topo,altura,subdivisoes voltaz,subdivisoes ao longo z
                    glPopMatrix();

                    //x,altura,z
                    //if(cuboDest==0)
                    if(colisoes(0.75,1.5,1.5+5.0*i,-2.2,0.75,-2.2)==1 || colisoes(-0.75,1.5,1.5+5.0*i,-2.2,-0.75,-2.2)==1){
                        if(som==1)
                            alSourcePlay(SourceEx);
                        break;
                    }else if(colisoes(0.75,1.5,1.5+5.0*i,-2.2,0.75,-2.2)==2 || colisoes(-0.75,1.5,1.5+5.0*i,-2.2,-0.75,-2.2)==2){
                        //fora do limite
                        break;
                    }
                }

                //disparo=0;
            }
        glPopMatrix();

        /*propulsao*/
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackPlasticAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlasticDif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlasticSpec);
        glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, blackPlasticCoef);
        glPushMatrix();
            glTranslatef(0.0,torusin,0.0);
            glRotatef(-90.0,1.0,0.0,0.0);
            glutSolidTorus(0.05,1.5,32,32);//(GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings)
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0,torusout,0.0);
            glRotatef(-90.0,1.0,0.0,0.0);
            glutSolidTorus(0.05,2,32,32);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0,torusin,0.0);
            glRotatef(-90.0,1.0,0.0,0.0);
            glutSolidTorus(0.05,2.4,32,32);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0,torusout,0.0);
            glRotatef(-90.0,1.0,0.0,0.0);
            glutSolidTorus(0.05,2.9,32,32);
        glPopMatrix();
    glPopMatrix();

}


/* ********************************
 * FUNCOES TIMER E CALLBACKS
 * ****************************** */

//timer adiciona inimigos ao ecra
void dropEnemy(int value){
    int i;
    if(pausegame==1){
        for(i=0;i<enemysList.size();i++){
            if(enemysList[i].position[1]>0)
                enemysList[i].position[1]-=0.5;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(enemyCreateDelay,dropEnemy, 1);
}

//timer inimigos
void Enemy(int value){
    int i,aux=0;

    if(iniciarjogo == 1 && pausegame==1){
        if(pastX == robotX && pastZ == robotZ){
            printf("MESMO SITIO\n");

            for(i=0;i<enemysList.size();i++){
                if(enemysList[i].position[0] == pastX && enemysList[i].position[2] == pastZ){
                    aux=1;
                    break;
                }
            }

            if(aux==0){
                enemy newe;
                newe.show=0;
                newe.position[0]=pastX;
                newe.position[1]=50.0;
                newe.position[2]=pastZ;
                newe.fuel=150;

                enemysList.push_back(newe);
            }
        }
        else{
            pastX = robotX;
            pastZ = robotZ;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(enemyDelay,Enemy, 1);
}

//timer molas
void Timer(int value)
{
    if(pausegame==1){
        torusin+=0.06*torusinDir;
        if(torusin<=-12.0)
            torusinDir=torusinDir*(-1);
        else if(torusin>=-10.0)
            torusinDir=torusinDir*(-1);

        torusout+=0.06*torusoutDir;
        if(torusout>=-10.0)
            torusoutDir=torusoutDir*(-1);
        else if(torusout<=-12.0)
            torusoutDir=torusoutDir*(-1);
    }

    glutPostRedisplay();
    glutTimerFunc(msecDelay,Timer, 1);
}

//timer salto robot
void TimerRobot(int value)
{
    if(pausegame==1){
        if(robotYDir==1)
            robotY+=0.06*robotYDir;
        else
            robotY+=0.09*robotYDir;
        if(robotY>=1.0)
            robotYDir=robotYDir*(-1);
        else if(robotY<=0.0)
            robotYDir=robotYDir*(-1);
    }
    glutPostRedisplay();
    glutTimerFunc(msecDelayRobot,TimerRobot, 1);
}

//timer cubo rotativo
void TimerCube(int value) {
    anguloCube += 10;
    glutPostRedisplay();
    glutTimerFunc(delay_cube,TimerCube, 1);

}

//timer chaleira rotativa
void TimerCha (int value) {

    move_cha += 0.1*cha_dir;
    if ((move_cha > 5.0) or (move_cha <0.0)) {
        cha_dir = cha_dir * -1.0;}

        glutPostRedisplay();
    glutTimerFunc(delay_cha,TimerCha, 1);
}

void idle (void)
{
    int    i;
    int j;
    if(pausegame==1){
        for(j=0;j<enemysList.size();j++){
          if (enemysList[j].fuel > 0 && enemysList[j].show==1)
            {
              for (i = 0; i < NUM_PARTICLES; i++)
                {
                  particles[i].position[0] += particles[i].speed[0] * 0.2;
                  particles[i].position[1] += particles[i].speed[1] * 0.2;
                  particles[i].position[2] += particles[i].speed[2] * 0.2;

                  particles[i].color[0] -= 1.0 / 500.0;
                  if (particles[i].color[0] < 0.0)
                {
                  particles[i].color[0] = 0.0;
                }

                  particles[i].color[1] -= 1.0 / 100.0;
                  if (particles[i].color[1] < 0.0)
                {
                  particles[i].color[1] = 0.0;
                }

                  particles[i].color[2] -= 1.0 / 50.0;
                  if (particles[i].color[2] < 0.0)
                {
                  particles[i].color[2] = 0.0;
                }
                }

                for (i = 0; i < NUM_DEBRIS; i++)
                {
                  debris[i].position[0] += debris[i].speed[0] * 0.1;
                  debris[i].position[1] += debris[i].speed[1] * 0.1;
                  debris[i].position[2] += debris[i].speed[2] * 0.1;

                  debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
                  debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
                  debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;
                }

              --enemysList[j].fuel;

              if(health==0 && enemysList[j].fuel == 0){
                    clear();
                }
            }
        }


    }

  glutPostRedisplay ();
}


/* ********************************
 * FUNCOES DE LUZES
 * ****************************** */

void defineLuzes()
{

    glEnable(GL_LIGHTING);

    //ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzGlobalCor );
    glEnable(GL_LIGHT0);


    //pontual
    GLfloat localCor[4]={0.4,0.4,0.4,1.0};
    GLfloat localCorDif[4]={1,1,1,1.0};
    GLfloat posicao3[4] = { 0, 50 ,0,1.0};
    glLightfv(GL_LIGHT3, GL_POSITION, posicao3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, localCor);
    glLightfv(GL_LIGHT3, GL_SPECULAR, localCorDif);

    GLfloat posicao4[4] = { 50, 50 ,0,1.0};
    glLightfv(GL_LIGHT4, GL_POSITION, posicao4);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, localCor);
    glLightfv(GL_LIGHT4, GL_SPECULAR, localCorDif);

    GLfloat posicao5[4] = { -50, -50 ,0,1.0};
    glLightfv(GL_LIGHT5, GL_POSITION, posicao5);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, localCor);
    glLightfv(GL_LIGHT5, GL_SPECULAR, localCorDif);

    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);


    //focos
    GLfloat Foco_direccao[] = { 0, 0, 1};       //=== Z
    GLfloat Foco1_cor[]     = { 0, 1,  0, 1};   //=== Cor da luz 1
    GLfloat Foco2_cor[]     = { 1, 0,  0, 1};   //=== Cor da luz 2
    GLfloat Foco_ak         = 1.0;
    GLfloat Foco_al         = 0.05f;
    GLfloat Foco_aq         = 0.0f;
    GLfloat Foco_Expon      = 2.0;              // Foco, SPOT_Exponent

    //=================================================================Foco Esquerda
    glLightfv(GL_LIGHT1,GL_POSITION,                Pos1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,                 Foco1_cor);
    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,     Foco_ak);
    glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,       Foco_al);
    glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,    Foco_aq);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,              anguloFoco);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,          Foco_direccao);
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,            Foco_Expon);

    //=================================================================Foco Direita
    glLightfv(GL_LIGHT2,GL_POSITION,                Pos2);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,                 Foco2_cor);
    glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,     Foco_ak);
    glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,       Foco_al);
    glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,    Foco_aq);
    glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,              anguloFoco);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,          Foco_direccao);
    glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,            Foco_Expon);


    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

/* ********************************
 * FUNCOES DE OPENGL TECLADO + RATO
 * ****************************** */

void passivemotion( int x, int y ) {  // mouse over

    // menu inicial
    if ((iniciarjogo == 0) && (opcoes == 0) && (pontos == 0) && (instrucoes == 0) && (acerca == 0) && (inicio == 1)) {
        if((x > 150) && (x < 350) && (y > 140) && (y < 170)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if((x > 150) && (x < 350) && (y > 190) && (y < 250)){ //opcoes
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if((x > 150) && (x < 350) && (y > 280) && (y < 320)){
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if((x > 150) && (x < 350) && (y > 360) && (y < 395)){
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if((x > 150) && (x < 350) && (y > 435) && (y < 470)){
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else {
           glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }


    //menu opcoes
    if ((iniciarjogo != 1) && (opcoes == 1) && (pontos != 1) && (instrucoes != 1) && (acerca != 1)){
        // escolher cenario
        if ((x>208) && (x<320) && (y>134) && (y<150)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
       else if ((x>208) && (x<320) && (y>159) && (y<175)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if ((x>208) && (x<320) && (y>185) && (y<205)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if ((x>208) && (x<320) && (y>212) && (y<230)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

        //ok
        if ((x>395) && (x<475) && (y>445) && (y<490)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

        //som
        if ((x>105) && (x<135) && (y>253) && (y<264)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else if ((x>104) && (x<142) && (y>279) && (y<288)) {
            glutSetCursor(GLUT_CURSOR_INFO);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

    }
}

// controlo do botao esq e dir do rato (iniciar novo jogo)
void cliqueRato(int butao, int estado, int x, int y)
{
    //printf("%d %d \n", x, y);
    //printf ("inicio %d, iniciarjogo %d, opcoes %d  \n", inicio, iniciarjogo, opcoes);
    // menu inicial
    if (inicio == 1) {
        if((iniciarjogo==0) && (x > 150) && (x < 350) && (y > 140) && (y < 170) && (opcoes != 1))  // start
        {
            inicio = 0;
            opcoes = 0;
            pontos = 0;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 1;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if((opcoes==0) && (x > 150) && (x < 350) && (y > 190) && (y < 250) && (iniciarjogo !=1)){ //opcoes
            inicio = 0;
            opcoes = 1;
            pontos = 0;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if((pontos==0) && (x > 150) && (x < 350) && (y > 280) && (y < 320) && (iniciarjogo !=1)){
            inicio = 0;
            opcoes = 0;
            pontos = 1;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if((instrucoes==0) && (x > 150) && (x < 350) && (y > 360) && (y < 395) && (iniciarjogo !=1)){
            inicio = 0;
            opcoes = 0;
            pontos = 0;
            instrucoes = 1;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if((acerca==0) && (x > 150) && (x < 350) && (y > 435) && (y < 470) && (iniciarjogo !=1)){
            inicio = 0;
            opcoes = 0;
            pontos = 0;
            instrucoes = 0;
            acerca = 1;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    //menu opcoes
    if ((iniciarjogo != 1) && (opcoes == 1) && (pontos != 1) && (instrucoes != 1) && (acerca != 1)){
        // escolher cenario
        if ((x>208) && (x<320) && (y>134) && (y<150)) {
            cenario = 1;
            pos_seta_y = 4.3;
            iniciarSkyBox();
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if ((x>208) && (x<320) && (y>159) && (y<175)) {
            cenario = 2;
            pos_seta_y = 3.4;
            iniciarSkyBox();
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if ((x>208) && (x<320) && (y>185) && (y<205)) {
            cenario = 3;
            pos_seta_y = 2.3;
            iniciarSkyBox();
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

        //ok
        if ((x>395) && (x<475) && (y>445) && (y<490)) {
            inicio = 1;
            opcoes = 0;
            pontos = 0;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

        //som
        if ((x>105) && (x<135) && (y>253) && (y<264)) {
            pos_seta_som_y = -0.4;
            som = 1;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        else if ((x>104) && (x<142) && (y>279) && (y<288)) {
            pos_seta_som_y = -1.2;
            som = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

        //robot
        if ((x>100) && (x<150) && (y>350) && (y<362)) {  //zinco
            op_robot = 1;
            pos_seta_robot_y = -4.1;
        }
        else if ((x>100) && (x<150) && (y>370) && (y<382)) { //bronze
            op_robot = 2;
            pos_seta_robot_y = -5;
        }
        else if ((x>100) && (x<150) && (y>387) && (y<405)) {  //mercurio
            op_robot = 3;
            pos_seta_robot_y = -5.8;
        }

    }

    // menu pontucao
    if ((iniciarjogo != 1) && (opcoes != 1) && (pontos == 1) && (instrucoes != 1) && (acerca != 1)){
        //ok
        if ((x>395) && (x<475) && (y>445) && (y<490)) {
            inicio = 1;
            opcoes = 0;
            pontos = 0;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    // menu instrucoes
    if ((iniciarjogo != 1) && (opcoes != 1) && (pontos != 1) && (instrucoes == 1) && (acerca != 1)){
        //ok
        if ((x>395) && (x<475) && (y>445) && (y<490)) {
            inicio = 1;
            opcoes = 0;
            pontos = 0;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    // menu acerca
    if ((iniciarjogo != 1) && (opcoes != 1) && (pontos != 1) && (instrucoes != 1) && (acerca == 1)){
        //ok
        if ((x>395) && (x<475) && (y>445) && (y<490)) {
            inicio = 1;
            opcoes = 0;
            pontos = 0;
            instrucoes = 0;
            acerca = 0;
            iniciarjogo = 0;
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }
    glutPostRedisplay();
}

void teclasUp(unsigned char tecla, int x, int y){
    if(tecla == 'p' && pausegame==1)
         disparo=0;

}

// controlo do teclado (jogar, ajuda, estatisticas, etc)
void teclado(unsigned char tecla, int x, int y)
{

    if(pausegame==1){
        //angulo braco
        if(tecla == '1'){
            angB+=10;
            if(angB>=45)
                angB=45;
        }
        else if(tecla == '2'){
            angB-=10;
            if(angB<=-45)
                angB=-45;

        }

        //angulo cabeca
       /* else if(tecla == '3'){
            angC+=10;
            if(angC>=45)
                angC=45;
        }
        else if(tecla == '4'){
            angC-=10;
            if(angC<=-45)
                angC=-45;
        }*/

        //andar (direcao)
        else if(tecla == 'w'){

            robotZ-=1*cos(angR*PI/180);
            robotX-=1*sin(angR*PI/180);

            checkLimit();

            obsPini[0] = robotX + incVisao * cos(aVisao);
            obsPini[2] = robotZ + incVisao * sin(aVisao);

        }
        else if(tecla == 's'){

            robotZ+=1*cos(angR*PI/180);
            robotX+=1*sin(angR*PI/180);

            checkLimit();

            obsPini[0] = robotX + incVisao * cos(aVisao);
            obsPini[2] = robotZ + incVisao * sin(aVisao);
        }
        else if(tecla == 'd'){

            robotZ-=1*cos((90-angR)*PI/180);
            robotX+=1*sin((90-angR)*PI/180);

            checkLimit();

            obsPini[0] = robotX + incVisao * cos(aVisao);
            obsPini[2] = robotZ + incVisao * sin(aVisao);

        }
        else if(tecla == 'a'){

            robotZ+=1*cos((90-angR)*PI/180);
            robotX-=1*sin((90-angR)*PI/180);

            checkLimit();

            obsPini[0] = robotX + incVisao * cos(aVisao);
            obsPini[2] = robotZ + incVisao * sin(aVisao);
        }

        //angulo robot (rotacao corpo)
        else if(tecla == 'q'){
            angR+=10;
            if(angR == 360)
                angR=0;
            aVisao = (aVisao - 0.175);

            checkLimit();

            obsPini[0] = robotX + incVisao * cos(aVisao);
            obsPini[2] = robotZ + incVisao * sin(aVisao);
           // printf("robot angulo %d\n", angR);
        }
        else if(tecla == 'e'){
            angR-=10;
            if(angR == -360)
                angR=0;
            aVisao = (aVisao + 0.175);

            checkLimit();

            obsPini[0] = robotX + incVisao * cos(aVisao);
            obsPini[2] = robotZ + incVisao * sin(aVisao);

            //printf("robot angulo %d\n", angR);
        }

        else if(tecla == 'c')
            CAMERA = CAMERA * -1;

        //disparo
        else if(tecla == 'p')
             disparo=1;

    }

    //ESC sai do jogo
    if(tecla == 27)
    {
        exit(0);
    }
    if(tecla == 32){ //space bar
        puts("pause game");
        pausegame*=-1;
    }

    if(tecla == 8){ //backspace
        puts("reboot game");
        clear();
    }

    atualizarVisao();

}

// funcao utilizada para redimensionar a janela
void redimensionarJanela(GLsizei lar, GLsizei alt)
{
    largura = lar;
    altura = alt;
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glutPostRedisplay();
}

// funcao principal de apresentacao do ecra
void ecra()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(PRETO);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, 0.0, 150.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    sprintf(textoPontos, "%d", score);

    if (inicio == 1) {
        menu();
    }
    else if (opcoes == 1) {
        func_opcoes();
    }
    else if (pontos == 1) {
        func_pontos();
    }
    else if (acerca == 1) {
        func_about();
    }
    else if (instrucoes == 1) {
        func_instrucoes();
    }
    else if (iniciarjogo == 1){

        //vida e pontuacao no ecra (ortho)
        sprintf(textoPontos, "Health: %d\nScore : %d", health,score);
        desenhaTexto(textoPontos);

        if(cenario==3)
            initNevoeiro();
        if(cenario==2)
            desenhaChuva();
        if(cenario!=3)
            glDisable(GL_FOG);

        //atualiza pontos
        if (health <= 0) {
             FILE *fp1;
             char new_point[20];
             //char old_point[20];
             int antigus_point = 0;

             //ler anterior
             fp1 = fopen("F_pontos.txt","r");
             fscanf(fp1,"%d",&antigus_point);
             fclose(fp1);

             if (score > antigus_point){ //actualiza pontos
                fp1 = fopen("F_pontos.txt","w");
                sprintf(new_point, "%d", score);
                fputs(new_point,fp1);
                fclose(fp1);
             }
        }


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90, largura/altura, 1, 200);
        glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();

        //tipo de camera
        if(CAMERA == -1)
            gluLookAt(50, 20, 50, robotX, obsPfin[1], robotZ, 0, 1, 0);
        else if(CAMERA == 1)
            gluLookAt(obsPini[0], obsPini[1], obsPini[2], robotX, obsPfin[1], robotZ, 0, 1, 0);


        if(health>0)
            desenhaRobot();
        desenhaLagrima();
        desenhaBombas();
        desenhaEletricidade(glIsEnabled(GL_LIGHT1),glIsEnabled(GL_LIGHT2));
        colisoesInimigos();
        pufpuf();// explosao

        //foco verde
        glPushMatrix();
            if (glIsEnabled(GL_LIGHT1)) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greenPlasticAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greenPlasticDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, greenPlasticSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, greenPlasticCoef);
            }else{
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackPlasticAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlasticDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlasticSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, blackPlasticCoef);
            }

            glTranslatef(Pos2[0],Pos2[1],Pos2[2]);
            glutSolidSphere(1.0, 100, 100);
        glPopMatrix();

        //foco vermelho
        glPushMatrix();
            if (glIsEnabled(GL_LIGHT2)) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redPlasticAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redPlasticDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, redPlasticSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, redPlasticCoef);
            }else{
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackPlasticAmb);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackPlasticDif);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blackPlasticSpec);
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, blackPlasticCoef);
            }

            glTranslatef(Pos1[0],Pos1[1],Pos1[2]);
            glutSolidSphere(1.0, 100, 100);
        glPopMatrix();



        /*chao refletor*/
        glEnable(GL_STENCIL_TEST); //Activa o uso do stencil buffer
            glColorMask(0, 0, 0, 0); //Nao escreve no color buffer
            glDisable(GL_DEPTH_TEST); //Torna inactivo o teste de profundidade
            glStencilFunc(GL_ALWAYS, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            //Coloca a 1 todos os pixels no stencil buffer que representam a superfície reflectora

            //textura
            desenhaSkyBox();

            glColorMask(1, 1, 1, 1); //Activa a escrita de cor
            glEnable(GL_DEPTH_TEST); //Activa o teste de profundidade
            glStencilFunc(GL_EQUAL, 1, 1);//O stencil test passa apenas quando o pixel tem o valor 1 no stencil buffer
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Stencil buffer read-only
            //Desenha o objecto com a reflexão onde stencil buffer é 1*/

            //DESENHAR OBJECTO REFLECTIDO
            glPushMatrix();
                glScalef(1,-1,1);
                glTranslatef(0.0,27.0,0);

                if(health>0)
                    desenhaRobot();
                desenhaLagrima();
                desenhaBombas();
                desenhaEletricidade(glIsEnabled(GL_LIGHT1),glIsEnabled(GL_LIGHT2));
                pufpuf();
            glPopMatrix();

        glDisable(GL_STENCIL_TEST); //Desactiva a utilização do stencil buffer
        glEnable(GL_BLEND); //Blending
            glColor4f(1, 1, 1, 0.7);
            desenhaSkyBox();
        glDisable(GL_BLEND);
    }

    glutSwapBuffers();
}


// auxiliar para iniciar necessidades do jogo
void iniciarPrograma()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);


    defineLuzes();

    glEnable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    iniciarTexturas();
    iniciarSkyBox();

}

int main(int argc, char** argv)
{
     textoPontos = (char*) malloc(sizeof(char*));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(largura, altura);
    glutInitWindowPosition(340, 100);
    glutCreateWindow("Bombs away!! [Projeto CG 2014 by DiFa, JoPe, PeRe]");
    iniciarPrograma();
    init();
    glutDisplayFunc(ecra);
    glutReshapeFunc(redimensionarJanela);
    glutKeyboardFunc(teclado);
    //glutSpecialFunc(teclasNotAscii);
    glutKeyboardUpFunc(teclasUp);
    glutPassiveMotionFunc(passivemotion);



    glutIdleFunc (idle);
    srand (time (NULL));

    alutInit(NULL, 0);
    alGetError();
    // Load the wav data.
    if(LoadALData() == AL_FALSE)
    {
        printf("Error loading data.");
        return 0;
    }
    SetListenerValues();
    // Setup an exit procedure.
    atexit(KillALData);


    glutTimerFunc(enemyCreateDelay, dropEnemy, 1);
    glutTimerFunc(enemyDelay, Enemy, 1);
    glutTimerFunc(msecDelay, Timer, 1);
    glutTimerFunc(msecDelayRobot, TimerRobot, 1);

    glutTimerFunc(delay_cube,TimerCube, 1);
    glutTimerFunc(delay_cha,TimerCha, 1);


    glutMouseFunc(cliqueRato);
    glutMainLoop();


    return 0;
}
