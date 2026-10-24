# Random Jumpscare (Geode)

Mostra um jumpscare (imagem + áudio extraídos do seu vídeo) em momento aleatório durante a gameplay.
O jogo NÃO pausa; o overlay não bloqueia toques. Nenhuma outra função é alterada.

Ajustes: constantes no topo de `src/main.cpp` (intervalos). Troque `id`/`developer` em `mod.json`.

## Compilar -> `.geode`

### Opção A (mais fácil, sem instalar nada): GitHub Actions
1. Crie um repositório no GitHub e envie estes arquivos.
2. Aba **Actions** -> "Build Geode Mod" -> rode (ou espere o push).
3. Baixe o artefato **Build Output**: dentro está `meudev.random-jumpscare.geode`.

### Opção B (local)
1. Instale o Geode CLI: https://docs.geode-sdk.org/getting-started/geode-cli
2. `geode sdk install` e `geode sdk install-binaries`
3. Na pasta do projeto: `geode build` (gera o `.geode`)

## Instalar
Copie o `.geode` para a pasta `geode/mods` do jogo (ou abra pelo botão Geode no jogo).
Versão alvo: GD 2.2081 / Geode 5.x. Se você usa outra versão do GD, ajuste `gd` em `mod.json`.
