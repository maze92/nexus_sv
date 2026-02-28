CHANGELOGS - NEXUS OS DEVELOPMENT

17.02.2026
[OPT] 001 - diff_001 - Redução de overhead de I/O em motions/efeitos via caching de ficheiros inexistentes (src-client/Client/GameLib/RaceMotionData.cpp, src-client/Client/EffectLib/EffectData.cpp).
[FIX] 002 - diff_002 - Implementação de escaping consistente e builders seguros em queries de Player e Lua (server/db/src/ClientManagerPlayer.cpp, server/game/src/questlua.cpp (+3)).
[FIX] 003 - diff_003 - Padronização de packing/alinhamento e validação de integridade de headers/opcodes (server/game/src/packet.h).
[FIX] 004 - diff_005 - Reforço de validação de inputs e verificações de estado no sistema de ranking de guildas (server/game/src/input_main.cpp, server/game/src/char.h (+2)).
[FIX] 005 - diff_006 - Implementação de limites (clamping) de EXP e valores para prevenir transbordamento aritmético (server/game/src/char_battle.cpp).
[FIX] 006 - diff_008 - Implementação de null-guards em handlers para prevenir encerramentos inesperados em eventos de instância (server/game/src/dungeon.cpp).
[FIX] 007 - diff_009 - Bounds-check rigoroso na variável display_pos para mitigar corrupção de memória e acessos OOB (server/game/src/exchange.cpp).
[FIX] 008 - diff_010 - Implementação de guards de coordenadas no movimento de entidades para prevenir estados inválidos na grid (server/game/src/entity.h, server/game/src/entity_view.cpp (+1)).
[FIX] 009 - diff_011 - Utilização de pcall para isolar falhas de execução de scripts e prevenir crashes no sistema de alertas (server/liblua/src/lib/lauxlib.c).
[FIX] 010 - diff_012 - Migração para 64-bit e implementação de clamps para prevenir overflow em cálculos de taxas e lojas (server/game/src/shop.cpp).
[FIX] 011 - diff_013 - Implementação de whitelist e higienização de strings para bloquear injeções no postfix de tabelas DB (server/db/src/Main.cpp).
[FIX] 012 - diff_015 - Clamp de underflow em valores negativos para estabilização de pontuação no GuildLadder (server/db/src/GuildManager.cpp).
[FIX] 013 - diff_016 - Validação de charset e comprimento de strings para prevenir injeções SQL e crashes em títulos de lojas (server/game/src/input_main.cpp).
[FIX] 014 - diff_017 - Implementação de guards de ID e ponteiros nulos no sistema de perseguição de alvos (server/game/src/char.h, server/game/src/char.cpp).
[FIX] 015 - diff_018 - Implementação de lock atómico na adição de atributos para reduzir race conditions e duplicação de itens (server/game/src/char_item.cpp, server/game/src/questlua_attr67add.cpp).
[FIX] 016 - diff_019 - Validação de intervalos de stamina para prevenir estados de overflow/underflow em montadas (server/game/src/horse_rider.cpp).
[FIX] 017 - diff_020 - Padronização na limpeza de buffers de skills e affects para evitar dessincronização no reskill (server/game/src/char_skill.cpp).
[FIX] 018 - diff_021 - Reforço na validação de quantidades em operações de split-stack para mitigar duplicação e acessos OOB (server/game/src/safebox.cpp).
[FIX] 019 - diff_022 - Implementação de aritmética 64-bit e clamps de segurança no sistema de alinhamento (server/game/src/char_battle.cpp).
[FIX] 020 - diff_023 - Validação de fluxos monetários (overflow/negativos) e bloqueio de remoções durante períodos de guerra (server/game/src/guild.cpp).
[FIX] 021 - diff_024 - Bloqueio rigoroso de movimentação e remoção de itens com flag de bloqueio no armazém (server/game/src/safebox.cpp).
[FIX] 022 - diff_025 - Persistência de estado em janelas de guilda para prevenir dessincronização de inventário (server/game/src/guild.cpp).
[FIX] 023 - diff_026 - Reset mandatório de estados de observador durante o login para prevenir persistência de flags inválidas (server/game/src/input_login.cpp).
[FIX] 024 - diff_027 - Implementação de verificações de segurança em operações de swap/refine no deck de Alquimia (server/game/src/char.h, server/game/src/char.cpp (+2)).
[FIX] 025 - diff_028 - Clamp de comprimento no nome de flags para prevenir corrupção de strings no motor de quests (server/game/src/questlua_pc.cpp).
[FIX] 026 - diff_029 - Reforço de segurança em tabelas de refinamento de pesca para prevenir acessos a índices inválidos (server/game/src/char_item.cpp).
[FIX] 027 - diff_030 - Adição de validações de estado e segurança no processo de troca de reino (server/game/src/char_change_empire.cpp).
[FIX] 028 - diff_031 - Implementação de guards adicionais em tabelas de combate para evitar falhas de lógica em trocas de estado (server/game/src/char_battle.cpp).
[FIX] 029 - diff_032 - Sanity-check em pacotes de rede para bloquear dados malformados na entrada principal (server/game/src/input_main.cpp).
[FIX] 030 - diff_033 - Revisão de integridade e robustez nas comunicações da header de base de dados (server/game/src/db.h).
[FIX] 031 - diff_034 - Validação rigorosa de pacotes e inputs de personagens para bloquear dados corrompidos (server/game/src/char.h, server/game/src/char.cpp (+1)).
[FIX] 032 - diff_035 - Implementação de validações de segurança no processamento de entidades (server/game/src/entity.cpp).
[FIX] 033 - diff_036 - Reforço na estabilidade e validação de buffers no sistema de detecção gráfica (src-client/Client/EterLib/GrpDetector.cpp).
[FIX] 034 - diff_037 - Adição de validações de segurança no carregamento de recursos de áudio (src-client/Client/MilesLib/SoundManager.cpp).
[FIX] 035 - diff_038 - Sanity-check de pacotes para bloquear o envio de dados malformados em operações de itens (server/game/src/char_item.cpp).
[FIX] 036 - diff_039 - Limitação do comprimento de strings de flags para evitar corrupção de memória no sistema de quests (server/game/src/char.h, server/game/src/char.cpp (+1)).
[FIX] 037 - diff_040 - Implementação de limites contra overflow/underflow na renderização de texto do cliente (src-client/Client/EterLib/GrpTextInstance.cpp, src-client/Client/EterLib/GrpTextInstance.h (+1)).
[FIX] 038 - diff_041 - Sanity-check em comunicações entre peers para bloquear pacotes de rede inválidos (server/db/src/PeerBase.cpp, server/game/src/desc.cpp (+1)).
[FIX] 039 - diff_042 - Melhoria na estabilidade e validação de operações base de ficheiros (src-client/Client/EterBase/FileBase.cpp, src-client/Client/EterBase/FileBase.h).
[FIX] 040 - diff_043 - Bloqueio de pacotes malformados em operações específicas de dungeon (server/game/src/dungeon.cpp, server/game/src/dungeon.h).
[FIX] 041 - diff_044 - Implementação de guards de tamanho e limites mínimos para prevenir leituras inválidas de ficheiros proto (server/game/src/item.h).
[FIX] 042 - diff_045 - Implementação de Frustum e Distance Culling para reduzir carga de CPU em sombras e efeitos (src-client/Client/GameLib/MapOutdoorRender.cpp, src-client/Client/GameLib/Area.cpp).

