# Tetris em C

Jogo de Tetris desenvolvido em linguagem C para placa com processador ARM e FPGA. O jogo mostra a tela em um monitor VGA, usa botões físicos para iniciar ou pausar e utiliza o acelerômetro (ADXL345) para controlar as peças.

## Estrutura do projeto

- `src/` - Código principal do jogo (`tetris.c`).
- `src/drivers/` - Arquivos responsáveis pela comunicação com a tela VGA, botões e acelerômetro.
- `include/` - Arquivos de cabeçalho (`.h`) usados pelo código.
- `tests/` - Programas simples para testar partes do hardware, como os botões.
- `build/` - Pasta onde ficam os arquivos gerados na compilação.

## O que é preciso para compilar

- Compilador `gcc` e ferramenta `make`
- Biblioteca `intelfpgaup` instalada no sistema da placa
- Suporte a `pthread`

## Como compilar e executar

Para compilar o jogo:

```bash
make
```

Para compilar e executar o jogo:

```bash
make run
```

Para compilar o teste de botões:

```bash
make test
```

Para apagar os arquivos gerados pela compilação:

```bash
make clean
```
