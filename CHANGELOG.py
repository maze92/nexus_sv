CHANGELOGS

17.02.2026
[OPT] 001 - diff_001 - Otimiza carregamento de motions/efeitos (reduz overhead de I/O) (src-client/Client/GameLib/RaceMotionData.cpp, src-client/Client/EffectLib/EffectData.cpp);
[FIX] 002 - diff_002 - Hardening SQL: escaping/format seguro em queries de player e Lua (server/metin2/Source/Server/db/src/ClientManagerPlayer.cpp, server/metin2/Source/Server/game/src/questlua.cpp (+3));
[FIX] 003 - diff_003 - packet.h: alinhamento/packing consistente + validação de headers/opcodes (server/metin2/Source/Server/game/src/packet.h);
[FIX] 004 - diff_004 - Inventário (client UI): mapeamento de slots/tabs consistente + extend inven (client/pack/root/uiinventory.py, client/pack/root/uiscript/inventorywindow.py (+1));
[FIX] 005 - diff_005 - Security: valida janelas/inputs e protege rotinas do GuildLadder (server/metin2/Source/Server/game/src/input_main.cpp, server/metin2/Source/Server/game/src/char.h (+2));
[FIX] 006 - diff_006 - char_battle: bloqueia exploit mob-puller + clamp de EXP/valores contra overflow (server/metin2/Source/Server/game/src/char_battle.cpp);
[FIX] 007 - diff_007 - Party invite: sanity-check de packet/IDs para evitar crash/abuso (server/metin2/Source/Server/game/src/input_main.cpp);
[FIX] 008 - diff_008 - Dungeon events: null-guards em callbacks/handlers para evitar crash (server/metin2/Source/Server/game/src/dungeon.cpp);
[FIX] 009 - diff_009 - Exchange: bounds-check em display_pos para prevenir OOB/memory corruption (server/metin2/Source/Server/game/src/exchange.cpp);
[FIX] 010 - diff_010 - Sectree move: guards adicionais em movimentação (coords/sectree) contra estados inválidos (server/metin2/Source/Server/game/src/entity.h, server/metin2/Source/Server/game/src/entity_view.cpp (+1));
[FIX] 011 - diff_011 - Lua alert: chamada protegida (pcall) para evitar crash por script (server/metin2/Source/Server/liblua/src/lib/lauxlib.c);
[FIX] 012 - diff_012 - Shop/Tax: cálculos em 64-bit + clamps para impedir overflow (server/metin2/Source/Server/game/src/shop.cpp);
[FIX] 013 - diff_013 - DB table postfix: sanitize/whitelist para evitar injection e paths inválidos (server/metin2/Source/Server/db/src/Main.cpp);
[FIX] 014 - diff_014 - item_award: lock inflight por sessão para evitar dupla atribuição (server/metin2/Source/Server/db/src/ClientManager.cpp);
[FIX] 015 - diff_015 - Guild ladder: clamp de underflow/negativos em ranking/pontos (server/metin2/Source/Server/db/src/GuildManager.cpp);
[FIX] 016 - diff_016 - MyShop sign: sanitize string (tamanho/charset) para evitar injection/crash (server/metin2/Source/Server/game/src/input_main.cpp);
[FIX] 017 - diff_017 - Target follow: resolução segura de target (null/id guard) (server/metin2/Source/Server/game/src/char.h, server/metin2/Source/Server/game/src/char.cpp);
[FIX] 018 - diff_018 - Attr67: lock ao adicionar item (evita race/dup em operações concorrentes) (server/metin2/Source/Server/game/src/char_item.cpp, server/metin2/Source/Server/game/src/questlua_attr67add.cpp);
[FIX] 019 - diff_019 - Horse stamina: sanitize ranges para evitar under/overflow e estados inválidos (server/metin2/Source/Server/game/src/horse_rider.cpp);
[FIX] 020 - diff_020 - Reskill: limpa skills/affects coerentemente para evitar desync de estado (server/metin2/Source/Server/game/src/char_skill.cpp);
[FIX] 021 - diff_021 - Safebox split-stack: valida quantidade/stack para evitar dupe e OOB (server/metin2/Source/Server/game/src/safebox.cpp);
[FIX] 022 - diff_022 - Alignment: 64-bit + clamps contra overflow/underflow (server/metin2/Source/Server/game/src/char_battle.cpp);
[FIX] 023 - diff_023 - Guild bank: valida money changes (overflow/negativos) + bloqueia remoções em war (server/metin2/Source/Server/game/src/guild.cpp);
[FIX] 024 - diff_024 - Safebox: bloqueia mover/remover itens locked (server/metin2/Source/Server/game/src/safebox.cpp);