19.02.2026
[FIX] 043 - Correção na lógica de transição e movimento de equipamentos para o inventário (client/pack/root/uiinventory.py).

21.02.2026
[FIX] 044 - diff_046 - Implementação de estimativa de movimento para mitigar rubberbanding e endurecer proteção contra micro-speedhack (server/game/src/char.h, server/game/src/char.cpp (+1)).
[FIX] 045 - diff_047 - Transição para geradores de números aleatórios mais seguros para mitigar previsibilidade em rolls críticos (server/libthecore/include/utils.h, server/libthecore/src/utils.c (+2)).
[FIX] 046 - diff_048 - Definição de limites máximos de payload para bloquear abusos e ataques de negação de serviço na rede (server/game/src/input.cpp).
[FIX] 047 - diff_050 - Implementação de proteção no pickup de ofertas de guilda para bloquear abusos de concorrência (server/game/src/input_main.cpp).
[FIX] 048 - diff_051 - Clamp de durações negativas para prevenir a aplicação de buffs/debuffs inválidos (server/game/src/char_affect.cpp).
[FIX] 049 - diff_052 - Implementação de trinco atómico no sistema de trocas para bloquear concorrência e métodos de duplicação (server/game/src/exchange.h, server/game/src/exchange.cpp).
[FIX] 050 - diff_053 - Sincronização consistente de estados de troca com a DB para prevenir dessincronização de dados (server/game/src/exchange.cpp).
[FIX] 051 - diff_055 - Sistema de VID único e seguro em instâncias para prevenir colisões de entidades (server/game/src/dungeon.h, server/game/src/dungeon.cpp).
[FIX] 052 - diff_056 - Implementação de versões thread-safe de localtime/gmtime para mitigar data races no servidor (server/libthecore/src/utils.c, server/libthecore/src/log.c (+7)).
[FIX] 053 - diff_057 - Sincronização de funções de tempo para prevenir data races em módulos gráficos do cliente (src-client/Client/EterPythonLib/PythonGraphicModule.cpp, src-client/Client/GameLib/Property.cpp (+2)).
[OPT] 054 - Otimização do motor de janela no Windows para melhoria de performance e compatibilidade (src-client/Client/EterLib/MSWindow.cpp, src-client/Client/EterLib/MSWindow.h, src-client/Client/UserInterface/PythonApplication.cpp (+1)).
[RVD] 055 - Remoção definitiva do sistema EXTENDED_ERROR_LOG da infraestrutura do servidor.
[RVD] 056 - Remoção definitiva do sistema ENABLE_CRASH_RPT dos ficheiros de servidor.

