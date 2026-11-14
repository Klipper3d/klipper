# Cartão nativo BMCU no dashboard

[English](../Ender3_V3_SE_Mainsail.md) | **Português brasileiro**

<a id="supported-versions"></a>
## Versões suportadas

A sobreposição 2.0.1 usa Mainsail **2.19.0**, commit
`5fb9e77fb9f4e60cf0725d9dc7f57cf7b84bbd70`, e a integração Ender com
BMCU-Klipper **1.1.2** / firmware **1.0.4**. É um componente nativo do dashboard,
não iframe, segundo servidor ou substituto do pacote BMCU oficial.
Conclua a [manutenção de pacote/firmware](releases/v2.0.1.md#upgrade-and-rollback)
antes de instalar o cartão. O instalador não grava nenhum dos controladores.

<a id="display-and-editing"></a>
## Visualização e edição

Quatro rolos coloridos representam os canais físicos 1–4. Material/cor são
metadados nativos BMCU, não prova da posição do filamento nem perfil térmico
automático. Os rótulos distinguem carregado, estacionado, vazio, incerto e offline.
O cartão usa tema, layout, ícone de rolo e conexão Moonraker do Mainsail. Oferece
inglês, português brasileiro e duas colunas no celular. Se estiver oculto,
habilite BMCU nos ajustes de layout do dashboard.

Selecione um rolo com impressora e BMCU prontas e paradas para editar material
e cor. A gravação usa `BMCU_ENDER_SET_FILAMENT`, valida a revisão atual do
canal e delega a persistência ao handler nativo. Não aquece, move, seleciona
ferramenta ou reconcilia rotas. Impressão, pausa, operação, rota incerta,
shutdown e desconexão desabilitam a edição. Se o diálogo ficar desatualizado,
feche e abra novamente; não repita uma gravação incerta às cegas. Confira
primeiro o console e os metadados atuais do canal.

A projeção somente leitura `bmcu_ender_integration.panel` consulta estado atual
em memória, não o snapshot público BMCU congelado durante impressão/pausa.
A renderização não atualiza o dispositivo nem envia UART. O módulo Ender se
chama `bmcu_ender_panel.py`; o `bmcu_panel.py` oficial é preservado.

No fim do filamento, insira a reposição no canal indicado e use **Retomar**
normal no Mainsail ou na tela original. A carga nativa BMCU e a purga/limpeza
Ender terminam antes de continuar. Falha na carga mantém a pausa. Consulte
[recuperação da impressão](Ender3_V3_SE_Print_Recovery.md). Isso não recupera
impressões após reinício ou queda de energia.

<a id="install-and-roll-back"></a>
## Instalar e reverter

Após a publicação, obtenha `mainsail-bmcu.zip` e `SHA256SUMS` da mesma release
da fonte do host. Conclua impressão e operações BMCU. Confira o diretório web
real na configuração do servidor; o exemplo considera uma instalação Mainsail
existente, sem Git, em `~/mainsail`. Não aponte para um checkout de fonte, raiz
do sistema de arquivos ou outro site.

```sh
sha256sum --check SHA256SUMS
python3 scripts/ender3-v3-se/install_mainsail_panel.py \
  --archive mainsail-bmcu.zip --destination "$HOME/mainsail" \
  --sha256 YOUR_ARCHIVE_SHA256
```

Use o checksum do ZIP da lista verificada da release. O instalador confere
versão fixa, fingerprints da sobreposição/receita canônicas, licenças, hashes
de todos os arquivos e caminhos do pacote antes de ativar. Verifica duas vezes
o estado parado pelo Moonraker local, preserva toda a árvore web anterior, copia
`config.json` e `.theme` e ativa por renomeação. Não altera nginx/Moonraker nem
implanta em outro host.

Atualize o navegador depois da instalação. O registro de rollback exibido
aponta para o backup privado ao lado do diretório web. Para restaurar:

```sh
python3 scripts/ender3-v3-se/install_mainsail_panel.py \
  --rollback /absolute/path/to/mainsail-panel-backup-ID/rollback.json
```

O rollback recusa descartar alterações posteriores de arquivos/diretórios,
inclusive tema/configuração particulares. Revise as alterações em vez de
apagá-las para forçar o rollback. Mantenha a fonte 2.0.1 correspondente para
usar esse utilitário.

O atualizador comum do Mainsail substitui este build e remove o cartão. Revise
atualizações automáticas desse diretório; mudar a versão upstream exige nova
verificação da sobreposição/build. O instalador não desabilita o atualizador
oficial silenciosamente.

<a id="rebuild-and-verify"></a>
## Recompilar e verificar

Use Node.js **24.21.0**, npm e Python 3 no Linux. A exportação da revisão Git
exata ignora arquivos privados/não rastreados. O lockfile upstream fixa as
dependências; o pacote usa caminhos ordenados, datas fixas e manifesto SHA-256.
Entradas de empacotamento idênticas geram o mesmo ZIP; isso não promete bundles
idênticos entre sistemas operacionais ou ferramentas diferentes.

```sh
git clone --depth 1 --branch v2.19.0 \
  https://github.com/mainsail-crew/mainsail.git /path/to/mainsail-upstream
python3 scripts/ender3-v3-se/build_mainsail_panel.py \
  --source /path/to/mainsail-upstream --output /path/to/new-build \
  --artifact /path/to/mainsail-bmcu.zip
```

O builder executa testes unitários upstream/sobreposição, lint da fonte,
formatação da sobreposição, checagem TypeScript e build de produção/PWA; inclui
`Mainsail-LICENSE` e `Ender-Integration-LICENSE`. A publicação exige os jobs
de firmware e cartão e verifica ambos os artefatos antes de publicar.

Para inspecionar no navegador local, prepare outro build com `--prepare-only`,
instale suas dependências e sirva em loopback. Abra `tests/bmcu-preview.html`;
ele usa apenas estado simulado e registra comandos localmente, sem conexão
com impressora ou Moonraker. Essa entrada de teste não integra o ZIP de produção.
Carga física, desprendimento da purga e aceitação real do runout ainda exigem
comissionamento supervisionado.
