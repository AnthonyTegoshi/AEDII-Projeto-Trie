import subprocess

variacoes = [
    ("./exp_trie4.exe", "resultados/trie4"),
    ("./exp_triebin.exe", "resultados/triebin")
]

experimentoDuplVar = [
    ("bancos/seq_31_5000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_10000.txt", "consultas/seq_31_10000.txt"),
    ("bancos/seq_31_50000.txt", "consultas/seq_31_50000.txt"),
    ("bancos/seq_31_100000.txt", "consultas/seq_31_100000.txt"),
    ("bancos/seq_31_500000.txt", "consultas/seq_31_500000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_1000000.txt")
]

experimentoVarConsulta = [
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_10000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_50000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_100000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_500000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_1000000.txt")
]

experimentoVarBanco = [
    ("bancos/seq_31_5000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_10000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_50000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_100000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_500000.txt", "consultas/seq_31_5000.txt"),
    ("bancos/seq_31_1000000.txt", "consultas/seq_31_5000.txt")
]

for programa, prefixo_resultado in variacoes:
    # experimento variando tamanho do banco e consultas para analisar os tempos de execução de forma separada
    with open(prefixo_resultado + "_duplvar.csv", "w", encoding="utf-8") as f:
        f.write("Inserções,Tempo Inserção (s),Buscas,Tempo Busca (s),Nós Trie,Espaço Aprox. (kB)\n")
        for arq1, arq2 in experimentoDuplVar:
            print(f"{prefixo_resultado}_duplvar: {arq1} {arq2}")
            saida = subprocess.check_output(
                [programa, arq1, arq2],
                text=True
            )
            f.write(saida)

    # experimento variando o número de consultas para um banco grande do mesmo tamanho
    with open(prefixo_resultado + "_varconsulta.csv", "w", encoding="utf-8") as f:
        f.write("Inserções,Tempo Inserção (s),Buscas,Tempo Busca (s),Nós Trie,Espaço Aprox. (kB)\n")
        for arq1, arq2 in experimentoVarConsulta:
            print(f"{prefixo_resultado}_varconsulta: {arq1} {arq2}")
            saida = subprocess.check_output(
                [programa, arq1, arq2],
                text=True
            )
            f.write(saida)

    # experimento variando tamanho do banco e fazendo mesmo quantidade pequena de consultas
    with open(prefixo_resultado + "_varbanco.csv", "w", encoding="utf-8") as f:
        f.write("Inserções,Tempo Inserção (s),Buscas,Tempo Busca (s),Nós Trie,Espaço Aprox. (kB)\n")
        for arq1, arq2 in experimentoVarBanco:
            print(f"{prefixo_resultado}_varbanco: {arq1} {arq2}")
            saida = subprocess.check_output(
                [programa, arq1, arq2],
                text=True
            )
            f.write(saida)