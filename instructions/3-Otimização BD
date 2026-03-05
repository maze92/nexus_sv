Passo 1: Otimização da Base de Dados
Contexto: Estamos a otimizar o servidor de Metin2 (C++) para evitar bloqueios na Main Thread (lag spikes).
Tarefa:
1. Analisa os ficheiros db.cpp e log.cpp, etc. Procura por chamadas a DirectQuery ou métodos síncronos equivalentes que são usados para tarefas não críticas (como gravar logs de chat, logs de itens, atualizações de posição de personagens, ou mortes de monstros).
2. Substitui essas chamadas síncronas pela interface AsyncSQL do jogo (geralmente usando DBManager::instance().AsyncQuery ou a fila assíncrona já existente).
3. Regra de Segurança: Não alteres chamadas de Base de Dados críticas que exijam uma resposta imediata na mesma frame (como o carregamento da personagem no login ou verificações de segurança). Foca-te apenas em operações de Write/Insert/Update que podem ocorrer em background.
4. Gera o código atualizado garantindo que não há fugas de memória (memory leaks) na passagem das strings SQL.

Passo 2: Criar a Infraestrutura do "ThreadPool"
Contexto: Preciso de um motor de Multithreading robusto e seguro para o meu servidor de Metin2.
Tarefa:
Cria dois novos ficheiros: ThreadPool.h e ThreadPool.cpp.
Implementa uma classe ThreadPool que gira um número fixo de Worker Threads (ex: 4 ou 8 baseados no std::thread::hardware_concurrency()).
Inclui uma Fila de Tarefas (Task Queue) que seja 100% Thread-Safe, utilizando std::mutex, std::condition_variable e std::function<void()>.
O ThreadPool deve permitir o enqueue (enfileiramento) de tarefas usando std::bind ou lambdas genéricas, e deve devolver um std::future caso seja necessário aguardar o resultado.
Garante que no destrutor da classe todas as threads são terminadas graciosamente (join) sem deixar processos zombie. Mostra-me o código completo destes dois ficheiros.

Passo 3: Isolar a Lógica de Pathfinding (O Passo Crucial)
Contexto: Vamos preparar a função de cálculo de caminho (A* / Pathfinding) para ser executada de forma assíncrona. O código original interage diretamente com as entidades globais, o que causa colisões de memória (Race Conditions).
Tarefa:
Analisa a função de cálculo de rota no sectree_manager.cpp, etc. (geralmente ligada à geração de caminhos ou IA de movimento de monstros).
Refatora (reescreve) a função para que ela seja puramente "Data-Driven" (pura função matemática). Ela não pode receber o ponteiro LPCHARACTER ou modificar o monstro.
Ela deve receber apenas uma struct simples (Passagem por Valor) contendo: startX, startY, targetX, targetY, e mapa (ou ponteiro constante para o grid do SECTREE).
A função deve retornar um vetor de coordenadas std::vector<std::pair<int, int>> (o caminho calculado) e nada mais.
Mostra-me como fica a nova função de Pathfinding desacoplada do objeto do jogo.

Passo 4: Ligar o Pathfinding ao ThreadPool
Contexto: Temos a infraestrutura ThreadPool pronta e a função de Pathfinding já desacoplada.
Tarefa:
Modifica a rotina de IA de movimento dos monstros (geralmente em char_state.cpp no estado StateMove ou na atualização periódica do monstro).
Quando o monstro precisar de andar, ele não calcula a rota imediatamente. Em vez disso, envia a struct de coordenadas (StartX, StartY, TargetX, TargetY) para o ThreadPool usando um método assíncrono.
O monstro deve entrar num estado temporário de "A aguardar rota".
Quando o std::future (a resposta da thread) estiver pronto com o vetor do caminho, a Main Thread do jogo recupera esse vetor no próximo "Tick" (update) e aplica o movimento ao monstro.
Usa trincos de segurança (std::mutex) no objeto do monstro apenas na hora de gravar o caminho final resultante na variável de destino, garantindo que não há corrupção se a entidade for eliminada durante o cálculo.
                                                                                                                                                                                    