22.02.2026
[FIX] 057 - diff_058 - Implementação de guarda contra entidades "ghost" no sectree para prevenir estados de mapeamento inválidos (game/src/sectree.cpp).
[FIX] 058 - diff_059 - Reforço na integridade de escritas assíncronas para mitigar race conditions no sistema de ranking (game/src/Ranking.cpp).
[FIX] 059 - diff_060 - Utilização de clock monotonic para prevenir regressões temporais e manipulações de timers no sistema (libthecore/src/utils.c).
[FIX] 060 - diff_061 - Implementação de guarda na stack Lua para mitigar transbordamentos e crashes no motor de quests (home/metin2/Source/Server/game/src/questmanager.h, server/game/src/questmanager.cpp (+1)).
[FIX] 061 - diff_062 - Gestão segura de descritores para prevenir o envio de pacotes a sockets encerrados (home/metin2/Source/Server/game/src/desc.cpp, server/game/src/desc_manager.cpp).
[FIX] 062 - diff_064 - Reforço de segurança contra vulnerabilidades de Format String no processamento de personagens e itens (game/src/char_item.cpp, game/src/char_manager.cpp (+3)).
[FIX] 063 - diff_068 - Sincronização de raio de visão entre cliente e servidor para mitigar o efeito de pop-in de monstros (game/src/config.cpp, UserInterface/PythonApplication.cpp).
[FIX] 064 - diff_070 - Higienização de buffers no módulo Locale do cliente para prevenir transbordamentos de memória (Client/UserInterface/Locale.cpp).
[FIX] 065 - diff_073 - Definição de limites estritos de payload no gestor LZO para bloquear abusos de rede (game/src/lzo_manager.h, game/src/lzo_manager.cpp (+1)).
[FIX] 066 - diff_074 - Melhorias de robustez e estabilidade na integração do Discord Rich Presence (Client/UserInterface/PythonNetworkStreamPhaseGame.cpp).
[FIX] 067 - diff_075 - Revisão de segurança e robustez em pontos críticos de lógica (DragonSoul/Auth) (server/home/metin2/Source/Server/game/src/DragonSoul.cpp, server/game/src/auth_brazil.cpp (+7)).
[FIX] 068 - diff_076 - Auditoria e correção de fluxos incompletos em novos sistemas integrados no motor DB (server/server/home/metin2/Source/Server/game/src/db.cpp, server/game/src/auth_brazil.cpp (+7)).
[FIX] 069 - diff_077 - Implementação de controlo de fluxo no pool de consultas SQL para prevenir sobrecarga de processos (metin2/Source/Server/libsql/AsyncSQL.cpp, server/game/src/log.cpp).
[FIX] 070 - diff_078 - Bloqueio de manipulação de tipos de janelas via rede para impedir acesso indevido a funções de NPCs (metin2/Source/Server/game/src/char_item.cpp).
#[FIX] 071 - diff_079 - Transição para o sistema Raw Input do Windows e implementação de ponteiros de instância estáticos (src-client/Client/EterLib/Input.h, src-client/Client/EterLib/Input.cpp (+2)).
#[FIX] 072 - diff_080 - Auditoria de estabilidade no renderizador de atores e controlo de áudio do cliente (src-client/Client/GameLib/ActorInstanceRender.cpp, src-client/Client/UserInterface/UserInterface.cpp (+1)).
[FIX] 073 - diff_081 - Implementação de guardas contra o fenómeno de Object Slicing em classes base de itens e personagens (metin2/Source/Server/game/src/item.h, server/game/src/char.h).
[FIX] 074 - diff_082 - Otimização na infraestrutura de sockets para reduzir acumulação de ligações no Auth (metin2/Source/Server/libthecore/src/socket.c).
#[FIX] 075 - diff_067 - Validação rigorosa de timing e pacotes de voo para bloqueio de wait-hacks e speed-hacks.
[FIX] 076 - diff_069 - Implementação de validações adicionais de estado para prevenir trocas de itens inválidas.
#[FIX] 077 - diff_071 - Sistema de limpeza de callbacks e eventos pendentes para prevenir fugas de memória no servidor.
[FIX] 078 - diff_072 - Higienização de codificação de strings para prevenir encerramentos forçados (freezes) no cliente/servidor.

27.02.2026
[ADD] 079 - Adição definitiva do sistema USE_FOX_FS nos ficheiros de servidor.
[ADD] 080 - Adição definitiva do sistema ENABLE_RAW_INPUT nos ficheiros do servidor (src-client/Client/GameLib/Input.cpp, src-client/Client/EterLib/Input.h, src-client/Client/UserInterface/PythonApplicationProcedure.cpp (+1)).
[OPT] 081 - Analise dos ficheiros do servidor e do Python para verificar inconsistências e ficheiros não usados.
