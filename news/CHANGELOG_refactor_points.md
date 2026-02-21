# Changelog (refatoração por pontos)

Data: 2026-02-21

Este changelog corresponde às alterações incluídas nos patches por ponto (1..4). Descrições curtas e explícitas.

## 1_backend_core_performance.patch
- libthecore/socket_keepalive(): mantém SO_KEEPALIVE e adiciona tuning agressivo de TCP keepalive (idle/intvl/cnt) quando suportado, para limpar sockets half-open mais cedo.
- libthecore/socket.c: inclui <netinet/tcp.h> (non-win) para macros TCP_KEEP*.
- game/battlepass_manager.cpp: UPDATE de battlepass_completed passou de DirectQuery para AsyncQuery (write não-crítica), reduzindo micro-freezes por DB lento.

## 2_leaks_memory_effects.patch
- EffectLib/EffectInstance: adiciona watchdog de 'orphan effects' (efeitos presos em (0,0,0)); se permanecerem perto da origem por 20s, marca o efeito como morto para ser removido pelo EffectManager.
- Inicializa contador m_fOrphanSeconds no construtor e reseta quando o efeito não está perto da origem.

## 3_client_visual_texttail_perf.patch
- UserInterface/PythonTextTail::ArrangeTextTail(): throttle automático quando existem muitos text tails (>50). Limita o rearranjo pesado a ~10Hz para melhorar FPS em cidades, mantendo legibilidade.

## 4_network_anticheat_zaxis.patch
- game/char_battle.cpp: adiciona validação conservadora de Z-axis durante Attack() para PCs contra mobs (monster/stone). Rejeita ataques com diferença extrema de altura (|ΔZ| > 2000) para reduzir wall/range exploits por desnível.
