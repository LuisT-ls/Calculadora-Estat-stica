#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <map>
#include <fstream>
#include <cstdlib> // Para usar a função system
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/normal.hpp>

using namespace std;

// Função para calcular a moda
vector<double> calcularModa(const vector<double> &dados)
{
    map<double, int> frequenciaValores;
    for (int i = 0; i < dados.size(); i++)
    {
        frequenciaValores[dados[i]]++;
    }

    vector<double> modas;
    int maxFrequencia = 0;

    for (auto it = frequenciaValores.begin(); it != frequenciaValores.end(); it++)
    {
        if (it->second > maxFrequencia)
        {
            modas.clear();
            modas.push_back(it->first);
            maxFrequencia = it->second;
        }
        else if (it->second == maxFrequencia)
        {
            modas.push_back(it->first);
        }
    }

    return modas;
}

// Função para calcular a assimetria (coeficiente de assimetria)
double calcularAssimetria(const vector<double> &dados)
{
    int n = dados.size();
    double media = 0.0;
    for (int i = 0; i < n; i++)
    {
        media += dados[i];
    }
    media /= n;

    double somaDiferencasCubo = 0.0;
    for (int i = 0; i < n; i++)
    {
        double diferenca = dados[i] - media;
        somaDiferencasCubo += diferenca * diferenca * diferenca;
    }

    double desvioPadraoCubo = sqrt(somaDiferencasCubo / n);

    return somaDiferencasCubo / (n * desvioPadraoCubo * desvioPadraoCubo * desvioPadraoCubo);
}

// Função para calcular a curtose (coeficiente de curtose)
double calcularCurtose(const vector<double> &dados)
{
    int n = dados.size();
    double media = 0.0;
    for (int i = 0; i < n; i++)
    {
        media += dados[i];
    }
    media /= n;

    double somaDiferencasQuarta = 0.0;
    for (int i = 0; i < n; i++)
    {
        double diferenca = dados[i] - media;
        somaDiferencasQuarta += diferenca * diferenca * diferenca * diferenca;
    }

    double desvioPadraoQuarta = sqrt(somaDiferencasQuarta / n);

    return somaDiferencasQuarta / (n * desvioPadraoQuarta * desvioPadraoQuarta * desvioPadraoQuarta * desvioPadraoQuarta) - 3.0;
}

