#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstring> // Para manipular strings
#include <cmath>
#include <cstdio>


// Variáveis globais para a cor e posição da bolinha
float corAtual[3] = {1.0f, 0.0f, 0.0f}; // Exemplo: vermelho
float posicaoX = 0.0f, posicaoY = 0.0f; // Posição da bolinha
int acertos = 0, erros = 0; // Contadores de acertos e erros
int teclaCorreta = GLUT_KEY_DOWN; // Tecla associada à cor atual (vermelho)
int tempoLimite = 3000; // Tempo limite em milissegundos
bool temporizadorAtivo = false; // Controla se o temporizador está ativo

// Função para desenhar um círculo
void desenharCirculo(float x, float y, float raio, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angulo = i * 3.14159f / 180;
        glVertex2f(x + raio * cos(angulo), y + raio * sin(angulo));
    }
    glEnd();
}

// Função para desenhar texto na tela
void desenharTexto(const char *texto, float x, float y) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(texto); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
}
// Função para gerar uma cor aleatória e associar a uma tecla
void gerarNovaCorEPosicao() {
    // Gera uma cor aleatória
    int cor = rand() % 4;
    switch (cor) {
        case 0: // Amarelo
            corAtual[0] = 1.0f; corAtual[1] = 1.0f; corAtual[2] = 0.0f;
            teclaCorreta = GLUT_KEY_UP; // Seta para cima
            break;
        case 1: // Vermelho
            corAtual[0] = 1.0f; corAtual[1] = 0.0f; corAtual[2] = 0.0f;
            teclaCorreta = GLUT_KEY_DOWN; // Seta para baixo
            break;
        case 2: // Azul
            corAtual[0] = 0.0f; corAtual[1] = 0.0f; corAtual[2] = 1.0f;
            teclaCorreta = GLUT_KEY_LEFT; // Seta para a esquerda
            break;
        case 3: // Verde
            corAtual[0] = 0.0f; corAtual[1] = 1.0f; corAtual[2] = 0.0f;
            teclaCorreta = GLUT_KEY_RIGHT; // Seta para a direita
            break;
    }

    // Gera uma posição aleatória entre -0.9 e 0.9 no eixo X e Y
    posicaoX = -0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.8f)));
    posicaoY = -0.9f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.8f)));
}
// Função de temporizador para mudar a cor e posição a cada 3 segundos
void temporizador(int valor) {
    // Se o temporizador expirar, gera nova cor e posição
    gerarNovaCorEPosicao();
    glutPostRedisplay(); // Redesenha a tela

    // Marca o temporizador como inativo
    temporizadorAtivo = false; // O temporizador não está mais ativo
}



// Função para capturar as teclas de seta
void tecladoEspecial(int key, int x, int y) {
    if (key == teclaCorreta) {
        acertos++;
    } else {
        erros++;
    }

    // Gera uma nova cor e posição após a tecla ser pressionada
    gerarNovaCorEPosicao();
    glutPostRedisplay(); // Redesenha a tela

    // Reinicia o temporizador somente se não estiver ativo
    if (!temporizadorAtivo) {
        temporizadorAtivo = true; // Marca o temporizador como ativo
        glutTimerFunc(tempoLimite, temporizador, 0);
    }
}

// Função para desenhar a cena
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha a bolinha na posição e cor atuais
    desenharCirculo(posicaoX, posicaoY, 0.1f, corAtual[0], corAtual[1], corAtual[2]);

    // Exibe o número de acertos no canto superior esquerdo
    char texto[50];
    sprintf(texto, "Acertos: %d", acertos);
    desenharTexto(texto, -0.95f, 0.9f); // Posição no canto superior esquerdo

    // Exibe o número de erros no canto superior esquerdo
    sprintf(texto, "Erros: %d", erros);
    desenharTexto(texto, -0.95f, 0.85f); // Posição logo abaixo dos acertos

    // Troca os buffers para exibir a bolinha
    glutSwapBuffers();
}



int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); // Inicializa o gerador de números aleatórios

    // Inicializa o GLUT e cria a janela
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tempo de Reação");

    // Define a cor de fundo da janela (branco)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Registra as funções de callback
    glutDisplayFunc(display);
    glutSpecialFunc(tecladoEspecial); // Teclas especiais (setas)
    
    // Gera a cor e a posição iniciais
    gerarNovaCorEPosicao();

    // Inicia o loop principal do GLUT
    glutMainLoop();
    return 0;
}
