Otimização Brutal de Memória (Memory Management)

O maior inimigo de um servidor de Metin2 não são os hackers, são os Memory Leaks (fugas de memória). O servidor liga consumindo 500MB de RAM e, passados 3 dias, está a consumir 8GB e vai abaixo (Core Dump).

Substituir o CMemoryPool: A Ymir criou o seu próprio gestor de memória ineficiente. Desenvolvedores avançados removem isso e ligam o servidor a alocadores modernos e muito mais rápidos, como o jemalloc (criado pelo Facebook) ou o tcmalloc (da Google).

Smart Pointers (C++11/17): Reescrever partes críticas do código onde os monstros, itens e personagens são criados (M2_NEW / delete) para usarem std::unique_ptr e std::shared_ptr. Isto garante que, quando um item é destruído no jogo, a memória é sempre devolvida ao servidor, eliminando fugas.