int main()
{
    int n;
    cout << "Escolha a opção:" << endl;
    cout << "1. Inserir dados diretamente" << endl;
    cout << "2. Inserir tabela de classe (formato: limite_inferior limite_superior frequencia)" << endl;
    int escolha;
    cin >> escolha;

    vector<double> dados;

    int numClasses = 0;
    double minLimiteInferior = numeric_limits<double>::max();
    double maxLimiteSuperior = numeric_limits<double>::min();

    int tamanho = 0; // Adicione esta declaração aqui

    if (escolha == 1)
    {
        cout << "Digite o tamanho da amostra: ";
        if (!(cin >> n) || n <= 0)
        {
            cerr << "Entrada inválida. Por favor, insira um número inteiro positivo." << endl;
            return 1; // Encerra o programa com código de erro
        }
        cout << "Digite os dados da amostra:" << endl;
        dados.resize(n);
        for (int i = 0; i < n; i++)
        {
            if (!(cin >> dados[i]))
            {
                cerr << "Entrada inválida. Por favor, insira um número válido." << endl;
                return 1; // Encerra o programa com código de erro
            }
            // Atualize minLimiteInferior e maxLimiteSuperior conforme necessário
            if (dados[i] < minLimiteInferior)
            {
                minLimiteInferior = dados[i];
            }
            if (dados[i] > maxLimiteSuperior)
            {
                maxLimiteSuperior = dados[i];
            }
        }
        sort(dados.begin(), dados.end());
        numClasses = ceil(1 + 3.322 * log10(n));
        tamanho = n; // Atualize o tamanho aqui
    }
    else if (escolha == 2)
    {
        cout << "Digite o número de classes: ";
        if (!(cin >> numClasses) || numClasses <= 0)
        {
            cerr << "Entrada inválida. Por favor, insira um número inteiro positivo." << endl;
            return 1; // Encerra o programa com código de erro
        }
        cout << "Digite a tabela de classe (formato: limite_inferior limite_superior frequencia):" << endl;

        for (int i = 0; i < numClasses; i++)
        {
            double limiteInferior, limiteSuperior;
            int frequencia;
            if (!(cin >> limiteInferior >> limiteSuperior >> frequencia) || limiteInferior >= limiteSuperior || frequencia < 0)
            {
                cerr << "Entrada inválida. Por favor, insira valores válidos para a tabela de classe." << endl;
                return 1; // Encerra o programa com código de erro
            }
            if (limiteInferior < minLimiteInferior)
            {
                minLimiteInferior = limiteInferior;
            }
            if (limiteSuperior > maxLimiteSuperior)
            {
                maxLimiteSuperior = limiteSuperior;
            }
            for (int j = 0; j < frequencia; j++)
            {
                double valor = (limiteInferior + limiteSuperior) / 2;
                dados.push_back(valor);
            }
        }
        sort(dados.begin(), dados.end());
        tamanho = dados.size(); // Atualize o tamanho aqui
    }
    else
    {
        cerr << "Opção inválida." << endl;
        return 1; // Encerra o programa com código de erro
    }

    // Mostrar o rol
    cout << "\nRol:" << endl;
    for (int i = 0; i < tamanho; i++) // Use tamanho em vez de dados.size()
    {
        cout << dados[i] << " ";
    }

    // Calcula assimetria e curtose
    double assimetria = calcularAssimetria(dados);
    double curtose = calcularCurtose(dados);
    
    // Calcular intervalo de confiança
    double confianca = 0.95; // Nível de confiança (por exemplo, 95%)
    double alpha = 1 - confianca;

    double soma = 0.0;
    for (int i = 0; i < tamanho; i++) // Use tamanho em vez de dados.size()
    {
        soma += dados[i];
    }
    double media = soma / tamanho;

    double somaDiferencasQuadradas = 0.0;
    for (int i = 0; i < tamanho; i++) // Use tamanho em vez de dados.size()
    {
        double diferenca = dados[i] - media;
        somaDiferencasQuadradas += diferenca * diferenca;
    }
    double variancia = somaDiferencasQuadradas / (tamanho - 1);
    double desvioPadraoAmostra = sqrt(variancia);
    double erroPadrao = desvioPadraoAmostra / sqrt(tamanho);

    boost::math::normal_distribution<double> distribuicaoNormal;
    double z_score = boost::math::quantile(boost::math::complement(distribuicaoNormal, alpha / 2));

    double limiteInferior = media - z_score * erroPadrao;
    double limiteSuperior = media + z_score * erroPadrao;

    cout << "\n\nIntervalo de Confiança (" << confianca * 100 << "%): [" << limiteInferior << ", " << limiteSuperior << "]" << endl;

    // Realizar um teste de hipótese para a média da amostra
    double valorHipoteseNula = 50.0; // Valor da hipótese nula
    double estatisticaTeste = (media - valorHipoteseNula) / (desvioPadraoAmostra / sqrt(tamanho));

    int grausDeLiberdade = tamanho - 1; // Graus de liberdade para um teste t de duas caudas
    boost::math::students_t_distribution<double> distribuicaoT(grausDeLiberdade);

    double p_valor = 2 * boost::math::cdf(boost::math::complement(distribuicaoT, abs(estatisticaTeste)));

    cout << "Estatística de Teste: " << estatisticaTeste << endl;
    cout << "Graus de Liberdade: " << grausDeLiberdade << endl;
    cout << "Valor-p: " << p_valor << endl;

    double nivelSignificancia = 0.05; // Nível de significância (por exemplo, 5%)
    if (p_valor < nivelSignificancia)
    {
        cout << "Rejeitar a hipótese nula" << endl;
    }
    else
    {
        cout << "Não rejeitar a hipótese nula" << endl;
    }

    cout << endl;

    cout << "\nTabela de Frequência:" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Classe | Amplitude |   Li   |   LS   |   Pm   |  fi  |   Fi   |  fri   | Fri%" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------" << endl;

    double amplitudeTotal = maxLimiteSuperior - minLimiteInferior;
    double amplitudeClasse = amplitudeTotal / numClasses;

    vector<int> frequenciaAbsoluta(numClasses, 0);
    vector<double> frequenciaRelativa(numClasses, 0.0);
    vector<double> frequenciaAcumuladaRel(numClasses, 0.0);

    for (int i = 0; i < numClasses; i++)
    {
        double limiteInferior = minLimiteInferior + i * amplitudeClasse;
        double limiteSuperior = limiteInferior + amplitudeClasse;
        double pontoMedio = (limiteInferior + limiteSuperior) / 2;

        for (int j = 0; j < tamanho; j++) // Use tamanho em vez de dados.size()
        {
            if (dados[j] >= limiteInferior && dados[j] < limiteSuperior)
            {
                frequenciaAbsoluta[i]++;
            }
        }

        frequenciaRelativa[i] = static_cast<double>(frequenciaAbsoluta[i]) / tamanho;

        if (i == 0)
        {
            frequenciaAcumuladaRel[i] = frequenciaRelativa[i];
        }
        else
        {
            frequenciaAcumuladaRel[i] = frequenciaRelativa[i] + frequenciaAcumuladaRel[i - 1];
        }

        cout << setw(6) << i + 1 << " | " << setw(9) << fixed << setprecision(2) << amplitudeClasse << " | " << setw(7) << setprecision(2) << limiteInferior << " | " << setw(7) << setprecision(2) << limiteSuperior << " | " << setw(7) << setprecision(2) << pontoMedio << " | " << setw(4) << setprecision(0) << frequenciaAbsoluta[i] << " | " << setw(7) << setprecision(0) << frequenciaAcumuladaRel[i] * tamanho << " | " << setw(6) << setprecision(2) << frequenciaRelativa[i] << " | " << setw(5) << setprecision(2) << frequenciaAcumuladaRel[i] * 100 << "%" << endl;
    }
    cout << "---------------------------------------------------------------------------------------------------------------------" << endl;

    cout << "\nNúmero de Classes: " << numClasses << endl;
    cout << "Tamanho da Amostra: " << tamanho << endl;
    cout << "Amplitude Total: " << setprecision(2) << amplitudeTotal << endl;

    // Exibir a mediana e a moda
    cout << "Média: " << setprecision(2) << media << endl;

    double mediana;
    if (tamanho % 2 == 0)
    {
        mediana = (dados[tamanho / 2 - 1] + dados[tamanho / 2]) / 2.0;
    }
    else
    {
        mediana = dados[tamanho / 2];
    }

    cout << "Mediana: " << setprecision(2) << mediana << endl;

    // Calcular a moda
    vector<double> modas = calcularModa(dados);

    if (!modas.empty())
    {
        cout << "Moda: ";
        if (modas.size() == 1)
        {
            cout << "unimodal";
        }
        else if (modas.size() == 2)
        {
            cout << "bimodal";
        }
        else if (modas.size() == 3)
        {
            cout << "trimodal";
        }
        else
        {
            cout << "polimodal";
        }
        cout << " (valores: ";
        for (size_t i = 0; i < modas.size(); i++)
        {
            cout << setprecision(2) << modas[i];
            if (i < modas.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << ")" << endl;
    }
    else
    {
        cout << "Moda: Essa amostra é amodal." << endl;
    }

    cout << "Desvio Padrão: " << setprecision(2) << desvioPadraoAmostra << endl;
    cout << "Variância: " << setprecision(2) << variancia << endl;
    cout << "Assimetria: " << setprecision(2) << assimetria << endl;
    cout << "Curtose: " << setprecision(2) << curtose << endl;

    char calcularQuartis;
    cout << "\nDeseja calcular os quartis? (S/N): ";
    cin >> calcularQuartis;

    if (calcularQuartis == 'S' || calcularQuartis == 's')
    {
        int quartil1_pos = tamanho * 0.25;
        int quartil2_pos = tamanho * 0.5;
        int quartil3_pos = tamanho * 0.75;

        double quartil1 = (dados[quartil1_pos - 1] + dados[quartil1_pos]) / 2.0;
        double quartil2 = mediana;
        double quartil3 = (dados[quartil3_pos - 1] + dados[quartil3_pos]) / 2.0;

        cout << "Quartil 1: " << setprecision(2) << quartil1 << endl;
        cout << "Quartil 2 (Mediana): " << setprecision(2) << quartil2 << endl;
        cout << "Quartil 3: " << setprecision(2) << quartil3 << endl;

        char calcularIntervaloInterquartil;
        cout << "\nDeseja calcular o intervalo interquartil? (S/N): ";
        cin >> calcularIntervaloInterquartil;

        if (calcularIntervaloInterquartil == 'S' || calcularIntervaloInterquartil == 's')
        {
            double intervaloInterquartil = quartil3 - quartil1;
            cout << "Intervalo Interquartil: " << setprecision(2) << intervaloInterquartil << endl;
        }

        char calcularDecis;
        cout << "\nDeseja calcular os decis? (S/N): ";
        cin >> calcularDecis;

        if (calcularDecis == 'S' || calcularDecis == 's')
        {
            // Solicitar ao usuário quais decis deseja calcular
            vector<int> decis_posicoes;
            int decis_posicao;
            do
            {
                cout << "Informe a posição do decis desejado (1 a 9): ";
                cin >> decis_posicao;
                if (decis_posicao >= 1 && decis_posicao <= 9)
                {
                    decis_posicoes.push_back(decis_posicao);
                }
                else
                {
                    cout << "Posição inválida. Digite um número de 1 a 9." << endl;
                }
                char continuar;
                cout << "Deseja calcular outro decis? (S/N): ";
                cin >> continuar;
                if (continuar != 'S' && continuar != 's')
                {
                    break;
                }
            } while (true);

            // Calcular e exibir os decis solicitados
            for (int i = 0; i < decis_posicoes.size(); i++)
            {
                int decis_pos = tamanho * (decis_posicoes[i] / 10.0);
                double decis = dados[decis_pos];
                cout << "Decil " << decis_posicoes[i] << ": " << setprecision(2) << decis << endl;
            }
        }
    }

    // Salvar os dados em um arquivo para o gnuplot
    ofstream dataFile("dados_boxplot.txt");
    if (dataFile.is_open())
    {
        for (int i = 0; i < tamanho; i++) // Use tamanho em vez de dados.size()
        {
            dataFile << dados[i] << endl;
        }
        dataFile.close();
    }
    else
    {
        cerr << "Não foi possível abrir o arquivo de dados para escrita." << endl;
        return 1;
    }

    // Executar o gnuplot para criar o gráfico de box plot
    system("gnuplot -e \"set title 'Box Plot'; set xlabel 'Amostras'; set ylabel 'Valores'; set label font ',12'; set terminal png size 800,400; set output 'boxplot.png'; plot 'dados_boxplot.txt' using 1 with boxes; plot 'dados_boxplot.txt' using (0):1:1 with labels notitle\"");

    return 0;
}
