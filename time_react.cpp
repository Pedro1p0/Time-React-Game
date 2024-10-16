#include <cmath>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>

// Variáveis globais para a cor e posição da bolinha
float corAtual[3] = {1.0f, 0.0f, 0.0f}; // Exemplo: vermelho
float posicaoX = 0.0f, posicaoY = 0.0f; // Posição da bolinha
int acertos = 0, erros = 0; // Contadores de acertos e erros
int teclaCorreta = GLUT_KEY_DOWN; // Tecla associada à cor atual (vermelho)
float tempoAcumulado = 0.0f; // Variável de tempo acumulado
float intervaloTempo = 3000.0f; // Intervalo de 3 segundos em milissegundos
int tempoAnterior = 0; // Armazena o tempo do frame anterior
float somaTemposReacao = 0.0f; // Soma dos tempos de reação corretos
float tempoTotalJogo = 0.0f; // Tempo total de jogo em milissegundos
const float limiteTempo = 30000.0f; // Limite de tempo de 30 segundos
bool jogoIniciado = false; // Variável de controle para verificar se o jogo já começou
int tempoIniciarBolinha = 0; // Armazena o tempo em que a bolinha aparece

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

// Função para capturar as teclas de seta
void tecladoEspecial(int key, int x, int y) {
    if (jogoIniciado && tempoTotalJogo < limiteTempo) { // Somente verifica teclas se o jogo tiver começado
        if (key == teclaCorreta) {
            acertos++;
            int tempoReacao = glutGet(GLUT_ELAPSED_TIME) - tempoIniciarBolinha; // Calcula o tempo de reação
            somaTemposReacao += tempoReacao; // Soma o tempo de reação ao acertar
            tempoAcumulado = 0.0f; // Zera o tempo acumulado ao acertar
            gerarNovaCorEPosicao(); // Gera nova cor e posição após a tecla ser pressionada
            tempoIniciarBolinha = glutGet(GLUT_ELAPSED_TIME); // Reinicia o tempo da bolinha
        } else {
            erros++;
            tempoAcumulado = 0.0f; // Zera o tempo acumulado ao errar
            gerarNovaCorEPosicao(); // Gera nova cor e posição após o erro
            tempoIniciarBolinha = glutGet(GLUT_ELAPSED_TIME); // Reinicia o tempo da bolinha
        }
    }
    glutPostRedisplay(); // Redesenha a tela
}

// Função para capturar a barra de espaço e iniciar o jogo
void teclado(unsigned char key, int x, int y) {
    if (!jogoIniciado && key == 32) { // 32 é o código ASCII para a barra de espaço
        jogoIniciado = true;
        tempoAnterior = glutGet(GLUT_ELAPSED_TIME); // Inicia o tempo quando o jogo começa
        gerarNovaCorEPosicao(); // Gera a primeira bolinha
        tempoIniciarBolinha = tempoAnterior; // Inicializa o tempo da bolinha
    }
}

// Função para calcular a porcentagem de acertos
float calcularPorcentagemAcertos() {
    int totalTentativas = acertos + erros;
    if (totalTentativas == 0) return 0.0f; // Evita divisão por zero
    return (static_cast<float>(acertos) / totalTentativas) * 100.0f;
}

// Função para calcular o tempo médio de reação
float calcularTempoMedioReacao() {
    if (acertos == 0) return 0.0f; // Evita divisão por zero
    return somaTemposReacao / acertos / 1000.0f; // Tempo médio em segundos
}

// Função para desenhar a cena
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!jogoIniciado) {
        // Tela inicial com instruções
        glColor3f(0.0f, 0.0f, 0.0f); // Cor preta para o texto
        desenharTexto("Pressione a barra de espaco para comecar", -0.5f, 0.9f); // Ajustado para o canto superior esquerdo

        // Instruções com as cores correspondentes
        glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
        desenharTexto("Seta para cima: Amarelo", -0.4f, 0.7f); // Ajustado para o canto superior esquerdo

        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
        desenharTexto("Seta para baixo: Vermelho", -0.4f, 0.6f); // Ajustado para o canto superior esquerdo

        glColor3f(0.0f, 0.0f, 1.0f); // Azul
        desenharTexto("Seta para esquerda: Azul", -0.4f, 0.5f); // Ajustado para o canto superior esquerdo

        glColor3f(0.0f, 1.0f, 0.0f); // Verde
        desenharTexto("Seta para direita: Verde", -0.4f, 0.4f); // Ajustado para o canto superior esquerdo
    } else {
        // Desenha a bolinha
        desenharCirculo(posicaoX, posicaoY, 0.1f, corAtual[0], corAtual[1], corAtual[2]);

        // Atualiza o tempo total do jogo
        tempoTotalJogo = glutGet(GLUT_ELAPSED_TIME) - tempoAnterior;
        glColor3f(0.0f, 0.0f, 0.0f); // Cor preta para o texto
        // Exibe estatísticas
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Acertos: %d Erros: %d", acertos, erros);
        desenharTexto(buffer, -0.9f, 0.9f); // Exibe acertos e erros no canto superior esquerdo

        float porcentagemAcertos = calcularPorcentagemAcertos();
        snprintf(buffer, sizeof(buffer), "Porcentagem de Acertos: %.2f%%", porcentagemAcertos);
        desenharTexto(buffer, -0.9f, 0.85f); // Exibe a porcentagem de acertos

        float tempoMedioReacao = calcularTempoMedioReacao();
        snprintf(buffer, sizeof(buffer), "Tempo Medio de Reacao: %.2f segundos", tempoMedioReacao);
        desenharTexto(buffer, -0.9f, 0.8f); // Exibe o tempo médio de reação

        // Controla o tempo acumulado
        tempoAcumulado += (glutGet(GLUT_ELAPSED_TIME) - tempoAnterior);
        tempoAnterior = glutGet(GLUT_ELAPSED_TIME);

        // Verifica se o tempo acumulado atingiu o intervalo de 3 segundos
        if (tempoAcumulado >= intervaloTempo) {
            erros++; // Conta como erro
            tempoAcumulado = 0.0f; // Reseta o tempo acumulado
            gerarNovaCorEPosicao(); // Gera nova bolinha
            tempoIniciarBolinha = glutGet(GLUT_ELAPSED_TIME); // Reinicia o tempo da bolinha
        }

        // Verifica se o tempo total de jogo ultrapassou 30 segundos
        if (tempoTotalJogo >= limiteTempo) {
            jogoIniciado = false; // Finaliza o jogo
            // Aqui você pode adicionar lógica para mostrar resultados finais
            // Resetar os contadores, se necessário
        }
    }

    glutSwapBuffers(); // Troca os buffers para exibir a nova cena
}

// Função para inicializar o OpenGL
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 0); // Cor de fundo branca
    glColor3f(0.0f, 0.0f, 0.0f);// Cor do texto preta
    glPointSize(10.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Função principal
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); // Inicializa a semente aleatória
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Teste de Reacao");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutMainLoop();
    return 0;
}
