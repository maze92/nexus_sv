CHANGELOGS

17.02.2026
[OPT] 001 - diff_001 - Otimização de I/O em motion/effects no client (src-client/Client/GameLib/RaceMotionData.cpp, src-client/Client/EffectLib/EffectData.cpp);
[FIX] 002 - diff_002 - Endurecimento de escaping SQL e builder de queries (server/metin2/Source/Server/db/src/ClientManagerPlayer.cpp, server/metin2/Source/Server/game/src/questlua.cpp, server/metin2/Source/Server/game/src/minigame_roulette.cpp, server/metin2/Source/Server/game/src/minigame_rumi.cpp, ... (+1));
[FIX] 003 - diff_003 - Alinhamento e validação de header de pacotes (server/metin2/Source/Server/game/src/packet.h);
[FIX] 004 - diff_004 - Correções de slots e extensão do inventário (client/pack/root/uiinventory.py, client/pack/root/uiscript/inventorywindow.py, server/metin2/Source/Server/game/src/char_item.cpp);
[FIX] 005 - diff_005 - Guards de segurança e correções no guild ladder (server/metin2/Source/Server/game/src/input_main.cpp, server/metin2/Source/Server/game/src/char.h, server/metin2/Source/Server/game/src/cube.cpp, server/metin2/Source/Server/game/src/war_map.cpp);
[FIX] 006 - diff_006 - Bloqueio mob-puller e clamps de overflow de EXP (server/metin2/Source/Server/game/src/char_battle.cpp);
[FIX] 007 - diff_007 - Validação do pacote de convite de party (server/metin2/Source/Server/game/src/input_main.cpp);
[FIX] 008 - diff_008 - Guards de null em eventos de dungeon (server/metin2/Source/Server/game/src/dungeon.cpp);
[FIX] 009 - diff_009 - Bounds check na posição de troca (server/metin2/Source/Server/game/src/exchange.cpp);
[FIX] 010 - diff_010 - Guards adicionais no movimento do sectree (server/metin2/Source/Server/game/src/entity.h, server/metin2/Source/Server/game/src/entity_view.cpp, server/metin2/Source/Server/game/src/char.cpp);
[FIX] 011 - diff_011 - Protected call para evitar crash em Lua alert (server/metin2/Source/Server/liblua/src/lib/lauxlib.c);
[FIX] 012 - diff_012 - Taxa de loja em 64-bit com clamps (server/metin2/Source/Server/game/src/shop.cpp);
[FIX] 013 - diff_013 - Sanitização do postfix de tabelas na DB (server/metin2/Source/Server/db/src/Main.cpp);
[FIX] 014 - diff_014 - Lock inflight para evitar double-processing em item_award (server/metin2/Source/Server/db/src/ClientManager.cpp);
[FIX] 015 - diff_015 - Clamp de underflow no guild ladder (DB) (server/metin2/Source/Server/db/src/GuildManager.cpp);
[FIX] 016 - diff_016 - Sanitização do texto do myshop sign (server/metin2/Source/Server/game/src/input_main.cpp);
[FIX] 017 - diff_017 - Resolução segura de target follow (server/metin2/Source/Server/game/src/char.h, server/metin2/Source/Server/game/src/char.cpp);
[FIX] 018 - diff_018 - Lock ao adicionar item (attr67) (server/metin2/Source/Server/game/src/char_item.cpp, server/metin2/Source/Server/game/src/questlua_attr67add.cpp);
[FIX] 019 - diff_019 - Sanitização de stamina do cavalo (server/metin2/Source/Server/game/src/horse_rider.cpp);
[FIX] 020 - diff_020 - Limpeza de affects ao reskill (server/metin2/Source/Server/game/src/char_skill.cpp);
[FIX] 021 - diff_021 - Validação de split stack no safebox (server/metin2/Source/Server/game/src/safebox.cpp);
[FIX] 022 - diff_022 - Alignment em 64-bit com clamps (server/metin2/Source/Server/game/src/char_battle.cpp);
[FIX] 023 - diff_023 - Clamps no banco de guild e remoção em war segura (server/metin2/Source/Server/game/src/guild.cpp);
[FIX] 024 - diff_024 - Bloqueio de itens locked no safebox (server/metin2/Source/Server/game/src/safebox.cpp);
