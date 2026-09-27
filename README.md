# 🍷 Projeto: O caso da Vinheria Agnello (CP1) - Sistema de Monitoramento
- Na Vinheria Agnello, a qualidade do vinho é influenciada diretamente pelas condições de temperatura, umidade e luminosidade do ambiente. Para garantir o armazenamento ideal, o projeto propõe um sistema automatizado de monitoramento focado na leitura de luminosidade para o interior da adega.
- A solução foi desenvolvida utilizando um Arduino Uno R3 com um Display LCD 16x2 (I2C). Possui um medidor preciso de luz em tempo real e o sistema tem uma interface aprimorada que exibe uma animação gráfica exclusiva da adega como tela de carregamento inicial.

# ⚙️ Funcionalidades
- Animação de Inicialização: Ao ligar o sistema, o display LCD mostra uma animação gráfica desenhando uma garrafa de vinho com a logo da empresa Ctrl + 5, seguida de um letreiro de boas-vindas.
- Leitura de Luminosidade: Utilização de dois sensores LDR com cálculo de média para evitar falsos alarmes causados por sombras momentâneas.

Alertas Visuais e Sonoros:
🔴 Alarme (Crítico): LED Vermelho e Buzzer ativados se a luz estiver abaixo de 20% ou acima de 90%.
🟡 Aviso (Atenção): LED Amarelo ativado (buzzer desligado) se a luz estiver entre 20%-39% ou 70%-89%.
🟢 Seguro (Ideal): LED Verde ativado para luminosidade entre 40% e 69%.

Interface em Tempo Real: O display LCD exibe continuamente a porcentagem de luminosidade e o status atual do ambiente (ALARME, AVISO ou SEGURO).

# 🛠️ Hardware e Componentes Utilizados
- 1x Arduino Uno R3
- 1x Display LCD 16x2 com módulo I2C
- 1x Breadboard
- 16x Fios Jumpers (Macho-Macho)
- 4x Fios Jumper (Macho-Fêmea)
- 2x Sensores de Luminosidade (Fotorresistores LDR)
- 3x LEDs (Vermelho, Amarelo e Verde)
- 1x Buzzer
- 5x Resistores (300 Ω)

## 📦 Bibliotecas Necessárias
- Wire
- LiquidCrystal_I2C

Para que o código funcione, é preciso instalar a biblioteca no Arduino IDE:
1. Abra o programa Arduino IDE.
2. Clique no canto superior esquerdo **Sketch** > **Include Library** > **Manage Libraries... or Ctrl + Shift + I**.
3. Pesquise por **LiquidCrystal_I2C** by Martin Kubovčík, Frank de Brabander.
4. Clique em **Install**.
*(Opcional: Caso o seu display não acender ou mostrar caracteres estranhos, pode ser necessário rodar um script de I2C Scanner para descobrir o endereço hexadecimal do seu módulo, que geralmente é "0x27" ou "0x3F").*

## 🚀 Como Executar no Arduino IDE
1. Clone este repositório ou faça o download dos arquivos em formato ZIP.
2. Abra o arquivo ".ino" no Arduino IDE.
3. Conecte o seu Arduino Uno ao computador via cabo USB.
4. Clique em **Tools** > **Board** > **Arduino AVR Boards** e selecione o modelo correto do seu Arduino.
5. Clique em **Tools** > **Port** e selecione a porta COM correspondente.
6. Clique no botão **Upload** (ícone de seta para a direita) para compilar e carregar o código para o Arduino.

## 🎓 Sobre o Projeto
Este é um projeto acadêmico desenvolvido para a vinícola fictícia do caso da Vinheria Agnello, como parte da avaliação (CP1 - Checkpoint 1) da FIAP (Faculdade de Informática e Administração Paulista).
**Disciplina:** Edge Computing & Computer Systems
**Professor:** Dr. Fábio Henrique Cabrini
**Grupo (CTRL + 5):** Eduardo Ambra Giordano de Castro, Flávia Sirahata Hasegava, Gabriel Souza Bore de Carvalho, Lirity Ribeiro de Paiva e Nicolle Lima Nascimento.

## 📄 Licença
Este projeto está licenciado sob a licença [MIT](LICENSE).
