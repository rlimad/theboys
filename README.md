# 🦸‍♂️ The Boys – Simulador de Missões Orientado a Eventos

![C99](https://img.shields.io/badge/lang-C%20(C99)-blue.svg)
![Simulação](https://img.shields.io/badge/tipo-simula%C3%A7%C3%A3o%20por%20eventos-orange.svg)
![Programação 1](https://img.shields.io/badge/projeto-acad%C3%AAmico-success)

> Simulador inspirado no universo de *The Boys*, desenvolvido como projeto acadêmico.

## 🧠 Descrição

Simula um mundo com:
- **Heróis** com paciência, velocidade e conjunto de habilidades
- **Bases** com lotação limitada e filas de espera
- **Missões** com requisitos de habilidades
- **Eventos** (CHEGADA, ENTRADA, MISSAO, etc.) gerenciados em uma **fila de prioridade**

Toda a simulação é dirigida por eventos em ordem temporal, com controle de comportamento e estatísticas.

## 🛠️ Tecnologias e TADs

- **Linguagem:** C (C99)
- **TADs utilizados:**
  - `conjunto` – conjunto de inteiros (habilidades)
  - `lista` – lista duplamente ligada (fila de espera)
  - `fprio` – fila de prioridades encadeada (eventos)
- Simulação orientada a eventos com logs e métricas

## ▶️ Como executar

```bash
make
./theboys
```

## 📂 Estrutura do Projeto

```c
/theboys
├── theboys.c            # Ponto de entrada
├── simulation.c         # Lógica principal da simulação
├── entity.c             # Tipos do mundo: Herói, Base, Missão
├── event.c              # Criação e tipos de eventos
├── log.c                # Geração dos logs
├── utils.c              # Funções auxiliares (random, distância)
└── tads/                # TADs customizados: conjunto.o, lista, fprio
```

## 📊 Saída

Ao final, o programa imprime:
  - Estado final de heróis (vivos/mortos, habilidades, experiência);
  - Desempenho das bases (lotação, missões, filas);
  - Taxa de sucesso, tentativas por missão e uso de Composto V.
